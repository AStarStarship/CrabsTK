    /** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/uniscanner.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <stdafx.h>
#include "../include/expression.h"
#include "../include/log.h"

namespace _ {

const char* RxStateString (byte state) {
    static const char* strings[] = {
        "RxScanningStringState",
        "RxScanningVarIntState",
        "RxScanningAddressState",
        "RxScanningPodState",
        "RxScanningArgsState",
        "RxHandlingErrorState",
        "RxMemeberNotFoundErrorState",
        "RxScanningHashState",
        "RxLockedState" };
    if (state >= RxLockedState)
        return strings[RxLockedState];
    return strings[state];
}

const char* TxStateStrings (byte state) {
    static const char* strings[] = {
        "TxWritingState",
        "TxLockedState"
    };
    if (state >= TxLockedState)
        return strings[TxLockedState];
    return strings[state];
}

Bin* ExpressionRx (Expression* a) {
    return a == nullptr?nullptr:
        reinterpret_cast<Bin*>(reinterpret_cast<byte*>(a) +
            a->rx_offset);
}

Bout* ExpressionTx (Expression* a) {
    return a == nullptr?nullptr:
        reinterpret_cast<Bout*>(reinterpret_cast<byte*>(a) +
            a->tx_offset);
}

Expression* ExpressionInit (byte* buffer, uint_t buffer_size,
                        uint_t stack_count, Operable* root) {
    if (buffer == nullptr)
        return nullptr;
    if (buffer_size < kMinBufferSize)
        return nullptr;
    if (stack_count == 0) stack_count = 1;    //< Minimum stack size.

    Expression* expr = reinterpret_cast<Expression*> (buffer);

    uint_t total_stack_size = (stack_count - 1) * (2 * sizeof (void*));
    // Calculate the size of the Monoid and Stack.
    uint_t size = (buffer_size - sizeof (Expression) -
        total_stack_size + 1) >> 1;  // >>1 to divide by 2
    expr->type = 0;
    expr->rx_state = RxLockedState;
    expr->tx_state = 0;
    expr->stack_count = 0;
    expr->verify_count = 0;
    expr->stack_size = stack_count;
    expr->num_states = 0;
    expr->op = nullptr;
#if DEBUG_CHINESE_ROOM
    printf ("\nInitializing Stack:\n"
        "sizeof (Stack): %u\n"
        "(stack_count * (2 * sizeof (void*))): %u\n"
        "stack_count: %u buffer_size: %u size: %u\n"
        "!!! stack_count: %u &stack_count: 0x%p !!!\n",
        sizeof (Expression), (stack_count *
        (2 * sizeof (void*))), stack_count,
        buffer_size, size, stack_count, &stack_count);
#endif //< DEBUG_CHINESE_ROOM
    expr->bytes_left = 0;
    uint_t offset = sizeof (Expression) + total_stack_size - sizeof (void*),
        rx_offset = sizeof (Bin) + total_stack_size + offset;
    expr->rx_offset = rx_offset;
    expr->tx_offset = rx_offset + size;
    expr->header = 0;
    expr->headers = 0;
    printf ("\n\n!!!\nroot: 0x%p\n", root);
    expr->op = root;
    printf ("a->op: 0x%p\n", expr->op);
    MonoidRxInit (ExpressionRx (expr), size);
    MonoidTxInit (ExpressionTx (expr), size);
    return expr;
}

/** Expression** ExpressionStack (Expression* a) {
    auto a = reinterpret_cast<byte*> (a) + sizeof (Expression) +
        a->stack_count * a->stack_size * sizeof (const uint_t*);
    return reinterpret_cast<Expression**> (a);
} */

bool ExpressionIsDynamic (Expression* a) {
    return a->type % 2 == 1;
}

byte* ExpressionEndAddress (Expression* a) {
    return MonoidRxEndAddress (ExpressionRx (a));
}

ticket_t ExpressionReset (Expression* a) {
    return 0;
}

const Operation* Push (Expression* a, Operable* b) {
    if (a == nullptr)
    {
        if (b == nullptr)
            return nullptr;
        return b->Star (0, nullptr);  //< Return d's header.
    }
    if (b == nullptr)
        return reinterpret_cast<const Operation*> (Report (NullDevicePushError));
    if (a->stack_count >= a->stack_size)
        return reinterpret_cast<const Operation*> (Report (StackOverflowError));
    ExpressionStack (a)[a->stack_count++] = b;
    return 0;
}

