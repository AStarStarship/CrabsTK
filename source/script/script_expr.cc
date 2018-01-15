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
#include "expr.h"
#include "clock.h"
#include "bsq.h"
#include "console.h"
#include "hash.h"
#include "token.h"

namespace _ {

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Op Result. */
inline const Op* ExprError (Expr* expr, Error error) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (out << "\n| Expr " << ErrorString (error)
              << " Error!");
    #endif
    return reinterpret_cast<const Op*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expr.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Op Result. */
inline const Op* ExprError (Expr* expr,
                                     Error error,
                                     const uint_t* header) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (out << "\n| Expr " << ErrorString (error)
          << " Error!");
    #endif
    return reinterpret_cast<const Op*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expr.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Op Result. */
inline const Op* ExprError (Expr* expr,
                                Error error,
                                const uint_t* header,
                                byte offset) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (out << "\n| Expr " << ErrorString (error)
          << " Error!");
    #endif
    return reinterpret_cast<const Op*> (1);
}

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expr.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Op Result. */
inline const Op* ExprError (Expr* expr,
                            Error error,
                            const uint_t* header,
                            byte offset,
                            char* address) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (out << "\n| Expr " << ErrorString (error)
          << " Error!");
    #endif
    return reinterpret_cast<const Op*> (1);
}

uintptr_t* ExprBinAddress (Expr* expr) {
    if (!expr)
        return nullptr;
    return reinterpret_cast<uintptr_t*>(expr) + expr->header_size;
}

char* ExprBuffer (Expr* expr) {
    char* ptr = reinterpret_cast<char*> (expr);
    return ptr + sizeof (Expr);
}

BIn* ExprBIn (Expr* expr) {
    return reinterpret_cast<BIn*> (ExprBinAddress (expr));
}

uintptr_t* ExprBOutAddress (Expr* expr) {
    if (!expr)
        return nullptr;
    return reinterpret_cast<uintptr_t*>(expr) + expr->header_size;
}

BOut* ExprBOut (Expr* expr) {
    return reinterpret_cast<BOut*> (ExprBOutAddress (expr));
}

Expr* ExprInit (uintptr_t* buffer, uint_t buffer_size,  uint_t stack_size,
                Operand* root, uintptr_t* unpacked_buffer,
                uintptr_t unpacked_size) {
    if (!buffer) {
        return nullptr;
}
    if (buffer_size < Expr::kMinBufferSize) {
        return nullptr;
    }
    if (stack_size < kMinStackSize) {
        stack_size = kMinStackSize;    //< Minimum stack size.
    }
    if (unpacked_buffer == nullptr) {
        std::cout << "\n| Error: unpacked_buffer was nil!";
    }

    if (root == nullptr) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
        std::cout << "\n| Error: root can't be nil.";
    #endif
        return nullptr;
    }

    Expr* expr = reinterpret_cast<Expr*> (buffer);

    uint_t total_stack_size = (stack_size - 1) * (2 * sizeof (Operand*));
    // Calculate the size of the Slot and Stack.
    uint_t size = (buffer_size - sizeof (Expr) -
           total_stack_size + 1) >> 1;  // >>1 to divide by 2
    expr->bout_state   = BOut::kStateDisconnected;
    expr->bin_state    = BIn::kStateDisconnected;
    expr->stack_count  = 1;
    expr->type   = NIL;
    expr->stack_size   = stack_size;
    expr->num_states   = 0;
    expr->operand      = nullptr;
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    std::cout << "\n| Initializing Stack with size:" << stack_size
              << " buffer_size:" << buffer_size << " size:" << size;
    #endif //< SCRIPT_DEBUG
    expr->bytes_left = 0;
    //uint_t offset    = sizeof (Expr) + total_stack_size - sizeof (void*);
    //bin_offset       = sizeof (BIn) + total_stack_size + offset;
    expr->header_size  = sizeof (Expr) + 2 * sizeof (void*) * stack_size;
    expr->hash         = kLargest16BitPrime;
    expr->result       = nullptr;
    expr->header       = nullptr;
    expr->header_start = nullptr;
    expr->root = root;
    SlotInit (&expr->slot, reinterpret_cast<char*> (expr) + sizeof (Expr) +
                    stack_size * sizeof (uint_t), unpacked_size);
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    out << "expr->op:0x" << out.Pointer (expr->operand)
           << std::cout ;
    #endif //< SCRIPT_DEBUG
    BInInit  (ExprBinAddress  (expr), size);
    BOutInit (ExprBOutAddress (expr), size);
    return expr;
}

