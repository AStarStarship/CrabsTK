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

const Operation* Result (Expression* expr, Bin::Error error) {
    std::cout << "\n| Expression " << BinErrorString (error) 
              << " Error!";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Bin::Error error,
                         const uint_t* header) {
    std::cout << "\n| Expression " << BinErrorString (error)
              << " Error!";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Bin::Error error, 
                         const uint_t* header, byte offset) {
    std::cout << "\n| Expression " << BinErrorString (error)
              << " Error!";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Bin::Error error, 
                         const uint_t* header, byte offset, byte* address) {
    std::cout << "\n| Expression " << BinErrorString (error)
              << " Error!";
    return reinterpret_cast<const Operation*> (1);
}

const Operation* Result (Expression* expr, Bin::Error error, 
                         const uint_t* header, byte offset, 
                         uintptr_t* address) {
    std::cout << "\n| Expression " << BinErrorString (error)
              << " Error!";
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
                            uint_t stack_size, Operand* root,
                            uintptr_t* unpacked_buffer,
                            uintptr_t unpacked_size) {
    if (buffer == nullptr) {
        return nullptr;
}
    if (buffer_size < Expression::kMinBufferSize) {
        return nullptr;
    }
    if (stack_size < kMinStackSize) {
        stack_size = kMinStackSize;    //< Minimum stack size.
    }
    if (unpacked_buffer == nullptr) {
#if DEBUG_SCRIPT 
        std::cout << "\n| Error: unpacked_buffer was null!";
#endif  //< DEBUG_SCRIPT
    }

    if (root == nullptr) {
#if DEBUG_SCRIPT
        std::cout << "\n| Error: root can't be null.";
#endif  //< DEBUG_SCRIPT
        return nullptr;
    }

    Expression* expr = reinterpret_cast<Expression*> (buffer);

    uint_t total_stack_size = (stack_size - 1) * (2 * sizeof (Operand*));
    // Calculate the size of the Slot and Stack.
    uint_t size = (buffer_size - sizeof (Expression) -
        total_stack_size + 1) >> 1;  // >>1 to divide by 2
    //expr->type         = 0;       //< @todo Delete me
    expr->bout_state   = Bout::LockedState;
    expr->bin_state    = Bin::DisconnectedState;
    expr->stack_count  = 1;
    expr->type   = NIL;
    expr->stack_size   = stack_size;
    expr->num_states   = 0;
    expr->operand      = nullptr;
#if DEBUG_SCRIPT
    printf ("\n| Initializing Stack: "
        "\n| sizeof (Stack): %u"
        "\n| (stack_count * (2 * sizeof (void*))): %u"
        "\n| stack_count: %u buffer_size: %u size: %u",
        sizeof (Expression), (stack_size *
        (2 * sizeof (void*))), stack_size,
        buffer_size, size); 
#endif //< DEBUG_SCRIPT
    expr->bytes_left = 0;
    uint_t offset = sizeof (Expression) + total_stack_size - sizeof (void*),
        bin_offset = sizeof (Bin) + total_stack_size + offset;
    expr->header_size = sizeof (Expression) + 2 * sizeof (void*) * stack_size;
    expr->hash = kLargest16BitPrime;         //< Reset hash to largest 16-bit prime.
    expr->result = nullptr;
    expr->buffer = reinterpret_cast<byte*> (unpacked_buffer);
    expr->buffer_size = unpacked_size;
    expr->buffer_left = unpacked_size;
    expr->header = nullptr;
    expr->header_start = nullptr;
    expr->root = root;
    printf ("expr->op: 0x%p", expr->operand);
    BinInit  (ExpressionBinAddress  (expr), size);
    BoutInit (ExpressionBoutAddress (expr), size);
    return expr;
}

//bool ExpressionIsDynamic (Expression* expr) {
//    return expr->type % 2 == 1;
//}

uintptr_t* ExpressionEndAddress (Expression* expr) {
    //return BinEndAddress (ExpressionBin (expr));
    return 0;
}

const Operation* ExpressionReset (Expression* expr) {
    return 0;
}

