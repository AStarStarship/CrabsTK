/** The Chinese Room
    @version 0.x
    @file    /.../uniprinter.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                        All right reserved (R).
    Licensed under the Apache License, Version 2.0 (the "License"); you may
    not use this file except in compliance with the License. You may obtain
    a copy of the License at
                http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef CHINESEROOM_UNIPRINTER_H
#define CHINESEROOM_UNIPRINTER_H

#include "device.h"

namespace _ {

/** A universal all-in-one printer and scanner for Rx and Unityper slots.
    A Uniprinter connects two portals (@see ChineseRoom::Portal) between two
    rooms using Rx and Unityper ring buffers as depicted below:
    # Ring Buffer Streaming Diagram
    @code
        |>-------------------- Ring Buffer --------------------------->|
        ________________________________________________________________
    Rx  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Rx-->|>-Buffer->|
        |__________|___________________|____________________|__________|
        ________________________________________________________________
    Unityper  |>-Buffer->|>-Async Portal Unityper->|>-Sync User Writes->|>-Buffer->|
        |__________|___________________|____________________|__________|
    @endcode
    The Uniprinter needs to run as fast as possible, so no virtual members are
    allowed in that class. Portals are created using an Device and Portal
    interface. The Uniprinter needs to have an object stack, which requires
    three stacks of pointers: 2 for scanning the headers for sanitizing input,
    and another Device* stack for running the sequences.
    # Uniprinter Memory Layout
    @code
        ______________________________
        |   Scan Stack of Device**   |
        |----------------------------|
        |   Scan Stack of uint_t**   |
     ^  |----------------------------|
     |  |      Uniprinter Class      |
    0x0 |____________________________|
    @endcode
*/
struct Uniprinter {
    byte type;                     /*< What type of Uniprinter it is.
                                            -1 = interprocess no dynamic memory.
                                            0 = no dynamic memory.
                                            1 = dynamic memory.
                                            2 =  interprocess dynamic memory. */
    volatile byte rx_state,         //< Rx streaming state.
        last_rx_state,              //< Last Rx state.
        tx_state;                   //< Unityper streaming state.
    hash16_t rx_hash,               //< Rx hash to verify data integrity.
        tx_hash;                    //< Rx hash to verify data integrity.
    uint_t bytes_left,              //< Countdown counter for parsing POD types.
        tx_offset,                  //< The offset to the Unityper slot.
        rx_offset,                  //< The offset to the Unityper slot.
        stack_height,               //< Number of Device(s) on the stack.
        stack_size,                 //< Stack buffer size.
        num_verifying,              //< Height of header and cursors stacks.
        states_size,                //< Size of the state stack.
        num_states;                 //< Number of states on the state stack.
    Device* device;                 //< Device in the ESC being verified.
    volatile const uint_t* header;  //< Pointer to the header being verified.
    const uint_t* headers;          //< First header ptr in the scan array.
};

enum {
    kMinStackSize = 16,             //< The size of the juggle stack.
    kMinBufferSize = 2,             //< The minimum buffer size.
};

typedef enum RxStates {
    RxParsingStringState = 0,       //< RxState 0: Scanning STX.
    RxParsingVarIntState,           //< RxState 1: Scanning varint.
    RxAddressState,                 //< RxState 2: Scanning address.
    RxLockedState,                  //< RxState 3: Uniprinter Locked.
    RxParsingPODState,              //< RxState 4: Parsing plain-old-data.
    RxScanningArgsState,            //< RxState 5: arguments.
    RxHandlingErrorState,           //< RxState 6: Handling an error state.
    RxMemeberNotFoundErrorState,    //< RxState 7: Member not found.
    RxScanningHashState,            //< RxState 8: Stand the 32-bit hash.
    RxInvalidState,                 //< The number of RxState (s).
} RxState;

typedef enum TxStates {
    TxWritingState = 0,             //< Unityper State 0: Most common state.
    TxLockedState,                  //< Unityper State 1: Do nothing.
} TxState;

/*< Gets a pointer to the Rx slot. */
inline Rx* UniprinterRx (Uniprinter* io) {
    return io == nullptr ? nullptr :
           reinterpret_cast<Rx*>(reinterpret_cast<byte*>(io) + io->rx_offset);
}