//bool ExprIsDynamic (Expr* expr) {
//    return expr->type % 2 == 1;
//}

char* ExprEndAddress (Expr* expr) {
    return BInEnd (ExprBIn (expr));
}

const Op* ExprReset (Expr* expr) {
    return 0;
}

byte ExprExitState (Expr* expr) {
    // We are guaranteed expr is not nil at this point.
    //if (!expr) {
    //    return  ExprError (ExprBin (expr), kErrorImplementation);
    //}
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    std::cout << "\n| Exiting " << BInState ()[expr->bin_state]
              << " state back to the " << BInState ()[expr->last_bin_state]
              << " state .";
    #endif
    byte state = expr->last_bin_state;
    expr->bin_state = state;
    return state;
}

const Op* ExprSetState (Expr* expr, BIn::State state) {
    // We are guaranteed expr is not nil at this point.
    //if (!expr) {
    //    return  ExprError (ExprBin (expr), kErrorImplementation);
    //}
    if (state == BIn::LockedState) {
        return ExprError (expr, kErrorObjectLocked);
    }
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    std::cout << "\n| Entering " << BInState ()[state]
              << " state:" << state;
    #endif
    expr->bin_state = state;
    return nullptr;
}

const Op* ExprEnterState (Expr* expr, 
                                       BIn::State state) {
    // We are guaranteed expr is not nil at this point.
    //if (!expr) {
    //    return  ExprError (ExprBin (expr), kErrorImplementation);
    //}
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    std::cout << "\n| Entering " << BInState ()[state]
              << " state:" << state;
    #endif
    expr->last_bin_state = expr->bin_state;
    expr->bin_state = state;
    return nullptr;
}

byte ExprStreamBOut (Expr* expr) {
    return BOutStreamByte (ExprBOut (expr));
}

const Op* Push (Expr* expr, Operand* operand) {
    if (!expr) {
        return ExprError (expr, kErrorImplementation);
    }
    if (!operand) {
        return ExprError (expr, kErrorInvalidOperand);
    }
    #if SCRIPT_DEBUG == SCRIPT_EXPR == SCRIPT_EXPRESSION
    std::cout << "\n| Pushing " << operand->Star ('?', nullptr)->name 
         << " onto the stack";
    #endif
    uint_t stack_count = expr->stack_count;
    if (stack_count >= expr->stack_size) {
        return ExprError (expr, kErrorStackOverflow);
    }
    ExprStack (expr)[stack_count - 1] = expr->operand;
    expr->operand = operand;
    expr->stack_count = stack_count + 1;
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (ExprPrintStack (expr, out));
    #endif
    return nullptr;
}

const Op* Pop (Expr* expr) {
    uint_t stack_count = expr->stack_count;
    if (stack_count == 0) { // This should not happen.
        return ExprError (expr, kErrorInvalidOperand);
    }
    if (stack_count == 1) {
        // We ever pop off the root.
        ExprClose (expr);
        return 0;
    }
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    std::cout << "\n| Popping " << OperandName (expr->operand)
              << " off the stack.";
    #endif
    expr->operand = ExprStack (expr)[stack_count - 2];
    expr->stack_count = stack_count - 1;
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    Strand sout = out;
    COut (sout << "\n| Top of stack is now " 
          << OperandName (expr->operand) << "." 
          << ExprPrintStack (expr, sout));
    #endif
    return nullptr;
}