const Operation* Push (Expression* expr, Operand* operand) {
    if (expr == nullptr) {
        return Result (expr, Bin::RoomError);
    }
    if (operand == nullptr) {
        return Result (expr, Bin::InvalidOpeartionError);
    }
    const Operation* op = operand->Star ('?', nullptr);
    std::cout << "\n| Pushing " << op->name << " onto the stack";
    uint_t stack_count = expr->stack_count;
    if (stack_count >= expr->stack_size) {
        return Result (expr, Bin::StackOverflowError);
    }
    ExpressionStack (expr)[stack_count - 1] = expr->operand;
    expr->operand = operand;
    expr->stack_count = stack_count + 1;
    ExpressionPrintStack (expr);
    return nullptr;
}

const Operation* Pop (Expression* expr) {
    uint_t stack_count = expr->stack_count;
    if (stack_count == 0) { // This should not happen.
        return Result (expr, Bin::InvalidOpeartionError);
    }
    if (stack_count == 1) {
        // We ever pop off the root.
        ExpressionClose (expr);
        return 0;
    }
#if DEBUG_SCRIPT
    std::cout << "\n| Popping " << OperandName (expr->operand)
              << " off the stack.";
#endif  //< DEBUG_SCRIPT
    expr->operand = ExpressionStack (expr)[stack_count - 2];
#if DEBUG_SCRIPT
    std::cout << "\n| Top of stack is now " << OperandName (expr->operand)
        << ".";
#endif  //< DEBUG_SCRIPT
    expr->stack_count = stack_count - 1;
    ExpressionPrintStack (expr);
    return nullptr;
}

byte ExpressionExitState (Expression* expr) {
    // We are guaranteed expr is not null at this point.
    //if (expr == nullptr) {
    //    return  BinResult (ExpressionBin (expr), Bin::RoomError);
    //}
#if DEBUG_SCRIPT
    std::cout << "\n| Exiting " << BinStateString (expr->bin_state)
              << " state back to the " << BinStateString (expr->last_bin_state) 
              << " state .";
#endif  //< DEBUG_SCRIPT
    byte state = expr->last_bin_state;
    expr->bin_state = state;
    return state;
}

const Operation* ExpressionSetState (Expression* expr, Bin::State state) {
    // We are guaranteed expr is not null at this point.
    //if (expr == nullptr) {
    //    return  BinResult (ExpressionBin (expr), Bin::RoomError);
    //}
    if (state == Bin::LockedState) {
        return BinResult (ExpressionBin (expr), Bin::LockedError);
    }
#if DEBUG_SCRIPT
    std::cout << "\n| Entering " << BinStateString (state)
              << " state:" << state;
#endif  //< DEBUG_SCRIPT
    expr->bin_state = state;
    return nullptr;
}

const Operation* ExpressionEnterState (Expression* expr, 
                                       Bin::State state) {
    // We are guaranteed expr is not null at this point.
    //if (expr == nullptr) {
    //    return  BinResult (ExpressionBin (expr), Bin::RoomError);
    //}
#if DEBUG_SCRIPT
    std::cout << "\n| Entering " << BinStateString (state)
              << " state:" << state;
#endif  //< DEBUG_SCRIPT
    expr->last_bin_state = expr->bin_state;
    expr->bin_state = state;
    return nullptr;
}

byte ExpressionStreamBout (Expression* expr) {
    return BoutStreamByte (ExpressionBout (expr));
}

