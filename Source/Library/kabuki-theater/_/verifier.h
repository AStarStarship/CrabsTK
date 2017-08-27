/** The Chinese Room
    @version 0.x
    @file    /.../verifier.h
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

#ifndef CHINESEROOM_VERIFIER_H
#define CHINESEROOM_VERIFIER_H

#include "device.h"

namespace _ {

/** A universal all-in-one printer and scanner for rx and tx slots.
    A Verifier connects two portals (@see ChineseRoom::Portal) between two
    rooms using rx and tx ring buffers as depicted below:
    # Ring Buffer Streaming Diagram
    @code
        |>-------------------- Ring Buffer --------------------------->|
        ________________________________________________________________
    Rx  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Rx-->|>-Buffer->|
        |__________|___________________|____________________|__________|
        ________________________________________________________________
    Tx  |>-Buffer->|>-Async Portal Tx->|>-Sync User Writes->|>-Buffer->|
        |__________|___________________|____________________|__________|
    @endcode
    The Verifier needs to run as fast as possible, so no virtual members are
    allowed in that class. Portals are created using an Device and Portal
    interface. The Verifier needs to have an object stack, which requires
    three stacks of pointers: 2 for scanning the headers for sanitizing input,
    and another Device* stack for running the sequences.
    # Verifier Memory Layout
    @code
        ______________________________
        |         Unityper           |
        |____________________________|
        |         Rx Buffer          |
        |____________________________|
        |   Scan Stack of Device**   |
        |----------------------------|
        |   Scan Stack of uint_t**   |
     ^  |----------------------------|
     |  |       Verifier Class       |
    0x0 |____________________________|
    @endcode
*/
struct Verifier {
    byte type,                      /*< What type of Verifier it is.
                                            -1 = interprocess no dynamic memory.
                                            0 = no dynamic memory.
                                            1 = dynamic memory.
                                            2 =  interprocess dynamic memory. */
         num_members,               //< The current number of members of the 
                                    //< Device being verified.
         first_member;              //< The current first member of the Device
                                    //< being verified.
    volatile byte rx_state,         //< Rx streaming state.
                  last_rx_state,    //< Last Rx state.
                  tx_state;         //< Unityper streaming state.
    hash16_t hash;                  //< Rx data verification hash.
    int16_t  timeout_us;            //< The timeout time.
    uint_t bytes_left,              //< Countdown counter for parsing POD types.
           tx_offset,               //< The offset to the Unityper slot.
           rx_offset,               //< The offset to the Unityper slot.    
           stack_height,            //< Number of Device(s) on the stack.
           stack_size,              //< Stack buffer size.
           num_verifying,           //< Height of header and cursors stacks.
           type_index,              //< The index in the current type being 
                                    //< scanned.
           num_states;              //< Number of states on the state stack.
    timestamp_t last_time;          //< The last time the Verifier was scanned.
    Device* device,                 //< Device in the Device being verified.
          * operand;                //< Pointer to the Device this device is 
                                    //< operating on.
    const char* return_address;     //< The return address.
    volatile const uint_t* header;  //< Pointer to the header being verified.
    const uint_t* headers;          //< First header ptr in the scan array. 
};

enum {
    kMinStackSize = 16,             //< The size of the juggle stack.
    kMinBufferSize = 2,             //< The minimum buffer size.
};

/** List of the Verifier Rx states. */
typedef enum RxStates {
    RxVerifyingAddressState = 0,    //< RxState 0: Verifying address.
    RxVerifyingArgsState,           //< RxState 1: Verifying arguments.
    RxVerifyingStringState,         //< RxState 2: Verifying STX.
    RxVerifyingVarintState,         //< RxState 3: Verifying varint.
    RxVerifyingPodState,            //< RxState 4: Verifying plain-old-data.
    RxHandlingErrorState,           //< RxState 5: Handling an error state.
    RxMemeberNotFoundErrorState,    //< RxState 6: Member not found.
    RxVerifyingHashState,           //< RxState 7: Stand the 32-bit hash.
    RxLockedState,                  //< RxState N: Idle state.
} RxState;