const Op* ExprUnpack (Expr* expr) {
    if (!expr) {
        return ExprError (expr, kErrorImplementation);
    }

    uint_t            size,        //< Size of the ring buffer.
                      space,       //< Space left in the right buffer.
                      length,      //< Length of the ring buffer data.
                      type,        //< Current type.
                      bytes_left,  //< Num bytes left to scan.
                      array_type,  //< The type of array.
                      shift_bits,  //< Num bytes left to scan.
                      bytes_shift; //< Num of bytes to shift to scan OBJ.
    byte              bin_state,   //< Current bin FSM state.
                      b;           //< Current byte being verified.
    hash16_t          hash,        //< Expected hash of the B-Sequence.
                      found_hash;  //< Found B-Sequence hash.
    time_us_t         timestamp,   //< Last time when the expression ran.
                      delta_t;     //< Time delta between the last timestamp.
    const Op        * op;          //< Current Op.
    Operand         * operand;     //< The operand.
    BIn             * bin;         //< BIn.
    char            * bin_begin,   //< Beginning of the ring buffer.
                    * bin_start,   //< Start of the ring buffer data.
                    * bin_stop,    //< Stop of the ring buffer data.
                    * bin_end,     //< End of the ring buffer.
                    * slot_begin = expr->slot.begin,
                    * slot_start = expr->slot.start, //< Write cursor,
                    * slot_stop  = expr->slot.stop,
                    * slot_end   = expr->slot.end;
    const Op        * result;      //< The result of the Scan.
    const uint_t    * header = expr->header;
    //< Header of the current Op being verified.
    op = nullptr;
//    if (input == nullptr) {
//#if SCRIPT_DEBUG == SCRIPT_EXPR
//        PrintDebug ("input = nil");
//#endif
//        return;
//    }

    bin_state = expr->bin_state;    //< This isn't an optimal solution.
    bin = ExprBIn (expr);
    size = bin->size;
    hash = expr->hash;
    timestamp = ClockTimestampNow ();
    delta_t = timestamp - expr->last_time;

    if (delta_t <= expr->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch (rare)
            delta_t *= -1;
    }

    bin_begin  = BInBegin (bin);
    bin_end    = bin_begin + size;
    bin_start  = bin_begin + bin->start;
    bin_stop   = bin_begin + bin->stop;
    space  = SlotSpace (bin_start, bin_stop, size);
    length = size - space;
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (out << "\n| Scanning Expr:0x" << out.Pointer (expr)
          << " with length:" << length);
    #endif
    for (; length != 0; --length) {
        b = *bin_start;
        *slot_start++ = b;
        #if SCRIPT_DEBUG == SCRIPT_EXPR
        COut (out << out.Line ('=')
              << "\n| " << length << ":\'" << out.Clear ();
              << ((b < 32 || b == 127) ? (out << AsciiText ((AsciiCode)b)) 
                                       : (out << b))
              << "\' " << BInState ()[bin_state] << " state"
                  << out.Line ()));
        #endif

        if (++bin_start >= bin_end) bin_start -= size;
        // Process the rest of the bytes in a loop to reduce setup overhead.
        switch (bin_state) {
            case BIn::AddressState: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out << "\n| hash:" << out.Hex (hash));
                #endif
                // When verifying an address, there is guaranteed to be an
                // expr->op set. We are just looking for nil return values
                // from the Do (byte, Stack*): const Operand* function, 
                // pushing Star(string) on to the Star stack, and looking for
                // the first procedure call.
                //
                if (b == op->pop) {
                    Pop (expr);
                    break;
                }
                if (b == op->close) {
                    Pop (expr);
                    expr->bytes_left = 1;
                    ExprSetState (expr, BIn::HashState);
                    bin_state = BIn::HashState;
                    break;
                }
                operand = expr->operand;

                op = operand->Star ('?', nullptr);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out << "\n| Current Op is \"" << op->name << '\"');
                #endif

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
                    //ExprPushScan (expr, expr->operand);
                    // Clear the buffer and return.
                    //ExprClear (expr); //< Do I really need to clear?
                    //return expr->result;
                    return ExprForceDisconnect (expr, kErrorInvalidOperand);
                }
                const uint_t* params = op->params;
                uintptr_t num_ops = reinterpret_cast<uintptr_t> (params);
                if (num_ops > kParamsMax) {
                    // It's an Op.
                    // The software implementer pushes the Op on the stack.
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Found Op with params "
                          << BsqPrint (params, out));
                    #endif
                    result = ExprScanHeader (expr, params);
                    if (result) {
                        #if SCRIPT_DEBUG == SCRIPT_EXPR
                        COut (out << "Expr::Error reading address.");
                        #endif
                        return ExprForceDisconnect (expr, kErrorImplementation);
                    }

                    operand = expr->operand;
                    if (!operand) {
                        #if SCRIPT_DEBUG == SCRIPT_EXPR
                        COut (out << "\n| Null operand found!");
                        #endif
                        return ExprForceDisconnect (expr, kErrorInvalidOperand);
                    }
                    header = op->params;
                    expr->params_left = *header;
                    expr->header = header;      //< +1 to bypass the number of params
                    expr->header_start = header;    //< Used to print current header.
                    ExprEnterState (expr, BIn::ArgsState);
                    bin_state = BIn::ArgsState;
                    type = *(++expr->header);   //< Setup to read first type.
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Next TType to scan:\'" << TypeString (type)
                          << "\' with alignment "
                          << TypeAlign (slot_start, type) << '.');
                    #endif
                    slot_start += TypeAlign (slot_start, type);
                    break;
                }
                op = operand->Star (b, expr);
                break;
            }
            case BIn::ArgsState: {
                // In this state, a procedure has been called to scan on a valid
                // operand. This state is responsible for loading the next 
                // header argument and checking for the end of the procedure 
                // call.

                if (expr->params_left-- == 0) {
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Bsq successfully scanned.");
                    #endif

                    break;
                }
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out <<"\n| hash:" << out.Hex (hash));
                #endif

                // Switch to next state
                if (type <= ADR) {
                    if (type < ADR) {   // Address type.
                        #if SCRIPT_DEBUG == SCRIPT_EXPR
                        COut ("\n| Scanning address.");
                        #endif
                        ExprError (expr, kErrorInvalidType);
                        ExprEnterState (expr, BIn::LockedState);
                        bin_state = BIn::LockedState;
                        break;
                    }
                    ExprEnterState (expr,
                                            BIn::AddressState);
                    bin_state = BIn::AddressState;
                    break;

                } else if (type == STR) { // UTF-8/ASCII string type.
                    // Read the max number of chars off the header.
                    bytes_left = *(++expr->header);
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Scanning STR with max length "
                          << bytes_left);
                    #endif
                    ExprEnterState (expr, BIn::Utf8State);
                    bin_state = BIn::Utf8State;
                    break;
                } else if (type < DBL)  { // Plain-old-data type.
                    bytes_left = TypeSize (type);
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Scanning POD with "
                              << bytes_left << bytes_left);
                    #endif
                    if (bytes_left == 1) {
                        // No need to enter a state because there is only one
                        // byte to parse and we already have the byte loaded.
                        #if SCRIPT_DEBUG == SCRIPT_EXPR
                        COut (out << "\n| Done scanning without state change "
                              "for \"" << TypeString (type) << '\"';
                        #endif
                        // Setup to read the next type.
                        type = *(++expr->header);
                        #if SCRIPT_DEBUG == SCRIPT_EXPR
                        COut (out << "\n| Next TType to scan:\'" 
                              << TypeString (type) << "\' with alignment "
                              << TypeAlign (slot_start, type) << '.');
                        #endif
                        slot_start += TypeAlign (slot_start, type);
                        break;
                    }
                    ExprEnterState (expr, BIn::PodState);
                    bin_state = BIn::PodState;
                    break;
                } else if (type < UV8)  { // Varint type.
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Scanning Varint.");
                    #endif
                    bytes_left = TypeSize (type);
                    ExprEnterState (expr, BIn::VarintState);
                    bin_state = BIn::VarintState;
                    break;
                /*
                } else if (type == ST2) { // UTF-16 string type.
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Scanning ST2.");
                #endif
                    if (bytes_left == 1) {
                        expr->last_byte = b;
                        break;
                    }
                    // Read the max number of chars off the header.
                    bytes_left = *expr->header++ * 2;
                    ExprEnterState (expr,
                                    BIn::Utf16State);
                    bin_state = BIn::Utf16State;
                    break;
                }
                else if (type == ST4) { // UTF-32 string type.
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Scanning ST4.");
                #endif
                    // Read the max number of chars off the header.
                    bytes_left = *expr->header++ * 4;
                    ExprEnterState (expr, BIn::Utf32State);
                    bin_state = BIn::Utf32State;*/
                } else { // It's not a POD type.
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Scanning TObject.");
                    #endif
                    // Multi-dimension arrays are parsed just like any other
                    // TObject.
                    array_type &= 0x3;
                    bytes_left = b;
                    if (array_type == 0) {
                        // We don't need to enter a state here because we
                        // already have the size_bytes. :-)
                        bin_state = BIn::PodState;
                        break;
                    } else if (array_type == 1) {
                        bytes_shift = 0;
                        shift_bits  = 16;
                        ExprEnterState (expr, BIn::ObjectState);
                        bin_state = BIn::ObjectState;
                        break;
                    } else if (array_type == 2) {
                        bytes_shift = 0;
                        shift_bits  = 32;
                        ExprEnterState (expr, BIn::ObjectState);
                        bin_state = BIn::ObjectState;
                        break;
                    } else {    //< array_type == 3
                        bytes_shift = 0;
                        shift_bits  = 64;
                        ExprEnterState (expr, BIn::ObjectState);
                        bin_state = BIn::ObjectState;
                        break;
                    }
                }
                break;
            }
            case BIn::Utf8State: {
                if (bytes_left == 0) {
                    ExprError (expr, kErrorTextOverflow,
                            const_cast<const uint_t*>(expr->header), 0, bin_start);
                    break;
                }
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out << "\n| hash:" << out.Hex (hash));
                #endif
                // Hash byte.
                // Check if char terminated.
                if (b == 0) {
                    // Check if there is another argument to scan.
                    ExprExitState (expr);
                    bin_state = BIn::ArgsState;
                    //< We can't go back from OBJ to POD for Text Types.
                    // Setup to read next type.
                    type = *(++expr->header);
                    if (expr->params_left == 0) {
                        ExprSetState (expr, BIn::AddressState);
                        bin_state = BIn::AddressState;
                        break;
                    }
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Next TType to scan:\'" 
                          << TypeString (type) << "\' with alignment "
                          << TypeAlign (slot_start, type) << '.');
                    #endif
                    slot_start += TypeAlign (slot_start, type);
                    break;
                }
                --bytes_left;
                break;
            }
            case BIn::Utf16State: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("\n| hash:" << out.Hex (hash));
                #endif
                ExprExitState (expr);
                break;
            }
            case BIn::Utf32State: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("\n| hash:" << out.Hex (hash));
                #endif
                ExprExitState (expr);
                break;
            }
            case BIn::VarintState: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("\n| hash:" << out.Hex (hash));
                    #endif
                // When verifying a varint, there is a max number of bytes for 
                // the type (3, 5, or 9) but the varint may be complete before 
                // this number of bytes. We're just basically counting down and 
                // looking for an overflow situation.
                // Hash byte.

                if (bytes_left == 1) {
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("Checking last byte:");
                    #endif

                    // @warning I am not current saving the offset. I'm not 
                    // sure  what to do here. The header class uses a variadic 
                    // template, I'm kind of tempted to switch to an int 
                    // type for the headers so I can just use a normal 
                    // array bracket initializer. The other option is to 
                    // add 32 to the first byte.

                    if ((b >> 7) != 1) {
                        const uint_t* header =
                            const_cast<const uint_t*>(expr->header);
                        
                        ExprEnterState (expr, BIn::ErrorState);
                        return ExprError (expr, kErrorVarintOverflow, header, 0, bin_start);
                    }

                    break;
                }
                if (b > 127) {
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Done scanning varint: ");
                    #endif
                    // Setup to read the next type.
                    type = *(++header);
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Next TType to scan:\'" <<
                          TypeString (type) << "\' with alignment " <<
                          TypeAlign (slot_start, type) << '.');
                    #endif
                    slot_start += TypeAlign (slot_start, type);
                }
                --bytes_left;
                break;
            }
            case BIn::ObjectState: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("\n| hash:" << out.Hex (hash));
                #endif
                if (bytes_shift >= shift_bits) {
                    // Done shifting.
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Loading object of size:" << bytes_left);
                    #endif
                    ExprExitState (expr);
                    ExprEnterState (expr, BIn::PodState);
                    bin_state = BIn::PodState;
                    break;
                }
                bytes_left &= ((uint_t)b) << bytes_shift;
                shift_bits += 8;
                break;
            }
            case BIn::HashState: {
                if (expr->bytes_left != 0) {  // One more byte to load.
                    expr->last_byte = b;
                    expr->bytes_left = 0;
                    break;
                }
                found_hash = b;
                found_hash = found_hash << 8;
                found_hash |= expr->last_byte;
                if (hash != found_hash) {
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Error: expecting hash:" <<
                          out.Hex (hash) << " and found " << 
                          out.Hex (found_hash));
                    #endif
                    return ExprForceDisconnect (expr, kErrorInvalidHash);
                }
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("\n| Success reading hash!");
                #endif
                hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("\n| Resetting hash.\n");
                #endif
                break;
            }
            case BIn::ErrorState: {
                break;
            }
            case BIn::kStateDisconnected: {
                if (b) {
                    ExprSetState (expr, BIn::ErrorState);
                    bin_state = BIn::ErrorState;
                } else {
                    ExprSetState (expr, BIn::AckState);
                    bin_state = BIn::AckState;
                }
                break;
            }
            case  BIn::AckState: {
                if (b) {
                    ExprSetState (expr, BIn::ErrorState);
                    bin_state = BIn::ErrorState;
                } else {
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("\n| Resetting hash.");
                    #endif
                    hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
                    expr->operand = expr->root;
                    expr->result = nullptr;
                    bin_state = BIn::AddressState;
                    ExprSetState (expr, BIn::AddressState);
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Root scope: \"" <<
                          OperandName (expr->operand) << '\"');
                    #endif
                }
                break;
            }
            case BIn::LockedState: {
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut ("Locked");
                #endif
                break;
            }
            default: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out << "\n| hash:" << out.Hex (hash));
                #endif
                // parsing plain-old-data.
                if (!bytes_left) {
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut ("... done!");
                    #endif
                    ExprExitState (expr);
                    bin_state = expr->bin_state;

                    // Setup to read the next type.
                    type = *(++header);
                    #if SCRIPT_DEBUG == SCRIPT_EXPR
                    COut (out << "\n| Next TType to scan:\'" << TypeString (type)
                          << "\' with alignment " << TypeAlign (slot_start, type)
                          << '.');
                    #endif
                    slot_start += TypeAlign (slot_start, type);
                    break;
                }
                --bytes_left;
                //b = input->Pull ();
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out << "\n| Loading next byte:" << out.Hex (b));
                #endif
                hash = Hash16 (b, hash);
                *bin_start = b;
                ++bin_start;
                break;
            }
        }
    }
    // 
    expr->hash = hash;
    expr->bytes_left = bytes_left;
    bin->start = (uint_t)MemoryVector (bin_begin, bin_start);
    return nullptr;
}

