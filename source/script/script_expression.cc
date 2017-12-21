    /** Kabuki Toolkit
        @version 0.x
        @file    ~/source/script/script_expression.cc
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
#include "expression.h"
#include "ascii.h"
#include "clock.h"
#include "params.h"
#include "args.h"
#include "text.h"
#include "hash.h"

using namespace std;

namespace _ {

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Expression* expr,
                                Error error) {
#if SCRIPT_DEBUG
    std::cout << "\n| Expression " << ErrorString (error)
              << " Error!";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Expression* expr,
                                Error error,
                                const uint_t* header) {
#if SCRIPT_DEBUG
    std::cout << "\n| Expression " << ErrorString (error)
              << " Error!";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Expression* expr,
                                Error error,
                                const uint_t* header,
                                byte offset) {
#if SCRIPT_DEBUG
    std::cout << "\n| Expression " << ErrorString (error)
              << " Error!";
#endif  //< SCRIPT_DEBUG
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Expression* expr,
                                Error error,
                                const uint_t* header,
                                byte offset,
                                byte* address) {
#if SCRIPT_DEBUG
    std::cout << "\n| Expression " << ErrorString (error)
              << " Error!";
#endif  //< SCRIPT_DEBUG
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
#if SCRIPT_DEBUG
    Text text;
#endif  //< SCRIPT_DEBUG
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
        std::cout << "\n| Error: unpacked_buffer was null!";
    }

    if (root == nullptr) {
#if SCRIPT_DEBUG
        std::cout << "\n| Error: root can't be null.";
#endif  //< SCRIPT_DEBUG
        return nullptr;
    }

    Expression* expr = reinterpret_cast<Expression*> (buffer);

    uint_t total_stack_size = (stack_size - 1) * (2 * sizeof (Operand*));
    // Calculate the size of the Slot and Stack.
    uint_t size = (buffer_size - sizeof (Expression) -
        total_stack_size + 1) >> 1;  // >>1 to divide by 2
    expr->bout_state   = Bout::LockedState;
    expr->bin_state    = Bin::DisconnectedState;
    expr->stack_count  = 1;
    expr->type   = NIL;
    expr->stack_size   = stack_size;
    expr->num_states   = 0;
    expr->operand      = nullptr;
#if SCRIPT_DEBUG
    text <<"\n| Initializing Stack with size:" << stack_size 
         << " buffer_size:" << buffer_size << " size:" << size << text.COut ();
#endif //< SCRIPT_DEBUG
    expr->bytes_left = 0;
    //uint_t offset = sizeof (Expression) + total_stack_size - sizeof (void*);
        //bin_offset = sizeof (Bin) + total_stack_size + offset;
    expr->header_size = sizeof (Expression) + 2 * sizeof (void*) * stack_size;
    expr->hash = kLargest16BitPrime;         //< Reset hash to largest 16-bit prime.
    expr->result = nullptr;
    expr->buffer = reinterpret_cast<byte*> (unpacked_buffer);
    expr->buffer_size = unpacked_size;
    expr->buffer_left = unpacked_size;
    expr->header = nullptr;
    expr->header_start = nullptr;
    expr->root = root;
#if SCRIPT_DEBUG
    Print (text << "expr->op:0x" << text.Pointer (expr->operand));
#endif //< SCRIPT_DEBUG
    BinInit  (ExpressionBinAddress  (expr), size);
    BoutInit (ExpressionBoutAddress (expr), size);
    return expr;
}

//bool ExpressionIsDynamic (Expression* expr) {
//    return expr->type % 2 == 1;
//}

byte* ExpressionEndAddress (Expression* expr) {
    return BinEndAddress (ExpressionBin (expr));
}

const Operation* ExpressionReset (Expression* expr) {
    return 0;
}

const Operation* Push (Expression* expr, Operand* operand) {
    if (!expr) {
        return ErrorReport (expr, kErrorImplementation);
    }
    if (!operand) {
        return ErrorReport (expr, kErrorInvalidOperand);
    }
#if SCRIPT_DEBUG
    std::cout << "\n| Pushing " << operand->Star ('?', nullptr)->name 
         << " onto the stack";
#endif  //< SCRIPT_DEBUG
    uint_t stack_count = expr->stack_count;
    if (stack_count >= expr->stack_size) {
        return ErrorReport (expr, kErrorStackOverflow);
    }
    ExpressionStack (expr)[stack_count - 1] = expr->operand;
    expr->operand = operand;
    expr->stack_count = stack_count + 1;
#if SCRIPT_DEBUG
    Text text = ExpressionPrintStack (expr, text).COut ();
#endif  //< SCRIPT_DEBUG
    return nullptr;
}

const Operation* Pop (Expression* expr) {
    uint_t stack_count = expr->stack_count;
    if (stack_count == 0) { // This should not happen.
        return ErrorReport (expr, kErrorInvalidOperand);
    }
    if (stack_count == 1) {
        // We ever pop off the root.
        ExpressionClose (expr);
        return 0;
    }
#if SCRIPT_DEBUG
    std::cout << "\n| Popping " << OperandName (expr->operand)
         << " off the stack.";
#endif  //< SCRIPT_DEBUG
    expr->operand = ExpressionStack (expr)[stack_count - 2];
    expr->stack_count = stack_count - 1;
#if SCRIPT_DEBUG
    Text text;
    std::cout <<"\n| Top of stack is now " << OperandName (expr->operand)
         << "." << ExpressionPrintStack (expr, text);
#endif  //< SCRIPT_DEBUG
    return nullptr;
}

byte ExpressionExitState (Expression* expr) {
    // We are guaranteed expr is not null at this point.
    //if (!expr) {
    //    return  ErrorReport (ExpressionBin (expr), kErrorImplementation);
    //}
#if SCRIPT_DEBUG
    std::cout << "\n| Exiting " << BinState ()[expr->bin_state]
              << " state back to the " << BinState ()[expr->last_bin_state]
              << " state .";
#endif  //< SCRIPT_DEBUG
    byte state = expr->last_bin_state;
    expr->bin_state = state;
    return state;
}

const Operation* ExpressionSetState (Expression* expr, Bin::State state) {
    // We are guaranteed expr is not null at this point.
    //if (!expr) {
    //    return  ErrorReport (ExpressionBin (expr), kErrorImplementation);
    //}
    if (state == Bin::LockedState) {
        return ErrorReport (expr, kErrorObjectLocked);
    }
#if SCRIPT_DEBUG
    std::cout << "\n| Entering " << BinState ()[state]
              << " state:" << state;
#endif  //< SCRIPT_DEBUG
    expr->bin_state = state;
    return nullptr;
}

const Operation* ExpressionEnterState (Expression* expr, 
                                       Bin::State state) {
    // We are guaranteed expr is not null at this point.
    //if (!expr) {
    //    return  ErrorReport (ExpressionBin (expr), kErrorImplementation);
    //}
#if SCRIPT_DEBUG
    std::cout << "\n| Entering " << BinState ()[state]
              << " state:" << state;
#endif  //< SCRIPT_DEBUG
    expr->last_bin_state = expr->bin_state;
    expr->bin_state = state;
    return nullptr;
}

byte ExpressionStreamBout (Expression* expr) {
    return BoutStreamByte (ExpressionBout (expr));
}

const Operation* ExpressionScan (Expression* expr) {
    if (!expr) {
        return ErrorReport (expr, kErrorImplementation);
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
                      //bout_state,   //< Current bin FSM state.
                      b;            //< Current byte being verified.
    hash16_t          hash,         //< Expected hash of the B-Sequence.
                      found_hash;   //< Found B-Sequence hash.
    time_us_t       timestamp,    //< Last time when the expression ran.
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
    //uintptr_t         buffer_left = expr->buffer_left;
    //< Header of the current Operation being verified.

//    if (input == nullptr) {
//#if SCRIPT_DEBUG
//        PrintDebug ("input = null");
//#endif  //< SCRIPT_DEBUG
//        return;
//    }

    bin_state = expr->bin_state;    //< This isn't an optimal solution.
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
#if SCRIPT_DEBUG
    Text text;
    std::cout << "\n| Scanning Expression:0x" << text.Pointer (start)
         << " with length:" << length << text.Clear ();
#endif  //< SCRIPT_DEBUG
    for (; length != 0; --length) {
        b = *start;
        *write++ = b;
#if SCRIPT_DEBUG
        text << text.Line ('=')
            << "\n| " << length << ":\'" << text.Clear ();
        if (b < 32 || b == 127) {
            text << AsciiText ((AsciiCode)b);
        }
        else {
            text << b;
        }

        std::cout << "\' " << BinState ()[bin_state] << " state"
             << Text ().Line () << text.Clear ();
#endif  //< SCRIPT_DEBUG

        if (++start >= end) start -= size;
        // Process the rest of the bytes in a loop to reduce setup overhead.
        switch (bin_state) {
            case Bin::AddressState: {
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
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
                    Pop (expr);
                    expr->bytes_left = 1;
                    ExpressionSetState (expr, Bin::HashState);
                    bin_state = Bin::HashState;
                    break;
                }
                operand = expr->operand;

                op = operand->Star ('?', nullptr);
#if SCRIPT_DEBUG
                text << "\n| Current Operation is \"" << op->name << '\"';
#endif  //< SCRIPT_DEBUG

                op = operand->Star (b, nullptr);
                if (op == nullptr) {
                    // Could be an invalid op or a Star Stack push.
                    // I'm not sure what I was doing here with the no-op, maybe 
                    // trying to save some CPU resources prematurely and 
                    // sinfully optimizing things that don't need to be 
                    // optimized.
                    //result = expr->result;
                    //if (result == nullptr) {
                    //    return Result (expr, kErrorInvalidOperand);
                    //}
                    //ExpressionPushScan (expr, expr->operand);
                    // Clear the buffer and return.
                    //ExpressionClear (expr); //< Do I really need to clear?
                    //return expr->result;
                    return ExpressionForceDisconnect (expr, kErrorInvalidOperand);
                }
                const uint_t* params = op->params;
                uintptr_t num_ops = reinterpret_cast<uintptr_t> (params);
                if (num_ops > kMaxNumParams) {
                    // It's an Operation.
                    // The software implementer pushes the Op on the stack.
#if SCRIPT_DEBUG
                    text << "\n| Found Operation with params "
                        << ParamsPrint (params, text);
#endif  //< SCRIPT_DEBUG
                    result = ExpressionScanHeader (expr, params);
                    if (result) {
#if SCRIPT_DEBUG
                        text << "Expression::Error reading address.";
#endif  //< SCRIPT_DEBUG
                        return ExpressionForceDisconnect (expr, kErrorImplementation);
                    }

                    operand = expr->operand;
                    if (!operand) {
#if SCRIPT_DEBUG
                        text << "\n| Null operand found!";
#endif  //< SCRIPT_DEBUG
                        return ExpressionForceDisconnect (expr, kErrorInvalidOperand);
                    }
                    header = op->params;
                    expr->params_left = *header;
                    expr->header = header;      //< +1 to bypass the number of params
                    expr->header_start = header;    //< Used to print current header.
                    ExpressionEnterState (expr, Bin::ArgsState);
                    bin_state = Bin::ArgsState;
                    type = *(++expr->header);   //< Setup to read first type.
#if SCRIPT_DEBUG
                    text << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< SCRIPT_DEBUG
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
#if SCRIPT_DEBUG
                    text << "\n| Params successfully scanned.";
#endif  //< SCRIPT_DEBUG

                    break;
                }
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG

                // Switch to next state
                if (type <= ADR) {
                    if (type < ADR) {   // Address type.
#if SCRIPT_DEBUG
                        text << "\n| Scanning address.";
#endif  //< SCRIPT_DEBUG
                        ErrorReport (expr, kErrorInvalidType);
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
#if SCRIPT_DEBUG
                    text << "\n| Scanning STR with max length "
                              << bytes_left;
#endif  //< SCRIPT_DEBUG
                    ExpressionEnterState (expr, Bin::Utf8State);
                    bin_state = Bin::Utf8State;
                    break;
                } else if (type == ST2) { // UTF-16 string type.
#if SCRIPT_DEBUG
                    text << "\n| Scanning ST2.";
#endif  //< SCRIPT_DEBUG
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
#if SCRIPT_DEBUG
                    text << "\n| Scanning ST4.";
#endif  //< SCRIPT_DEBUG
                    // Read the max number of chars off the header.
                    bytes_left = *expr->header++ * 4;
                    ExpressionEnterState (expr, Bin::Utf32State);
                    bin_state = Bin::Utf32State;
                } else if (type < DBL)  { // Plain-old-data type.
                    bytes_left = SizeOf (type);
#if SCRIPT_DEBUG
                    text << "\n| Scanning POD." << " bytes_left:"
                              << bytes_left;
#endif  //< SCRIPT_DEBUG
                    if (bytes_left == 1) {
                        // No need to enter a state because there is only one
                        // byte to parse and we already have the byte loaded.
#if SCRIPT_DEBUG
                        text << "\n| Done scanning without state change for \""
                                  << TypeString (type) << '\"';
#endif  //< SCRIPT_DEBUG
                        // Setup to read the next type.
                        type = *(++expr->header);
#if SCRIPT_DEBUG
                        text << "\n| Next TType to scan:\'" << TypeString (type)
                            << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< SCRIPT_DEBUG
                        write += TypeAlign (write, type);
                        break;
                    }
                    ExpressionEnterState (expr, Bin::PodState);
                    bin_state = Bin::PodState;
                    break;
                } else if (type < UV8)  { // Varint type.
#if SCRIPT_DEBUG
                    text << "\n| Scanning Varint.";
#endif  //< SCRIPT_DEBUG
                    bytes_left = SizeOf (type);
                    ExpressionEnterState (expr, Bin::VarintState);
                    bin_state = Bin::VarintState;
                    break;
                } else { // It's a TObject.
#if SCRIPT_DEBUG
                    text << "\n| Scanning TObject.";
#endif  //< SCRIPT_DEBUG
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
                break;
            }
            case Bin::Utf8State: {
                if (bytes_left == 0) {
                    ErrorReport (expr, kErrorTextOverflow,
                            const_cast<const uint_t*>(expr->header), 0, start);
                    break;
                }
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
                // Hash byte.
                // Check if char terminated.
                if (b == 0) {
                    // Check if there is another argument to scan.
                    ExpressionExitState (expr);
                    bin_state = Bin::ArgsState;
                    //< We can't go back from OBJ to POD for Text Types.
                    // Setup to read next type.
                    type = *(++expr->header);
#if SCRIPT_DEBUG
                    if (expr->params_left == 0) {
                        ExpressionSetState (expr, Bin::AddressState);
                        bin_state = Bin::AddressState;
                        break;
                    }
                    text << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< SCRIPT_DEBUG
                    write += TypeAlign (write, type);
                    break;
                }
                --bytes_left;
                break;
            }
            case Bin::Utf16State: {
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
                ExpressionExitState (expr);
                break;
            }
            case Bin::Utf32State: {
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
                ExpressionExitState (expr);
                break;
            }
            case Bin::VarintState: {
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
// When verifying a varint, there is a max number of bytes for 
// the type (3, 5, or 9) but the varint may be complete before 
// this number of bytes. We're just basically counting down and 
// looking for an overflow situation.
// Hash byte.

                if (bytes_left == 1) {
#if SCRIPT_DEBUG
                    text << "Checking last byte:";
#endif  //< SCRIPT_DEBUG

                    // @warning I am not current saving the offset. I'm not 
                    // sure  what to do here. The header class uses a variadic 
                    // template, I'm kind of tempted to switch to an int 
                    // type for the headers so I can just use a normal 
                    // array bracket initializer. The other option is to 
                    // add 32 to the first byte.

                    if ((b >> 7) != 1) {
                        const uint_t* header =
                            const_cast<const uint_t*>(expr->header);
                        
                        ExpressionEnterState (expr, Bin::ErrorState);
                        return ErrorReport (expr, kErrorVarintOverflow, header, 0, start);
                    }

                    break;
                }
                if (b > 127) {
#if SCRIPT_DEBUG
                    text << "\n| Done scanning varint: ";
#endif  //< SCRIPT_DEBUG
                    // Setup to read the next type.
                    type = *(++header);
#if SCRIPT_DEBUG
                    text << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< SCRIPT_DEBUG
                    write += TypeAlign (write, type);
                }
                --bytes_left;
                break;
            }
            case Bin::ObjectState: {
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
                if (bytes_shift >= shift_bits) {
                    // Done shifting.
#if SCRIPT_DEBUG
                    text << "\n| Loading object of size:" << bytes_left;
#endif  //< SCRIPT_DEBUG
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
#if SCRIPT_DEBUG
                    printf ("\n| Error: expecting hash:0x%x and found 0x%x.", hash, found_hash);
#endif  //< SCRIPT_DEBUG
                    return ExpressionForceDisconnect (expr, kErrorInvalidHash);
                }
#if SCRIPT_DEBUG
                text << "\n| Success reading hash!";
#endif  //< SCRIPT_DEBUG
                hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
#if SCRIPT_DEBUG
                text << "\n| Resetting hash.\n";
#endif  //< SCRIPT_DEBUG
                break;
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
#if SCRIPT_DEBUG
                    text << "\n| Resetting hash.";
#endif  //< SCRIPT_DEBUG
                    hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
                    expr->operand = expr->root;
                    expr->result = nullptr;
                    bin_state = Bin::AddressState;
                    ExpressionSetState (expr, Bin::AddressState);
#if SCRIPT_DEBUG
                    text << "\n| Root scope: \""
                              << OperandName (expr->operand) << '\"';
#endif  //< SCRIPT_DEBUG
                }
                break;
            }
            case  Bin::LockedState: {
#if SCRIPT_DEBUG
                text << "Locked";
#endif  //< SCRIPT_DEBUG
                break;
            }
            default: {
                hash = Hash16 (b, hash);
#if SCRIPT_DEBUG
                printf ("\n| hash:0x%x", hash);
#endif  //< SCRIPT_DEBUG
                // parsing plain-old-data.
                if (bytes_left == 0) {
#if SCRIPT_DEBUG
                    text << "... done!";
#endif  //< SCRIPT_DEBUG
                    ExpressionExitState (expr);
                    bin_state = expr->bin_state;

                    // Setup to read the next type.
                    type = *(++header);
#if SCRIPT_DEBUG
                    text << "\n| Next TType to scan:\'" << TypeString (type)
                        << "\' with alignment " << TypeAlign (write, type) << '.';
#endif  //< SCRIPT_DEBUG
                    write += TypeAlign (write, type);
                    break;
                }
                --bytes_left;
                //b = input->Pull ();
#if SCRIPT_DEBUG
                printf ("\n| Loading next byte:%x", b);
#endif  //< SCRIPT_DEBUG
                hash = Hash16 (b, hash);
                *start = b;
                ++start;
                break;
            }
        }
    }
    expr->hash = hash;
    expr->bytes_left = bytes_left;
    bin->start = (uint_t)Diff (begin, start);
    return nullptr;
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
        return ErrorReport (expr, kErrorStackOverflow, header);
    }

    return 0;
}

const uint_t* ExpressionHeaderStack (Expression* expr) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<byte*>
        (expr) + sizeof (Expression) + expr->stack_count);
}

void ExpressionClose (Expression* expr) {
#if SCRIPT_DEBUG
    std::cout << "\n| Closing expression.";
#endif  //< SCRIPT_DEBUG
    expr->stack_count = 1;
}

void ExpressionCancel (Expression* expr) {
#if SCRIPT_DEBUG
    std::cout << "\n| Canceling expression.";
#endif  //< SCRIPT_DEBUG
    expr->stack_count = 1;
    expr->bin_state = Bin::AddressState;
    //ExpressionPush (expr->root);
}

void ExpressionClear (Expression* expr) {
    // Erase the buffer by writing zeros to it.

    Bin* bin = ExpressionBin (expr);

    byte* begin = BinBuffer (bin),
        * end   = begin + bin->size,
        * start = begin + bin->start,
        * stop  = begin + bin->stop;

    //uint_t buffer_space = SlotSpace (start, stop, size);

    if (start == stop) return; //< Nothing to do.
    if (start > stop) {
        MemoryClear (start, end - start);
        MemoryClear (begin, start - begin);
        return;
    }
    MemoryClear (start, stop - start);
    bin->start = (uint_t)Diff (expr, begin);
    bin->stop  = (uint_t)Diff (expr, start + 1);
}

void ExpressionRingBell (Expression* expr, const char* address) {
    BoutRingBell (ExpressionBout (expr), address);
    
}

void ExpressionAckBack (Expression* expr, const char* address) {
    BoutAckBack (ExpressionBout (expr), address);
}

const Operation* ExpressionForceDisconnect (Expression* expr, Error error) {
    expr->bin_state = Bin::DisconnectedState;
    return ErrorReport (expr, error);
}

const Operation* ExpressionArgs (Expression* expr, const uint_t* params, void** args) {
    return BinRead (ExpressionBin (expr), params, args);
}


const Operation* ExprResult (Expression* expr, const uint_t* params, void** args) {
    return BoutWrite (ExpressionBout (expr), params, args);
}

const Operation* ExpressionOperand (Expression* expr, const Operation* operation) {
    if (expr) {
        if (!operation) {
            return OperationInvalid ();
        }
        void* args[2];
        uintptr_t num_operations = (uintptr_t)operation->params,
            first_operation = (uintptr_t)operation->result;
        // @todo Write params to expr!
        return ExprResult (expr, Params<5, STR, kOperationMaxNameLength,
                           UV8, UV8,
                           STR, kOperationMaxDescriptionLength> (),
                           Args (args, operation->name,
                           &num_operations, &first_operation,
                           operation->description));
    }
    return operation;
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
    return reinterpret_cast <byte*>(bin) + sizeof (Bin);
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
    return reinterpret_cast<byte*>(bin) + sizeof (Bin) + bin->size;
}

const Operation* ExpressionOperation (Expression* expr, const Operation* operation) {
    if (expr) {
        if (!operation) {
            return OperationInvalid ();
        }
        void* args[2];
        uintptr_t num_operations = (uintptr_t)operation->params,
            first_operation = (uintptr_t)operation->result;
        return ExprResult (expr, Params<5, STR, kOperationMaxNameLength,
                           UV8, UV8,
                           STR, kOperationMaxDescriptionLength> (),
                           Args (args, operation->name,
                           &num_operations, &first_operation,
                           operation->description));
    }
    return operation;
}

#if SCRIPT_USING_TEXT
Text& ExpressionPrintStack (Expression* expr, Text& text) {
    if (!expr) {
        return text;
    }

    uint_t i,
        stack_count;
    const Operation* op;
    Operand* operand;
    Operand** stack = ExpressionStack (expr);
    stack_count = expr->stack_count;
    text << "\n| Operand stack_count:" << stack_count;

    if (stack_count == 1) {
        return text << "\n| Stack Item 1: " << OperandName (expr->root);
    }
    for (i = 0; i < stack_count - 1; ++i) {
        text << "\n| Stack Item " << i + 1 << ":\"";
        operand = stack[i];
        op = operand->Star ('?', nullptr);
        text << op->name << '\"';
    }
    op = expr->operand->Star ('?', nullptr);
    return text << "\n| Stack Item " << i + 1 << ":\"" << op->name << "\"";
}/*

Text& ExpressionPrintStateStack (Expression* expr, Text& text) {
    text << text.Line ()
        << "\n| Expression State Stack:    ";
    if (!expr) {
        printf ("null");
        return text.Line ();
    }
    return text.Line ();
}*/

