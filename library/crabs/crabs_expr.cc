/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_expression.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

#include "clock.h"
#include "bsq.h"
#include "hash.h"
#include "hex.h"
#include "line.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_BSQ(header, bsq) {\
    enum {\
        kTextBufferSize = 1024,\
        kTextBufferSizeWords = kTextBufferSize >> kWordBitCount\
     };\
    char bsq_buffer[kTextBufferSizeWords];\
    PrintBsq (bsq, ,bsq_buffer, bsq_buffer + kTextBufferSize);\
    printf   ("\n    %s%s", header, bsq_buffer);\
}
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_BSQ(header, bsq)
#endif

namespace _ {

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Op Result. */
inline const Op* ExprError (Expr* expr, Error error) {
    PRINTF ("\nExpr %s Error!", ErrorString (error))
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
    PRINTF ("\nExpr %s Error!", ErrorString (error))
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
    PRINTF ("\nExpr %s Error!", ErrorString (error))
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
    PRINTF ("\nExpr %s Error!", ErrorString (error))
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
    if (!expr) {
        return nullptr;
    }
    return reinterpret_cast<uintptr_t*>(expr) + expr->header_size;
}

BOut* ExprBOut (Expr* expr) {
    return reinterpret_cast<BOut*> (ExprBOutAddress (expr));
}

Expr* ExprInit (uintptr_t* buffer, uint_t buffer_size, uint_t stack_size,
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
        PRINTF ("\nError: unpacked_buffer was nil!")
    }