bool ExprContains (Expr* expr, void* address) {
    if (address < reinterpret_cast<uintptr_t*>(expr))
        return false;
    if (address > ExprEndAddress (expr)) return false;
    return true;
}

const Op* ExprScanHeader (Expr* expr, const uint_t* header) {
    if (expr->stack_count >= expr->stack_size) {
        // Handle overflow cleanup:
        return ExprError (expr, kErrorStackOverflow, header);
    }

    return 0;
}

const uint_t* ExprHeaderStack (Expr* expr) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<char*>
        (expr) + sizeof (Expr) + expr->stack_count);
}

void ExprClose (Expr* expr) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    COut ("\n| Closing expression.");
    #endif
    expr->stack_count = 1;
}

void ExprCancel (Expr* expr) {
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    COut ("\n| Canceling expression.");
    #endif
    expr->stack_count = 1;
    expr->bin_state = BIn::AddressState;
    //ExprPush (expr->root);
}

void ExprClear (Expr* expr) {
    // Erase the buffer by writing zeros to it.

    BIn* bin = ExprBIn (expr);

    char* begin = BInBegin (bin),
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
    bin->start = (uint_t)MemoryVector (expr, begin);
    bin->stop  = (uint_t)MemoryVector (expr, start + 1);
}

void ExprRingBell (Expr* expr, const char* address) {
    BOutRingBell (ExprBOut (expr), address);
    
}