ticket_t ExpressionPop (Expression* a) {
    if (a->stack_count == 0)
        return Report (TooManyPopsError);
    a->op = ExpressionStack (a)[--a->stack_count];
    return 0;
}

byte* ExpressionStateStack (Expression* a) {
    return reinterpret_cast<byte*> (a) + sizeof (Expression);
}

ticket_t ExpressionExitRxState (Expression* a) {
    auto count = a->stack_count;
    if (count == 0)
        return Report (TooManyPopsError);
    a->rx_state = ExpressionStateStack (a)[--count];
    a->stack_count = count;
    return 0;
}

ticket_t ExpressionEnterRxState (Expression* a, RxState state) {

    if (state >= RxLockedState)
        return Report (InvalidRxStateError);
    auto count = a->stack_count;
    if (count >= a->stack_size)
        return Report (StackOverflowError);
    ExpressionStateStack (a)[count] = a->rx_state;
    count = a->stack_count + 1;
    a->rx_state = state;
    return 0;
}

ticket_t ExpressionPushScanHeader (Expression* a, const uint_t* header) {
    if (header == nullptr)
        return Report (NullPointerError, header, ExpressionTx (a));
    uint_t verify_count = a->verify_count,
        stack_size = a->stack_size;
    a->type_index = *header++;
    if (verify_count >= stack_size)
        return Report (StackOverflowError, header, ExpressionTx (a));

    // Move the current header to the scan stack
    const uint_t* current_header = const_cast<const uint_t*> (a->header);
    a->header = header;
    const uint_t** headers = (const uint_t**)&a->headers;
    headers[stack_size] = current_header;
    a->verify_count = verify_count + 1;
    return 0;
}

ticket_t ExpressionPushScanHeader (Expression* a,
    volatile const uint_t* header) {
    const uint_t** headers;
    uint_t verify_count = a->verify_count;
    if (verify_count >= a->stack_size)
        return Report (StackOverflowError);

    headers = (const uint_t**)a->headers;
    ExpressionExitRxState (a);
    a->header = *headers;
    verify_count = verify_count;
    return 0;
}

ticket_t ExpressionPopScanHeader (Expression* a) {
    uint_t verify_count = a->verify_count;
    if (verify_count == 0)
        return Report (TooManyPopsError);

    verify_count = verify_count - 1;
    return 0;
}

void ExpressionScanNextType (Expression* a) {
    uint_t* header = const_cast<uint_t*> (a->header);
    if (header == nullptr) {
        ExpressionEnterRxState (a, RxScanningArgsState);
        return;
    }

    uint_t type = *header;
    if (type == NIL) {  // Done scanning args.
        ExpressionPopScanHeader (a);
        return;
    }
    ++header;
    a->header = header;
    a->rx_state = a->last_rx_state;
    //type = *header;
}

byte ExpressionStreamTxByte (Expression* a) {
    return MonoidTxStreamByte (ExpressionTx (a));
}