/** List of the Verifier Tx states. */
typedef enum TxStates {
    TxWritingState = 0,             //< TxState 0: Most common state.
    TxLockedState,                  //< TxState 1: Locked.
} TxState;

/** Gets a a string for printing out the rx_state. */
INLINE const char* RxStateString (byte state) {
    static const char* strings[] = {
        "RxVerifyingStringState",
        "RxVerifyingVarIntState",
        "RxVerifyingAddressState",
        "RxVerifyingPodState",
        "RxVerifyingArgsState",
        "RxHandlingErrorState",
        "RxMemeberNotFoundErrorState",
        "RxVerifyingHashState",
        "RxLockedState" };
    if (state >= RxLockedState)
        return strings[RxLockedState];
    return strings[state];
}

/** Gets a a string for printing out the tx_state. */
INLINE const char* TxStateStrings (byte state) {
    static const char* strings[] = {
        "TxWritingState",
        "TxLockedState"
    };
    if (state >= TxLockedState)
        return strings[TxLockedState];
    return strings[state];
}

/*< Gets a pointer to the Rx slot. */
INLINE Uniprinter* VerifierRx (Verifier* io) {
    return io == nullptr ? nullptr :
           reinterpret_cast<Uniprinter*>(reinterpret_cast<byte*>(io) + 
                                         io->rx_offset);
}

/*< Gets a pointer to the Unityper slot. */
INLINE Unityper* VerifierTx (Verifier* io) {
    return io == nullptr ? nullptr :
           reinterpret_cast<Unityper*>(reinterpret_cast<byte*>(io) +
                                       io->tx_offset);
}
    
/** Constructs a Verifier with equal sized rx and tx slots.
    @param root The root-scope device. */
INLINE Verifier* VerifierInit (byte* buffer, uint_t buffer_size,
                               uint_t stack_height, Device* root = nullptr) {
    if (buffer == nullptr)
        return nullptr;
    if (buffer_size < kMinBufferSize)
        return nullptr;
    if (stack_height == 0) stack_height = 1;    //< Minimum stack size.

    Verifier* io = reinterpret_cast<Verifier*> (buffer);

    uint_t total_stack_size = (stack_height - 1) * (2 * sizeof (void*));
    // Calculate the size of the Unityper and Verifier.
    uint_t size = (buffer_size - sizeof (Verifier) -
                   total_stack_size + 1) >> 1;  // >>1 to divide by 2
    io->type = 0;
    io->rx_state = RxLockedState;
    io->tx_state = 0;
    io->stack_height = 0;
    io->num_verifying = 0;
    io->stack_size = stack_height;
    io->num_states = 0;
#if DEBUG_CHINESEROOM
    printf ("\nInitializing Verifier:\nsizeof (Verifier): %u\n "
            "(stack_height * (2 * sizeof (void*))): %u\nstack_height: "
            "%u buffer_size: %u size: %u\n!!!\nstack_height: %u &stack_height: "
            "0x%p\n",
            sizeof (Verifier), (stack_height * 
                                (2 * sizeof (void*))), stack_height, 
                                buffer_size, size, stack_height, &stack_height);
#endif //< DEBUG_CHINESEROOM
    io->bytes_left = 0;
    uint_t offset = sizeof (Verifier) + total_stack_size -
        sizeof (void*),
        rx_offset = sizeof (Uniprinter) + total_stack_size + offset;
    io->rx_offset = rx_offset;
    io->tx_offset = rx_offset + size;
    io->header = 0;
    io->headers = 0;
    printf ("\n\n!!!\nroot: 0x%p\n", root);
    io->device = root;
    printf ("io->device: 0x%p\n", io->device);
    UniprinterInit (VerifierRx (io), size);
    UnityperInit (VerifierTx (io), size);
    return io;
}

/** Gets the base address of the device stack. */
INLINE Device** VerifierDeviceStack (Verifier* io) {
    auto a = reinterpret_cast<byte*> (io) + sizeof (Verifier) +
             io->stack_height * io->stack_size * sizeof (const uint_t*);
    return reinterpret_cast<Device**> (a);
}