void ExpressionScan (Expression* expr, Portal* input) {
    if (expr == nullptr) {
        PrintDebug ("a = null");
        return;
    }

    uint_t            size,         //< Size of the ring buffer.
                      space,        //< Space left in the right buffer.
                      length,       //< Length of the ring buffer data.
                      type,         //< Current type.
                      bytes_left,   //< Num bytes left to scan.
                      array_type,   //< The type of array.
                      shift_bits,   //< Num bytes left to scan.
                      bytes_shift;  //< Num of bytes to shift to scan OBJ.
    byte              bin_state,    //< Current bin FSM state.
                      bout_state,   //< Current bin FSM state.
                      b;            //< Current byte being verified.
    hash16_t          hash,         //< Expected hash of the B-Sequence.
                      found_hash;   //< Found B-Sequence hash.
    timestamp_t       timestamp,    //< Last time when the expression ran.
                      delta_t;      //< Time delta between the last timestamp.
    //Expression    * expression;   //< Current Expression.
    const Operation * op;           //< Current Operation.
    Operand         * operand;      //< The operand.
    Bin             * bin;          //< Bin.
    byte            * begin,        //< Beginning of the ring buffer.
                    * end,          //< End of the ring buffer.
                    * start,        //< Start of the ring buffer data.
                    * stop,         //< Stop of the ring buffer data.
                    * write = expr->buffer; //< The write location.
    const Operation * result;       //< The result of the Scan.
    const uint_t    * header = expr->header;
    uintptr_t         buffer_left = expr->buffer_left;
    buffer_left = expr->bytes_left;
    //< Header of the current Operation being verified.

    if (input == nullptr) {
        PrintDebug ("input = null");
        return;
    }

    bin_state = expr->bin_state;    //< This isn't an optimal solution.
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

    begin  = BinBuffer (bin);
    end    = begin + size;
    start  = begin + bin->start;
    stop   = begin + bin->stop;
    space  = SlotSpace (start, stop, size);
    length = size - space;
    printf ("\n| Scanning Expression:0x%p with length:%u", start,
            length);
    for (length; length != 0; --length) {
        b = *start;
        *write++ = b;
        PrintLine ("|", '=');
        std::cout << "\n| " << length << ":\'";
        if (b < 32 || b == 127) {
            std::cout << AsciiString ((AsciiCode)b);
        }
        else {
            std::cout << b;
        }

        std::cout << "\' " << BinStateString (bin_state) << " state";
        PrintLine ("|", '=');

        if (++start >= end) start -= size;
        // Process the rest of the bytes in a loop to reduce setup overhead.
        switch (bin_state) {
            case Bin::AddressState: {
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
                // When verifying an address, there is guaranteed to be an
                // expr->op set. We are just looking for null return values
                // from the Do (byte, Stack*): const Operand* function, 
                // pushing Star(string) on to the Star stack, and looking for
                // the first procedure call.
                //
                if (b == BS) {
                    Pop (expr);
                    break;
                }
                if (b == CR) {
                    expr->bytes_left = 1;
                    ExpressionSetState (expr, Bin::HashState);
                    bin_state = Bin::HashState;
                    break;
                }
                operand = expr->operand;

                std::cout << "\n| Current Operation is \"";
                op = operand->Star ('?', nullptr);
                std::cout << op->name << '\"';

                op = operand->Star (b, nullptr);
                if (op == nullptr) {
                    // Could be an invalid op or a Star Stack push.
                    if (expr->result == nullptr) {
                        PrintDebug ("No op found.");
                        break;
                    }
                    //ExpressionPushScan (a, expr->operand);
                    // Clear the buffer and return.
                    ExpressionClear (expr);
                    return;
                }
                const uint_t* params = op->params;
                uintptr_t num_ops = reinterpret_cast<uintptr_t> (params);
                if (num_ops > kMaxNumParams) {
                    // It's an Operation.
                    // The software implementer pushes the Op on the stack.
                    std::cout << "\n| Found Operation with params ";
                    ParamsPrint (params);
                    if (result = ExpressionScanHeader (expr, params)) {
                        PrintDebug ("Expression::Error reading address.");
                        ExpressionClear (expr);
                    }

                    operand = expr->operand;
                    if (operand == nullptr) {
                        std::cout << "\n| Null operand found!";
                        ExpressionClear (expr);
                        return;
                    }
                    header = op->params;
                    expr->params_left = *header;
                    expr->header = header;      //< +1 to bypass the number of params
                    expr->header_start = header;    //< Used to print current header.
                    ExpressionEnterState (expr, Bin::ArgsState);
                    bin_state = Bin::ArgsState;
                    type = *(++expr->header);   //< Setup to read first type.
#if DEBUG_SCRIPT
                    std::cout << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< DEBUG_SCRIPT
                    write += TypeAlign (write, type);
                    break;
                }
                op = operand->Star (b, expr);
                break;
            }
            case Bin::ArgsState: {
                // In this state, a procedure has been called to scan on a valid
                // operand. This state is responsible for loading the next 
                // header argument and checking for the end of the procedure 
                // call.

                if (expr->params_left-- == 0) {
                    std::cout << "\n| Params successfully scanned.";

                    break;
                }
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT

                // Switch to next state
                if (type <= ADR) {
                    if (type < ADR) {   // Address type.
                        PrintDebug ("\n| Scanning address.");
                        Result (expr, Bin::InvalidTypeError);
                        ExpressionEnterState (expr, Bin::LockedState);
                        bin_state = Bin::LockedState;
                        break;
                    }
                    ExpressionEnterState (expr,
                                            Bin::AddressState);
                    bin_state = Bin::AddressState;
                    break;

                } else if (type == STR) { // UTF-8/ASCII string type.
                    // Read the max number of chars off the header.
                    bytes_left = *(++expr->header);
                    std::cout << "\n| Scanning STR with max length "
                              << bytes_left;
                    ExpressionEnterState (expr, Bin::Utf8State);
                    bin_state = Bin::Utf8State;
                    break;
                } else if (type == ST2) { // UTF-16 string type.
                    PrintDebug ("\n| Scanning ST2.");
                    if (bytes_left == 1) {
                        expr->last_byte = b;
                        break;
                    }
                    // Read the max number of chars off the header.
                    bytes_left = *expr->header++ * 2;
                    ExpressionEnterState (expr,
                                            Bin::Utf16State);
                    bin_state = Bin::Utf16State;
                    break;
                } else if (type == ST4) { // UTF-32 string type.
                    PrintDebug ("\n| Scanning ST4.");
                    // Read the max number of chars off the header.
                    bytes_left = *expr->header++ * 4;
                    ExpressionEnterState (expr, Bin::Utf32State);
                    bin_state = Bin::Utf32State;
                } else if (type < DBL)  { // Plain-old-data type.
                    bytes_left = SizeOf (type);
#if DEBUG_SCRIPT
                    std::cout << "\n| Scanning POD." << " bytes_left:"
                              << bytes_left;
#endif  //< DEBUG_SCRIPT
                    if (bytes_left == 1) {
                        // No need to enter a state because there is only one
                        // byte to parse and we already have the byte loaded.
                        std::cout << "\n| Done scanning without state change for \"" 
                                  << TypeString (type) << '\"';
                        // Setup to read the next type.
                        type = *(++expr->header);
#if DEBUG_SCRIPT
                        std::cout << "\n| Next TType to scan:\'" << TypeString (type)
                            << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< DEBUG_SCRIPT
                        write += TypeAlign (write, type);
                        break;
                    }
                    ExpressionEnterState (expr, Bin::PodState);
                    bin_state = Bin::PodState;
                    break;
                } else if (type < UV8)  { // Varint type.
                    PrintDebug ("\n| Scanning Varint.");
                    bytes_left = SizeOf (type);
                    ExpressionEnterState (expr, Bin::VarintState);
                    bin_state = Bin::VarintState;
                    break;
                } else { // It's a TObject.
                    PrintDebug ("\n| Scanning TObject.");
                    // Multi-dimension arrays are parsed just like any other
                    // TObject.
                    array_type &= 0x3;
                    bytes_left = b;
                    if (array_type == 0) {
                        // We don't need to enter a state here because we
                        // already have the size_bytes. :-)
                        bin_state = Bin::PodState;
                        break;
                    } else if (array_type == 1) {
                        bytes_shift = 0;
                        shift_bits  = 16;
                        ExpressionEnterState (expr, Bin::ObjectState);
                        bin_state = Bin::ObjectState;
                        break;
                    } else if (array_type == 2) {
                        bytes_shift = 0;
                        shift_bits  = 32;
                        ExpressionEnterState (expr, Bin::ObjectState);
                        bin_state = Bin::ObjectState;
                        break;
                    } else {    //< array_type == 3
                        bytes_shift = 0;
                        shift_bits  = 64;
                        ExpressionEnterState (expr, Bin::ObjectState);
                        bin_state = Bin::ObjectState;
                        break;
                    }
                }
            }
            case Bin::Utf8State: {
                if (bytes_left == 0) {
Result (expr, Bin::StringOverflowError,
        const_cast<const uint_t*>(expr->header), 0, start);
break;
                }
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
                // Hash byte.
                // Check if char terminated.
                if (b == 0) {
                    // Check if there is another argument to scan.
                    ExpressionExitState (expr);
                    bin_state = Bin::ArgsState;
                    //< We can't go back from OBJ to POD for String Types.
                    // Setup to read next type.
                    type = *(++expr->header);
#if DEBUG_SCRIPT
                    if (expr->params_left == 0) {
                        ExpressionSetState (expr, Bin::AddressState);
                        bin_state = Bin::AddressState;
                        break;
                    }
                    std::cout << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< DEBUG_SCRIPT
                    write += TypeAlign (write, type);
                    break;
                }
                --bytes_left;
                break;
            }
            case Bin::Utf16State: {
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
                ExpressionExitState (expr);
                break;
            }
            case Bin::Utf32State: {
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
                ExpressionExitState (expr);
                break;
            }
            case Bin::VarintState: {
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
// When verifying a varint, there is a max number of bytes for 
// the type (3, 5, or 9) but the varint may be complete before 
// this number of bytes. We're just basically counting down and 
// looking for an overflow situation.
// Hash byte.

                if (bytes_left == 1) {
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
                        Result (expr, Bin::VarintOverflowError, header,
                                0, start);
                        ExpressionEnterState (expr,
                                              Bin::ErrorState);
                        bin_state = Bin::ErrorState;
                        return;
                    }

                    break;
                }
                if (b > 127) {
                    std::cout << "\n| Don't scanning varint: " << 0;
                    // Setup to read the next type.
                    type = *(++header);
#if DEBUG_SCRIPT
                    std::cout << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< DEBUG_SCRIPT
                    write += TypeAlign (write, type);
                }
                --bytes_left;
                break;
            }
            case Bin::ObjectState: {
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
                if (bytes_shift >= shift_bits) {
                    // Done shifting.
                    std::cout << "\n| Loading object of size:" << bytes_left;
                    ExpressionExitState (expr);
                    ExpressionEnterState (expr, Bin::PodState);
                    bin_state = Bin::PodState;
                    break;
                }
                bytes_left &= ((uint_t)b) << bytes_shift;
                shift_bits += 8;
                break;
            }
            case Bin::HashState: {
                if (expr->bytes_left != 0) {  // One more byte to load.
                    expr->last_byte = b;
                    expr->bytes_left = 0;
                    break;
                }
                found_hash = b;
                found_hash = found_hash << 8;
                found_hash |= expr->last_byte;
                if (hash != found_hash) {
#if DEBUG_SCRIPT
                    printf ("\n | Error: expecting hash:0x%x and found 0x%x.", hash, found_hash);
#endif  //< DEBUG_SCRIPT
                    ExpressionClear (expr);
                    return;
                }
#if DEBUG_SCRIPT
                std::cout << "\n | Success reading hash!";
#endif  //< DEBUG_SCRIPT
                hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
#if DEBUG_SCRIPT
                std::cout << "\n| Resetting hash.\n";
#endif  //< DEBUG_SCRIPT
            }
            case Bin::ErrorState: {
                break;
            }
            case Bin::DisconnectedState: {
                if (b != _::BEL) {
                    ExpressionSetState (expr, Bin::ErrorState);
                    bin_state = Bin::ErrorState;
                } else {
                    ExpressionSetState (expr, Bin::AckState);
                    bin_state = Bin::AckState;
                }
                break;
            }
            case  Bin::AckState: {
                if (b != _::ACK) {
                    ExpressionSetState (expr, Bin::ErrorState);
                    bin_state = Bin::ErrorState;
                } else {
#if DEBUG_SCRIPT
                    std::cout << "\n| Resetting hash.";
#endif  //< DEBUG_SCRIPT
                    hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
                    expr->operand = expr->root;
                    expr->result = nullptr;
                    bin_state = Bin::AddressState;
                    ExpressionSetState (expr, Bin::AddressState);
#if DEBUG_SCRIPT
                    std::cout << "\n| Root scope: \"" 
                              << OperandName (expr->operand) << '\"';
#endif  //< DEBUG_SCRIPT
                }
                break;
            }
            case  Bin::LockedState: {
                PrintDebug ("Locked");
                break;
            }
            default: {
                hash = Hash16 (b, hash);
#if DEBUG_SCRIPT
                printf ("\n| hash:0x%x", hash);
#endif  //< DEBUG_SCRIPT
                // parsing plain-old-data.
                if (bytes_left == 0) {
                    PrintDebug ("... done!");
                    ExpressionExitState (expr);
                    bin_state = expr->bin_state;

                    // Setup to read the next type.
                    type = *(++header);
#if DEBUG_SCRIPT
                    std::cout << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< DEBUG_SCRIPT
                    write += TypeAlign (write, type);
                    break;
                }
                --bytes_left;
                b = input->Pull ();
                PrintDebugHex ("Loading next byte", b);
                hash = Hash16 (b, hash);
                *start = b;
                ++start;
                break;
            }
        }
    }
    expr->hash = hash;
    expr->bytes_left = bytes_left;
    bin->start = Diff (begin, start);
}