void ExpressionScan (Expression* a, Portal* input) {
    uint_t            size,         //< The size of the ring buffer.
                      space,        //< The space left in the right buffer.
                      length,       //< The length of the ring buffer data.
                      type;         //< The current type.
    byte              rx_state,     //< The current rx FSM state.
                      b;            //< The current byte being verified.
                                    //              temp_ui1;       //< Used for verifying AR1 only.
    ticket_t          result;       //< An error ticket procedure return value.
    hash16_t          hash;         //< The hash of the ESC being verified.
                                    //uint16_t      temp_ui2;       //< Used for calculating AR2 and BK2 size.
                                    //uint32_t      temp_ui4;       //< Used for calculating AR4 and BK4 size.
                                    //uint64_t      temp_ui8;       //< Used for calculating AR8 and BK8 size.
    time_t            timestamp,    //< The last time when the automata ran.
                      delta_t;      //< The time delta between the last timestamp.
    Expression      * expr;         //< The current Expression.
    const Operation * op;           //< The current Set.
    const uint_t    * header;       //< The current Set header being verified.
    Bin             * rx;                //< The rx Bin.
    byte            * begin,        //< The beginning of the ring buffer.
                    * end,          //< The end of the ring buffer.
                    * start,        //< The start of the ring buffer data.
                    * stop;         //< The stop of the ring buffer data.

    if (a == nullptr) {
        PrintDebug ("a = null");
        return;
    }
    if (input == nullptr) {
        PrintDebug ("input = null");
        return;
    }

    rx_state = a->rx_state;
    rx = ExpressionRx (a);
    size = rx->size;
    hash = a->hash;
    timestamp = TimestampNow ();
    delta_t = timestamp - a->last_time;

    if (delta_t <= a->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch (rare)
            delta_t *= -1;
    }

    begin = MonoidRxBaseAddress (ExpressionRx (a));
    end = begin + size;
    start = begin + rx->start;
    stop = begin + rx->stop;
    space = MonoidSpace (start, stop, size);
    length = size - space + 1;

    printf ("\n\n| Scanning address 0x%p:\n| rx_state: %s\n| length: %u\n", a,
        RxStateString (rx_state), length);

    // Manually load first byte:
    b = input->Pull ();
    //b = MonoidStreamByte (rx);
    hash = Hash16 (b, hash);
    *start = b;
    ++start;
    while (input->Length ()) {
        // Process the rest of the bytes in a loop to reduce setup overhead.
        if (rx_state == RxScanningStringState) {
            PrintDebug ("RxScanningStringState");

            if (a->bytes_left == 0) {
                PrintDebug ("Done parsing string.");
                Report (RxStringBufferOverflowError, a->header, start);
                return;
            }
            // Hash byte.

            // Check if string terminated.
            if (b == 0) {
                PrintDebug ("string terminated.");
                // Check if there is another argument to scan.
                // 
                ExpressionExitRxState (a);
                return;
            }
            PrintDebug ("b != 0");
            --a->bytes_left;
            return;
        } else if (rx_state == RxScanningVarintState) {
            // When verifying a varint, there is a max number of bytes for the
            // type (3, 5, or 9) but the varint may be complete before this
            // number of bytes. We're just basically counting down and looking
            // for an overflow situation.

            PrintDebug ("RxScanningVarintState.");
            // Hash byte.

            if (a->bytes_left == 1) {
                PrintDebug ("Checking last byte:");

                // @warning I am not current saving the offset. I'm not sure 
                //          what to do here. The header class uses a variadic 
                //          template, I'm kind of tempted to switch to an int 
                //          type for the headers so I can just use a normal 
                //          array bracket initializer. The other option is to 
                //          add 32 to the first byte.

                if ((b >> 7) != 1) {
                    Report (VarintOverflowError, a->header, start);
                    ExpressionEnterRxState (a, RxHandlingErrorState);
                    return;
                }

                return;
            }
            --a->bytes_left;
            return;
        } else if (rx_state == RxScanningAddressState) {
            // When verifying an address, there is guaranteed to be an
            // a->op set. We are just looking for null return values
            // from the Do (byte, Stack*): const Operable* function, 
            // pushing Star(s) on to the Star stack, and looking for the 
            // first procedure call.

            PrintDebugHex ("| RxScanningAddressState", b);
            if (b == ESC) {     // Start processing a new ESC.
                PrintDebug ("Start of ESC:");
                ++a->header;
                ExpressionPushScanHeader (a, a->header);
                return;
            }

            device = a->op;
            a->operand = nullptr;
            op = device->Star (b, nullptr);
            if (op == nullptr) {
                // Could be an invalid op or a Star Stack push.
                if (a->operand == nullptr) {
                    PrintDebug ("No op found.");
                    return;
                }
                //ExpressionPushScan (a, a->operand);
            }
            if (result = ExpressionPushScanHeader (a, op->params)) {
                PrintDebug ("Error reading address.");
                return;
            }
            ExpressionEnterRxState (a, RxScanningArgsState);
            return;
        } else if (rx_state == RxScanningArgsState) {
            // In this state, a procedure has been called to scan on a valid
            // device. This state is responsible for loading the next header
            // argument and checking for the end of the procedure call.

            PrintDebug ("RxScanningArgs.");

            device = a->op;
            if (device == nullptr) {
                // Check if it is a Procedure Call or Star.
                device = a->op;
                a->operand = nullptr;
                op = device->Star (b, nullptr);
                device = a->operand;
                if (!device) {
                    if (op == nullptr) {
                        PrintError ("Invalid op");
                        ExpressionEnterRxState (a, RxLockedState);
                        return;
                    }
                    // Else it was a function call.
                    ExpressionEnterRxState (a, RxScanningArgsState);
                    return;
                }
            } else {
                // Verify byte as address.
                header = const_cast<const uint_t*> (a->header);
                if (!a->header) {
                    return;
                }
                if (a->type_index == 0) {
                    PrintDebug ("Procedure verified.");
                    ExpressionExitRxState (a);
                }
                // Get next type.
                type = *header;

                PrintDebug (TypeString (type));

                // Word-align the start of the buffer we're reading from.
                start += TypeAlign (start, type);

                // Switch to next state
                if (type <= SOH) {
                    if (type < SOH) {
                        Report (ReadInvalidTypeError);
                        ExpressionEnterRxState (a, RxLockedState);
                    } else {
                        ExpressionEnterRxState (a, RxScanningAddressState);
                    }
                } else if (type == STX) {   // String type.
                    ExpressionEnterRxState (a, RxScanningStringState);
                } else if (type < DBL) {   // Plain-old-data types.
                    a->bytes_left = SizeOf (type);
                    ExpressionEnterRxState (a, RxScanningPodState);
                } else if (type < UV8) {   // Varint types
                    a->bytes_left = SizeOf (type);
                    ExpressionEnterRxState (a, RxScanningVarintState);
                } else if (type <= AR8) {

                } else if (type == ESC) {

                } else if (type <= BK8) {

                } else if (type > US) {
                    Report (InvalidRxTypeError);
                } else {    // It's a US
                    PrintDebug ("Scanning Unit");
                    a->bytes_left = kUnitSize;
                    ExpressionEnterRxState (a, RxScanningPodState);
                }

            }
        } else if (rx_state == RxHandlingErrorState) {
            PrintDebug ("RxHandlingErrorState.");

        } else if (rx_state >= RxLockedState) {
            PrintDebug ("RxLockedState.");
        } else {    // parsing plain-old-data.
            if (a->bytes_left-- == 0) {
                PrintDebug ("Done verifying POD type.");
                ExpressionScanNextType (a);
            } else {
                b = input->Pull ();
                PrintDebugHex ("Loading next byte", b);
                a->hash = Hash16 (b, hash);
                *start = b;
                ++start;
            }
        }
    }
    rx->start = Diff (begin, start);
}