/** Returns true if the Verifier uses dynamic memory. */
INLINE bool VerifierIsDynamic (Verifier* io) {
    return io->type % 2 == 1;
}

INLINE byte* VerifierEndAddress (Verifier* io) {
    return UniprinterEndAddress (VerifierRx (io));
}

/** Resets this Verifier to the initial state. */
INLINE ticket_t VerifierReset (Verifier* io) {
    return 0;
}

/** Attempts to push the Device at the given index of the current
    device control onto the stack.
    @return Returns nullptr upon success and a pointer to a string
    upon failure. */
INLINE const Member* Push (Verifier* io, Device* d) {
    if (io == nullptr)
        return d->Op (0, nullptr);  //< Return d's header.
    if (d == nullptr)
        return reinterpret_cast<const Member*> (Report (NullDevicePushError));
    if (io->stack_height >= io->stack_size)
        return reinterpret_cast<const Member*> (Report (StackOverflowError));
    VerifierDeviceStack (io)[io->stack_height++] = d; 
    return 0;
}

/** Attempts to pop an Device off the stack and returns a pointer to a
    string upon failure. */
INLINE ticket_t VerifierPop (Verifier* io) {
    if (io->stack_height == 0)
        return Report (TooManyPopsError);
    io->device = VerifierDeviceStack (io)[--io->stack_height];
    return 0;
}

/** Gets the base address of the state stack. */
INLINE byte* VerifierStateStack (Verifier* io) {
    return reinterpret_cast<byte*> (io) + sizeof (Verifier);
}

/** Exits the current state. */
INLINE ticket_t VerifierExitRxState (Verifier* io) {
    auto a = io->stack_height;
    if (a == 0)
        return Report (TooManyPopsError);
    io->rx_state = VerifierStateStack (io)[--a];
    io->stack_height = a;
    return 0;
}

/** Pushes the new state onto the verifier stack. */
INLINE ticket_t VerifierEnterRxState (Verifier* io, RxState state) {
    
    if (state >= RxLockedState)
        return Report (InvalidRxStateError);
    auto a = io->stack_height;
    if (a >= io->stack_size)
        return Report (StackOverflowError);
    VerifierStateStack (io)[a] = io->rx_state;
    a = io->stack_height + 1;
    io->rx_state = state;
    return 0;
}

/** Selects the given member for scanning.
    @warning Function does not check for null pointers. You have been warned. */
INLINE ticket_t VerifierPushScanHeader (Verifier* io, const uint_t* header) {
    if (header == nullptr)
        return Report (NullPointerError, header, VerifierTx (io));
    uint_t num_verifying = io->num_verifying,
           stack_size    = io->stack_size;
    io->type_index = *header++;
    if (num_verifying >= stack_size)
        return Report (StackOverflowError, header, VerifierTx (io));
    
    // Move the current header to the scan stack
    const uint_t* current_header = const_cast<const uint_t*> (io->header);
    io->header = header;
    const uint_t** headers = (const uint_t**)&io->headers;
    headers[stack_size] = current_header;
    io->num_verifying = num_verifying + 1;
    return 0;
}

/** Selects the given member for scanning.
    @warning Function does not check for null pointers. You have been warned. */
INLINE ticket_t VerifierPushScanHeader (Verifier* io,
                                        volatile const uint_t* header) {
    const uint_t** headers;
    uint_t num_verifying = io->num_verifying;
    if (num_verifying >= io->stack_size)
        return Report (StackOverflowError);
    
    headers = (const uint_t**)io->headers;
    VerifierExitRxState (io);
    io->header = *headers;
    num_verifying = num_verifying;
    return 0;
}

/** Pops a header off the scan stack. */
INLINE ticket_t VerifierPopScanHeader (Verifier* io) {
    uint_t num_verifying = io->num_verifying;
    if (num_verifying == 0)
        return Report (TooManyPopsError);
    
    num_verifying = num_verifying - 1;
    return 0;
}

