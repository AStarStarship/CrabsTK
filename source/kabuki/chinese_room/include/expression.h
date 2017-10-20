/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/expression.h
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

#ifndef CHINESE_ROOM_EXPRESSION_H
#define CHINESE_ROOM_EXPRESSION_H

#include "set.h"
#include "portal.h"
#include "operation.h"
#include "monoid.h"

namespace _ {

/** A universal all-in-one printer and scanner for slots in doors in Chinese 
    Rooms.
    A Stack connects two portals (@see ChineseRoom::Portal) between two
    rooms using rx and tx ring buffers as depicted below:
    # Ring Buffer Streaming Diagram
    @code
        |>---------------------- Ring Buffer ------------------------->|
        ________________________________________________________________
    Rx  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Rx-->|>-Buffer->|
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
        ______________________________
        |         Monoid             |
        |____________________________|
        |         Rx Buffer          |
        |____________________________|
        |  Scan stack of Operable**  |
        |----------------------------|
        |   Scan stack of uint_t**   |
     ^  |----------------------------|
     |  |      Expression Class      |
    0x0 |____________________________|
    @endcode
*/
struct Expression {
    byte          type,             /*< What type of Stack it is.
                                             -1 = interprocess no dynamic memory.
                                             0 = no dynamic memory.
                                             1 = dynamic memory.
                                             2 =  interprocess dynamic memory. */
                  num_members,      //< The current number of members of the 
                                    //< Star being verified.
                  first_member;     //< The current first op of the Star
                                    //< being verified.
    volatile byte rx_state,         //< Rx streaming state.
                  last_rx_state,    //< Last Rx state.
                  tx_state;         //< Monoid streaming state.
    hash16_t      hash;             //< Rx data verification hash.
    int16_t       timeout_us;       //< The timeout time.
    uint_t        bytes_left,       //< Countdown counter for parsing POD types.
                  tx_offset,        //< The offset to the Monoid slot.
                  rx_offset,        //< The offset to the Monoid slot.    
                  stack_count,      //< Number of Star(s) on the stack.
                  stack_size,       //< Stack buffer size and 1/4 the state
                                    //< stack height.
                  verify_count,     //< Height of header and cursors stacks.
                  type_index,       //< The index in the current type being
                                    //< scanned.
                  num_states;       //< Number of states on the state stack.
    timestamp_t   last_time;        //< The last time the Stack was scanned.
    Operable    * op,               //< Current Operable object being verified.
                * operand;          //< Pointer to the Operable object this 
                                    //< expression is operating on.
    const char  * return_address;   //< The return address.
    volatile const uint_t* header;  //< Pointer to the header being verified.
    const uint_t* headers;          //< First header ptr in the scan array.
};

enum {
    kMinStackSize = 16,             //< The size of the juggle stack.
    kMinBufferSize = 2,             //< The minimum buffer size.
};

/** List of the Stack Rx states. */
typedef enum RxStates {
    RxScanningAddressState = 0,     //< RxState 0: Scanning address.
    RxScanningArgsState,            //< RxState 1: Scanning arguments.
    RxScanningStringState,          //< RxState 2: Scanning STX.
    RxScanningVarintState,          //< RxState 3: Scanning varint.
    RxScanningPodState,             //< RxState 4: Scanning plain-old-data.
    RxHandlingErrorState,           //< RxState 5: Handling an error state.
    RxMemeberNotFoundErrorState,    //< RxState 6: Set not found.
    RxScanningHashState,            //< RxState 7: Stand the 32-bit hash.
    RxLockedState,                  //< RxState N: Idle state.
} RxState;

/** List of the Stack Tx states. */
typedef enum TxStates {
    TxWritingState = 0,             //< TxState 0: Most common state.
    TxLockedState,                  //< TxState 1: Locked.
} TxState;

/** Gets a a string for printing out the rx_state. */
KABUKI const char* RxStateString (byte state);

/** Gets a a string for printing out the tx_state. */
KABUKI const char* TxStateStrings (byte state);

/** Gets a pointer to the Bin slot. */
KABUKI Bin* ExpressionRx (Expression* expr);

/** Gets a pointer to the Monoid slot. */
KABUKI Bout* ExpressionTx (Expression* expr);

/** Constructs a Stack with equal sized rx and tx slots.
    @param root The root-scope device. */
KABUKI Expression* ExpressionInit (byte* buffer, uint_t buffer_size,
                               uint_t stack_count,
                               Operable* root = nullptr);

/** Gets the base address of the device stack. */
KABUKI Operable** ExpressionStack (Expression* expr);

/** Returns true if the Stack uses dynamic memory. */
KABUKI bool ExpressionIsDynamic (Expression* expr);

KABUKI byte* ExpressionEndAddress (Expression* expr);

/** Resets this Stack to the initial state. */
KABUKI ticket_t ExpressionReset (Expression* expr);

/** Attempts to push the Star at the given index of the current
    device control onto the stack.
    @return Returns nullptr upon success and a pointer to a string
    upon failure. */
KABUKI const Operation* Push (Expression* expr, Operable* op);

/** Attempts to pop an Star off the stack and returns a pointer to a
    string upon failure. 
KABUKI Operation* ExpressionPop (Expression* expr);*/

/** Gets the base address of the state stack. */
KABUKI byte* ExpressionStateStack (Expression* expr);

/** Exits the current state. */
KABUKI ticket_t ExpressionExitRxState (Expression* expr);

/** Pushes the new state onto the automata stack. */
KABUKI ticket_t ExpressionEnterRxState (Expression* expr, RxState state);

/** Selects the given op for scanning.
    @warning Function does not check for null pointers. You have been warned. */
KABUKI ticket_t ExpressionPushScanHeader (Expression* expr, const uint_t* header);

/** Selects the given op for scanning.
    @warning Function does not check for null pointers. You have been warned. */
KABUKI ticket_t ExpressionPushScanHeader (Expression* expr,
    volatile const uint_t* header);

/** Pops a header off the scan stack. */
KABUKI ticket_t ExpressionPopScanHeader (Expression* expr);

/** Scans the next type header type. */
KABUKI void ExpressionScanNextType (Expression* expr);

/** Streams a tx byte. */
KABUKI byte ExpressionStreamTxByte (Expression* expr);

/** Scans the Rx buffer and marks the data as being ready to execute.
    @param a The Stack to scan. */
KABUKI void ExpressionScan (Expression* expr, Portal* io);

/** Returns true if the given Stack contains the given address. */
KABUKI bool ExpressionContains (Expression* expr, void* address);

/** Pushes a header onto the scan stack.*/
KABUKI ticket_t ExpressionPushHeader (Expression* expr, const uint_t* header);

/** Gets the base address of the header stack. */
KABUKI const uint_t* ExpressionHeaderStack (Expression* expr);

/** Closes the current expression and cues it for execution. */
KABUKI void ExpressionCloseOperable (Expression* expr);

/** Cancels the current expression. */
KABUKI void ExpressionCancelOperable (Expression* expr);

/** Cancels the current expression and writes zeros to the buffer. */
KABUKI void ExpressionScrubOperable (Expression* expr);

/** Calls the Read function for the Rx slot. */
KABUKI const Operable* Read (Expression* expr, const uint_t* esc, void** args);

/** Calls the Write function for the Tx slot. */
KABUKI const Operable* Write (Expression* expr, const uint_t* esc, void** args);

/** Calls the Write function for the Tx slot. */
KABUKI const Operable* Write (Expression* expr, const char* address,
                         const uint_t* esc, void** args);

/** Prints the given Expression to the console. */
KABUKI void ExpressionPrint (Expression* expr);

}       //< namespace _
#endif  //< CHINESE_ROOM_EXPRESSION_H