bool ExpressionContains (Expression* a, void* address) {
    if (address < reinterpret_cast<byte*>(a))
        return false;
    if (address > ExpressionEndAddress (a)) return false;
    return true;
}

ticket_t ExpressionPushHeader (Expression* a, const uint_t* header) {
    if (a->stack_count >= a->stack_size) {
        // Handle overflow cleanup:
        return Report (StackOverflowError, header);
    }

    //if (dc == nullptr) return noDevceSelectedError ();

    return 0;
}

const uint_t* ExpressionHeaderStack (Expression* a) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<byte*>
        (a) + sizeof (Expression) + a->stack_count);
}

void ExpressionCloseExpression (Expression* a) {
    PrintDebug ("[FF]");
}

void ExpressionCancelExpression (Expression* a) {
    PrintDebug ("[CAN]");
    //stopAddress = txOpen;
}

void ExpressionScrubExpression (Expression* a) {
    // Erase the buffer by writing zeros to it.

    Bin* rx = ExpressionRx (a);
    uint_t size = rx->size;

    byte* begin = MonoidRxBaseAddress (a, a->rx_offset),
        *end = begin + rx->size,
        *start = begin + rx->start,
        *stop = begin + rx->stop;

    uint_t buffer_space = MonoidSpace (start, stop, size);

    if (start == stop) return; //< Nothing to do.
    if (start > stop) {
        memset (start, 0, end - start);
        memset (begin, 0, start - begin);
        return;
    }
    memset (start, 0, stop - start);
    rx->start = Diff (a, begin);
    rx->stop = Diff (a, start + 1);
}

const Operable* Read (Expression* a, const uint_t* esc, void** args) {
    return reinterpret_cast<const Operable*> (Read (ExpressionRx (a), esc,
        args));
}

const Operable* Write (Expression* a, const uint_t* esc, void** args) {
    return reinterpret_cast<const Operable*> (Write (ExpressionTx (a),
        a->return_address, esc,
        args));
}

const Operable* Write (Expression* a, const char* address,
    const uint_t* esc, void** args) {
    return reinterpret_cast<const Operable*> (Write (ExpressionTx (a), address,
        esc, args));
}

