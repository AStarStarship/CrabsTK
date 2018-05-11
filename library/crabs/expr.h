/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/expr.h
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

#ifndef HEADER_FOR_CRABS_EXPR
#define HEADER_FOR_CRABS_EXPR

#include "operand.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4

#include "bout.h"
#include "bin.h"
#include "error.h"
#include "args.h"
#include "bsq.h"

namespace _ {

/** A full-duplex Expr.
    Exprs (expr(s)) must be word-aligned in order to run correctly
    so it's best to scan and word align the data types in the same sweep.

    Exprs are composed of one B-Input (BIn) and one B-Output (BOut) socket. The
    App/Driver/User writes to the end of the Tx buffer and the driver reads from
    the beginning. The user writes are synchronous and the driver reads are
    asynchronous.
    
    # Ring Buffer Streaming Diagram
    
    @code
          |>---------------------- Ring Buffer ------------------------->|
          ________________________________________________________________
    BOut  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Rx-->|>-Buffer->|
          |__________|___________________|____________________|__________|
          ________________________________________________________________
    BIn   |>-Buffer->|>-Async Portal Tx->|>-Sync User Writes->|>-Buffer->|
          |__________|___________________|____________________|__________|
    @endcode
    
    Almost all block of memory in Script has something that grows up and another
    that grows down.
    
    # Stack Memory Layout
    
    @code
        |=========================|
        |     Evaluated Result    |
        |            |            |
        |            v            |
        |=========================|
        |          Buffer         |
        |=========================|
        |          Slot           |
        |=========================|
        |   Stack of Operand**    |
     +  |=========================|
     |  |       Expr struct       |
    0xN |=========================|
    @endcode
*/
struct Expr {

    /** List of Expr Error Ops. */
    typedef enum Errors {
        kErrorBufferOverflow = 0,
        kErrorBufferUnderflow,
        kErrorInvalidOperand,
        kErrorStackOverflow,
        kErrorLockedState,
        kErrorTextOverflow,
        kErrorVarintOverflow,
        kErrorReadInvalidType,
        kErrorImplementation
    } Error;

    enum {
        kMinStackSize = 16,       //< Size of the expr stack.
        kMinBufferSize = 2,       //< Min buffer size.
    };
    