/** Scans the next type header type. */
INLINE void VerifierScanNextType (Verifier* io) {
    uint_t* header = const_cast<uint_t*> (io->header);
    if (header == nullptr) {
        VerifierEnterRxState (io, RxVerifyingArgsState);
        return;
    }

    uint_t type = *header;
    if (type == NIL) {  // Done scanning args.
        VerifierPopScanHeader (io);
        return;
    }
    ++header;
    io->header = header;
    io->rx_state = io->last_rx_state;
    //type = *header;
}

/** Streams a tx byte. */
INLINE byte VerifierStreamTxByte (Verifier* io) {
    return UnityperStreamByte (VerifierTx (io));
}

/** Scans the Rx buffer and marks the data as being ready to execute.
    @param io The Verifier to scan. */
INLINE void VerifierScan (Verifier* io) {
    // It is fastest do a few branches as possible, and to compare to zero 
    // as much as possible. In order to optimize the switch, it is fastest 
    // to decrement the bytes_left, compare it to zero, and only process 
    // events. When the data comes in it will be packed and not word 
    // aligned. In order for most CPU to use the data, the data, we will 
    // need to be realigned.
    uint_t        size,
                  space_left,
                  length,
                  type;
    byte          rx_state,
                  b;
    ticket_t      result;
    hash16_t      hash;
    timestamp_t   timestamp,
                  delta_t;
    Device      * device;       //< The current Device.
    const Member* m;            //< The current Member.
    const uint_t* header;       //< The current Member header being verified.
    Unityper    * tx;
    byte        * begin,
                * end,
                * start,
                * stop;

    if (io == nullptr) {
        std::cout << "io null\n";
        return;
    }

    rx_state = io->rx_state;
    tx        = VerifierTx (io);
    size      = tx->size;
    hash      = io->hash;
    timestamp = TimestampNow ();
    delta_t   = timestamp - io->last_time;

    if (delta_t <= io->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch
            delta_t *= -1;
    }

    begin = UniprinterBaseAddress (VerifierRx (io));
    end   = begin + size;
    start = begin + tx->start;
    stop  = begin + tx->stop;
    space_left  = RingBufferSpace (start, stop, size);
    length = size - space_left + 1;

    printf ("\n\n| Verifying address 0x%p:\n| rx_state: %s\n| length: %u\n", io,
            RxStateString (rx_state), length);

    // Manually load first byte:
    b = UnityperStreamByte (tx);
    hash = Hash16 (b, hash);
    *start = b;
    ++start;
    for (; length > 0; --length) {
        // Process the rest of the bytes in a loop to reduce setup overhead.
        if (rx_state == RxVerifyingStringState) {
            PrintDebug ("RxVerifyingStringState");

            if (io->bytes_left == 0) {
                PrintDebug ("Done parsing string.");
                Report (RxStringBufferOverflowError, io->header, start);
                return;
            }
            // Hash byte.

            // Check if string terminated.
            if (b == 0) {
                PrintDebug ("string terminated.");
                // Check if there is another argument to scan.
                // 
                VerifierExitRxState (io);
                return;
            }
            PrintDebug ("b != 0\n");
            --io->bytes_left;
            return;
        } else if (rx_state == RxVerifyingVarintState) {
            // When verifying a varint, there is a max number of bytes for the
            // type (3, 5, or 9) but the varint may be complete before this
            // number of bytes. We're just basically counting down and looking
            // for an overflow situation.

            PrintDebug ("RxVerifyingVarintState.");
            // Hash byte.

            if (io->bytes_left == 1) {
                PrintDebug ("Checking last byte for error.\n");

                // @warning I am not current saving the offset. I'm not sure 
                //          what to do here. The header class uses a variadic 
                //          template, I'm kind of tempted to switch to an int 
                //          type for the headers so I can just use a normal 
                //          array bracket initializer. The other option is to 
                //          add 32 to the first byte.

                if ((b >> 7) != 1) {
                    Report (VarintOverflowError, io->header, start);
                    VerifierEnterRxState (io, RxHandlingErrorState);
                    return;
                }

                return;
            }
            --io->bytes_left;
            return;
        } else if (rx_state == RxVerifyingAddressState) {
            // When verifying an address, there is guaranteed to be an
            // io->device set. We are just looking for null return values
            // from the Do (byte, Verifier*): const Member* function, 
            // pushing Device(s) on to the Device stack, and looking for the 
            // first procedure call.

            PrintDebugHex ("| RxVerifyingAddressState", b);
            if (b == ESC) {     // Start processing a new ESC.
                PrintDebug ("Start of ESC:");
                ++io->header;
                VerifierPushScanHeader (io, io->header);
                return;
            }

            device = io->device;
            io->operand = nullptr;
            m = device->Op (b, nullptr);
            if (m == nullptr) {
                // Could be an invalid member or a Device Stack push.
                if (io->operand == nullptr) {
                    PrintDebug ("No member found.");
                    return;
                }
                //VerifierPushScan (io, io->operand);
            }
            if (result = VerifierPushScanHeader (io, m->rx_header)) {
                PrintDebug ("Error reading address.");
                return;
            }
            VerifierEnterRxState (io, RxVerifyingArgsState);
            return;
        } else if (rx_state == RxVerifyingArgsState) {
            // In this state, a procedure has been called to scan on a valid
            // device. This state is responsible for loading the next header
            // argument and checking for the end of the procedure call.

            PrintDebug ("RxVerifyingArgs.");

            device = io->device;
            if (device == nullptr) {
                // Check if it is a Procedure Call or Device.
                device = io->device;
                io->operand = nullptr;
                m = device->Op (b, nullptr);
                device = io->operand;
                if (!device) {
                    if (m == nullptr) {
                        PrintError ("Invalid member");
                        VerifierEnterRxState (io, RxLockedState);
                        return;
                    }
                    // Else it was a function call.
                    VerifierEnterRxState (io, RxVerifyingArgsState);
                    return;
                }
            } else {
                header = const_cast<const uint_t*> (io->header);
                if (!io->header) {
                    return;
                }
                if (io->type_index == 0) {
                    PrintDebug ("Procedure verified.");
                    VerifierExitRxState (io);
                }
                // Get next type.
                type = *header;

                PrintDebug (TypeString (type));

                // Word-align the start of the buffer we're reading from.
                start += TypeAlign (start, type);

                // Switch to next state
                if (type <= SOH) {
                    if (type < SOH) {
                        Report (ReadInvalidTypeError);
                        VerifierEnterRxState (io, RxLockedState);
                    } else {
                        VerifierEnterRxState (io, RxVerifyingAddressState);
                    }
                } else if (type == STX) {   // String type.
                    VerifierEnterRxState (io, RxVerifyingStringState);
                } else if (type < DBL)  {   // Plain-old-data types.
                    io->bytes_left = SizeOf (type);
                    VerifierEnterRxState (io, RxVerifyingVarintState);
                } else if (type < UV8)  {   // Varint types
                    io->bytes_left = SizeOf (type);
                    VerifierEnterRxState (io, RxVerifyingVarintState);
                } else if (type <= AR8) {

                } else if (type == ESC) {

                } else if (type <= BK8) {

                } else if (type > US) {
                    Report (InvalidRxTypeError);
                } else {    // It's a US
                    io->bytes_left = kUnitSize;
                    VerifierEnterRxState (io, RxVerifyingPodState);
                }
                
            }
        } else if (rx_state == RxHandlingErrorState) {
            PrintDebug ("RxHandlingErrorState.");
        } else if (rx_state == RxLockedState) {
            PrintDebug ("RxLockedState.");
        } else if (rx_state >= RxLockedState) {
            Report (InvalidRxStateError);
        } else {    // parsing plain-old-data.
            --io->bytes_left;
            if (io->bytes_left == 0) {
                PrintDebug ("Done verifying POD type.");
                VerifierScanNextType (io);
            } else {
                b = VerifierStreamTxByte (io);
                PrintDebugHex ("Loading next byte", b);
                io->hash = Hash16 (b, hash);
                *start = b;
                ++start;
            }
        }
    }
}

