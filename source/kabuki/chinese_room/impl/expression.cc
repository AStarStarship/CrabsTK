    /** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/expression.cc
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
#include "../include/slot.h"

namespace _ {

const char* ExpressionErrorString (byte state) {
    return 0;
}

const char* ExpressionStateString (Expression::State state) {
    static const char* strings[] = {
        "Disconnected",
        "Scanning address",
        "Scanning string",
        "Scanning args",
        "Scanning POD",
        "Handling error",
        "Scanning varint",
        "Locked" };
    return strings[state];
}

const Operation* Result (Expression* expr, Expression::Error error) {
    // @todo Write me.
    return 0;
}

const Operation* Result (Expression* expr, Expression::Error error, const uint_t* header) {
    // @todo Write me.
    return 0;
}

const Operation* Result (Expression* expr, Expression::Error error, const uint_t* header,
    byte offset) {
    // @todo Write me.
    return 0;
}

const Operation* Result (Expression* expr, Expression::Error error, const uint_t* header,
    byte offset, byte* address) {
    // @todo Write me.
    return 0;
}

const Operation* Result (Expression* expr, Expression::Error error, 
                                   const uint_t* header, byte offset, void* address)
{
    return 0;
}

byte* ExpressionBinAddress (Expression* expr) {
    if (!expr)
        return nullptr;
    return reinterpret_cast<byte*>(expr) + expr->header_size;
}

Bin* ExpressionBin (Expression* expr) {
    return reinterpret_cast<Bin*> (ExpressionBinAddress (expr));
}

byte* ExpressionBoutAddress (Expression* expr) {
    if (!expr)
        return nullptr;
    return reinterpret_cast<byte*>(expr) + expr->header_size;
}

Bout* ExpressionBout (Expression* expr) {
    return reinterpret_cast<Bout*> (ExpressionBoutAddress (expr));
}

Expression* ExpressionInit (byte* buffer, uint_t buffer_size,
                            uint_t stack_count, Operand* root) {
    if (buffer == nullptr)
        return nullptr;
    if (buffer_size < Expression::kMinBufferSize)
        return nullptr;
    if (stack_count == 0)
        stack_count = 1;    //< Minimum stack size.

    Expression* expr = reinterpret_cast<Expression*> (buffer);

    uint_t total_stack_size = (stack_count - 1) * (2 * sizeof (void*));
    // Calculate the size of the Slot and Stack.
    uint_t size = (buffer_size - sizeof (Expression) -
        total_stack_size + 1) >> 1;  // >>1 to divide by 2
    expr->type = 0;
    expr->bout_state = Bout::LockedState;
    expr->bin_state = 0;
    expr->stack_count = 0;
    expr->verify_count = 0;
    expr->stack_size = stack_count;
    expr->num_states = 0;
    expr->operand = nullptr;
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
        bin_offset = sizeof (Bin) + total_stack_size + offset;
    expr->header_size = sizeof (Expression) + 2 * sizeof (void*) * stack_count;
    expr->header = 0;
    expr->headers = 0;
    printf ("\n\n!!!\nroot: 0x%p\n", root);
    expr->operand = root;
    printf ("expr->op: 0x%p\n", expr->operand);
    BinInit (ExpressionBinAddress (expr), size);
    BoutInit (ExpressionBoutAddress (expr), size);
    return expr;
}

Operand** ExpressionStack (Expression* expr) {
    auto a = reinterpret_cast<byte*> (expr) + sizeof (Expression) +
             expr->stack_count * expr->stack_size * sizeof (const uint_t*);
    return reinterpret_cast<Operand**> (expr);
}

bool ExpressionIsDynamic (Expression* expr) {
    return expr->type % 2 == 1;
}

byte* ExpressionEndAddress (Expression* expr) {
    //return BinEndAddress (ExpressionBin (expr));
    return 0;
}

const Operation* ExpressionReset (Expression* expr) {
    return 0;
}

const Operation* Push (Expression* expr, Operand* operand) {
    if (expr == nullptr)
    {
        if (operand == nullptr)
            return nullptr;
        return operand->Star (0, nullptr);  //< Return d's header.
    }
    if (operand == nullptr)
        return Result (expr, Expression::InvalidOpeartionError);
    if (expr->stack_count >= expr->stack_size)
        return Result (expr, Expression::InvalidOpeartionError);
    ExpressionStack (expr)[expr->stack_count++] = operand;
    return 0;
}

const Operation* Pop (Expression* expr) {
    if (expr->stack_count == 0)
        return Result (expr, Expression::InvalidOpeartionError);
    expr->operand = ExpressionStack (expr)[--expr->stack_count];
    return 0;
}

byte* ExpressionStates (Expression* expr) {
    return reinterpret_cast<byte*> (expr) + sizeof (Expression);
}

const Operation* ExpressionExitState (Expression* expr) {
    auto count = expr->stack_count;
    if (count == 0)
        return Result (expr, Expression::InvalidOpeartionError);
    expr->bout_state = ExpressionStates (expr)[--count];
    expr->stack_count = count;
    return 0;
}

const Operation* ExpressionEnterState (Expression* expr, Expression::State state) {

    if (state == Bout::LockedState) {
        return Result (expr, Expression::LockedStateError);
    }
    auto count = expr->stack_count;
    if (count >= expr->stack_size)
        return Result (expr, Expression::StackOverflowError);
    ExpressionStates (expr)[count] = expr->bout_state;
    count = expr->stack_count + 1;
    expr->bout_state = state;
    return 0;
}

const Operation* ExpressionPushScanHeader (Expression* expr, const uint_t* header) {
    if (header == nullptr)
        return Result (expr, Expression::RoomError);
    uint_t verify_count = expr->verify_count,
        stack_size = expr->stack_size;
    expr->type_index = *header++;
    if (verify_count >= stack_size)
        return Result (expr, Expression::StackOverflowError, header, 0, ExpressionBout (expr));

    // Move the current header to the scan stack
    const uint_t* current_header = const_cast<const uint_t*> (expr->header);
    expr->header = header;
    const uint_t** headers = (const uint_t**)&expr->headers;
    headers[stack_size] = current_header;
    expr->verify_count = verify_count + 1;
    return 0;
}

const Operation* ExpressionPushScanHeader (Expression* expr,
    volatile const uint_t* header) {
    const uint_t** headers;
    uint_t verify_count = expr->verify_count;
    if (verify_count >= expr->stack_size)
        return Result (expr, Expression::StackOverflowError);

    headers = (const uint_t**)expr->headers;
    ExpressionExitState (expr);
    expr->header = *headers;
    verify_count = verify_count;
    return 0;
}

const Operation* ExpressionPopScanHeader (Expression* expr) {
    uint_t verify_count = expr->verify_count;
    if (verify_count == 0)
        return Result (expr, Expression::InvalidOpeartionError);

    verify_count = verify_count - 1;
    return 0;
}

void ExpressionScanNextType (Expression* expr) {
    uint_t* header = const_cast<uint_t*> (expr->header);
    if (header == nullptr) {
        ExpressionEnterState (expr, Expression::ScanningArgsState);
        return;
    }

    uint_t type = *header;
    if (type == NIL) {  // Done scanning args.
        ExpressionPopScanHeader (expr);
        return;
    }
    ++header;
    expr->header = header;
    expr->bout_state = expr->last_rx_state;
    //type = *header;
}

byte ExpressionStreamBout (Expression* expr) {
    return BoutStreamByte (ExpressionBout (expr));
}

void ExpressionScan (Expression* expr, Portal* input) {
    uint_t            size,         //< Size of the ring buffer.
                      space,        //< Space left in the right buffer.
                      length,       //< Length of the ring buffer data.
                      type;         //< Current type.
    byte              bout_state,     //< Current bin FSM state.
                      b;            //< Current byte being verified.
    hash16_t          hash;         //< Hash of the ESC being verified.
    timestamp_t       timestamp,    //< Last time when the expression ran.
                      delta_t;      //< Time delta between the last timestamp.
//    Expression      * expression;   //< Current Expression.
    const Operation * op;           //< Current Operation.
    const uint_t        * header;       //< Header of the current Operation being verified.
    Bin             * bin;          //< Bin.
    byte            * begin,        //< Beginning of the ring buffer.
                    * end,          //< End of the ring buffer.
                    * start,        //< Start of the ring buffer data.
                    * stop;         //< Stop of the ring buffer data.
    const Operation * result;       //< The result of the Scan.
    Operand         * operand;      //< The operand.

    if (expr == nullptr) {
        PrintDebug ("a = null");
        return;
    }
    if (input == nullptr) {
        PrintDebug ("input = null");
        return;
    }

    bout_state = expr->bout_state;
    bin = ExpressionBin (expr);
    size = bin->size;
    hash = expr->hash;
    timestamp = TimestampNow ();
    delta_t = timestamp - expr->last_time;

    if (delta_t <= expr->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch (rare)
            delta_t *= -1;
    }

    begin  = &ExpressionBin (expr)->buffer;
    end    = begin + size;
    start  = begin + bin->start;
    stop   = begin + bin->stop;
    space  = SlotSpace (start, stop, size);
    length = size - space + 1;

    printf ("\n\n| Scanning address 0x%p:\n| bout_state: %s\n| length: %u\n", start,
        ExpressionStateString ((Expression::State)expr->bout_state), length);

    // Manually load first byte:
    b = input->Pull ();
    //b = SlotStreamByte (bout);
    hash = Hash16 (b, hash);
    *start = b;
    ++start;
    while (input->Length ()) {
        // Process the rest of the bytes in a loop to reduce setup overhead.
        if (bout_state == Expression::ScanningStringState) {
            PrintDebug ("ScanningStringState");

            if (expr->bytes_left == 0) {
                PrintDebug ("Done parsing string.");
                Result (expr, Expression::StringOverflowError,
                        const_cast<const uint_t*>(expr->header), 0, start);
                return;
            }
            // Hash byte.

            // Check if string terminated.
            if (b == 0) {
                PrintDebug ("string terminated.");
                // Check if there is another argument to scan.
                // 
                ExpressionExitState (expr);
                return;
            }
            PrintDebug ("b != 0");
            --expr->bytes_left;
            return;
        } else if (bout_state == Expression::ScanningVarintState) {
            // When verifying a varint, there is a max number of bytes for the
            // type (3, 5, or 9) but the varint may be complete before this
            // number of bytes. We're just basically counting down and looking
            // for an overflow situation.

            PrintDebug ("ScanningVarintState.");
            // Hash byte.

            if (expr->bytes_left == 1) {
                PrintDebug ("Checking last byte:");

                // @warning I am not current saving the offset. I'm not sure 
                //          what to do here. The header class uses a variadic 
                //          template, I'm kind of tempted to switch to an int 
                //          type for the headers so I can just use a normal 
                //          array bracket initializer. The other option is to 
                //          add 32 to the first byte.

                if ((b >> 7) != 1) {
                    const uint_t* header = const_cast<const uint_t*>(expr->header);
                    Result (expr, Expression::VarintOverflowError, header, 0, start);
                    ExpressionEnterState (expr, Expression::HandlingErrorState);
                    return;
                }

                return;
            }
            --expr->bytes_left;
            return;
        } else if (bout_state == Expression::ScanningAddressState) {
            // When verifying an address, there is guaranteed to be an
            // expr->op set. We are just looking for null return values
            // from the Do (byte, Stack*): const Operand* function, 
            // pushing Star(s) on to the Star stack, and looking for the 
            // first procedure call.

            PrintDebugHex ("| ScanningAddressState", b);
            if (b == ESC) {     // Start processing a new ESC.
                PrintDebug ("Start of ESC:");
                ++expr->header;
                ExpressionPushScanHeader (expr, expr->header);
                return;
            }

            operand = expr->operand;
            expr->result = nullptr;
            op = operand->Star (b, nullptr);
            if (op == nullptr) {
                // Could be an invalid op or a Star Stack push.
                if (expr->result == nullptr) {
                    PrintDebug ("No op found.");
                    return;
                }
                //ExpressionPushScan (a, expr->operand);
            }
            if (result = ExpressionPushScanHeader (expr, op->params)) {
                PrintDebug ("Expression::Error reading address.");
                return;
            }
            ExpressionEnterState (expr, Expression::ScanningArgsState);
            return;
        } else if (bout_state == Expression::ScanningArgsState) {
            // In this state, a procedure has been called to scan on a valid
            // operand. This state is responsible for loading the next header
            // argument and checking for the end of the procedure call.

            PrintDebug ("BoutScanningArgs.");

            operand = expr->operand;
            if (operand == nullptr) {
                // Check if it is a Procedure Call or Star.
                operand = expr->operand;
                expr->result = nullptr;
                op = operand->Star (b, nullptr);
                operand = expr->result;
                if (!operand) {
                    if (op == nullptr) {
                        PrintError ("Invalid op");
                        ExpressionEnterState (expr, Expression::LockedState);
                        return;
                    }
                    // Else it was a function call.
                    ExpressionEnterState (expr, Expression::ScanningArgsState);
                    return;
                }
            } else {
                // Verify byte as address.
                header = const_cast<const uint_t*> (expr->header);
                if (!expr->header) {
                    return;
                }
                if (expr->type_index == 0) {
                    PrintDebug ("Procedure verified.");
                    ExpressionExitState (expr);
                }
                // Get next type.
                type = *header;

                PrintDebug (TypeString (type));

                // Word-align the start of the buffer we're reading from.
                start += TypeAlign (start, type);

                // Switch to next state
                if (type <= SOH) {
                    if (type < SOH) {
                        Result (expr, Expression::ReadInvalidTypeError);
                        ExpressionEnterState (expr, Expression::LockedState);
                    } else {
                        ExpressionEnterState (expr, Expression::ScanningAddressState);
                    }
                } else if (type == STX) {   // String type.
                    ExpressionEnterState (expr, Expression::ScanningStringState);
                } else if (type < DBL) {   // Plain-old-data types.
                    expr->bytes_left = SizeOf (type);
                    ExpressionEnterState (expr, Expression::ScanningPodState);
                } else if (type < UV8) {   // Varint types
                    expr->bytes_left = SizeOf (type);
                    ExpressionEnterState (expr, Expression::ScanningVarintState);
                } else if (type <= AR8) {

                } else if (type == ESC) {

                } else if (type <= FS) {

                } else if (type > US) {
                    Result (expr, Expression::RoomError);
                } else {    // It's a US
                    PrintDebug ("Scanning Unit");
                    expr->bytes_left = kUnitSize;
                    ExpressionEnterState (expr, Expression::ScanningPodState);
                }

            }
        } else if (bout_state == Expression::HandlingErrorState) {
            PrintDebug ("HandlingErrorState.");

        } else if (bout_state >= Bout::LockedState) {
            PrintDebug ("Bout::LockedState.");
        } else {    // parsing plain-old-data.
            if (expr->bytes_left-- == 0) {
                PrintDebug ("Done verifying POD type.");
                ExpressionScanNextType (expr);
            } else {
                b = input->Pull ();
                PrintDebugHex ("Loading next byte", b);
                expr->hash = Hash16 (b, hash);
                *start = b;
                ++start;
            }
        }
    }
    bin->start = Diff (begin, start);
}

bool ExpressionContains (Expression* expr, void* address) {
    if (address < reinterpret_cast<byte*>(expr))
        return false;
    if (address > ExpressionEndAddress (expr)) return false;
    return true;
}

const Operation* ExpressionPushHeader (Expression* expr, const uint_t* header) {
    if (expr->stack_count >= expr->stack_size) {
        // Handle overflow cleanup:
        return Result (expr, Expression::StackOverflowError, header);
    }

    //if (dc == nullptr) return noDevceSelectedError ();

    return 0;
}

const uint_t* ExpressionHeaderStack (Expression* expr) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<byte*>
        (expr) + sizeof (Expression) + expr->stack_count);
}

void ExpressionClose (Expression* expr) {
    PrintDebug ("[FF]");
}

void ExpressionCancel (Expression* expr) {
    PrintDebug ("[CAN]");
    //stopAddress = txOpen;
}

void ExpressionClear (Expression* expr) {
    // Erase the buffer by writing zeros to it.

    Bin* bin = ExpressionBin (expr);
    uint_t size = bin->size;

    byte* begin = &bin->buffer,
        * end   = begin + bin->size,
        * start = begin + bin->start,
        * stop  = begin + bin->stop;

    uint_t buffer_space = SlotSpace (start, stop, size);

    if (start == stop) return; //< Nothing to do.
    if (start > stop) {
        memset (start, 0, end - start);
        memset (begin, 0, start - begin);
        return;
    }
    memset (start, 0, stop - start);
    bin->start = Diff (expr, begin);
    bin->stop  = Diff (expr, start + 1);
}

const Operation* Result (Expression* expr, const uint_t* params, void** args) {
    return BoutWrite (ExpressionBout (expr), params, args);
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
              << "\n| header_size:   " << expr->header_size
              << "\n| stack_count:   " << expr->stack_count
              << "\n| stack_size:    " << expr->stack_size
              << "\n| verify_count:  " << expr->verify_count
              << "\n| num_states:    " << expr->num_states;
    PrintLine ("|", '-');
    OperandPrint (expr->operand);
    std::cout << "| header: ";
    ParamsPrint (const_cast<const uint_t*>(expr->header));
    std::cout << "| Scan Stack: " << expr->verify_count;
    const uint_t** headers = (const uint_t**)expr->headers;
    if (headers == nullptr) {
        std::cout << " null";
    } else {
        for (uint_t i = 0; i < expr->stack_count; ++i) {
            std::cout << "| " << i << ": ";
            ParamsPrint (headers[i]);
        }
    }
    PrintLine ("|", '_');
    //system ("PAUSE");
}

Bin* ExpressionInit (byte* buffer, uint_t size) {
    if (buffer == nullptr)
        return nullptr;
    Bin* bin = reinterpret_cast<Bin*>(buffer);
    bin->size = size - sizeof (Bin);
    bin->start = 0;
    bin->stop = 0;
    bin->read = 0;
    return bin;
}

Bin* ExpressionInit (Bin* bin, uint_t size) {
    if (bin == nullptr)
        return nullptr;
    bin->size = size - sizeof (Bin);
    bin->start = 0;
    bin->stop = 0;
    bin->read = 0;
    return bin;
}

/** Gets the start of the Bin ring buffer. 
byte* BBaseAddress (void* ptr) {
    return reinterpret_cast <byte*>(ptr) + kSlotHeaderSize;
}*/