    if (root == nullptr) {
    #if DEBUG_CRABS_EXPR
        PRINTF ("\nError: root can't be nil.";
    #endif
        return nullptr;
    }

    Expr* expr = reinterpret_cast<Expr*> (buffer);

    uint_t total_stack_size = (stack_size - 1) * (2 * sizeof (Operand*));
    // Calculate the size of the Slot and Stack.
    uint_t size = (buffer_size - sizeof (Expr) - total_stack_size + 1) >> 1;

    //< >>1 to divide by 2
    expr->bout_state = kBOutStateDisconnected;
    expr->bin_state = kBInStateDisconnected;
    expr->stack_count = 1;
    expr->type = NIL;
    expr->stack_size = stack_size;
    expr->num_states = 0;
    expr->operand = nullptr;
#if DEBUG_CRABS_EXPR
    PRINTF ("\nInitializing Stack with size:" << stack_size
            << " buffer_size:" << buffer_size << " size:" << size;
#endif
    expr->bytes_left = 0;
    //uint_t offset    = sizeof (Expr) + total_stack_size - sizeof (void*);
    //bin_offset       = sizeof (BIn) + total_stack_size + offset;
    expr->header_size = sizeof (Expr) + 2 * sizeof (void*) * stack_size;
    expr->hash = kLargest16BitPrime;
    expr->result = nullptr;
    expr->header = nullptr;
    expr->header_start = nullptr;
    expr->root = root;
    uintptr_t* base_ptr = reinterpret_cast<uintptr_t*> (expr) + sizeof (Expr) +
                          stack_size * sizeof (uint_t);
    expr->slot.Set (base_ptr, unpacked_size);
#if DEBUG_CRABS_EXPR
    PRINTF ("expr->op:" << OutHex (expr->operand);
#endif
    BInInit (ExprBinAddress (expr), size);
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
    //    return  ExprError (ExprBIn (expr), kErrorImplementation);
    //}
#if DEBUG_CRABS_EXPR
    PRINTF ("\nExiting " << BInState ()[expr->bin_state]
            << " state back to the " << BInState ()[expr->last_bin_state]
            << " state .";
#endif
    byte state = expr->last_bin_state;
    expr->bin_state = state;
    return state;
}

const Op* ExprSetState (Expr* expr, BInState state) {
    // We are guaranteed expr is not nil at this point.
    //if (!expr) {
    //    return  ExprError (ExprBIn (expr), kErrorImplementation);
    //}
    if (state == kBInStateLocked) {
        return ExprError (expr, kErrorObjectLocked);
    }
#if DEBUG_CRABS_EXPR
    PRINTF ("\nEntering " << BInState ()[state]
            << " state:" << state;
#endif
    expr->bin_state = state;
    return nullptr;
}

const Op* ExprEnterState (Expr* expr, BInState state) {
    // We are guaranteed expr is not nil at this point.
    //if (!expr) {
    //    return  ExprError (ExprBIn (expr), kErrorImplementation);
    //}
#if DEBUG_CRABS_EXPR
    PRINTF ("\nEntering " << BInState ()[state]
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
    PRINTF ("\nPushing %s onto the stack", operand->Star ('?', nullptr)->name);
    uint_t stack_count = expr->stack_count;
    if (stack_count >= expr->stack_size) {
        return ExprError (expr, kErrorStackOverflow);
    }
    ExprStack (expr)[stack_count - 1] = expr->operand;
    expr->operand = operand;
    expr->stack_count = stack_count + 1;
#if DEBUG_CRABS_EXPR
    PRINTF (ExprPrintStack (expr, Print ());
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
    PRINTF ("\nPopping %s off the stack.", OperandName (expr->operand))
        expr->operand = ExprStack (expr)[stack_count - 2];
    expr->stack_count = stack_count - 1;
#if DEBUG_CRABS_EXPR
    Slot sout = out;
    PRINTF ("\nTop of stack is now "
            << OperandName (expr->operand) << "."
            << ExprPrintStack (expr, Print ());
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
                    * slot_stop = expr->slot.stop,
                    * slot_end = expr->slot.end;
    const Op        * result;      //< Result of the Scan.
    const uint_t    * header = expr->header;

    //< Header of the current Op being verified.
    op = nullptr;
//    if (input == nullptr) {
//#if DEBUG_CRABS_EXPR
//        PrintDebug ("input = nil");
//#endif
//        return;
//    }

    bin_state = expr->bin_state;    //< This isn't an optimal solution.
    bin = ExprBIn (expr);
    size = bin->size;
    hash = expr->hash;
    timestamp = ClockNow ();
    delta_t = timestamp - expr->last_time;

    if (delta_t <= expr->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch (rare)
            delta_t *= -1;
    }

    bin_begin = BInBegin (bin);
    bin_end = bin_begin + size;
    bin_start = bin_begin + bin->start;
    bin_stop = bin_begin + bin->stop;
    space = (uint_t)SlotSpace (bin_start, bin_stop, size);
    length = size - space;
    PRINTF ("\n    Scanning Expr:0x%p with length:%i", expr, length)
        for (; length != 0; --length) {
            b = *bin_start;
            *slot_start++ = b;
        #if DEBUG_CRABS_EXPR
            PRINTF (out.Line ('=') << '\n' << length
                    << ":\'"
                    << ((b < 32 || b == 127) ? (PRINTF (AsciiText ((AsciiCode)b))
                    : (PRINTF (b))
                    << "\' " << BInState ()[bin_state] << " state"
                    << out.Line ()));
        #endif

            if (++bin_start >= bin_end) bin_start -= size;
            // Process the rest of the bytes in a loop to reduce setup overhead.
            switch (bin_state) {
                case kBInStateAddress: {
                    hash = Hash16 (b, hash);
                    PRINTF ("\nhash:0x%x", hash)
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
                        ExprSetState (expr, kBInStateVerifyingHash);
                        bin_state = kBInStateVerifyingHash;
                        break;
                    }
                    operand = expr->operand;

                    op = operand->Star ('?', nullptr);
                    PRINTF ("\nCurrent Op is \"%s\"", op->name)

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
                    const uint_t* params = op->in;
                    uintptr_t num_ops = reinterpret_cast<uintptr_t> (params);
                    if (num_ops > kParamsMax) {
                        // It's an Op.
                        // The software implementer pushes the Op on the stack.

                        //PRINT_BSQ ("\nFound Op with params ", params)

                        result = ExprScanHeader (expr, params);
                        if (result) {
                        #if DEBUG_CRABS_EXPR
                            PRINTF ("Expr::Error reading address.");
                        #endif
                            return ExprForceDisconnect (expr, kErrorImplementation);
                        }

                        operand = expr->operand;
                        if (!operand) {
                        #if DEBUG_CRABS_EXPR
                            PRINTF ("\nNull operand found!");
                        #endif
                            return ExprForceDisconnect (expr, kErrorInvalidOperand);
                        }
                        header = op->in;
                        expr->params_left = *header;
                        expr->header = header;      //< +1 to bypass the number of params
                        expr->header_start = header;    //< Used to print current header.
                        ExprEnterState (expr, kBInStatePackedArgs);
                        bin_state = kBInStatePackedArgs;
                        type = *(++expr->header);   //< Setup to read first type.
                    #if DEBUG_CRABS_EXPR
                        PRINTF ("\nNext TType to scan:\'" <<
                                TypeString (type) << "\' with alignment "
                                << TypeAlign (slot_start, type) << '.');
                    #endif
                        slot_start += TypeAlign (slot_start, type);
                        break;
                    }
                    op = operand->Star (b, expr);
                    break;
                }
                case kBInStatePackedArgs: {
                    // In this state, a procedure has been called to scan on a valid
                    // operand. This state is responsible for loading the next 
                    // header argument and checking for the end of the procedure 
                    // call.

                    if (expr->params_left-- == 0) {
                    #if DEBUG_CRABS_EXPR
                        PRINTF ("\nBSQ successfully scanned.");
                    #endif

                        break;
                    }
                    hash = Hash16 (b, hash);
                #if DEBUG_CRABS_EXPR
                    Print () << "\nhash:" << PrintHex (hash));
        #endif

        // Switch to next state
            if (type <= ADR) {
                if (type < ADR) {   // Address type.
                #if DEBUG_CRABS_EXPR
                    Write ("\nScanning address.");
                #endif
                    ExprError (expr, kErrorInvalidType);
                    ExprEnterState (expr, kBInStateLocked);
                    bin_state = kBInStateLocked;
                    break;
                }
                ExprEnterState (expr,
                                kBInStateAddress);
                bin_state = kBInStateAddress;
                break;

            }
            else if (type == STR) { // UTF-8/ASCII string type.
             // Read the max number of chars off the header.
                bytes_left = *(++expr->header);
            #if DEBUG_CRABS_EXPR
                PRINTF ("\nScanning STR with max length " <<
                        bytes_left);
            #endif
                ExprEnterState (expr, kBInStatePackedUtf8);
                bin_state = kBInStatePackedUtf8;
                break;
            }
            else if (type < DBL) { // Plain-old-data type.
                bytes_left = TypeSize (type);
            #if DEBUG_CRABS_EXPR
                PRINTF ("\nScanning POD with "
                        << bytes_left << bytes_left);
            #endif
                if (bytes_left == 1) {
                    // No need to enter a state because there is only one
                    // byte to parse and we already have the byte loaded.
                #if DEBUG_CRABS_EXPR
                    PRINTF ("\nDone scanning without state change "
                            "for \"" << TypeString (type) << '\"';
                #endif
                // Setup to read the next type.
                    type = *(++expr->header);
                #if DEBUG_CRABS_EXPR
                    PRINTF ("\nNext TType to scan:\'"
                            << TypeString (type) << "\' with alignment "
                            << TypeAlign (slot_start, type) << '.');
                #endif
                    slot_start += TypeAlign (slot_start, type);
                    break;
                }
                ExprEnterState (expr, kBInStatePackedPod);
                bin_state = kBInStatePackedPod;
                break;
            }
            else if (type < UV8) { // Varint type.
            #if DEBUG_CRABS_EXPR
                Write ("\nScanning Varint.");
            #endif
                bytes_left = TypeSize (type);
                ExprEnterState (expr, kBInStatePackedVarint);
                bin_state = kBInStatePackedVarint;
                break;
            /*
            } else if (type == ST2) { // UTF-16 string type.
                #if DEBUG_CRABS_EXPR
                Write ("\nScanning ST2.");
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
                #if DEBUG_CRABS_EXPR
                Write ("\nScanning ST4.");
                #endif
                // Read the max number of chars off the header.
                bytes_left = *expr->header++ * 4;
                ExprEnterState (expr, BIn::Utf32State);
                bin_state = BIn::Utf32State;*/
            }
            else { // It's not a POD type.
                #if DEBUG_CRABS_EXPR
                Write ("\nScanning TObject.");
                #endif
                // Multi-dimension arrays are parsed just like any other OBJ.
                array_type &= 0x3;
                bytes_left = b;
                if (array_type == 0) {
                    // We don't need to enter a state here because we
                    // already have the size_bytes. :-)
                    bin_state = kBInStatePackedPod;
                    break;
                }
                else if (array_type == 1) {
                    bytes_shift = 0;
                    shift_bits = 16;
                    ExprEnterState (expr, kBInStatePackedObject);
                    bin_state = kBInStatePackedObject;
                    break;
                }
                else if (array_type == 2) {
                    bytes_shift = 0;
                    shift_bits = 32;
                    ExprEnterState (expr, kBInStatePackedObject);
                    bin_state = kBInStatePackedObject;
                    break;
                }
                else {    //< array_type == 3
                    bytes_shift = 0;
                    shift_bits = 64;
                    ExprEnterState (expr, kBInStatePackedObject);
                    bin_state = kBInStatePackedObject;
                    break;
                }
            }
            break;
                }
                case kBInStatePackedUtf8: {
                    if (bytes_left == 0) {
                        ExprError (expr, kErrorTextOverflow,
                                   const_cast<const uint_t*>(expr->header), 0, bin_start);
                        break;
                    }
                    hash = Hash16 (b, hash);
                    #if DEBUG_CRABS_EXPR
                    PRINTF ("\nhash:" << PrintHex (hash));
                    #endif
                    // Hash byte.
                    // Check if char terminated.
                    if (b == 0) {
                        // Check if there is another argument to scan.
                        ExprExitState (expr);
                        bin_state = kBInStatePackedArgs;
                        //< We can't go back from OBJ to POD for Text Types.
                        // Setup to read next type.
                        type = *(++expr->header);
                        if (expr->params_left == 0) {
                            ExprSetState (expr, kBInStateAddress);
                            bin_state = kBInStateAddress;
                            break;
                        }
                        #if DEBUG_CRABS_EXPR
                        PRINTF ("\nNext TType to scan:\'"
                                << TypeString (type) << "\' with alignment "
                                << TypeAlign (slot_start, type) << '.');
                        #endif
                        slot_start += TypeAlign (slot_start, type);
                        break;
                    }
                    --bytes_left;
                    break;
                }
                case kBInStatePackedUtf16: {
                    hash = Hash16 (b, hash);
                #if DEBUG_CRABS_EXPR
                    Write ("\nhash:" << PrintHex (hash));
                #endif
                    ExprExitState (expr);
                    break;
                }
                case kBInStatePackedUtf32: {
                    hash = Hash16 (b, hash);
                #if DEBUG_CRABS_EXPR
                    Write ("\nhash:" << PrintHex (hash));
                #endif
                    ExprExitState (expr);
                    break;
                }
                case kBInStatePackedVarint: {
                    hash = Hash16 (b, hash);
                #if DEBUG_CRABS_EXPR
                    Write ("\nhash:" << PrintHex (hash));
                #endif
            // When verifying a varint, there is a max number of bytes for 
            // the type (3, 5, or 9) but the varint may be complete before 
            // this number of bytes. We're just basically counting down and 
            // looking for an overflow situation.
            // Hash byte.

                    if (bytes_left == 1) {
                    #if DEBUG_CRABS_EXPR
                        Write ("Checking last byte:");
                    #endif

                    // @warning I am not current saving the offset. I'm not 
                    // sure  what to do here. The header class uses a variadic 
                    // template, I'm kind of tempted to switch to an int 
                    // type for the headers so I can just use a normal 
                    // array bracket newializer. The other option is to 
                    // add 32 to the first byte.

                        if ((b >> 7) != 1) {
                            const uint_t* header =
                                const_cast<const uint_t*>(expr->header);

                            ExprEnterState (expr, kBInStateHandlingError);
                            return ExprError (expr, kErrorVarintOverflow, header, 0, bin_start);
                        }

                        break;
                    }
                    if (b > 127) {
                    #if DEBUG_CRABS_EXPR
                        Write ("\nDone scanning varint: ");
                    #endif
                    // Setup to read the next type.
                        type = *(++header);
                    #if DEBUG_CRABS_EXPR
                        PRINTF ("\nNext TType to scan:\'" <<
                                TypeString (type) << "\' with alignment " <<
                                TypeAlign (slot_start, type) << '.');
                    #endif
                        slot_start += TypeAlign (slot_start, type);
                    }
                    --bytes_left;
                    break;
                }
                case kBInStatePackedObject: {
                    hash = Hash16 (b, hash);
                #if DEBUG_CRABS_EXPR
                    Write ("\nhash:" << PrintHex (hash));
                #endif
                    if (bytes_shift >= shift_bits) {
                        // Done shifting.
                    #if DEBUG_CRABS_EXPR
                        PRINTF ("\nLoading object of size:" << bytes_left);
                    #endif
                        ExprExitState (expr);
                        ExprEnterState (expr, kBInStatePackedPod);
                        bin_state = kBInStatePackedPod;
                        break;
                    }
                    bytes_left &= ((uint_t)b) << bytes_shift;
                    shift_bits += 8;
                    break;
                }
                case kBInStateVerifyingHash: {
                    if (expr->bytes_left != 0) {  // One more byte to load.
                        expr->last_byte = b;
                        expr->bytes_left = 0;
                        break;
                    }
                    found_hash = b;
                    found_hash = found_hash << 8;
                    found_hash |= expr->last_byte;
                    if (hash != found_hash) {
                    #if DEBUG_CRABS_EXPR
                        Write ("\nError: expecting hash:" <<
                               PrintHex (hash) << " and found " <<
                               PrintHex (found_hash));
                    #endif
                        return ExprForceDisconnect (expr, kErrorInvalidHash);
                    }
                #if DEBUG_CRABS_EXPR
                    Write ("\nSuccess reading hash!");
                #endif
                    hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
                #if DEBUG_CRABS_EXPR
                    Write ("\nResetting hash.\n");
                #endif
                    break;
                }
                case kBInStateHandlingError: {
                    break;
                }
                case kBInStateDisconnected: {
                    if (b) {
                        ExprSetState (expr, kBInStateHandlingError);
                        bin_state = kBInStateHandlingError;
                    }
                    else {
                        ExprSetState (expr, kBInStateAck);
                        bin_state = kBInStateAck;
                    }
                    break;
                }
                case  kBInStateAck: {
                    if (b) {
                        ExprSetState (expr, kBInStateHandlingError);
                        bin_state = kBInStateHandlingError;
                    }
                    else {
                    #if DEBUG_CRABS_EXPR
                        Write ("\nResetting hash.");
                    #endif
                        hash = kLargest16BitPrime; //< Reset hash to largest 16-bit prime.
                        expr->operand = expr->root;
                        expr->result = nullptr;
                        bin_state = kBInStateAddress;
                        ExprSetState (expr, kBInStateAddress);
                    #if DEBUG_CRABS_EXPR
                        PRINTF ("\nRoot scope: \"" <<
                                OperandName (expr->operand) << '\"');
                    #endif
                    }
                    break;
                }
                case kBInStateLocked: {
                #if DEBUG_CRABS_EXPR
                    Write ("Locked");
                #endif
                    break;
                }
                default: {
                    hash = Hash16 (b, hash);
                #if DEBUG_CRABS_EXPR
                    PRINTF ("\nhash:" << PrintHex (hash));
                #endif
                // parsing plain-old-data.
                    if (!bytes_left) {
                    #if DEBUG_CRABS_EXPR
                        Write ("... done!");
                    #endif
                        ExprExitState (expr);
                        bin_state = expr->bin_state;

                        // Setup to read the next type.
                        type = *(++header);
                    #if DEBUG_CRABS_EXPR
                        PRINTF ("\nNext TType to scan:\'" << TypeString (type)
                                << "\' with alignment " << TypeAlign (slot_start, type)
                                << '.');
                    #endif
                        slot_start += TypeAlign (slot_start, type);
                        break;
                    }
                    --bytes_left;
                    //b = input->Pull ();
                #if DEBUG_CRABS_EXPR
                    PRINTF ("\nLoading next byte:" << PrintHex (b));
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
#if DEBUG_CRABS_EXPR
    Write ("\nClosing expression.");
#endif
    expr->stack_count = 1;
}

void ExprCancel (Expr* expr) {
#if DEBUG_CRABS_EXPR
    Write ("\nCanceling expression.");
#endif
    expr->stack_count = 1;
    expr->bin_state = kBInStateAddress;
    //ExprPush (expr->root);
}

void ExprClear (Expr* expr) {
    // Erase the buffer by writing zeros to it.

    BIn* bin = ExprBIn (expr);

    char* begin = BInBegin (bin),
        *end = begin + bin->size,
        *start = begin + bin->start,
        *stop = begin + bin->stop;

    //uint_t buffer_space = SlotSpace (start, stop, size);

    if (start == stop) return; //< Nothing to do.
    if (start > stop) {
        MemoryClear (start, end - start);
        MemoryClear (begin, start - begin);
        return;
    }
    MemoryClear (start, stop - start);
    bin->start = (uint_t)MemoryVector (expr, begin);
    bin->stop = (uint_t)MemoryVector (expr, start + 1);
}

void ExprRingBell (Expr* expr, const char* address) {
    BOutRingBell (ExprBOut (expr), address);

}

void ExprAckBack (Expr* expr, const char* address) {
    BOutAckBack (ExprBOut (expr), address);
}

const Op* ExprForceDisconnect (Expr* expr, Error error) {
    expr->bin_state = kBInStateDisconnected;
    return ExprError (expr, error);
}

const Op* ExprQuery (Expr* expr, const Op& op) {
    if (expr) {
        void    * args[2];
        uintptr_t num_ops = (uintptr_t)op.in,
            first_op = (uintptr_t)op.out;
  // @todo Write params to expr!
        static const uint_t* header = Params<5, STR, kOpNameLengthMax, UV8, UV8,
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
    if (!bin) {
        return ~0;
    }

    char* base = ExprBaseAddress (bin);
    return (uint_t)SlotSpace (base + bin->start, base + bin->stop, bin->size);
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
        return BOutWrite (ExprBOut (expr),
                          Params<5, STR, kOpNameLengthMax, UV8, UV8,
                          STR, kOpDescriptionLengthMax> (),
                          Args (args, op->name, op->in,
                          op->out, op->description));
    }
    return op;
}

#if USING_PRINTER
Printer& PrintExprStack (Printer& print, Expr* expr) {
    assert (expr);

    uint_t    i,
              stack_count;
    const Op* op;
    Operand * operand;
    Operand** stack = ExprStack (expr);

    stack_count = expr->stack_count;
    print << "\nOperand stack_count:" << stack_count;

    if (stack_count == 1) {
        return print << "\nStack Item 1: " << OperandName (expr->root);
    }
    for (i = 0; i < stack_count - 1; ++i) {
        print << "\nStack Item " << i + 1 << ":\"";
        operand = stack[i];
        op = operand->Star ('?', nullptr);
        print << op->name << '\"';
    }
    op = expr->operand->Star ('?', nullptr);
    return print << "\nStack Item " << i + 1 << ":\"" << op->name << "\"";
}

Printer& PrintExpr (Printer& print, Expr* expr) {
    assert (expr);

    return print << Line ('~', 80) << "\nStack:    "
                 << Hex<uintptr_t> (expr) << '\n' << Line ('_', 80)
                 << "\nbytes_left : " << expr->bytes_left
                 << "\nheader_size: " << expr->header_size
                 << "\nstack_count: " << expr->stack_count
                 << "\nstack_size : " << expr->stack_size
                 << "\nbin_state  : " << BInStateStrings ()[expr->bin_state]
                 << "\nbout_state : " << BOutStateStrings ()[expr->bout_state]
                 << "\nnum_states : " << expr->num_states
                 << "\nheader_size: " << expr->header_size
                 << Line ('-', 80)
                 << expr->operand
                 << "\nheader     : " << Bsq (expr->header_start)
                 << Line ('-', 80)
                 << PrintExprStack (print, expr)
                 << Line ('~', 80);
}

#endif

}       //< namespace _

#undef PRINTF
#undef PUTCHAR
#endif  //> #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