void ExprAckBack (Expr* expr, const char* address) {
    BOutAckBack (ExprBOut (expr), address);
}

const Op* ExprForceDisconnect (Expr* expr, Error error) {
    expr->bin_state = BIn::kStateDisconnected;
    return ExprError (expr, error);
}

const Op* ExprQuery (Expr* expr, const Op& op) {
    if (expr) {
        void    * args[2];
        uintptr_t num_ops  = (uintptr_t)op.params,
                  first_op = (uintptr_t)op.result;
        // @todo Write params to expr!
        static const uint_t* header = Bsq<5, STR, kOpNameLengthMax, UV8, UV8,
                                             STR, kOpDescriptionLengthMax> ();
        return BOutWrite (ExprBOut (expr),
                          header,
                          Args (args, op.name, &num_ops, &first_op,
                                op.description));
    }
    return &op;
}

char* ExprBaseAddress (BIn* bin) {
    return reinterpret_cast <char*>(bin) + sizeof (BIn);
}

uint_t ExprSpace (BIn* bin) {
    if (bin == nullptr)
        return ~0;

    char* base = ExprBaseAddress (bin);
    return SlotSpace (base + bin->start, base + bin->stop, bin->size);
}

uintptr_t* ExprBaseAddress (void* ptr, uint_t rx_tx_offset) {
    enum {
        kSlotHeaderSize = sizeof (BIn) + sizeof (uintptr_t) -
        sizeof (BIn) % sizeof (uintptr_t),
        //< Offset to the start of the ring buffer.
    };
    char* result = reinterpret_cast <char*>(ptr) + rx_tx_offset + 
                   kSlotHeaderSize;
    return reinterpret_cast<uintptr_t*> (result);
}