bool ExpressionContains (Expression* expr, void* address) {
    if (address < reinterpret_cast<uintptr_t*>(expr))
        return false;
    if (address > ExpressionEndAddress (expr)) return false;
    return true;
}

const Operation* ExpressionScanHeader (Expression* expr, const uint_t* header) {
    if (expr->stack_count >= expr->stack_size) {
        // Handle overflow cleanup:
        return Result (expr, Bin::StackOverflowError, header);
    }

    return 0;
}

const uint_t* ExpressionHeaderStack (Expression* expr) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<byte*>
        (expr) + sizeof (Expression) + expr->stack_count);
}

void ExpressionClose (Expression* expr) {
#if DEBUG_SCRIPT
    std::cout << "\n| Closing expression.";
#endif  //< DEBUG_SCRIPT
    expr->stack_count = 1;
}

void ExpressionCancel (Expression* expr) {
#if DEBUG_SCRIPT
    std::cout << "\n| Canceling expression.";
#endif  //< DEBUG_SCRIPT
    expr->stack_count = 1;
    expr->bin_state = Bin::AddressState;
    //ExpressionPush (expr->root);
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

void ExpressionRingBell (Expression* expr, const char* address) {
    BoutRingBell (ExpressionBout (expr), address);
    
}

void ExpressionAckBack (Expression* expr, const char* address) {
    BoutAckBack (ExpressionBout (expr), address);
}

bool Args (Expression* expr, const uint_t* params, void** args) {
    return BinRead (ExpressionBin (expr), params, args);
}


const Operation* Result (Expression* expr, const uint_t* params, void** args) {
    return BoutWrite (ExpressionBout (expr), params, args);
}

void ExpressionPrintStack (Expression* expr) {
    if (expr == nullptr) {
        return;
    }

    uint_t i,
        stack_count;
    const Operation* op;
    Operand* operand;
    Operand** stack = ExpressionStack (expr);
    stack_count = expr->stack_count;
    std::cout << "\n| Operand stack_count:" << stack_count;

    if (stack_count == 1) {
        std::cout << "\n| Stack Item 1: " << OperandName (expr->root);
        return;
    }
    for (i = 0; i < stack_count - 1; ++i) {
        std::cout << "\n| Stack Item " << i + 1<< ":\"";
        operand = stack[i];
        op = operand->Star ('?', nullptr);
        std::cout << op->name << '\"';
    }
    op = expr->operand->Star ('?', nullptr);
    std::cout << "\n| Stack Item " << i + 1 << ":\"" << op->name << "\"";
}

void ExpressionPrintStateStack (Expression* expr) {
    PrintLine ("|", '-');
    std::cout << "\n| Expression State Stack:    ";
    if (expr == nullptr) {
        printf ("null");
        PrintLine ("|", '-');
        return;
    }
    PrintLine ("|", '-');
}

void ExpressionPrint (Expression* expr) {
    PrintLine ('~');
   std::cout << "\n| Stack:    ";
    if (expr == nullptr) {
        std::cout << "null";
        PrintLine ("|", '~');
        return;
    }
    printf ("0x%p", expr);
    PrintLine ("|", '_');
    //std::cout << "\n| type:          "
    //    << (expr->type == -1)?"interprocess no dynamic memory.":
    //       (expr->type == 0)?"no dynamic memory":
    //       (expr->type == 1)?"dynamic memory":
    //       (expr->type == 2)?"dynamic memory":"Invalid type";

    std::cout << "\n| bytes_left  : " << expr->bytes_left
              << "\n| header_size : " << expr->header_size
              << "\n| stack_count : " << expr->stack_count
              << "\n| stack_size  : " << expr->stack_size
              << "\n| bin_state   : " << BinStateString  (expr->bin_state )
              << "\n| bout_state  : " << BoutStateString (expr->bout_state)
              << "\n| num_states  : " << expr->num_states
              << "\n| header_size : " << expr->header_size;
    PrintLine ("|", '-');
    OperandPrint (expr->operand);
    std::cout << "\n| header: ";
    ParamsPrint (expr->header_start);
    PrintLine ("|", '-');
    ExpressionPrintStack (expr);
    PrintLine ("|", '~');
    //system ("PAUSE");
}
/*
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
}*/

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
