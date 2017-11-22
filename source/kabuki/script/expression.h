/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/expression.h
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

#ifndef SCRIPT_EXPRESSION_H

#define SCRIPT_EXPRESSION_H

#include "set.h"
#include "portal.h"
#include "operation.h"
#include "bin.h"
#include "bout.h"

namespace _ {

/** A full-duplex Expression.
    Expressions (exprs or expr) must be word-aligned in order to run correctly
    so it's best to scan and word align the data types in the same sweep.

    Exprs are composed of one B-Input (Bin) and one B-Output (Bout) socket. The
    App/Driver/User writes to the end of the Tx buffer and the driver reads from
    the beginning. The user writes are synchronous and the driver reads are
    asynchronous.
    
    # Ring Buffer Streaming Diagram
    
    @code
          |>---------------------- Ring Buffer ------------------------->|
          ________________________________________________________________
    Bout  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Rx-->|>-Buffer->|
          |__________|___________________|____________________|__________|
          ________________________________________________________________
    Bin   |>-Buffer->|>-Async Portal Tx->|>-Sync User Writes->|>-Buffer->|
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
        |            +            |
        |            |            |
        |        Expression       |
        |=========================|
        |   Stack of Operand**    |
     +  |=========================|
     |  |    Expression struct    |
    0xN |=========================|
    @endcode
*/
struct Expression {

    /** List of Expression Error Operations. */
    typedef enum Errors {
        BufferOverflowError  = 0,
        BufferUnderflowError = 1,
        InvalidOpeartionError,
        StackOverflowError,
        LockedStateError,
        StringOverflowError,
        VarintOverflowError,
        ReadInvalidTypeError,
        RoomError
    } Error;

    enum {
        kMinStackSize = 16,         //< The size of the expr stack.
        kMinBufferSize = 2,         //< The minimum buffer size.
    };
    
    uint_t           size,          //< The offset to the Bout slot.
                     header_size,   //< The total size of the header.   
                     stack_count,   //< Number of Operands(string) on the stack.
                     stack_size,    //< Stack buffer size and 1/4 the state
                                    //< stack height.
                     type,          //< Current type being scanned.
                     num_states,    //< Number of states on the state stack.
                     bytes_left,    //< Countdown counter for parsing POD types.
                     params_left;   //< Height of header and cursors stacks.
    byte             bout_state,    //< Bout streaming state.
                     bin_state,     //< Slot streaming state.
                     last_bin_state,//< Last Bin state.
                     last_byte;     //< The last byte read.
    uint32_t         utf32_char;    //< The current UTF-32 char being scanned.
    hash16_t         hash;          //< Bin data verification hash.
    uint32_t         timeout_us ;   //< The timeout time in microseconds.
    time_us_t      last_time;     //< The last time the Stack was scanned.
    const Operation* result;        //< The result of the Expression.
                                    //< expr is operating on.
    uintptr_t        buffer_size,   //< The expression buffer_size.
                     buffer_left;   //< Num bytes left in the buffer.
    byte           * buffer;        //< Pointer to the word-aligned Expression.
    const char     * return_address;//< The return address.
    const uint_t   * header,        //< Pointer to the header being verified.
                   * header_start;
    Operand        * operand,       //< Current script Operand.
                   * root;          //< The root-level scope Operand.
};

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr,
                                Bin::Error error);

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr,
                                Bin::Error error,
                                const uint_t* header);

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr, 
                                Bin::Error error,
                                const uint_t* header,
                                byte offset);

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr,
                                Bin::Error error,
                                const uint_t* header,
                                byte offset,
                                uintptr_t* address);

/** Returns the text label of the Bin::Error.
KABUKI const char* ExpressionErrorString (Bin::Error error); */

/** Prints out an error report to the stdout.
    @param e The error type.
    @param params      The parameter header.
    @param param_index The index in the params where the error occurred.
    @param source      The source buffer address.
KABUKI void ExpressionPrintError (Expression::Error error, const uint_t* params, 
                                  byte param_index, void* source); */