    uint_t        size,           //< Offset to the BOut slot.
                  header_size,    //< The total size of the header.
                  stack_count,    //< Stack Operand count.
                  stack_size,     //< Stack Operand buffer size.
                  type,           //< Current type being scanned.
                  num_states,     //< Number of states on the state stack.
                  bytes_left,     //< Countdown counter for parsing POD types.
                  params_left;    //< Height of header and cursors stacks.
    byte          bout_state,     //< BOut streaming state.
                  bin_state,      //< Slot streaming state.
                  last_bin_state, //< Last BIn state.
                  last_byte;      //< Last byte read.
    wchar_t       current_char;   //< Current Unicode char being scanned.
    hash16_t      hash;           //< Packed BSQ hash.
    uint32_t      timeout_us;     //< Timeout time in microseconds.
    time_us_t     last_time;      //< Last time the Stack was scanned.
    const Op    * result;         //< Result of the Expr.
    const uint_t* header,         //< Pointer to the header being verified.
                * header_start;   //< Start of the header being verified.
    Operand     * operand,        //< Current Script Operand.
                * root;           //< Root-level scope Operand.
    Slot        & args;           //< Arguments slot for running.
    Slot          slot;           //< Slot for unpacking B-Sequences to.
};

/** Gets a pointer to the BIn slot. */
KABUKI uintptr_t* ExprBinAddress (Expr* expr);

/** Gets the expr's buffer. */
KABUKI char* ExprBuffer (Expr* expr);

/** Gets a pointer to the BIn slot. */
KABUKI BIn* ExprBIn (Expr* expr);

/** Gets a pointer to the BOut slot. */
KABUKI uintptr_t* ExprBOutAddress (Expr* expr);

/** Gets a pointer to the BOut slot. */
KABUKI BOut* ExprBOut (Expr* expr);

/** Creates a Stack with equal sized rx and tx slots.
    @param root The root-scope device.
    @param unpacked_buffer The word-aligned expression buffer.
    @param unpacked_size   Size of the unpacked buffer. */
KABUKI Expr* ExprInit (uintptr_t* buffer, uint_t buffer_size,
                       uint_t stack_count, Operand* root,
                       uintptr_t* unpacked_buffer, uintptr_t unpacked_size);

/** Gets the base address of the device stack. */
inline Operand** ExprStack (Expr* expr) {
    return reinterpret_cast<Operand**> (&expr->root);
}

/** Returns true if the Stack uses dynamic memory. */
//KABUKI bool ExprIsDynamic (Expr* expr);

KABUKI char* ExprEndAddress (Expr* expr);

/** Resets this Stack to the newial state. */
KABUKI const Op* ExprReset (Expr* expr);

/** Pushes the operand at the given index of the current
    device control onto the stack.
    @return Returns nil upon success and a pointer to a char
            upon failure. */
KABUKI const Op* Push (Expr* expr, Operand* operand);

/** Attempts to pop an Star off the stack and returns a pointer to a
    char upon failure. */
KABUKI const Op* Pop (Expr* expr);

/** Exits the current state. */
KABUKI byte ExprExitState (Expr* expr);

/** Sets the new state onto the expression stack.
KABUKI const Op* ExprSetState (Expr* expr, BInState state); */

/** Saves the current bin_state and sets the bin_state to the new state. */
KABUKI const Op* ExprEnterState (Expr* expr, BInState state);

/** Streams a B-Output byte. */
KABUKI byte ExprStreamBOut (Expr* expr);

/** Scans the BOut buffer and marks the data as being ready to execute.
    @param a The Stack to scan. */
KABUKI const Op* ExprUnpack (Expr* expr);// , Portal* io);

/** Returns true if the given Stack contains the given address. */
KABUKI bool ExprContains (Expr* expr, void* address);

/** Pushes a header onto the scan stack.*/
KABUKI const Op* ExprScanHeader (Expr* expr, const uint_t* header);

/** Gets the base address of the header stack. */
KABUKI const uint_t* ExprHeaderStack (Expr* expr);

/** Closes the current expr and cues it for execution. */
KABUKI void ExprClose (Expr* expr);

/** Cancels the current expr. */
KABUKI void ExprCancel (Expr* expr);

/** Cancels the current expr and writes zeros to the buffer. */
KABUKI void ExprClear (Expr* expr);

/** Script Bell Op rings the bell of the given address. */
KABUKI void ExprRingBell (Expr* expr, const char* address = "");

/** Script Ack-back Op replies an ACK to a Bell Op. */
KABUKI void ExprAckBack (Expr* expr, const char* address = "");

/** Disconnects the expression. */
KABUKI const Op* ExprForceDisconnect (Expr* expr, Error error);

/** Reads the Expr args from the expr->slot. 
inline const Op* ExprArgs (Expr* expr, const uint_t* params, void** args) {
   const char* cursor = ArgsParse (expr->args_cursor, expr->args_end,
                                   params, args);
   if (!cursor) {
       
   }
}*/

/** Pops the args off the Expr Args Stack. */
inline const Op* ExprArgs (Expr* expr, const uint_t* params,
                           void** args) {
    assert (params);
    assert (args);
    Slot slot (ExprBIn (expr));
    return slot.Read (params, args);
}

/** Pops the args off the Expr Args Stack. */
inline const Op* ExprArgs (Expr* expr, const Op& op,
                           void** args) {
    assert (expr);
    assert (args);
    Slot slot (ExprBIn (expr));
    return slot.Read (op.in, args);
}

/** Writes the result to the Expr.
    @param  expr The resulting expression.
    @param  op   The Operation with result B-Sequence header.
    @param  args Pointers to the B-Sequence args. */
inline const Op* ExprResult (Expr* expr, const Op& op, void** args) {
    return BOutWrite (ExprBOut (expr), op.out, args);
}

/** Writes the result to the Expr.
@param  expr The resulting expression.
@param  op   The Operation with result B-Sequence header.
@param  args Pointers to the B-Sequence args. */
inline const Op* ExprResult (Expr* expr, const uint_t* params, void** args) {
    if (!params) {
        return nullptr;
    }
    return BOutWrite (ExprBOut (expr), params, args);
}

/** Writes the result to the Expr.
@param  expr The resulting expression.
@param  op   The Operation with result B-Sequence header.
@param  args Pointers to the B-Sequence args. */
inline const Op* ExprResult (Expr* expr, const Op* op, void** args) {
    if (!op) {
        return nullptr;
    }
    return BOutWrite (ExprBOut (expr), op->out, args);
}

/** Returns the Operand header or writes it to the Expr.
    @param expr   The expression to write the Op header to.
    @param header The Op header.
    @return Returns the header if expr is nil. */
KABUKI const Op* ExprQuery (Expr* expr, const Op& header);

/** Returns the Op header or writes it to the Expr.
    @param expr   The expression to write the Op header to.
    @param op     The Op header.
    @return Returns the header if expr is nil. */
KABUKI const Op* ExprQuery (Expr* expr, const Op* op);

#if USING_PRINTER
/** Prints the Expr stack to the Text buffer */
KABUKI Printer& PrintExpr (Printer& printer, Expr* expr);

/** Prints the Expr stack to the Text buffer */
KABUKI Printer& PrintExprStack (Printer& printer, Expr* expr);
#endif

}       //< namespace _

#if USING_PRINTER
/** Prints the given Expr to the Text buffer. */
inline _::Printer& operator<< (_::Printer& printer, _::Expr* expr) {
    return PrintExpr (printer, expr);
}
#endif

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
#endif  //< HEADER_FOR_CRABS_EXPR