/*< Gets a pointer to the Unityper slot. */
inline Unityper* UniprinterTx (Uniprinter* io) {
    return io == nullptr ? nullptr :
           reinterpret_cast<Unityper*>(reinterpret_cast<byte*>(io) + io->tx_offset);
}
    
/** Constructs a Uniprinter with equal sized rx and tx slots.
    @param root The root-scope device.
*/
static Uniprinter* UniprinterInit (byte* buffer, uint_t buffer_size, 
                            uint_t stack_height, Device* root = nullptr) {
    if (buffer == nullptr)
        return nullptr;
    if (buffer_size < kMinBufferSize)
        return nullptr;

    Uniprinter* io = reinterpret_cast<Uniprinter*> (buffer);

    uint_t size = (buffer_size - sizeof (Uniprinter) -
        (stack_height * (2 * sizeof (void*)))) >> 1;  // >>1 to /2

    io->rx_state = 0;
    io->tx_state = 0;
    stack_height = 0;
    io->num_verifying = 0;
    io->stack_size = stack_height;
#if DEBUG_CHINESEROOM
    printf ("\nInitializing Uniprinter:\nsizeof (Uniprinter): %u\n "
            "(stack_height * (2 * sizeof (void*))): %u\nstack_height: "
            "%u buffer_size: %u size_: %u\n!!!\n",
            sizeof (Uniprinter), (stack_height * (2 * 
                    sizeof (void*))), stack_height, buffer_size, size);
#endif //< DEBUG_CHINESEROOM
    io->bytes_left = 0;
    RxInit (UniprinterRx (io), buffer_size);
    TxInit (UniprinterTx (io), buffer_size);
    io->header = 0;
    io->device = root;
    //origin_ = door;
    return io;
}

/** Gets the base address of the device stack. */
static Device** UniprinterDeviceStack (Uniprinter* io) {
    auto a = reinterpret_cast<byte*> (io) + sizeof (Uniprinter) +
        io->stack_height * io->stack_size * sizeof (const uint_t*);
    return reinterpret_cast<Device**> (a);
}

/** Returns true if the Uniprinter uses dynamic memory. */
inline bool UniprinterIsDynamic (Uniprinter* io) {
    return io->type % 2 == 1;
}

inline byte* UniprinterEndAddress (Uniprinter* io) {
    return SlotEndAddress (UniprinterRx (io));
}

/** Resets this Uniprinter to the initial state. */
static ticket_t UniprinterReset (Uniprinter* io) {
    return 0;
}

/** Attempts to push the Device at the given index of the current
    device control onto the stack.
    @return Returns nullptr upon success and a pointer to a string
    upon failure. */
static const Member* Push (Uniprinter* io, Device* d) {
    if (d == nullptr)
        return reinterpret_cast<const Member*>(Report (NullDevicePushError));
    if (io->stack_height >= io->stack_size)
        return reinterpret_cast<const Member*>(Report (StackOverflowError));
    UniprinterDeviceStack (io)[io->stack_height++] = d;
    return 0;
}

/** Attempts to pop an Device off the stack and returns a pointer to a
    string upon failure. */
static ticket_t UniprinterPop (Uniprinter* io) {
    if (io->stack_height == 0)
        return Report (TooManyPopsError);
    io->device = UniprinterDeviceStack (io)[--io->stack_height];
    return 0;
}

/** Gets the base address of the state stack. */
static byte* UniprinterStateStack (Uniprinter* io) {
    return reinterpret_cast<byte*> (io) + sizeof (Uniprinter);
}

/** Exits the current state. */
static ticket_t UniprinterExitState (Uniprinter* io) {
    auto a = io->stack_height;
    if (a == 0)
        return Report (TooManyPopsError);
    io->rx_state = UniprinterStateStack (io)[--a];
    io->stack_height = a;
    return 0;
}

/** Pushes the new state onto the verifier stack. */
static ticket_t UniprinterEnterState (Uniprinter* io, byte state) {
    if (state >= RxInvalidState)
        return Report (InvalidStateError);
    auto a = io->stack_height;
    if (a >= io->stack_size)
        return Report (StackOverflowError);
    UniprinterStateStack (io)[a] = io->rx_state;
    a = io->stack_height + 1;
    io->rx_state = state;
    return 0;
}

