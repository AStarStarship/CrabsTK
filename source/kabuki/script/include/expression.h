/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/expression.h
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

#ifndef KABUKI_SCRIPT_EXPRESSION_H
#define KABUKI_SCRIPT_EXPRESSION_H

#include "set.h"
#include "portal.h"
#include "operation.h"
#include "bin.h"
#include "bout.h"

namespace _ {

/** A input-output expr for a Slot in a Door.
    A Expression connects two portals (@see _::Portal) between two
    rooms using rx and tx ring buffers as depicted below:
    
    # Ring Buffer Streaming Diagram
    
    @code
        |>---------------------- Ring Buffer ------------------------->|
        ________________________________________________________________
    Bout  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Bout-->|>-Buffer->|
        |__________|___________________|____________________|__________|
        ________________________________________________________________
    Tx  |>-Buffer->|>-Async Portal Tx->|>-Sync User Writes->|>-Buffer->|
        |__________|___________________|____________________|__________|
    @endcode
    
    The Stack needs to run as fast as possible, so no virtual members are
    allowed in that class. Portals are created using an Star and Portal
    interface. The Stack needs to have an object stack, which requires
    three stacks of pointers: 2 for scanning the headers for sanitizing input,
    and another Star pointer stack for running the sequences.
    
    # Stack Memory Layout
    
    @code
         ______________________
        |       Expression     |
        |           |          |
        |           v          |
        |======================|
        |         Buffer       |
        |======================|
        |           ^          |
        |           |          |
        |  B-Sequence Result   |
        |======================|
        |  Stack of Operand**  |
        |======================|
        |  Stack of const uint_t** |
     ^  |======================|
     |  |  Expression struct   |
    0x0 |______________________|
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

    /** List of Finite Expression States. */

    typedef enum States {
        DisconnectedState = 0,
        ScanningAddressState,
        ScanningStringState,
        ScanningArgsState,
        ScanningPodState,
        HandlingErrorState,
        ScanningVarintState,
        LockedState,
    } State;

    enum {
        kMinStackSize = 16,         //< The size of the expr stack.
        kMinBufferSize = 2,         //< The minimum buffer size.
    };
    
    uint_t        size,             //< The offset to the Bout slot.
                  header_size,      //< The total size of the header.   
                  stack_count,      //< Number of Operands(string) on the stack.
                  stack_size,       //< Stack buffer size and 1/4 the state
                                    //< stack height.
                  verify_count,     //< Height of header and cursors stacks.
                  type_index,       //< The index in the current type being
                                    //< scanned.
                  num_states,       //< Number of states on the state stack.
                  bytes_left;       //< Countdown counter for parsing POD types.
    byte          type;             /*< What type of Expression it is.
                                        -1 = interprocess no dynamic memory.
                                        0 = no dynamic memory.
                                        1 = dynamic memory.
                                        2 =  interprocess dynamic memory. */
    volatile byte bout_state,       //< Bout streaming state.
                  last_rx_state,    //< Last Bout state.
                  bin_state;        //< Slot streaming state.
    int16_t       reserved1;
    int           num_ops,          //< The number of operand.
                  first_op;         //< The first operation of the current Operand.
                                    //< being verified.
    hash16_t      hash;             //< Bin data verification hash.
    int16_t       timeout_us;       //< The timeout time.
    timestamp_t   last_time;        //< The last time the Stack was scanned.
    Operand     * operand,          //< Current Operand object being verified.
                * result;           //< Pointer to the Operand object this 
                                    //< expr is operating on.
    const char  * return_address;   //< The return address.
    volatile const uint_t* header;      //< Pointer to the header being verified.
    const uint_t    * headers;          //< First header ptr in the scan array.
};

/** Gets a a char for printing out the bin_state. */
KABUKI const char* ExpressionStateString (Expression::State state);

