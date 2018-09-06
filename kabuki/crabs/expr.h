/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/expr.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>
#ifndef INCLUDED_CRABS_EXPR
#define INCLUDED_CRABS_EXPR
#if SEAM >= SEAM_0_0_4
// Dependencies:
#include "args.h"
#include "bin.h"
#include "bout.h"
#include "bsq.h"
#include "error.h"
#include "operand.h"
// End dependencies.

namespace _ {

/* A full-duplex Expr.
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
  /* List of Expr Error Ops. */
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
    kMinStackSize = 16,  //< Size of the expr stack.
    kMinBufferSize = 2,  //< Min buffer size.
  };

  uint_t size,           //< Offset to the BOut slot.
      header_size,       //< The total size of the header.
      stack_count,       //< Stack Operand count.
      stack_size,        //< Stack Operand buffer size.
      type,              //< Current type being scanned.
      num_states,        //< Number of states on the state stack.
      bytes_left,        //< Countdown counter for parsing POD types.
      params_left;       //< Height of header and cursors stacks.
  byte bout_state,       //< BOut streaming state.
      bin_state,         //< Slot streaming state.
      last_bin_state,    //< Last BIn state.
      last_byte;         //< Last byte read.
  wchar_t current_char;  //< Current Unicode char being scanned.
  hash16_t hash;         //< Packed BSQ hash.
  uint32_t timeout_us;   //< Timeout time in microseconds.
  Tme last_time;         //< Last time the Stack was scanned.
  const Op* result;      //< Result of the Expr.
  const uint_t *header,  //< Pointer to the header being verified.
      *header_start;     //< Start of the header being verified.
  Operand *operand,      //< Current Script Operand.
      *root;             //< Root-level scope Operand.
  Slot& args;            //< Arguments slot for running.
  Slot slot;             //< Slot for unpacking B-Sequences to.
};

/* Gets a pointer to the BIn slot. */
DLL uintptr_t* ExprBinAddress(Expr* expr);

/* Gets the expr's buffer. */
DLL char* ExprBuffer(Expr* expr);

/* Gets a pointer to the BIn slot. */
DLL BIn* ExprBIn(Expr* expr);

/* Gets a pointer to the BOut slot. */
DLL uintptr_t* ExprBOutAddress(Expr* expr);

/* Gets a pointer to the BOut slot. */
DLL BOut* ExprBOut(Expr* expr);

/* Creates a Stack with equal sized rx and tx slots.
    @param root The root-scope device.
    @param unpacked_buffer The word-aligned expression buffer.
    @param unpacked_size   Size of the unpacked buffer. */
DLL Expr* ExprInit(uintptr_t* buffer, uint_t buffer_size, uint_t stack_count,
                      Operand* root, uintptr_t* unpacked_buffer,
                      uintptr_t unpacked_size);

/* Gets the base address of the device stack. */
inline Operand** ExprStack(Expr* expr) {
  return reinterpret_cast<Operand**>(&expr->root);
}

/* Returns true if the Stack uses dynamic memory. */
// DLL bool ExprIsDynamic (Expr* expr);

DLL char* ExprEndAddress(Expr* expr);

/* Resets this Stack to the newial state. */
DLL const Op* ExprReset(Expr* expr);

/* Pushes the operand at the given index of the current
    device control onto the stack.
    @return Returns nil upon success and a pointer to a char
            upon failure. */
DLL const Op* Push(Expr* expr, Operand* operand);

/* Attempts to pop an Star off the stack and returns a pointer to a
    char upon failure. */
DLL const Op* Pop(Expr* expr);

/* Exits the current state. */
DLL byte ExprExitState(Expr* expr);

/* Sets the new state onto the expression stack.
DLL const Op* ExprSetState (Expr* expr, BInState state); */

/* Saves the current bin_state and sets the bin_state to the new state. */
DLL const Op* ExprEnterState(Expr* expr, BInState state);

/* Streams a B-Output byte. */
DLL byte ExprStreamBOut(Expr* expr);

/* Scans the BOut buffer and marks the data as being ready to execute.
    @param a The Stack to scan. */