void ExpressionPrint (Expression* expr) {
    PrintLine ('_');
    printf ("| Stack:    ");
    if (expr == nullptr) {
        printf ("null\n");
        PrintLine ("|", '_');
        return;
    }
    printf ("0x%p", expr);
    PrintLine ("|", '_');
    std::cout << "| type:          "
        << (expr->type == -1)?"interprocess no dynamic memory.":
        (expr->type == 0)?"no dynamic memory":
        (expr->type == 1)?"dynamic memory":
        (expr->type == 2)?"dynamic memory":"Invalid type";

    std::cout << "\n| bytes_left:    " << expr->bytes_left
        << "\n| tx_offset:     " << expr->tx_offset
        << "\n| rx_offset:     " << expr->rx_offset
        << "\n| stack_count:   " << expr->stack_count
        << "\n| stack_size:    " << expr->stack_size
        << "\n| verify_count:  " << expr->verify_count
        << "\n| num_states:    " << expr->num_states;
    PrintLine ("|", '-');
    Print (expr->op);
    std::cout << "| header: ";
    PrintEsc (const_cast<const uint_t*>(expr->header));
    std::cout << "| Scan Stack: " << expr->verify_count;
    const uint_t** headers = (const uint_t**)expr->headers;
    if (headers == nullptr) {
        std::cout << " null";
    } else {
        for (uint_t i = 0; i < expr->stack_count; ++i) {
            std::cout << "| " << i << ": ";
            PrintEsc (headers[i]);
        }
    }
    PrintLine ("|", '_');
    //system ("PAUSE");
}