/** Selects the given member. */
static ticket_t UniprinterPushScanHeader (Uniprinter* io, volatile const uint_t* header) {
    uint16_t num_verifying = num_verifying;
    if (num_verifying >= io->stack_size) {
        return Report (StackOverflowError, header,
                            SocketBaseAddress (io, io->tx_offset));
    }
    uint_t** scan_headers_ptr = reinterpret_cast<uint_t**> (io) +
        (sizeof (Uniprinter) / sizeof (uint_t**)) +
        num_verifying;
    uint_t* ptr = const_cast<uint_t*> (header);
    *scan_headers_ptr = ptr;
    header = header;
    return 0;
}

/** Pops a header off the scan stack. */
static ticket_t UniprinterPopScanHeader (Uniprinter* io) {
    auto num_verifying = io->num_verifying;
    if (num_verifying == 0) {
        return Report (TooManyPopsError);
    }
    --num_verifying;
    uint_t** headers = reinterpret_cast<uint_t**> (io) +
        (sizeof (Uniprinter) / sizeof (uint_t**)) + num_verifying;
    UniprinterExitState (io);
    io->header = *headers;
    num_verifying = num_verifying;
    return 0;
}

/** Scans the next type header type. */
static void UniprinterScanNextType (Uniprinter* io) {
    uint_t* header = const_cast<uint_t*> (io->header);
    if (header == nullptr) {
        UniprinterEnterState (io, RxScanningArgsState);
        return;
    }

    uint_t type = *header;
    if (type == NIL) {  // Done scanning args.
        UniprinterPopScanHeader (io);
        return;
    }
    ++header;
    io->header = header;
    io->rx_state = io->last_rx_state;
    //type = *header;
}

/** Scans the Rx buffer and marks the data as being ready to execute. */
static void UniprinterScanBytes (Uniprinter* io) {
    // It is fastest do a few branches as possible, and to compare to zero 
    // as much as possible. In order to optimize the switch, it is fastest 
    // to decrement the bytes_left, compare it to zero, and only process 
    // events. When the data comes in it will be packed and not word 
    // aligned. In order for most CPU to use the data, the data, we will 
    // need to be realigned.

    byte rx_state = rx_state;

    // We need to write the byte to the buffer

    Unityper* tx = UniprinterTx (io);

    uint_t size = tx->size,
        space_left,
        length;
    byte b;
    hash16_t rx_hash = io->rx_hash;

    byte* begin = SocketBaseAddress (UniprinterRx (io)),
        *end    = begin + size,
        *start  = begin + tx->start,
        *stop   = begin + tx->stop;
    space_left  = RingBufferSpace (start, stop, size);
    length = size - space_left;

    for (; length > 0; --length) {
        *start = b;
        ++start;

        if (rx_state == RxParsingStringState) {
            printf ("| rx_state == RxParsingStringState\n");
            rx_hash = Hash16 (b, rx_hash);

            if (io->bytes_left == 0) {
                printf ("| io->bytes_left == 0\n");
                Report (RxStringBufferOverflowError, io->header, start);
                return;
            }
            // Hash byte.
            rx_hash = Hash16 (b, rx_hash);

            // Check if string terminated.
            if (b == 0) {
                printf ("| b == 0\n");
                UniprinterExitState (io);
                return;
            }
            printf ("| b != 0\n");
            --io->bytes_left;
            return;
        } else if (rx_state == RxParsingVarIntState) {
            printf ("| rx_state == RxParsingVarIntState\n");
            // Hash byte.
            rx_hash = Hash16 (b, rx_hash);

            if (io->bytes_left == 1) {
                printf ("| io->bytes_left == 1\n");
                // Check last byte for error.

                // @warning I am not current saving the offset. I'm not sure 
                //          what to do here. The header class uses a variadic 
                //          template, I'm kind of tempted to switch to an int 
                //          type for the headers so I can just use a normal 
                //          array bracket initializer. The other option is to 
                //          add 32 to the first byte.

                if ((b >> 7) != 1) {
                    printf ("| (b >> 7) != 1\n");
                    Report (VarintOverflowError, io->header, start);
                    UniprinterEnterState (io, RxHandlingErrorState);
                    return;
                }

                return;
            }
            --io->bytes_left;
            return;
        } else if (rx_state == RxAddressState) {
            printf ("| rx_state == RxAddressState\n");
            if (b == ESC) {     // Start processing a new ESC.
                printf ("| b == ESC\n");
                io->header++;
                UniprinterPushScanHeader (io, io->header);
                return;
            }

            // Check if it is a Procedure Call or Device.
            const Member* m = io->device->Op (b, nullptr);
            if (m == nullptr) {
                printf ("| m == nullptr\n");
                // The member does not exist.
                Report (VarintOverflowError, io->header, start);
                UniprinterEnterState (io, RxLockedState);
                return;
            }
            if (m->tx_header == nullptr) {   // Then it's a Device.
                printf ("| m->tx_header == nullptr\n");
                //device->Op (this, Log (), b);
                Push (io, io->device);
                return;
            }
            printf ("| it's a function call\n");
            // else it's a function call.
            rx_hash = Hash16 (b, rx_hash);
            UniprinterPushScanHeader (io, m->rx_header);
            UniprinterEnterState (io, RxScanningArgsState);
            return;
        } else if (rx_state == RxHandlingErrorState) {
            printf ("| rx_state == RxHandlingErrorState\n");
            return;
        } else if (rx_state == RxLockedState) {
            printf ("| rx_state == RxLockedState\n");
            return;
        }
        // else we're parsing POD data.

        --io->bytes_left;
        if (io->bytes_left == 0) {
            printf ("| io->bytes_left == 0\n");
            // Success parsing POD type.
            UniprinterScanNextType (io);
        }
    }
}