/** Returns true if the given Verifier contains the given address. */
INLINE bool VerifierContains (Verifier* io, void* address) {
    if (address < reinterpret_cast<byte*>(io))
        return false;
    if (address > VerifierEndAddress (io)) return false;
    return true;
}

/** Pushes a header onto the scan stack.*/
INLINE ticket_t VerifierPushHeader (Verifier* io, const uint_t* header) {
    if (io->stack_height >= io->stack_size) {
        // Handle overflow cleanup:
        return Report (StackOverflowError, header);
    }

    //if (dc == nullptr) return noDevceSelectedError ();

    return 0;
}

/** Gets the base address of the header stack. */
INLINE const uint_t* VerifierHeaderStack (Verifier* io) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<byte*>
        (io) + sizeof (Verifier) + io->stack_height);
}

/** Closes the current expression and cues it for execution. */
INLINE void VerifierCloseExpression (Verifier* io) {
    PrintDebug ("[FF]");
}

/** Cancels the current expression. */
INLINE void VerifierCancelExpression (Verifier* io) {
    PrintDebug ("[CAN]");
    //stopAddress = txOpen;
}

/** Cancels the current expression and writes zeros to the buffer. */
INLINE void VerifierScrubExpression (Verifier* io) {
    // Erase the buffer by writing zeros to it.

    Uniprinter* rx = VerifierRx (io);
    uint_t size = rx->size;

    byte* begin = UniprinterBaseAddress (io, io->rx_offset),
        *end = begin + rx->size,
        *start = begin + rx->start,
        *stop = begin + rx->stop;

    uint_t buffer_space = RingBufferSpace (start, stop, size);

    if (start == stop) return; //< Nothing to do.
    if (start > stop) {
        memset (start, 0, end - start);
        memset (begin, 0, start - begin);
        return;
    }
    memset (start, 0, stop - start);
    rx->start = Diff (io, begin);
    rx->stop = Diff (io, start + 1);
}