Bin* ExpressionInit (byte* buffer, uint_t size) {
    if (buffer == nullptr)
        return nullptr;
    Bin* rx = reinterpret_cast<Bin*>(buffer);
    rx->size = size - sizeof (Bin);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

Bin* ExpressionInit (Bin* rx, uint_t size) {
    if (rx == nullptr)
        return nullptr;
    rx->size = size - sizeof (Bin);
    rx->start = 0;
    rx->stop = 0;
    rx->read = 0;
    return rx;
}

/** Gets the start of the Bin ring buffer. 
byte* BBaseAddress (void* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}*/

byte* ExpressionBaseAddress (Bin* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}

uint_t ExpressionSpace (Bin* rx) {
    if (rx == nullptr) return ~0;

    byte* base = ExpressionBaseAddress (rx);
    return MonoidSpace (base + rx->start, base + rx->stop, rx->size);
}

byte* ExpressionBaseAddress (void* ptr, uint_t rx_tx_offset) {
    enum {
        kSlotHeaderSize = sizeof (Bin) + sizeof (uintptr_t) -
        sizeof (Bin) % sizeof (uintptr_t),
        //< Offset to the start of the ring buffer.
    };

    return reinterpret_cast <byte*>(ptr) + rx_tx_offset + kSlotHeaderSize;
}

byte* ExpressionEndAddress (Bin* rx) {
    return reinterpret_cast<byte*>(rx) + kSlotHeaderSize + rx->size;
}

ticket_t Read (Bin* rx, const uint_t* params, void** args) {
    if (rx == nullptr)
        Report (NullPointerError, 0, 0, 0);
    if (params == nullptr)
        Report (NullHeaderError, 0, 0, 0);
    if (args == nullptr)
        Report (NullHeaderError, 0, 0, 0);

    byte type,                  //< The current type we're reading.
        ui1;                    //< Temp variable to load most types.
    uint16_t ui2;               //< Temp variable for working with UI2 types.
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t ui4;
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t ui8;
#endif

    byte*     ui1_ptr;          //< Pointer to a UI1.
#if  USING_VARINT2 || USING_BK2
    uint16_t* ui2_ptr;          //< Pointer to a UI2.
#endif
#if USING_VARINT4 || USING_AR4 || USING_BK4
    uint32_t* ui4_ptr;          //< Pointer to a UI4.
#endif
#if USING_VARINT8 || USING_AR8 || USING_BK8
    uint64_t* ui8_ptr;          //< Pointer to a UI1.
#endif
    uint_t size,                //< The size of the ring buffer.
        length,                 //< The length of the data in the buffer.
        count,                  //< The argument length.
        index,                  //< The index in the escape sequence.
        num_params = *params;   //< The number of params.

    uintptr_t offset;           //< The offset to word align the current type.

    if (num_params == 0) {

#if DEBUG_CHINESE_ROOM
        std::cout << "\n\n| Reading Bin: ";
#endif
        return 0;
    }

    size = rx->size;

    byte* begin = ExpressionBaseAddress (rx), //< The beginning of the buffer.
        * end   = begin + size,             //< The end of the buffer.
        * start = begin + rx->start,        //< The start of the data.
        * stop  = begin + rx->stop;         //< The stop of the data.
    const uint_t* param = params + 1;       //< The current param.

    length = MonoidLength (start, stop, size);

#if DEBUG_CHINESE_ROOM
    std::cout << "\n\n| Reading Bin: \n";
    //PrintEsc (params);
    printf ("| begin: 0x%p start : %u stop : %u end : %u "
            "length: %u ", begin, Diff (begin, start), 
            Diff (begin, stop), Diff (begin, end), length);
#endif
    // When we scan, we are reading from the beginning of the Bin buffer.

    for (index = 0; index < num_params; ++index) {
        type = *param;
        ++param;
#if DEBUG_CHINESE_ROOM
        printf ("\n| index %2u: %s  start: %u, stop: %u hash: ", index,  
                TypeString (type), Diff (begin, start), Diff (begin, stop));
#endif

        switch (type) {
          case NIL:
              goto RxInvalidType;
          case SOH:    //< _R_e_a_d__S_t_r_i_n_g_-_8____________________________
          case STX:
              // Load buffered-type argument length and increment the index.
              count = *param;
              ++param;
#if DEBUG_CHINESE_ROOM
              printf ("\n|           Reading string with max length %u: ", count);
#endif
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr) break;

            // Read from buffer and write to the stack:
            ui1 = *start;
            if (++start >= end) start -= size;
            *ui1_ptr = ui1;
            ++ui1_ptr;

            while (ui1 != 0 && count != 0) {
                if (count-- == 0)
                    return Report (BufferUnderflowError, params, index,
                                        start);
#if DEBUG_CHINESE_ROOM
                putchar (ui1);
#endif
                ui1 = *start;       // Read byte from ring-buffer.
                if (++start >= end) start -= size;
                *ui1_ptr = ui1;     // Write byte to destination.
                ++ui1_ptr;
            }
#if DEBUG_CHINESE_ROOM
            printf (" done!\n");
#endif
            if (type != SOH) {
                *ui1_ptr = ui1;
            }
            break;
          case SI1:     //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_____________________
          case UI1:
          case BOL:
#if USING_1_BYTE_TYPES
            if (length == 0) 
                return Report (BufferUnderflowError, params, index, start);
            --length;

            // Read from buffer and write to the stack:
            ui1 = *start;                       //< Read
            if (++start >= end) start -= size;  //< Increment
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == 0)
                break;
            *ui1_ptr = ui1;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case SI2:     //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s____________________
          case UI2:
          case HLF:
#if USING_2_BYTE_TYPES
            Read2ByteType:
            // Word-align
            offset = TypeAlign2 (start);
            if (length < offset + 2)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset + 2;
            start  += offset;
            if (start >= end) start -= size;    //< Bound
            // Read from buffer and write to the stack:
            ui2_ptr = reinterpret_cast<uint16_t*>(start);
            ui2 = *ui2_ptr;                     //< Read
            start += sizeof (uint16_t);         //< Increment
            if (start >= end) start -= size;    //< Bound
            // Load next pointer and increment args.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == 0) break;
            *ui2_ptr = ui2;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case SI4:     //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s____________________
          case UI4:
          case FLT:
          case TMS:
#if USING_4_BYTE_TYPES
            Read4ByteType:
            // Word-align
            offset = TypeAlign4 (start);
            if (length < offset + 4)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset + 4;
            start += offset;
            if (start >= end) start -= size;    //< Bound
            // Read from buffer and write to the stack:
            ui4_ptr = reinterpret_cast<uint32_t*>(start);
            ui4 = *ui4_ptr;                     //< Read
            start += sizeof (int32_t);          //< Increment
            if (start >= end) start -= size;    //< Bound
            // Load next pointer and increment args.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == 0) break;
            *ui4_ptr = ui1;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case TMU:     //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s____________________
          case SI8:
          case UI8:
          case DBL:
#if USING_8_BYTE_TYPES
            Read8ByteType:
            // Word-align
            offset = TypeAlign8 (start);
            if (length < offset + 8)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset + 8;
            start += offset;
            if (start >= end) start -= size;    //< Bound
            // Read from buffer and write to the stack:
            ui8_ptr = reinterpret_cast<uint64_t*>(start);
            ui8 = *ui8_ptr;                     //< Read
            start += sizeof (uint64_t);         //< Increment
            if (start >= end) start -= size;    //< Bound
            // Load next pointer and increment args.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == 0) break;
            *ui8_ptr = ui8;                     //< Write
            break;
#else
            goto RxInvalidType;
#endif
          case SV2:     //< _R_e_a_d__V_a_r_i_n_t__2____________________________
          case UV2:
#if USING_VARINT2
            goto Read2ByteType;
#else
            goto RxInvalidType;
#endif
          case SV4:     //< _R_e_a_d__V_a_r_i_n_t__4____________________________
          case UV4:
#if USING_VARINT4
              goto Read4ByteType;
#else
            goto RxInvalidType;
#endif
          case SV8:     //< _R_e_a_d__V_a_r_i_n_t__8____________________________
          case UV8:
#if USING_VARINT8
            goto Read8ByteType;
#else
            goto RxInvalidType;
#endif
          case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1________________________________
#if USING_AR1
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;

            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto RxInvalidType;
#endif
          case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2________________________________
#if USING_AR2
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++; //< Get type from header
              
            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto RxInvalidType;
#endif
          case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4________________________________
#if USING_AR4
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;   //< Read type

            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto RxInvalidType;
#endif
          case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8________________________________
#if USING_AR8
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = *param++;   //< Read Type
              
            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
              goto RxInvalidType;
#endif
          case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e__________________
            // I'm not sure exactly how this should work. I can't do recursion
            // because of embedded limitations.
            break;
          case BK8: //< _R_e_a_d__B_o_o_k_8_____________________________________
            // Word-align
              offset = TypeAlign8 (start);
              if (length < offset + 128)
                  return Report (BufferUnderflowError, params, index, start);
              length -= offset;
              start += offset;
              if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = (uint_t)*ui8_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui8_ptr + 1);
            goto ReadBlock;
#if USING_AR8
#else
            goto RxInvalidType;
#endif
          case BK4: //< _R_e_a_d__B_o_o_k_4_______________________________________
            // Word-align
              offset = TypeAlign4 (start);
              if (length < offset + 64)
                  return Report (BufferUnderflowError, params, index, start);
              length -= offset;
              start += offset;
              if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = (uint_t)*ui4_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui4_ptr + 1);
            goto ReadBlock;