/** Used to return an erroneous result from a B-Input.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr,
                                Expression::Error error);

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr,
                                Expression::Error error,
                                const uint_t* header);

/** Used to return an erroneous result from a B-Input.
    @param  expr    The source Expression.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
KABUKI const Operation* Result (Expression* expr, 
                                Expression::Error error,
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
                                Expression::Error error,
                                const uint_t* header,
                                byte offset,
                                byte* address);

/** Returns the text label of the Expression::Error. */
KABUKI const char* ExpressionErrorString (Expression::Error error);

/** Prints out an error report to the stdout.
    @param e The error type.
    @param params      The parameter header.
    @param param_index The index in the params where the error occurred.
    @param source The source buffer address. */
KABUKI void ExpressionPrintError (Expression::Error error, const uint_t* params, 
                                  byte param_index, void* source);

/** Gets a pointer to the Bin slot. */
KABUKI byte* ExpressionBinAddress (Expression* expr);

/** Gets a pointer to the Bin slot. */
KABUKI Bin* ExpressionBin (Expression* expr);

/** Gets a pointer to the Bout slot. */
KABUKI byte* ExpressionBoutAddress (Expression* expr);

/** Gets a pointer to the Bout slot. */
KABUKI Bout* ExpressionBout (Expression* expr);

/** Creates a Stack with equal sized rx and tx slots.
    @param root The root-scope device. */
KABUKI Expression* ExpressionInit (byte* buffer, uint_t buffer_size,
                                   uint_t stack_count,
                                   Operand* root = nullptr);

/** Gets the base address of the device stack. */
KABUKI Operand** ExpressionStack (Expression* expr);

/** Returns true if the Stack uses dynamic memory. */
KABUKI bool ExpressionIsDynamic (Expression* expr);

KABUKI byte* ExpressionEndAddress (Expression* expr);

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

/** Gets the base address of the state stack. */
KABUKI byte* ExpressionStates (Expression* expr);

/** Exits the current state. */
KABUKI const Operation* ExpressionExitState (Expression* expr);

/** Pushes the new state onto the expression stack. */
KABUKI const Operation* ExpressionEnterState (Expression* expr, 
                                              Expression::State state);

/** Selects the given op for scanning.

    @warning Function does not check for null pointers. You have been warned. */
KABUKI const Operation* ExpressionPushScanHeader (Expression* expr, const uint_t* header);

/** Selects the given op for scanning.

    @warning Function does not check for null pointers. You have been warned. */
KABUKI const Operation* ExpressionPushScanHeader (Expression* expr,
                                                  volatile const uint_t* header);

/** Pops a header off the scan stack. */
KABUKI const Operation* ExpressionPopScanHeader (Expression* expr);

/** Scans the next type header type. */
KABUKI void ExpressionScanNextType (Expression* expr);

/** Streams a B-Output byte. */
KABUKI byte ExpressionStreamBout (Expression* expr);

/** Scans the Bout buffer and marks the data as being ready to execute.

    @param a The Stack to scan. */
KABUKI void ExpressionScan (Expression* expr, Portal* io);

/** Returns true if the given Stack contains the given address. */
KABUKI bool ExpressionContains (Expression* expr, void* address);

/** Pushes a header onto the scan stack.*/
KABUKI const Operation* ExpressionPushHeader (Expression* expr, const uint_t* header);

/** Gets the base address of the header stack. */
KABUKI const uint_t* ExpressionHeaderStack (Expression* expr);

/** Closes the current expr and cues it for execution. */
KABUKI void ExpressionClose (Expression* expr);

/** Cancels the current expr. */
KABUKI void ExpressionCancel (Expression* expr);

/** Cancels the current expr and writes zeros to the buffer. */
KABUKI void ExpressionClear (Expression* expr);

/** Calls the Read function for the Bout slot. */
KABUKI const Operation* ExpressionArgs (Expression* expr, const uint_t* params,
                                        void** args);

/** Calls the Write function for the Tx slot. */
KABUKI const Operation* Result (Expression* expr, const uint_t* params, void** args);

/** Prints the given Expression to the console. */
KABUKI void ExpressionPrint (Expression* expr);

}       //< namespace _
#endif  //< KABUKI_SCRIPT_EXPRESSION_H
