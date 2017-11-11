    /** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_expression.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
#include "../include/args.h"
#include "../include/clock.h"
#include "../include/room.h"
#include "../include/ascii.h"

namespace _ {

const char* ExpressionErrorString (byte state) {
    return 0;
}

const char* ExpressionStateString (Expression::State state) {
    static const char* strings[] = {
        "Disconnected",
        "Scanning address",
        "Scanning char",
        "Scanning args",
        "Scanning POD",
        "Handling error",
        "Scanning varint",
        "Locked" };
    return strings[state];
}

const Operation* Result (Expression* expr, Expression::Error error) {
    std::cout << "\nExpression " << ExpressionErrorString (error) 
              << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Expression::Error error,
                         const uint_t* header) {
    std::cout << "\nExpression " << ExpressionErrorString (error) 
              << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Expression::Error error, 
                         const uint_t* header, byte offset) {
    std::cout << "\nExpression " << ExpressionErrorString (error)
              << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Expression::Error error, 
                         const uint_t* header, byte offset, byte* address) {
    std::cout << "\nExpression " << ExpressionErrorString (error) 
              << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Expression::Error error, 
                         const uint_t* header, byte offset, 
                         uintptr_t* address) {
    std::cout << "\nExpression " << ExpressionErrorString (error)
              << " Error!\n";
    return reinterpret_cast<const Operation*> (1);
}

uintptr_t* ExpressionBinAddress (Expression* expr) {
    if (!expr)
        return nullptr;
    return reinterpret_cast<uintptr_t*>(expr) + expr->header_size;
}

byte* ExpressionBuffer (Expression* expr) {
    byte* ptr = reinterpret_cast<byte*> (expr);
    return ptr + sizeof (Expression);
}

Bin* ExpressionBin (Expression* expr) {
    return reinterpret_cast<Bin*> (ExpressionBinAddress (expr));
}

uintptr_t* ExpressionBoutAddress (Expression* expr) {
    if (!expr)
        return nullptr;
    return reinterpret_cast<uintptr_t*>(expr) + expr->header_size;
}

Bout* ExpressionBout (Expression* expr) {
    return reinterpret_cast<Bout*> (ExpressionBoutAddress (expr));
}

Expression* ExpressionInit (uintptr_t* buffer, uint_t buffer_size,
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
#if DEBUG_SCRIPT
    printf ("\nInitializing Stack:\n"
        "sizeof (Stack): %u\n"
        "(stack_count * (2 * sizeof (void*))): %u\n"
        "stack_count: %u buffer_size: %u size: %u\n"
        "!!! stack_count: %u &stack_count: 0x%p !!!\n",
        sizeof (Expression), (stack_count *
        (2 * sizeof (void*))), stack_count,
        buffer_size, size, stack_count, &stack_count);
#endif //< DEBUG_SCRIPT
    expr->bytes_left = 0;
    uint_t offset = sizeof (Expression) + total_stack_size - sizeof (void*),
        bin_offset = sizeof (Bin) + total_stack_size + offset;
    expr->header_size = sizeof (Expression) + 2 * sizeof (void*) * stack_count;
    expr->header = 0;
    expr->headers = 0;
    printf ("\n\n!!!\nroot: 0x%p\n", root);
    expr->operand = root;
    printf ("expr->op: 0x%p\n", expr->operand);
    BinInit  (ExpressionBinAddress  (expr), size);
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

uintptr_t* ExpressionEndAddress (Expression* expr) {
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

uintptr_t* ExpressionStates (Expression* expr) {
    return reinterpret_cast<uintptr_t*> (expr) + sizeof (Expression);
}

const Operation* ExpressionExitState (Expression* expr) {
    auto count = expr->stack_count;
    if (count == 0)
        return Result (expr, Expression::InvalidOpeartionError);
    expr->bout_state = ExpressionStates (expr)[--count];
    expr->stack_count = count;
    return 0;
}

const Operation* ExpressionEnterState (Expression* expr, 
                                       Expression::State state) {

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

const Operation* ExpressionPushScanHeader (Expression* expr, 
                                           const uint_t* header) {
    if (header == nullptr)
        return Result (expr, Expression::RoomError);
    uint_t verify_count = expr->verify_count,
           stack_size = expr->stack_size;
    expr->type_index = *header++;
    if (verify_count >= stack_size)
        return Result (expr, Expression::StackOverflowError, header);

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
    byte              bout_state,   //< Current bin FSM state.
                      b,            //< Current byte being verified.
                      array_type;   //< The type of array.
    hash16_t          hash;         //< Hash of the ESC being verified.
    timestamp_t       timestamp,    //< Last time when the expression ran.
                      delta_t;      //< Time delta between the last timestamp.
    //Expression    * expression;   //< Current Expression.
    const Operation * op;           //< Current Operation.
    Operand         * operand;      //< The operand.
    const uint_t    * header;       //< Header of the current Operation being verified.
    Bin             * bin;          //< Bin.
    byte            * begin,        //< Beginning of the ring buffer.
                    * end,          //< End of the ring buffer.
                    * start,        //< Start of the ring buffer data.
                    * stop;         //< Stop of the ring buffer data.
    const Operation * result;       //< The result of the Scan.

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
    timestamp = ClockTimestampNow ();
    delta_t = timestamp - expr->last_time;

    if (delta_t <= expr->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch (rare)
            delta_t *= -1;
    }

    begin  = ExpressionBuffer (expr);
    end    = begin + size;
    start  = begin + bin->start;
    stop   = begin + bin->stop;
    space  = SlotSpace (start, stop, size);
    length = size - space + 1;

    printf ("\n\n| Scanning address 0x%p:\n"
            "| bout_state: %s\n"
            "| length: %u\n", start,
    ExpressionStateString ((Expression::State)expr->bout_state), length);
    
    for (; length != 0; --length) {
        b = *start;
        if (++start >= end) start -= size;
        hash = Hash16 (b, hash);
        // Process the rest of the bytes in a loop to reduce setup overhead.
        switch (bout_state) {
            case Expression::Utf8State: {
                PrintDebug ("StringState");

                if (expr->bytes_left == 0) {
                    PrintDebug ("Done parsing char.");
                    Result (expr, Expression::StringOverflowError,
                            const_cast<const uint_t*>(expr->header), 0, start);
                    return;
                }
                // Hash byte.

                // Check if char terminated.
                if (b == 0) {
                    PrintDebug ("char terminated.");
                    // Check if there is another argument to scan.
                    // 
                    ExpressionExitState (expr);
                    return;
                }
                PrintDebug ("b != 0");
                --expr->bytes_left;
                return;
            }
            case Expression::VarintState: {
                // When verifying a varint, there is a max number of bytes for 
                // the type (3, 5, or 9) but the varint may be complete before 
                // this number of bytes. We're just basically counting down and 
                // looking for an overflow situation.

                PrintDebug ("VarintState.");
                // Hash byte.

                if (expr->bytes_left == 1) {
                    PrintDebug ("Checking last byte:");

                    // @warning I am not current saving the offset. I'm not 
                    // sure  what to do here. The header class uses a variadic 
                    // template, I'm kind of tempted to switch to an int 
                    // type for the headers so I can just use a normal 
                    // array bracket initializer. The other option is to 
                    // add 32 to the first byte.

                    if ((b >> 7) != 1) {
                        const uint_t* header = 
                            const_cast<const uint_t*>(expr->header);
                        Result (expr, Expression::VarintOverflowError, header, 
                                0, start);
                        ExpressionEnterState (expr, 
                                              Expression::HandlingErrorState);
                        return;
                    }

                    return;
                }
                --expr->bytes_left;
                return;
            }
            case Expression::AddressState: {
                // When verifying an address, there is guaranteed to be an
                // expr->op set. We are just looking for null return values
                // from the Do (byte, Stack*): const Operand* function, 
                // pushing Star(string) on to the Star stack, and looking for 
                // the first procedure call.

                PrintDebugHex ("| AddressState", b);
                if (b == BSC) {     // Start processing a new ESC.
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
            }
            case Expression::ScanningArgsState: {
                // In this state, a procedure has been called to scan on a valid
                // operand. This state is responsible for loading the next 
                // header argument and checking for the end of the procedure 
                // call.

                PrintDebug ("BoutScanningArgs.");

                operand = expr->operand;
                if (operand == nullptr) {
                    // Push the Room onto the stack.
                    operand = ChineseRoom ();
                    // Check if it is a Star Operation.
                    op = operand->Star (b, nullptr);
                    expr->result = op;
                    if (op == nullptr) {
                        PrintError ("Invalid op");
                        ExpressionEnterState (expr, Expression::LockedState);
                        return;
                    }

                    ExpressionEnterState (expr, Expression::ScanningArgsState);
                    return;
                } else {
                    // There is an operation on the stack.
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

                    array_type = type >> 5;  //< Mask off lower 5 bits.

                    // Switch to next state
                    if (type <= ADR) {
                        if (type < ADR) {   // Address type.
                            Result (expr, Expression::ReadInvalidTypeError);
                            ExpressionEnterState (expr, 
                                                  Expression::LockedState);
                            return;
                        }
                        ExpressionEnterState (expr,
                                              Expression::AddressState);

                    } else if (type == STR) { // UTF-8/ASCII string type.
                        ExpressionEnterState (expr, 
                                              Expression::Utf8State);
                    } else if (type == ST2) { // UTF-16 string type.
                        ExpressionEnterState (expr,
                                              Expression::Utf16State);
                    } else if (type == ST4) { // UTF-32 string type.
                        ExpressionEnterState (expr,
                                              Expression::Utf32State);
                    } else if (type < DBL)  { // Plain-old-data type.
                        expr->bytes_left = SizeOf (type);
                        ExpressionEnterState (expr, 
                                              Expression::PodState);
                    } else if (type < UV8)  { // Varint type.
                        expr->bytes_left = SizeOf (type);
                        ExpressionEnterState (expr, 
                                              Expression::VarintState);
                    } else { // It's a TObject.
                        PrintDebug ("Scanning TObject");
                        array_type = (byte)(type >> 5);
                        // We're scanning so we need to read the size from the 
                        ExpressionEnterState (expr, 
                                              Expression::ObjectState);
                    }
                }
            }
            case Expression::HandlingErrorState: {
                PrintDebug ("HandlingErrorState.");
            }
            case Expression::DisconnectedState: {
                if (b != ascii::BEL) {
                    ExpressionEnterState (expr, Expression::AwaitingAckState);
                } else {

                }

            }
            case  Bout::LockedState: {
                PrintDebug ("Bout::LockedState.");
            }
            default: {
                // parsing plain-old-data.
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
    }
    bin->start = Diff (begin, start);
}

bool ExpressionContains (Expression* expr, void* address) {
    if (address < reinterpret_cast<uintptr_t*>(expr))
        return false;
    if (address > ExpressionEndAddress (expr)) return false;
    return true;
}

const Operation* ExpressionPushHeader (Expression* expr, const uint_t* header) {
    if (expr->stack_count >= expr->stack_size) {
        // Handle overflow cleanup:
        return Result (expr, Expression::StackOverflowError, header);
    }

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

    byte* begin = BinBuffer (bin),
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

bool Args (Expression* expr, const uint_t* params, void** args) {
    return BinRead (ExpressionBin (expr), params, args);
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

Bin* ExpressionInit (uintptr_t* buffer, uint_t size) {
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

uintptr_t* ExpressionBaseAddress (void* ptr, uint_t rx_tx_offset) {
    enum {
        kSlotHeaderSize = sizeof (Bin) + sizeof (uintptr_t) -
        sizeof (Bin) % sizeof (uintptr_t),
        //< Offset to the start of the ring buffer.
    };
    byte* result = reinterpret_cast <byte*>(ptr) + rx_tx_offset + 
                   kSlotHeaderSize;
    return reinterpret_cast<uintptr_t*> (result);
}

byte* ExpressionEndAddress (Bin* bin) {
    return reinterpret_cast<byte*>(bin) + kSlotHeaderSize + bin->size;
}

}       //< namespace _