#if USING_BK4
#else
            goto RxInvalidType;
#endif
          case BK2: //< _R_e_a_d__B_o_o_k_2_______________________________________
            // Word-align
            offset = TypeAlign2 (start);
            if (length < offset + 32)
                return Report (BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == nullptr)
                return Report (NullPointerError, params, index, start);
            count = (uint_t)*ui2_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui2_ptr + 1);
            goto ReadBlock;
#if USING_BK2
#else
            goto RxInvalidType;
#endif
          case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r_____________________
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            count = kUnitSize;
            ReadBlock:
            {
                if (length < count)
                    return Report (BufferOverflowError, params, index, start);
                if (count == 0)
                    break;          //< Not sure if this is an error.
                if (start + count >= end) {
                    for (; size - count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    stop = begin - 1;
                    for (; count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    break;
                }
               for (; count > 0; --count) {
                    ui1 = *start;
                   if (++start >= end) start -= size;
                    *ui1_ptr = ui1;
                    ++ui1_ptr;
                }
                break;
            }
          default:
            RxInvalidType:
            {
                printf ("\n!!!Read invalid type %u\n", type);
                return Report (ReadInvalidTypeError, params, index, start);
            }
        }
        std::cout << " |";
    }

#if DEBUG_CHINESE_ROOM
    printf ("| Done reading\n");
    MonoidClear (begin, rx->start, start, stop, end, size);
#endif

    // Convert pointer back to offset
    rx->start = Diff (begin, start);

    return 0;
}

bool IsReadable (Bin* rx) {
    return rx->start != rx->stop;
}

void Print (Bin* rx) {
    if (rx == nullptr) return;
    uint_t size = rx->size;
    PrintLine ('_');
    printf ("| Bin %p: size: %u, start: %u, stop: %u, read: %u\n", rx, size,
            rx->start, rx->stop, rx->read);

    PrintMemory (ExpressionBaseAddress (rx), size);
}

}       //< namespace _
