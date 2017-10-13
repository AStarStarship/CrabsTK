/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/automata.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io> 
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software 
             distributed under the License is distributed on an "AS IS" BASIS, 
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_AUTOMATA_H
#define CHINESE_ROOM_AUTOMATA_H

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
        |   Scan stack of Star**     |
        |----------------------------|
        |   Scan stack of uint_t**   |
     ^  |----------------------------|
     |  |       Automata Class       |
    0x0 |____________________________|
    @endcode
*/
struct Automata {
    byte          type,             /*< What type of Stack it is.
                                             -1 = interprocess no dynamic memory.
                                             0 = no dynamic memory.
                                             1 = dynamic memory.
                                             2 =  interprocess dynamic memory. */
                  num_members,      //< The current number of members of the 
                                    //< Star being verified.
                  first_member;     //< The current first member of the Star
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
    Operation   * device,           //< Star in the Star being verified.
                * operand;          //< Pointer to the Star this device is
                                    //< operating on.
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

/** Gets a pointer to the MonoidRx slot. */
KABUKI MonoidRx* AutomataRx (Automata* a);

/** Gets a pointer to the Monoid slot. */
KABUKI MonoidTx* AutomataTx (Automata* a);

/** Constructs a Stack with equal sized rx and tx slots.
    @param root The root-scope device. */
KABUKI Automata* AutomataInit (byte* buffer, uint_t buffer_size,
    uint_t stack_count, Operation* root = nullptr);

/** Gets the base address of the device stack. */
KABUKI Operation** AutomataDeviceStack (Automata* a);

/** Returns true if the Stack uses dynamic memory. */
KABUKI bool AutomataIsDynamic (Automata* a);

KABUKI byte* AutomataEndAddress (Automata* a);

/** Resets this Stack to the initial state. */
KABUKI ticket_t AutomataReset (Automata* a);

/** Attempts to push the Star at the given index of the current
    device control onto the stack.
    @return Returns nullptr upon success and a pointer to a string
    upon failure. */
KABUKI const Set* Push (Automata* a, Operation* d);

/** Attempts to pop an Star off the stack and returns a pointer to a
    string upon failure. */
KABUKI ticket_t AutomataPop (Automata* a);

/** Gets the base address of the state stack. */
KABUKI byte* AutomataStateStack (Automata* a);

/** Exits the current state. */
KABUKI ticket_t AutomataExitRxState (Automata* a);

/** Pushes the new state onto the automata stack. */
KABUKI ticket_t AutomataEnterRxState (Automata* a, RxState state);

/** Selects the given member for scanning.
    @warning Function does not check for null pointers. You have been warned. */
KABUKI ticket_t AutomataPushScanHeader (Automata* a, const uint_t* header);

/** Selects the given member for scanning.
    @warning Function does not check for null pointers. You have been warned. */
KABUKI ticket_t AutomataPushScanHeader (Automata* a,
    volatile const uint_t* header);

/** Pops a header off the scan stack. */
KABUKI ticket_t AutomataPopScanHeader (Automata* a);

/** Scans the next type header type. */
KABUKI void AutomataScanNextType (Automata* a);

/** Streams a tx byte. */
KABUKI byte AutomataStreamTxByte (Automata* a);

/** Scans the Rx buffer and marks the data as being ready to execute.
    @param io The Stack to scan. */
KABUKI void AutomataScan (Automata* a, Portal* input);

/** Returns true if the given Stack contains the given address. */
KABUKI bool AutomataContains (Automata* a, void* address);

/** Pushes a header onto the scan stack.*/
KABUKI ticket_t AutomataPushHeader (Automata* a, const uint_t* header);

/** Gets the base address of the header stack. */
KABUKI const uint_t* AutomataHeaderStack (Automata* a);

/** Closes the current expression and cues it for execution. */
KABUKI void AutomataCloseExpression (Automata* a);

/** Cancels the current expression. */
KABUKI void AutomataCancelExpression (Automata* a);

/** Cancels the current expression and writes zeros to the buffer. */
KABUKI void AutomataScrubExpression (Automata* a);

/** Calls the Read function for the Rx slot. */
KABUKI const Set* Read (Automata* a, const uint_t* esc, void** args);

/** Calls the Write function for the Tx slot. */
KABUKI const Set* Write (Automata* a, const uint_t* esc, void** args);

/** Calls the Write function for the Tx slot. */
KABUKI const Set* Write (Automata* a, const char* address,
                         const uint_t* esc, void** args);

/** Prints the given Automata to the console. */
KABUKI void AutomataPrint (Automata* a);

}       //< namespace _
#endif  //< CHINESE_ROOM_AUTOMATA_H