/** Streams a tx byte. */
static byte UniprinterStreamTxByte (Uniprinter* io) {

    Unityper* tx = UniprinterTx (io);

    byte* begin = SocketBaseAddress (io, io->rx_offset),
        *end = begin + tx->size;
    byte* open = (byte*)begin + tx->read,
        *start = begin + tx->start,
        *cursor = start;

    int length = start < open ? open - start + 1 : (end - start) +
                    (open - begin) + 2;

    if (length < 1)
        return Report (BufferOverflowError, Esc<1, STX> (), 2,
                            start);

    byte b = *cursor;
    tx->stop = (++cursor > end) ? static_cast<uint_t> (Diff (io, begin)) :
                static_cast<uint_t> (Diff (io, cursor));
    return 0;
}

/** Returns true if the given Uniprinter contains the given address. */
inline bool UniprinterContains (Uniprinter* io, void* address) {
    if (address < reinterpret_cast<byte*>(io))
        return false;
    if (address > UniprinterEndAddress (io)) return false;
    return true;
}

/** Pushes a header onto the scan stack.*/
static ticket_t UniprinterPushHeader (Uniprinter* io, const uint_t* header) {
    if (io->stack_height >= io->stack_size) {
        // Big error! Stack overflow!
        return Report (StackOverflowError, header);
    }

    //if (dc == nullptr) return noDevceSelectedError ();

    return 0;
}

/** Gets the base address of the header stack. */
static const uint_t* UniprinterHeaderStack (Uniprinter* io) {
    return reinterpret_cast<const uint_t*> (reinterpret_cast<byte*>
        (io) + sizeof (Uniprinter) + io->stack_height);
}

/** Closes the current expression and cues it for execution. */
static void UniprinterCloseExpression (Uniprinter* io) {
#if DEBUG
    printf ("\r\n[FF]\r\n");
#endif
}

/** Cancels the current expression. */
static void UniprinterCancelExpression (Uniprinter* io) {
#if DEBUG
    printf ("\r\n[CAN]\r\n\n");
#endif
    //stopAddress = txOpen;
}

/** Cancels the current expression and writes zeros to the buffer. */
static void UniprinterScrubExpression (Uniprinter* io) {
    // Erase the buffer by writing zeros to it.

    Rx* rx = UniprinterRx (io);
    uint_t size = rx->size;

    byte* begin = SocketBaseAddress (io, io->rx_offset),
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
inline const Member* Read (Uniprinter* io, const uint_t* esc, void** args) {
    return reinterpret_cast<const Member*> (Read (UniprinterRx (io), esc, args));
}

/** Calls the Write function for the Tx slot. */
inline const Member* Write (Uniprinter* io, const uint_t* esc, void** args) {
    return reinterpret_cast<const Member*> (Write (UniprinterTx (io), esc, args));
}

}       //< namespace _
#endif  //< CHINESEROOM_UNIPRINTER_H