DLL const Op* ExprUnpack(Expr* expr);  // , Portal* io);

/* Returns true if the given Stack contains the given address. */
DLL bool ExprContains(Expr* expr, void* address);

/* Pushes a header onto the scan stack.*/
DLL const Op* ExprScanHeader(Expr* expr, const uint_t* header);

/* Gets the base address of the header stack. */
DLL const uint_t* ExprHeaderStack(Expr* expr);

/* Closes the current expr and cues it for execution. */
DLL void ExprClose(Expr* expr);

/* Cancels the current expr. */
DLL void ExprCancel(Expr* expr);

/* Cancels the current expr and writes zeros to the buffer. */
DLL void ExprClear(Expr* expr);

/* Script Bell Op rings the bell of the given address. */
DLL void ExprRingBell(Expr* expr, const char* address = "");

/* Script Ack-back Op replies an ACK to a Bell Op. */
DLL void ExprAckBack(Expr* expr, const char* address = "");

/* Disconnects the expression. */
DLL const Op* ExprForceDisconnect(Expr* expr, Error error);

/* Reads the Expr args from the expr->slot.
inline const Op* ExprArgs (Expr* expr, const uint_t* params, void** args) {
   const char* cursor = ArgsParse (expr->args_cursor, expr->args_end,
                                   params, args);
   if (!cursor) {
       










   }
}*/

/* Pops the args off the Expr Args Stack. */
inline const Op* ExprArgs(Expr* expr, const uint_t* params, void** args) {
  ASSERT(params);
  ASSERT(args);
  Slot slot(ExprBIn(expr));
  return slot.Read(params, args);
}

/* Pops the args off the Expr Args Stack. */
inline const Op* ExprArgs(Expr* expr, const Op& op, void** args) {
  ASSERT(expr);
  ASSERT(args);
  Slot slot(ExprBIn(expr));
  return slot.Read(op.in, args);
}

/* Writes the result to the Expr.
    @param  expr The resulting expression.
    @param  op   The Operation with result B-Sequence header.
    @param  args Pointers to the B-Sequence args. */
inline const Op* ExprResult(Expr* expr, const Op& op, void** args) {
  return BOutWrite(ExprBOut(expr), op.out, args);
}

/* Writes the result to the Expr.
@param  expr The resulting expression.
@param  op   The Operation with result B-Sequence header.
@param  args Pointers to the B-Sequence args. */
inline const Op* ExprResult(Expr* expr, const uint_t* params, void** args) {
  if (!params) {
    return nullptr;
  }
  return BOutWrite(ExprBOut(expr), params, args);
}

/* Writes the result to the Expr.
@param  expr The resulting expression.
@param  op   The Operation with result B-Sequence header.
@param  args Pointers to the B-Sequence args. */
inline const Op* ExprResult(Expr* expr, const Op* op, void** args) {
  if (!op) {
    return nullptr;
  }
  return BOutWrite(ExprBOut(expr), op->out, args);
}

/* Returns the Operand header or writes it to the Expr.
    @param expr   The expression to write the Op header to.
    @param header The Op header.
    @return Returns the header if expr is nil. */
DLL const Op* ExprQuery(Expr* expr, const Op& header);

/* Returns the Op header or writes it to the Expr.
    @param expr   The expression to write the Op header to.
    @param op     The Op header.
    @return Returns the header if expr is nil. */
DLL const Op* ExprQuery(Expr* expr, const Op* op);

#if CRABS_TEXT
/* Prints the Expr stack to the Text buffer */
DLL Utf8& PrintExpr(Utf8& printer, Expr* expr);

/* Prints the Expr stack to the Text buffer */
DLL Utf8& PrintExprStack(Utf8& printer, Expr* expr);
#endif

}  // namespace _

#if CRABS_TEXT
/* Prints the given Expr to the Text buffer. */
inline _::Utf8& operator<<(_::Utf8& printer, _::Expr* expr) {
  return _::PrintExpr(printer, expr);
}
#endif

#endif  //< #if SEAM >= SEAM_0_0_4
#endif  //< INCLUDED_CRABS_EXPR