char* ExprEndAddress (BIn* bin) {
    return reinterpret_cast<char*>(bin) + sizeof (BIn) + bin->size;
}

const Op* ExprQuery (Expr* expr, const Op* op) {
    if (expr) {
        if (!op) {
            return ExprError (expr, kErrorImplementation);
        }
        void* args[2];
        uintptr_t num_ops  = (uintptr_t)op->params,
                  first_op = (uintptr_t)op->result;
        return BOutWrite (ExprBOut (expr) ,
                           Bsq<5, STR, kOpNameLengthMax, UV8, UV8,
                                  STR, kOpDescriptionLengthMax> (),
                           Args (args, op->name, &first_op,
                                 &num_ops, op->description));
    }
    return op;
}

#if USING_SCRIPT_TEXT
Strand& ExprPrintStack (Expr* expr, Strand& strand) {
    if (!expr) {
        return strand;
    }

    uint_t i,
        stack_count;
    const Op* op;
    Operand* operand;
    Operand** stack = ExprStack (expr);
    stack_count = expr->stack_count;
    strand << "\n| Operand stack_count:" << stack_count;

    if (stack_count == 1) {
        return strand << "\n| Stack Item 1: " << OperandName (expr->root);
    }
    for (i = 0; i < stack_count - 1; ++i) {
        strand << "\n| Stack Item " << i + 1 << ":\"";
        operand = stack[i];
        op = operand->Star ('?', nullptr);
        strand << op->name << '\"';
    }
    op = expr->operand->Star ('?', nullptr);
    return strand << "\n| Stack Item " << i + 1 << ":\"" << op->name << "\"";
}