/** Calls the Read function for the Rx slot. */
INLINE const Member* Read (Verifier* io, const uint_t* esc, void** args) {
    return reinterpret_cast<const Member*> (Read (VerifierRx (io), esc, 
                                                  args));
}

/** Calls the Write function for the Tx slot. */
INLINE const Member* Write (Verifier* io, const uint_t* esc, void** args) {
    return reinterpret_cast<const Member*> (Write (VerifierTx (io),
                                                   io->return_address, esc,
                                                   args));
}

/** Calls the Write function for the Tx slot. */
INLINE const Member* Write (Verifier* io, const char* address,
                            const uint_t* esc, void** args) {
    return reinterpret_cast<const Member*> (Write (VerifierTx (io), address, 
                                                   esc, args));
}

/** Prints the given Verifier to the console. */
INLINE void Print (Verifier* io) {
    PrintLine ('_');
    printf ("| Verifier:    ");
    if (io == nullptr) {
        printf ("null\n");
        PrintLine ("|", '_');
        return;
    }
    printf ("0x%p", io);
    PrintLine ("|", '_');
    std::cout << "| type:          " <<
        (io->type == -1) ? "interprocess no dynamic memory." :
        (io->type == 0) ? "no dynamic memory" :
        (io->type == 1) ? "dynamic memory" :
        (io->type == 2) ? "dynamic memory" : "Invalid type";

    std::cout << "\n| bytes_left:    " << io->bytes_left
        << "\n| tx_offset:     " << io->tx_offset
        << "\n| rx_offset:     " << io->rx_offset
        << "\n| stack_height:  " << io->stack_height
        << "\n| stack_size:    " << io->stack_size
        << "\n| num_verifying: " << io->num_verifying
        << "\n| num_states:    " << io->num_states;
    PrintLine ("|", '-');
    Print (io->device);
    std::cout << "| header: ";
    PrintEsc (const_cast<const uint_t*>(io->header));
    std::cout << "| Scan Stack: " << io->num_verifying;
    const uint_t** headers = (const uint_t**)io->headers;
    if (headers == nullptr) {
        std::cout << " null";
    } else {
        for (uint_t i = 0; i < io->stack_height; ++i) {
            std::cout << "| " << i << ": ";
            PrintEsc (headers[i]);
        }
    }
    PrintLine ("|", '_');
    //system ("PAUSE");
}

}       //< namespace _
#endif  //< CHINESEROOM_VERIFIER_H