byte* ExpressionBaseAddress (Bin* bin) {
    return reinterpret_cast <byte*>(bin) + kSlotHeaderSize;
}

uint_t ExpressionSpace (Bin* bin) {
    if (bin == nullptr)
        return ~0;

    byte* base = ExpressionBaseAddress (bin);
    return SlotSpace (base + bin->start, base + bin->stop, bin->size);
}

byte* ExpressionBaseAddress (void* ptr, uint_t rx_tx_offset) {
    enum {
        kSlotHeaderSize = sizeof (Bin) + sizeof (uintptr_t) -
        sizeof (Bin) % sizeof (uintptr_t),
        //< Offset to the start of the ring buffer.
    };

    return reinterpret_cast <byte*>(ptr) + rx_tx_offset + kSlotHeaderSize;
}

byte* ExpressionEndAddress (Bin* bin) {
    return reinterpret_cast<byte*>(bin) + kSlotHeaderSize + bin->size;
}

const Operation* ExpressionRead (Expression* expr, const uint_t* params, void** args) {
    if (expr == nullptr)
        Result (expr, Expression::RoomError);
    Bin* bin = ExpressionBin (expr);
    if (bin == nullptr)     //< Is this a double check?
        Result (expr, Expression::RoomError);
    if (params == nullptr)
        Result (expr, Expression::RoomError);
    if (args == nullptr)
        Result (expr, Expression::RoomError);

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

    size = bin->size;

    byte* begin = ExpressionBaseAddress (bin), //< The beginning of the buffer.
        * end   = begin + size,             //< The end of the buffer.
        * start = begin + bin->start,        //< The start of the data.
        * stop  = begin + bin->stop;         //< The stop of the data.
    const uint_t* param = params + 1;       //< The current param.

    length = SlotLength (start, stop, size);

#if DEBUG_CHINESE_ROOM
    std::cout << "\n\n| Reading Bin: \n";
    //ParamsPrint (params);
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
              goto BoutInvalidType;
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
                    return Result (expr, Expression::BufferUnderflowError, params, index,
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
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
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
            goto BoutInvalidType;
#endif
          case SI2:     //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s____________________
          case UI2:
          case HLF:
#if USING_2_BYTE_TYPES
            Read2ByteType:
            // Word-align
            offset = TypeAlign2 (start);
            if (length < offset + 2)
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
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
            goto BoutInvalidType;
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
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
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
            goto BoutInvalidType;
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
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
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
            goto BoutInvalidType;
#endif
          case SV2:     //< _R_e_a_d__V_a_r_i_n_t__2____________________________
          case UV2:
#if USING_VARINT2
            goto Read2ByteType;
#else
            goto BoutInvalidType;
#endif
          case SV4:     //< _R_e_a_d__V_a_r_i_n_t__4____________________________
          case UV4:
#if USING_VARINT4
              goto Read4ByteType;
#else
            goto BoutInvalidType;
#endif
          case SV8:     //< _R_e_a_d__V_a_r_i_n_t__8____________________________
          case UV8:
#if USING_VARINT8
            goto Read8ByteType;
#else
            goto BoutInvalidType;
#endif
          case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1________________________________
#if USING_AR1
            // Load next pointer and increment args.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = *param++;

            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto BoutInvalidType;
#endif
          case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2________________________________
#if USING_AR2
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = *param++; //< Get type from header
              
            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto BoutInvalidType;
#endif
          case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4________________________________
#if USING_AR4
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = *param++;   //< Read type

            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
            goto BoutInvalidType;
#endif
          case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8________________________________
#if USING_AR8
            // Load the pointer to the destination.
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            if (ui1_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = *param++;   //< Read Type
              
            // Word-align
            offset = TypeAlign (start, count);
            if (length < offset)
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            count *= SizeOf (*param++);
            goto ReadBlock;
#else
              goto BoutInvalidType;
#endif
          case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e__________________
            // I'm not sure exactly how this should work. I can't do recursion
            // because of embedded limitations.
            break;
          case FS: //< _R_e_a_d__B_o_o_k_8_____________________________________
            // Word-align
              offset = TypeAlign8 (start);
              if (length < offset + 128)
                  return Result (expr, Expression::BufferUnderflowError, params, index, start);
              length -= offset;
              start += offset;
              if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
            if (ui8_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = (uint_t)*ui8_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui8_ptr + 1);
            goto ReadBlock;
#if USING_AR8
#else
            goto BoutInvalidType;
#endif
          case GS: //< _R_e_a_d__B_o_o_k_4_______________________________________
            // Word-align
              offset = TypeAlign4 (start);
              if (length < offset + 64)
                  return Result (expr, Expression::BufferUnderflowError, params, index, start);
              length -= offset;
              start += offset;
              if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
            if (ui4_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = (uint_t)*ui4_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui4_ptr + 1);
            goto ReadBlock;
#if USING_BK4
#else
            goto BoutInvalidType;
#endif
          case RS: //< _R_e_a_d__B_o_o_k_2_______________________________________
            // Word-align
            offset = TypeAlign2 (start);
            if (length < offset + 32)
                return Result (expr, Expression::BufferUnderflowError, params, index, start);
            length -= offset;
            start += offset;
            if (start >= end) start -= size;

            // Load the pointer to the destination.
            ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
            if (ui2_ptr == nullptr)
                return Result (expr, Expression::RoomError, params, index, start);
            count = (uint_t)*ui2_ptr;
            ui1_ptr = reinterpret_cast<byte*> (ui2_ptr + 1);
            goto ReadBlock;
#if USING_BK2
#else
            goto BoutInvalidType;
#endif
          case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r_____________________
            ui1_ptr = reinterpret_cast<byte*> (args[index]);
            count = kUnitSize;
            ReadBlock:
            {
                if (length < count)
                    return Result (expr, Expression::BufferOverflowError, params, index, start);
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
            BoutInvalidType:
            {
                printf ("\n!!!Read invalid type %u\n", type);
                return Result (expr, Expression::ReadInvalidTypeError, params, index, start);
            }
        }
        std::cout << " |";
    }

#if DEBUG_CHINESE_ROOM
    printf ("| Done reading\n");
    SlotClear (begin, bin->start, start, stop, end, size);
#endif

    // Convert pointer back to offset
    bin->start = Diff (begin, start);

    return 0;
}

bool IsReadable (Bin* bin) {
    return bin->start != bin->stop;
}

const Operation* ExpressionWrite (Expression* expr, const uint_t* params, void** args) {
    return 0;
}

}       //< namespace _