/** Gets a pointer to the Bin slot. */
KABUKI uintptr_t* ExpressionBinAddress (Expression* expr);

/** Gets the expr's buffer. */
KABUKI byte* ExpressionBuffer (Expression* expr);

/** Gets a pointer to the Bin slot. */
KABUKI Bin* ExpressionBin (Expression* expr);

/** Gets a pointer to the Bout slot. */
KABUKI uintptr_t* ExpressionBoutAddress (Expression* expr);

/** Gets a pointer to the Bout slot. */
KABUKI Bout* ExpressionBout (Expression* expr);

/** Creates a Stack with equal sized rx and tx slots.
    @param root The root-scope device. */
KABUKI Expression* ExpressionInit (uintptr_t* buffer, uint_t buffer_size,
                                   uint_t stack_count, Operand* root,
                                   uintptr_t* unpacked_buffer, 
                                   uintptr_t unpacked_size);

/** Gets the base address of the device stack. */
inline Operand** ExpressionStack (Expression* expr) {
    return reinterpret_cast<Operand**> (&expr->root);
}

/** Returns true if the Stack uses dynamic memory. */
//KABUKI bool ExpressionIsDynamic (Expression* expr);

KABUKI uintptr_t* ExpressionEndAddress (Expression* expr);

/** Resets this Stack to the initial state. */
KABUKI const Operation* ExpressionReset (Expression* expr);

/** Attempts to push the Star at the given index of the current
    device control onto the stack.
    @return Returns nullptr upon success and a pointer to a char
            upon failure. */
KABUKI const Operation* Push (Expression* expr, Operand* op);

/** Attempts to pop an Star off the stack and returns a pointer to a
    char upon failure. */
KABUKI const Operation* Pop (Expression* expr);

/** Exits the current state. */
KABUKI byte ExpressionExitState (Expression* expr);

/** Sets the new state onto the expression stack. */
KABUKI const Operation* ExpressionSetState (Expression* expr,
                                            Bin::State state);

/** Saves the current bin_state and sets the bin_state to the new state. */
KABUKI const Operation* ExpressionEnterState (Expression* expr, 
                                              Bin::State state);

/** Streams a B-Output byte. */
KABUKI byte ExpressionStreamBout (Expression* expr);

/** Scans the Bout buffer and marks the data as being ready to execute.
    @param a The Stack to scan. */
KABUKI void ExpressionScan (Expression* expr, Portal* io);

/** Returns true if the given Stack contains the given address. */
KABUKI bool ExpressionContains (Expression* expr, void* address);

/** Pushes a header onto the scan stack.*/
KABUKI const Operation* ExpressionScanHeader (Expression* expr,
                                              const uint_t* header);

/** Gets the base address of the header stack. */
KABUKI const uint_t* ExpressionHeaderStack (Expression* expr);

/** Closes the current expr and cues it for execution. */
KABUKI void ExpressionClose (Expression* expr);

/** Cancels the current expr. */
KABUKI void ExpressionCancel (Expression* expr);

/** Cancels the current expr and writes zeros to the buffer. */
KABUKI void ExpressionClear (Expression* expr);

/** Rings the bell of the given address. */
KABUKI void ExpressionRingBell (Expression* expr, const char* address = "");

KABUKI void ExpressionAckBack (Expression* expr, const char* address = "");

/** Calls the Read function for the Bout slot. */
KABUKI bool Args (Expression* expr, const uint_t* params, void** args);

/** Calls the Write function for the Tx slot. */
KABUKI const Operation* Result (Expression* expr, const uint_t* params, 
                                void** args);

/** Prints the Expression stack to the std::cout */
KABUKI void ExpressionPrintStack (Expression* expr);

/** Prints the given Expression to the console. */
KABUKI void ExpressionPrint (Expression* expr);

}       //< namespace _
#endif  //< SCRIPT_EXPRESSION_H