Text& ExpressionPrint (Expression* expr, Text& text) {
    text << text.Line ('~')
        << "\n| Stack:    ";
    if (!expr) {
        return text << "null" << text.Line ('~');
    }
    text.Pointer (expr) << text.Line ('_');

    return text << "\n| bytes_left  : " << expr->bytes_left
               << "\n| header_size : " << expr->header_size
               << "\n| stack_count : " << expr->stack_count
               << "\n| stack_size  : " << expr->stack_size
               << "\n| bin_state   : " << BinState ()[expr->bin_state]
               << "\n| bout_state  : " << BoutState ()[expr->bout_state]
               << "\n| num_states  : " << expr->num_states
               << "\n| header_size : " << expr->header_size
               << text.Line ('-', "\n>")
               << OperandPrint (expr->operand, text)
               << "\n| header: " << expr->header_start
               << text.Line ('-', "\n>")
               << ExpressionPrintStack (expr, text)
               << text.Line ('~');
    //system ("PAUSE");
}

#endif  //< SCRIPT_USING_TEXT
}       //< namespace _

#if SCRIPT_USING_TEXT
_::Text& operator<< (_::Text& text, _::Expression* expr) {
    return text << ExpressionPrint (expr, text);
}
#endif //< SCRIPT_USING_TEXT