Strand& ExprPrint (Expr* expr, Strand& strand) {
    strand << strand.Line ('~')
        << "\n| Stack:    ";
    if (!expr) {
        return strand << "nil" << strand.Line ('~');
    }
    strand.Pointer (expr) << strand.Line ('_');

    return strand << "\n| bytes_left  : " << expr->bytes_left
                  << "\n| header_size : " << expr->header_size
                  << "\n| stack_count : " << expr->stack_count
                  << "\n| stack_size  : " << expr->stack_size
                  << "\n| bin_state   : " << BInState ()[expr->bin_state]
                  << "\n| bout_state  : " << BOutState ()[expr->bout_state]
                  << "\n| num_states  : " << expr->num_states
                  << "\n| header_size : " << expr->header_size
                  << strand.Line ('-', "\n>")
                  << OperandPrint (expr->operand, strand)
                  << "\n| header: " << expr->header_start
                  << strand.Line ('-', "\n>")
                  << ExprPrintStack (expr, strand)
                  << strand.Line ('~');
}

#endif  //< USING_SCRIPT_TEXT

}       //< namespace _

#if USING_SCRIPT_TEXT
_::Strand& operator<< (_::Strand& strand, _::Expr* expr) {
    return ExprPrint (expr, strand);
}
#endif //< USING_SCRIPT_TEXT
