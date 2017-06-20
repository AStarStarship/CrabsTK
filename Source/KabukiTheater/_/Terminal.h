/** The Chinese Room
    @version 0.x
    @file    /.../Terminal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2017 Cale McCollough<https://calemccollough.github.io>

                        All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_TERMINAL_H
#define CHINESEROOM_TERMINAL_H

#include "Error.h"
#include "ErrorList.h"
#include "Types.h"

namespace _ {

static int stackSize;

enum
{
    Array = sizeof (uint_t) == 1 ? AR1 : sizeof (uint_t) == 2 ? AR2 : sizeof (uint_t) == 4 ? AR4 : AR8
};

class IDevice;
template<typename T>
class Door<T>;

inline void setStackSize(int newStackSize)
/*< Resizes the stack size. */
{
    if (stackSize > 0x7fff / 3) return;
    stackSize = newStackSize;
}

uint_t getStackSize()
/*< Gets the stack size. */
{
    return stackSize;
}

inline uintptr_t wordAlignOffset(void* ptr)
/*< Returns the number to add to word-align the given pointer to a 16-bit boundary. */
{
    #if _WordSize == 16
    return wordAlignOffset16 (ptr);
    #elif _WordSize == 32
    return wordAlignOffset32 (ptr);
    #else
    return wordAlignOffset64 (ptr);
    #endif
}

inline uintptr_t wordAlignOffset16(void* ptr)
/*< Returns the number to add to word-align the given pointer to a 16-bit boundary. */
{
    uintptr_t value = reinterpret_cast<uintptr_t>(ptr),
        mask = sizeof (uint16_t) - 1;
    return(value & mask);
}

inline uintptr_t wordAlignOffset32(void* ptr)
/*< Returns the number to add to word-align the given pointer to a 32-bit boundary.
    Invert the bits, add one, then mask with(sizeof(uint32_t)-1).
    0 00 -> invert bits -> 11(3 + 1 + 0) &(sizeof(uint32_t) -1 ) = 0
    1 01 -> invert bits -> 10(2 + 1 + 1) &(sizeof(uint32_t) -1 ) = 0
    2 10 -> invert bits -> 01(1 + 1 + 2) &(sizeof(uint32_t) -1 ) = 0
    3 11 -> invert bits -> 00(0 + 1 + 3) &(sizeof(uint32_t) -1 ) = 0
*/
{
    uintptr_t value = reinterpret_cast<uintptr_t>(ptr),
        mask = sizeof (uint32_t) - 1;
    return((~(value & mask)) & mask) + 1;
}

inline uintptr_t wordAlignOffset64(void* ptr)
/*< Returns the number to add to word-align the given pointer to a 64-bit boundary.
    Invert the bits, add one, then mask with(sizeof(uint32_t)-1).
    0 000 -> invert bits -> 111(7 + 1 + 0) &(sizeof(uint64_t) -1 ) = 0
    1 001 -> invert bits -> 110(6 + 1 + 1) &(sizeof(uint64_t) -1 ) = 0
    2 010 -> invert bits -> 101(5 + 1 + 2) &(sizeof(uint64_t) -1 ) = 0
    3 011 -> invert bits -> 100(4 + 1 + 3) &(sizeof(uint64_t) -1 ) = 0
    4 100 -> invert bits -> 011(3 + 1 + 4) &(sizeof(uint64_t) -1 ) = 0
    5 101 -> invert bits -> 010(2 + 1 + 5) &(sizeof(uint64_t) -1 ) = 0
    6 110 -> invert bits -> 001(1 + 1 + 6) &(sizeof(uint64_t) -1 ) = 0
    7 111 -> invert bits -> 000(0 + 1 + 7) &(sizeof(uint64_t) -1 ) = 0
*/
{
    uintptr_t value = reinterpret_cast<uintptr_t>(ptr),
        mask = sizeof (uint64_t) - 1;
    return((~(value & mask)) & mask) + 1;
}

inline uint_t calcRingBufferSpace(byte* start, byte* stop, uint_t size)
/*< Calculates the space left in the given ring buffer.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size  The size of the buffer.
*/
{
    uint_t delta_start_stop = static_cast<uint>(stop - start);
    return start < stop ? delta_start_stop : size - delta_start_stop;
}

inline uint_t calcRingSegmentLength(byte* start, byte* stop, uint_t size)
/*< Calculates the used ring buffer space.
    @param  Start The start of the data.
    @param  Stop  The stop of the data.
    @param  Size The size of the buffer.
*/
{
    uint_t delta_start_stop = static_cast<uint>(stop - start);
    return start < stop ? size - delta_start_stop : delta_start_stop;
}

class Terminal
/** An IO Terminal that streams escape sequences using a ring buffer.
    A Terminal connects two portals(@see ChineseRoom::Portal) between two rooms using 
    Rx and Tx ring buffers as depicted below:

    # Ring Buffer Streaming Diagram
    ```
        |>-------------------- Ring Buffer --------------------------->|
        ________________________________________________________________
    Rx  |>-Buffer->|>-Sync User Scan-->|>-Async Portal Rx-->|>-Buffer->|
        |__________|___________________|____________________|__________|
        ________________________________________________________________
    Tx  |>-Buffer->|>-Async Portal Tx->|>-Sync User Writes->|>-Buffer->|
        |__________|___________________|____________________|__________|
    ```

    The Terminal needs to run as fast as possible, so no virtual members are 
    allowed in that class. Portals are created using an IDevice and IPortal 
    interface. The Terminal needs to have an object stack, which requires 
    three stacks of pointers: 2 for scanning the headers for sanitizing input,
    and another IDevice* stack for running the sequences.

    # Terminal Memory Layout
        
        _______________________________
        |          Rx Buffer          |
        |-----------------------------|
        |          Tx Buffer          |
        |-----------------------------|
        |   Scan Stack of IDevice**   |
        |-----------------------------|
        |   Scan Stack of uint_t**    |
     ^  |-----------------------------|
     |  |       Terminal Class        |
    0x0 |_____________________________|

*/
{
    public:

    enum {
        MinStackSize                = 16,       //< The size of the juggle stack.
        MinBufferSize               = 2,        //< The minimum buffer size.
    };

    typedef enum
    {
        RxParsingStringState        = 0,        //< Rx State 0: Second most common state.
        RxParsingVarIntState,                   //< Rx State 1: Third most common state.
        RxAddressState,                         //< Rx State 2: The 
        RxLockedState,                          //< Rx State 3: The Terminal is locked from receiving bytes.
        RxParsingPODState,                      //< Rx State 4: Parsing plain-old-data.
        RxScanningArgsState,                    //< Rx State 5: arguments.
        RxHandlingErrorState,                   //< Rx State 6: Handling an error state.
        RxMemeberNotFoundErrorState,            //< Rx State 7: Member not found.
        RxScanningHashState,                    //< Rx State 8: Stand the 32-bit hash.
        RxNumStates,                            //< The number of Rx States.
    } RxState;

    typedef enum
    {
        TxWritingState              = 0,        //< Tx State 0: Most common state.
        TxLockedState,                          //< Tx State 1: Terminal does nothing.
    } TxState;

    Terminal ()
    /*< Default constructor that does nothing. */
    {
        //! Nothing to do here!
    }

    Terminal* init(uint_t bufferSize, byte aStackHeight, 
        IDevice* rootDevice = nullptr, Door* aDoor = nullptr)
    /*< Constructs a Terminal.
        @param rooDevice 
    */
    {
        if (bufferSize < MinBufferSize) bufferSize = MinBufferSize;

        uint l_slotSize = (bufferSize - sizeof (Terminal) -
            (aStackHeight * (2 * sizeof (void*)))) >> 1;  // >>1 to /2

        rxState = 0;
        txState = 0;
        stackHeight = 0;
        numVerifying = 0;
        stackSize = aStackHeight;
        slotSize = l_slotSize;
        printf ("\nInitializing Terminal:\nsizeof (Terminal): %u\n(aStackHeight"
            "* (2 * sizeof (void*))): %u\naStackSize: %u bufferSize: %u "
            "slotSize: %u\n!!!\n",
            sizeof (Terminal), (aStackHeight * (2 * sizeof (void*))),
            aStackHeight, bufferSize, l_slotSize);
        bytesLeft = 0;
        rxRead = 0;
        rxStart = 0;
        rxStop = 0;
        txStart = 0;
        txStop = 0;
        scanHeader = 0;
        device = rootDevice;
        origin = aDoor;

        return this;
    }

    ticket_t reset()
    /*< Resets this Terminal to the initial state. */
    {
        return 0;
    }

    const char* getStatus()
    /*< Returns a string describing the Rx and Tx state. */
    {
        return nullptr;
    }

    IDevice* dc()
    /*< Gets the device under control. */
    {
        return device;
    }

    ticket_t push(IDevice* d)
    /*< Attempts to push the IDevice at the given index of the current device control onto the stack.
        @return Returns nullptr upon success and a pointer to a string upon failure. */
    {
        if (d == nullptr) return errors ()->report (NullDevicePushError);
        if (stackHeight >= stackSize) return errors ()->report (StackOverflowError);
        getDeviceStackBase_ptr ()[stackHeight++] = d;
        return 0;
    }

    ticket_t pop()
    /*< Attempts to pop an IDevice off the stack and returns a pointer to a string upon failure. */
    {
        if (stackHeight == 0) return errors ()->report (TooManyPopsError);
        device = getDeviceStackBase_ptr ()[--stackHeight];
        return 0;
    }

    uint_t getRxLength ()
    /*< Gets the rx buffer length. */
    {
        byte* base = getTxBaseAddress ();
        return calcRingBufferSpace (base + rxStart, base + rxStop, slotSize);
    }

    uint_t getTxBufferSpace()
    /*< Gets the tx buffer space. */
    {
        byte* base = getTxBaseAddress ();
        return calcRingSegmentLength (base + rxStart, base + rxStop, slotSize);
    }

    uint_t getSlotSize ()
    /*< Gets the slot size. */
    {
        return slotSize;
    }

    bool isReadable()
    /*< Returns true if the Rx buffer contains any data. */
    {
        return getRxLength () > 0;
    }

    bool isWritable()
    /*< Returns true if the Tx buffer contains any data. */
    {
        return getTxBufferSpace () > 0;
    }

    void streamRxByte(byte B)
    /*< Streams an Rx byte. */
    {
        //! It is fastest do a few branches as possible, and to compare to zero as much as possible.
        //! In order to optimize the switch, it is fastest to decrement the bytesLeft, compare it to
        //! zero, and only process events .

        //! When the data comes in it will be packed and not word aligned. In order for most CPU to use the data, 
        //! the data, we will n

        byte l_rxState = rxState;

        #if DEBUG_CHINESEROOM
        printf ("%x", b);
        #endif

        uint_t l_slotSize = slotSize,
            spaceLeft;

        byte* begin = getTxBaseAddress (),
            *end = begin + l_slotSize,
            *start = begin + txStart,
            *stop = begin + txStop;
        spaceLeft = calcRingBufferSpace (start, stop, l_slotSize);
        *start = b;
        ++start;

        if (l_rxState == RxParsingStringState)
        {
            rxHash = hash32 (b, rxHash);

            if (bytesLeft == 0)
            {
                errors ()->report (RxStringBufferOverflowError, const_cast<const uint_t*>(scanHeader), 0, start);
                return;
            }
            //! Hash byte.
            rxHash = hashChar (b, rxHash);

            //! Check if string terminated.
            if (b == 0)
            {
                exitState ();
                return;
            }
            --bytesLeft;
            return;
        }
        else if (l_rxState == RxParsingVarIntState)
        {
            //! Hash byte.
            rxHash = hashChar (b, rxHash);

            if (bytesLeft == 1)
            {
                //! Check last byte for error.

                //! @warning I am not current saving the offset. I'm not sure what to do here. The params class uses a  
                //!          variadic template, I'm kind of tempted to switch to an int type for the headers so I can 
                //!          just use a normal array bracket initializer. The other option is to add 32 to the first byte.

                if ((b >> 7) != 1)
                {
                    errors ()->report (VarintOverflowError, const_cast<const uint_t*>(scanHeader), 0, start);
                    enterState (RxHandlingErrorState);
                    return;
                }

                return;
            }
            --bytesLeft;
            return;
        }
        else if (l_rxState == RxAddressState)
        {
            if (b == ESC)   //! Start processing a new ESC.
            {
                scanHeader++;
                pushScanHeader (const_cast<const uint_t*>(scanHeader));
                return;
            }
            //! Check if it is a Procedure Call or Device.
            const Member<T>* m = device->op (nullptr, b);
            if (m == nullptr)
            {
                //! The member does not exist.
                errors ()->report (VarintOverflowError, const_cast<const uint_t*>(scanHeader), 0, start);
                enterState (RxLockedState);
                return;
            }
            if (m->txHeader == nullptr) //! Then it's a Device.
            {
                device->op (this, b);
                return;
            }
            //! else it's a function call.
            rxHash = hashChar (b, rxHash);
            pushScanHeader (m->rxHeader);
            enterState (RxScanningArgsState);
            return;
        }
        else if (l_rxState == RxHandlingErrorState)
        {
            return;
        }
        else if (l_rxState == RxLockedState)
        {
            return;
        }
        /// else we're parsing POD data.

        --bytesLeft;
        if (bytesLeft == 0)
        {
            //! Success parsing POD type.
            scanNextType ();
        }
    }

    byte streamTxByte()
    /*< Streams a tx byte. */
    {
        byte* begin = getTxBaseAddress (),
            *end = begin + slotSize;
        byte* open = (byte*)begin + txOpen,
            *start = begin + txStart,
            *cursor = start;

        int length = start < open ? open - start + 1 : (end - start) + (open - begin) + 2;

        if (length < 1)
        {
            return errors ()->report (BufferOverflowError, Params<1, STR>::Header, 2, start);
        }

        byte b = *cursor;
        txStop = (++cursor > end) ? static_cast<uint_t>(diff (this, begin)) : static_cast<uint_t>(diff (this, cursor));
        return 0;
    }

    ticket_t log(const char* s)
    /*< Prints a single string to the console.
        @return Returns an index to the ErrorList ticket number. */
    {
        if (s == nullptr) return 0;

        //! Temp variables packed into groups of 8 bytes for memory alignment.
        char currentChar;
        uint_t bufferSpace,
            slotSize = slotSize;

        byte* begin = getTxBaseAddress (),
            *end = begin + slotSize,
            *start = begin + txStart,
            *stop = begin + txStop;

        bufferSpace = calcRingBufferSpace (start, stop, slotSize);

        currentChar = *s;
        *stop = currentChar;

        while (currentChar != 0)
        {
            if (--bufferSpace == 0)
            {
                return errors ()->report (BufferUnderflowError, Params<1, STR>::Header, 2, start);
            }
            ++s;
            currentChar = *s;               //! Read byte.
            ++stop;
            *stop = currentChar;            //! Write byte.

            if (++stop >= end) stop -= slotSize;
        }
        return 0;
    }

    bool contains(byte* address)
    /*< Returns true if this terminal contains the given address. */
    {
        byte* baseAddress = getTxBaseAddress ();
        return (address < baseAddress || address > baseAddress + (slotSize << 1)) ? false : true;    //< << 1 to * 2
    }

    ticket_t read(const uint_t* header, void** dataPointers)
    /*< Scans a message with the given header to the given Terminal. */
    {
        #if USING_PARAMS
        if (header == nullptr) errors ()->report (NullHeaderError, 0, 0, 0);

        //! Temp variables packed into groups of 8 bytes for memory alignment.
        byte type,
            ui1;
        #if USING_2_BYTE_TYPES
        uint16_t ui2;
        #endif
        #if USING_4_BYTE_TYPES
        uint32_t ui4;
        #endif
        #if USING_8_BYTE_TYPES
        uint64_t ui8;
        #endif

        byte*     ui1_ptr;      //< Pointer to a UI1, not ui1.
        #if USING_2_BYTE_TYPES
        uint16_t* ui2_ptr;      //< Pointer to a UI1, not ui1.
        #endif
        #if USING_4_BYTE_TYPES
        uint32_t* ui4_ptr;      //< Pointer to a UI1, not ui1.
        #endif
        #if USING_4_BYTE_TYPES
        uint64_t* ui8_ptr;      //< Pointer to a UI1, not ui1.
        #endif

        uint_t numParameters = *header,
            l_slotSize,
            length,
            temp,
            index;
        //collisionsSize;
        //byte alignOffset;

        if (numParameters == 0) return 0;   //< Nothing to do.

        l_slotSize = slotSize;

        byte* begin = getTxBaseAddress () + l_slotSize,
            *end = begin + l_slotSize,
            *start = begin + rxStart,
            *stop = begin + rxStop;
        //* temp_ptr;     //< A temporary pointer for storing memory alignment.

        length = calcRingSegmentLength (start, stop, l_slotSize);

        printf ("Reading: begin: %u start %u stop:%u end:%u\n"
            "length: %u\nnumParameters: %u Params: ",
            diff (this, begin), diff (this, start), diff (this, stop), diff (this, end),
            length, numParameters);
        printParams (header);

        //! When we scan, we are reading from the beginning of the Rx buffer.

        for (index = numParameters; index < numParameters; ++index)
        {
            ++header;
            type = *header;

            printf ("Reading type: %s\n", typeString (type));

            temp = getAlignment (type);

            switch (type)
            {
                case NIL: break;

                case STR: //< _R_e_a_d__S_t_r_i_n_g_-_8_____________________________

                    #if _UsingSTR_
                          //! Load next pointer and increment ptrs.
                    ui1_ptr = reinterpret_cast<byte*>(ptrs[index]);

                    if (ui1_ptr == nullptr) break;

                    ++header;
                    temp = *header;

                    printf ("Reading: ");

                    if (temp == 0) break;

                    ui1 = *start;
                    *ui1_ptr = ui1;

                    putchar (static_cast<char>(ui1));

                    while (ui1 != 0)
                    {
                        if (--temp == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                        ++start;
                        ui1 = *start;       //! Read byte
                        ++ui1_ptr;
                        *ui1_ptr = ui1;     //! Write byte.

                        putchar (static_cast<char>(ui1));

                        if (++start >= end) start -= l_slotSize;
                    }
                    putchar ('\n');
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case STX: //< _R_e_a_d__U_T_F_-_1_6__S_t_r_i_n_g____________________

                    #if USING_STX
                          //! Load next pointer and increment ptrs.
                    ui2_ptr = reinterpret_cast<uint16_t*>(ptrs[index]);


                    if (ui2_ptr == 0) break;

                    temp = *(++header);

                    if (temp == 0) break;

                    ui1 = *start;
                    *ui2_ptr = ui1;

                    while (ui1 != 0)
                    {
                        if (--temp == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                        ui1 = *(++start);     //! Read byte
                        *(++ui2_ptr) = ui1;    //! Write byte.

                        if (++start >= end) start -= l_slotSize;
                    }
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SI1: //< _R_e_a_d__1_B_y_t_e__T_y_p_e_s________________________
                case UI1:
                case BOL:
                    #if USING_1_BYTE_TYPES
                    if (length == 0) return errors ()->report (BufferUnderflowError, header, index, start);
                    --length;

                    // No need to memory align 1 byte.

                    // Load next pointer and increment ptrs.
                    ui1_ptr = reinterpret_cast<byte*>(ptrs[index]);


                    if (ui1_ptr == 0) break;
                    ui1 = *start;
                    if (++start >= end) start -= l_slotSize;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s______________________
                case UI2:
                    #if USING_2_BYTE_TYPES
                    if (length < 2) return errors ()->report (BufferUnderflowError, header, index, start);


                    length -= 2;

                    // Memory align and check for zero padding.
                    if (((uintptr_t)stop) & 0x1)
                    {
                        if ((ui1 = *stop) != 0)
                        {

                        }
                    }

                    // Load next pointer and increment ptrs.
                    ui2_ptr = reinterpret_cast<uint16_t*>(ptrs[index]);


                    if (ui2_ptr == 0) break; //< Not sure if we allow null or not yet.
                    *ui2_ptr = *start;
                    if ((start += 2) >= end) start -= l_slotSize;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s______________________
                case UI4:
                case FLT:
                case TMS:
                    #if USING_4_BYTE_TYPES
                    if (length < 4) return errors ()->report (BufferUnderflowError, header, index, start);
                    length -= 4;

                    // Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<uint32_t*>(ptrs[index]);


                    *ui4_ptr = *((uint32_t*)stop);
                    if ((stop += 4) > end) start -= l_slotSize;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________________
                case SI8:
                case UI8:
                case DBL:
                    #if USING_8_BYTE_TYPES
                    if (length < 8) return errors ()->report (BufferUnderflowError, header, index, start);
                    length -= 8;

                    // Load next pointer and increment ptrs.
                    ui8_ptr = reinterpret_cast<uint64_t*>(ptrs[index]);


                    *ui8_ptr = *((uint64_t*)stop);
                    if ((stop += 8) >= end) start -= l_slotSize;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SV2: //< _R_e_a_d__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

                    #if _Using4ByteVarints_
                          // Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<uint32_t*>(ptrs[index]);


                    if (ui4 >> 15) ui4 = 0x1 & (((~ui4) + 1) << 1);
                    else               ui4 = ui4 << 1;

                    //! Scan byte 1
                    ui1 = *start;
                    ui4 = ui1;
                    if (++start >= end) start -= l_slotSize;

                    while (ui1 >> 7 != 0)
                    {
                        if (--length == 0)
                            return errors ()->report (BufferUnderflowError, header, index, start);
                        ui1 = *(++start);
                        ui4 |= ((uint32_t)ui1) << (1 * 7);
                        if (++start >= end) start -= l_slotSize;
                    }
                    *ui4_ptr = (ui4 << 31) == 0 ? ui4 : (~ui4) - 1;
                    break;

                case UV2: //< _R_e_a_d__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
                    if (length == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                    // Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<uint32_t*>(ptrs[index]);


                    //! Scan byte 1.
                    ui1 = *start;
                    ui4 = ui1;
                    if (++start >= end) start -= l_slotSize;

                    while (ui1 >> 7 != 0)
                    {
                        if (--length == 0) return errors ()->report (BufferUnderflowError, header, index, start);
                        ui1 = *(++start);
                        ui4 |= ((uint32_t)ui1) << (1 * 7);
                        if (++start >= end) start -= l_slotSize;
                    }
                    *ui4_ptr = ui4;
                    break;
                    #else
                case UV2:
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SV4: //< _R_e_a_d__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

                    #if _Using4ByteVarints_
                          // Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<uint32_t*>(ptrs[index]);


                    if (ui4 >> 15) ui4 = 0x1 & (((~ui4) + 1) << 1);
                    else               ui4 = ui4 << 1;

                    //! Scan byte 1
                    ui1 = *start;
                    ui4 = ui1;
                    if (++start >= end) start -= l_slotSize;

                    while (ui1 >> 7 != 0)
                    {
                        if (--length == 0)
                            return errors ()->report (BufferUnderflowError, header, index, start);
                        ui1 = *(++start);
                        ui4 |= ((uint32_t)ui1) << (1 * 7);
                        if (++start >= end) start -= l_slotSize;
                    }
                    *ui4_ptr = (ui4 << 31) == 0 ? ui4 : (~ui4) - 1;
                    break;

                case UV4: //< _R_e_a_d__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
                    if (length == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                    // Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<uint32_t*>(ptrs[index]);


                    //! Scan byte 1.
                    ui1 = *start;
                    ui4 = ui1;
                    if (++start >= end) start -= l_slotSize;

                    while (ui1 >> 7 != 0)
                    {
                        if (--length == 0) return errors ()->report (BufferUnderflowError, header, index, start);
                        ui1 = *(++start);
                        ui4 |= ((uint32_t)ui1) << (1 * 7);
                        if (++start >= end) start -= l_slotSize;
                    }
                    *ui4_ptr = ui4;
                    break;
                    #else
                case UV4:
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SV8: //< _R_e_a_d__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

                    #if _Using8ByteVarints_
                          //! Load next pointer and increment ptrs.
                    ui8_ptr = reinterpret_cast<uint64_t*>(ptrs[index]);


                    if (ui8 >> 15) ui8 = 0x1 & (((~ui8) + 1) << 1);
                    else               ui8 = ui8 << 1;

                    //! Byte 1
                    ui1 = *start;
                    ui8 = ui1;
                    if (++start >= end) start -= l_slotSize;

                    while (ui1 >> 7 != 0)
                    {
                        if (--length == 0)
                            return errors ()->report (BufferUnderflowError, header, index, start);
                        ui1 = *(++start);
                        ui8 |= ((uint64_t)ui1) << (1 * 7);
                        if (++start >= end) start -= l_slotSize;
                    }
                    *ui8_ptr = (ui8 << 63) == 0 ? ui8 : (~ui8) - 1;
                    break;

                case UV8: //< _R_e_a_d__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
                    if (length == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                    // Load next pointer and increment ptrs.
                    ui8_ptr = reinterpret_cast<uint64_t*>(ptrs[index]);


                    //! Scan byte 1
                    ui1 = *start;
                    ui8 = ui1;
                    if (++start >= end) start -= l_slotSize;

                    while (ui1 >> 7 != 0)
                    {
                        if (--length == 0) return errors ()->report (BufferUnderflowError, header, index, start);
                        ui1 = *(++start);
                        ui8 |= ((uint64_t)ui1) << (1 * 7);
                        if (++start >= end) start -= l_slotSize;
                        if (ui1 >> 7 == 0) { *ui8_ptr = ui8; break; }
                    }
                    *ui8_ptr = ui8;
                    break;
                    #else
                case UV8:
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR1:  //< _R_e_a_d__A_r_r_a_y_-_1______________________________
                    #if USING_AR1
                    if (--length == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                    //! Load next pointer and increment ptrs.
                    ui1_ptr = reinterpret_cast<byte*>(ptrs[index]);


                    // Load array size.
                    ui1 = *start;

                    if ((start > stop) && (start + ui1 >= end))
                    {
                        //! Find length of upper chunk.
                        temp = (uint_t)(end - start);

                        memcpy (ui1_ptr, start, temp);
                        memcpy (ui1_ptr + temp, begin, (size_t)(ui1 - temp));
                        break;
                    }
                    memcpy (ui1_ptr, ui1_ptr, (size_t)ui1);
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR2:  //< _R_e_a_d__A_r_r_a_y_-_2______________________________
                    #if USING_AR2
                    alignOffset = wordAlignOffset (start);
                    if (length < sizeof (uint16_t) + alignOffset) return errors ()->report (BufferUnderflowError, header, index, start);
                    start -= (start + alignOffset >= end) * l_slotSize;

                    //! Load next pointer and increment ptrs.
                    ui2_ptr = reinterpret_cast<uint16_t*>(ptrs[index]);


                    ui2 = *ui2_ptr;

                    //! Copy array into memory.
                    if ((start > stop) && (start + ui2 >= end))
                    {
                        //! Find length of upper chunk.
                        temp = (uint_t)(end - start);

                        memcpy (ui2_ptr, start, temp);
                        memcpy (ui2_ptr + temp, begin, (size_t)(ui2 - temp));
                        break;
                    }
                    memcpy (ui2_ptr, ui2_ptr + ui2, (size_t)ui2);
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR4:  //< _R_e_a_d__A_r_r_a_y_-_4______________________________
                    #if USING_AR2
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR8:  //< _R_e_a_d__A_r_r_a_y_-_8______________________________
                    #if USING_AR8
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case ESC: //< _R_e_a_d__E_s_c_a_p_e__S_e_q_u_e_n_c_e________________
                    #if _UsingESC_
                    if (length == 0) return errors ()->report (BufferUnderflowError, header, index, start);
                    --length;

                    // Load next pointer and increment ptrs.
                    ui1_ptr = static_cast<byte*>(ptrs[index]);


                    if (ui1_ptr == 0)
                    {
                        //! o isn't an error, but we need to adjust the start pointer to just over the data.
                        break;
                    }
                    ui1 = *start;
                    if (++start >= end) start -= l_slotSize;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case BK8: //< _R_e_a_d__B_o_o_k_8___________________________________

                    #if USING_AR8
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case BK4: //< _R_e_a_d__B_o_o_k_4___________________________________

                    #if USING_BK4
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case BK2: //< _R_e_a_d__B_o_o_k_2___________________________________

                    #if USING_BK2
                    alignOffest = wordAlignOffset (start);
                    start
                        if ((start += sizeof (Book2)) >= end)
                        {
                            start -= l_slotSize;
                        }

                    //! To save stack space, we have to manually load the book header and calculate size.

                    start = wordAlignOffset (start, begin, end, sizeof (Book2));

                    //! Load numKeys.
                    ui2 = *start;
                    temp = ui2; //! Store temp variable

                                //! Load bufferSize.
                    ui2 = *(start + sizeof (byte));
                    temp += ui2;    //! Add the stored temp variable.
                    temp *= (sizeof (uint16_t) + sizeof (uint16_t) + (collisionsSize > 0) *(sizeof (byte) + sizeof (hash32_t)));
                    temp += collisionsSize;


                    ui2_ptr = reinterpret_cast<uint16_t*>(start);
                    //! Load headerSize.
                    ui2 = *(ui2_ptr + 2 * sizeof (uint16_t));
                    temp += ui2;
                    start += sizeof (uint16_t);

                    //! Load dataSize.
                    ui2 = *(ui2_ptr + 3 * sizeof (uint16_t));
                    temp += ui2;
                    start += sizeof (uint16_t);

                    temp += 5 * sizeof (uint16_t);   //! add the size of the header and we're done.

                    if (length < temp)
                        return errors ()->report (BufferUnderflowError, header, index, start);
                    length -= (uint_t)temp;

                    ui2_ptr = reinterpret_cast<uint16_t*>(ptrs);
                    if (ui2_ptr == nullptr) break;  //! This isn't an error because the user might want to pass in a nullptr.

                                                    //! Now we can copy the book into memory.
                    if ((start > stop) && (start + temp >= end))
                    {
                        //! Calculate upper chunk size.
                        temp = end - stop;
                        temp -= temp;

                        memcpy (start, ui2_ptr, temp);
                        memcpy (begin, ui2_ptr + temp, (size_t)temp);
                        start = begin + temp;
                        break;
                    }
                    memcpy (stop, ui2_ptr, (size_t)temp);
                    start += temp;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif


                case US: //< _R_e_a_d__U_n_i_t__S_e_p_e_r_a_t_o_r___________________
                    break;

                default:
                    return errors ()->report (ReadInvalidTypeError, header, index, start);
            }
        }

        txStop = static_cast<uint_t>(stop - begin);
        return 0;
        //byte* start = getTxBaseAddress() + getRxStart();
        #else
        return errors ()->report (UnsupportedTypeError, header, 0, getTxBaseAddress () + getRxLength ());
        #endif
    }

    ticket_t write(const uint_t* header, inline dataPointers, const char* address = "")
    /*< Prints a message with the given header to the given Terminal. */
    {
        #if USING_PARAMS
        if (header == nullptr) errors ()->report (NullHeaderError, 0, 0, 0);

        //! Temp variables packed into groups of 8 bytes for memory alignment.
        byte type,
            ui1;
        #if USING_2_BYTE_TYPES
        uint16_t ui2;
        #endif
        #if USING_4_BYTE_TYPES
        uint32_t ui4;
        #endif
        #if USING_8_BYTE_TYPES
        uint64_t ui8;
        #endif

        uint_t numParameters = *header,
            l_slotSize,
            bufferSpace,
            index;
        //temp,
        //collisionsSize;
        //byte alignOffset;                 //< Temp variable to store the memory alignment offset.

        if (numParameters == 0) return 0;    //< Nothing to do.

        l_slotSize = slotSize;

        byte* begin = getTxBaseAddress (),
            *end = begin + l_slotSize,
            *start = begin + txStart,
            *stop = begin + txStop;
        //    * temp_ptr;
        const byte* ui1_ptr;
        #if USING_2_BYTE_TYPES
        const uint16_t* ui2_ptr;
        #endif
        #if USING_2_BYTE_TYPES
        const uint32_t* ui4_ptr;
        #endif
        #if USING_2_BYTE_TYPES
        const uint64_t* ui8_ptr;
        #endif
        #if USING_STX
        const uint16_t* stop16_ptr,     //< Temp pointer for writing UTF-16 strings.
            *end16_ptr;                //< Temp pointer for writing UTF-16 strings.
        #endif

        bufferSpace = calcRingBufferSpace (start, stop, l_slotSize);
        printf ("start: %p stop: %p l_slotSize: %u bufferSpace: %u\n", start, stop, l_slotSize, bufferSpace);

        ++header;

        //const char** strings = reinterpret_cast<const char**>(ptrs);
        //printf ("\ntestStrings at start\nstring1: %s\nstring2: %s\n", strings[0], strings[1]);

        printf ("write: numParams: %u\n", numParameters);

        if (address == nullptr) goto WriteArgs;

    WriteArgs:
        //! Check if the buffer has enough room.
        if (bufferSpace == 0)
            return errors ()->report (BufferUnderflowError, header, index, start);

        --bufferSpace;

        //strings = reinterpret_cast<const char**>(ptrs);
        //printf ("\ntestStrings at after: 0x%p\nstring1: %s\nstring2: %s\n", ptrs, strings[0], strings[1]);

        //! Load the source data pointer and increment ptr.fs
        ui1_ptr = reinterpret_cast<const byte*>(ptrs[index]);

        printf ("Before trying to print 0x%p: %s\n", ui1_ptr, ui1_ptr);
        print ();

        printf ("\nWriting address: ");

        //! Write address.

        ui1 = *address; //! We don't want to write the null-term char.

        while (ui1 != 0)
        {
            if (bufferSpace-- == 0)
                return errors ()->report (BufferUnderflowError, header, index, start);

            *stop = ui1;        //! Write byte
            putchar (static_cast<char>(ui1));

            ++address;
            ui1 = *address;     //! Read byte.

            putchar (static_cast<char>(ui1));

            ++stop;

            if (stop >= end) stop -= l_slotSize;
        }

        //! Write data.
        for (index = 0; index < numParameters; ++index)
        {
            ++header;
            type = *header;
            printf ("%i:%u:%s\n", index, type, typeString (type));

            switch (type)
            {
                case NIL:
                    break;

                case STR: //< _W_r_i_t_e__A_S_C_I_I_/_U_T_F_-_8__S_t_r_i_n_g________


                case STX: //< _W_r_i_t_e__U_T_F_-_1_6__S_t_r_i_n_g__________________

                    #if USING_STX
                          //! Check if the buffer has enough room.
                    if (bufferSpace == 0)
                        return errors ()->report (BufferUnderflowError, header, index, start);

                    --bufferSpace;

                    //strings = reinterpret_cast<const char**>(ptrs);
                    //printf ("\ntestStrings at after: 0x%p\nstring1: %s\nstring2: %s\n", ptrs, strings[0], strings[1]);

                    //! Load the source data pointer and increment ptr.fs
                    ui1_ptr = reinterpret_cast<const byte*>(ptrs[index]);

                    //printf ("Before trying to print 0x%p: %s\n", ui1_ptr, ui1_ptr);
                    //print ();
                    //printf ("\nWriting chars: ");

                    //! We know we will always write at least one null-term char.
                    ui1 = *ui1_ptr;
                    *stop = ui1;

                    putchar (static_cast<char>(ui1));

                    while (ui1 != 0)
                    {
                        if (bufferSpace-- == 0)
                            return errors ()->report (BufferUnderflowError, header, index, start);

                        ++ui1_ptr;
                        ui1 = *ui1_ptr;     //! Read byte.

                        putchar (static_cast<char>(ui1));

                        ++stop;
                        *stop = ui1;        //! Write byte

                        if (stop >= end) stop -= l_slotSize;
                    }
                    //printf ("\nAfter\n");
                    //print ();
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SI1: //< _W_r_i_t_e__8_-_b_i_t__T_y_p_e_s________________________
                case UI1:
                case BOL:
                    #if USING_1_BYTE_TYPES
                    //! Check if the buffer has enough room.
                    if (bufferSpace-- == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                    //! Load next pointer and increment ptrs.
                    ui1_ptr = reinterpret_cast<const byte*>(ptrs[index]);


                    //! Write the data and increment the stop to wrap around to the beginning.
                    *stop = *ui1_ptr;
                    if (++stop >= end) stop -= l_slotSize;
                    break;
                    #endif

                case SI2: //< _W_r_i_t_e__1_6_-_b_i_t__T_y_p_e_s______________________
                case UI2:
                    #if USING_2_BYTE_TYPES
                    //! Align the buffer to a word boundary and check if the buffer has enough room.
                    alignOffset = wordAlignOffset16 (stop);
                    if (bufferSpace < alignOffset + sizeof (uint16_t)) return errors ()->report (BufferUnderflowError, header, index, start);
                    bufferSpace -= alignOffset + sizeof (uint16_t);
                    stop += alignOffset;

                    //! Load value to write and increment ptrs.
                    ui2_ptr = reinterpret_cast<const uint16_t*>(ptrs[index]);
                    ui2 = *ui2_ptr;

                    //! Write the data and increment the stop to wrap around to the beginning.
                    if ((stop + sizeof (uint16_t)) >= end) stop -= l_slotSize;
                    ui2_ptr = reinterpret_cast<uint16_t*>(stop);
                    *ui2_ptr = ui2;
                    stop += sizeof (uint16_t);
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SI4: //< _W_r_i_t_e__3_2_-_b_i_t__T_y_p_e_s______________________
                case UI4:
                case FLT:
                case TMS:
                    #if USING_4_BYTE_TYPES
                    //! Align the buffer to a word boundary and check if the buffer has enough room.
                    #if _WordSize == 16
                    alignOffset = wordAlignOffset16 (stop);
                    #else
                    alignOffset = wordAlignOffset32 (stop);
                    #endif
                    if (bufferSpace < alignOffset + sizeof (uint32_t)) return errors ()->report (BufferUnderflowError, header, index, start);
                    bufferSpace -= alignOffset + sizeof (uint32_t);
                    stop += alignOffset;

                    //! Load value to write and increment ptrs.
                    ui4_ptr = reinterpret_cast<const uint32_t*>(ptrs[index]);
                    ui4 = *ui4_ptr;

                    //! Write the data and increment the stop to wrap around to the beginning.
                    if ((stop + sizeof (uint32_t)) >= end) stop -= l_slotSize;
                    ui4_ptr = reinterpret_cast<uint32_t*>(stop);
                    *ui4_ptr = ui4;
                    stop += sizeof (uint32_t);
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case TMU: //< _W_r_i_t_e__6_4_-_b_i_t__T_y_p_e_s______________________
                case SI8:
                case UI8:
                case DBL:
                    #if USING_8_BYTE_TYPES
                    //! Align the buffer to a word boundary and check if the buffer has enough room.
                    alignOffset = wordAlignOffset (stop);
                    if (bufferSpace < alignOffset + sizeof (uint64_t)) return errors ()->report (BufferUnderflowError, header, index, start);
                    bufferSpace -= alignOffset + sizeof (uint64_t);
                    stop += alignOffset;

                    //! Load value to write and increment ptrs.
                    ui8_ptr = reinterpret_cast<const uint64_t*>(ptrs[index]);
                    ui8 = *ui8_ptr;

                    //! Write the data and increment the stop to wrap around to the beginning.
                    if ((stop + sizeof (uint64_t)) >= end) stop -= l_slotSize;
                    ui8_ptr = reinterpret_cast<uint64_t*>(stop);
                    *ui8_ptr = ui8;
                    stop += sizeof (uint64_t);
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SV2: //< _W_r_i_t_e__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

                    #if _Using2ByteVarint_
                          //! Check to see we at least have one byte left in the buffer.
                    if (bufferSpace == 0)
                        return errors ()->report (BufferUnderflowError, header, index, start);

                    //! Load number to write and increment ptrs.
                    ui2_ptr = reinterpret_cast<const uint16_t*>(ptrs[index]);

                    ui2 = *ui2_ptr;

                    //! We are using the same code to print both signed and unsigned 
                    //! varints. In order to convert from a negative 2's complement 
                    //! signed integer to a transmittable format, we need to invert the 
                    //! bits and add 1. Then we just shift the bits left one and put the
                    //! sign bit in the LSB.

                    if (ui2 >> ((sizeof (ui2) * 8 - 1)))
                        ui2 = 0x1 & (((~ui2) + 1) << 1);
                    else
                        ui2 = ui2 << 1;

                    goto WriteUV2;

                case UV2: //< _W_r_i_t_e__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___

                    if (bufferSpace == 0)
                        return errors ()->report (BufferUnderflowError, header, index, start);

                    //! Load next pointer and increment ptrs.
                    ui2_ptr = reinterpret_cast<const uint16_t*>(ptrs[index]);


                    ui4 = *ui2_ptr;

                WriteUV2:
                    if (--bufferSpace == 0)
                        return errors ()->report (BufferUnderflowError, header, index, start);

                    ui1 = ((byte)ui2) & 0x7f;
                    ui2 = ui2 >> 7;
                    if (++stop >= end) stop -= l_slotSize;
                    if (ui2 == 0)
                    {
                        *stop = ui1 | 0x80;
                        break;
                    }
                    *stop = ui1;

                    ui1 = ((byte)ui2) & 0x7f;
                    ui2 = ui2 >> 7;
                    if (++stop >= end) stop -= l_slotSize;
                    if (ui2 == 0)
                    {
                        *stop = ui1 | 0x80;
                        break;
                    }
                    *stop = ui1;

                    break;
                case UV2:
                    #endif

                case SV4: //< _W_r_i_t_e__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

                    #if _Using4ByteVarint_
                          //! Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<const uint32_t*>(ptrs[index]);


                    ui4 = *ui4_ptr;

                    //! We are using the same code to print both signed and unsigned 
                    //! varints. In order to convert from a negative 2's complement 
                    //! signed integer to a transmittable format, we need to invert the 
                    //! bits and add 1. Then we just shift the bits left one and put the
                    //! sign bit in the LSB.

                    if (ui4 >> ((sizeof (ui4) * 8 - 1)))
                        ui4 = 0x1 & (((~ui4) + 1) << 1);
                    else
                        ui4 = ui4 << 1;

                    goto WriteUV4;


                case UV4: //< _W_r_i_t_e__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___

                    if (bufferSpace == 0) return errors ()->report (BufferUnderflowError, header, index, start);

                    // Load next pointer and increment ptrs.
                    ui4_ptr = reinterpret_cast<const uint32_t*>(ptrs[index]);


                    ui4 = *ui4_ptr;

                WriteUV4:   //< Manual do while loop to optimize loop exit.
                    {

                        if (--bufferSpace == 0)
                            return errors ()->report (BufferUnderflowError, header, index, start);

                        ui1 = ((byte)ui4) & 0x7f;
                        ui4 = ui4 >> 7;
                        if (++stop >= end) stop -= l_slotSize;
                        if (ui4 == 0)
                        {
                            *stop = ui1 | 0x80;
                            break;
                        }
                        *stop = ui1;

                        goto WriteUV4;
                    }
                    #else
                case UV4:
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case SV8: //< _W_r_i_t_e__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

                    #if _Using8ByteVarint_
                          //! Load next pointer and increment ptrs.
                    ui8_ptr = reinterpret_cast<const uint64_t*>(ptrs[index]);


                    ui8 = *ui8_ptr;

                    //! We are using the same code to print both signed and unsigned 
                    //! varints. In order to convert from a negative 2's complement 
                    //! signed integer to a transmittable format, we need to invert the 
                    //! bits and add 1. Then we just shift the bits left one and put the
                    //! sign bit in the LSB.

                    if (ui8 >> ((sizeof (ui8) * 8 - 1))) ui8 = 0x1 & (((~ui8) + 1) << 1);
                    else               ui8 = ui8 << 1;

                    goto WriteUV8;

                case UV8: //< _W_r_i_t_e__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___

                    if (bufferSpace == 0)
                        return errors ()->report (BufferUnderflowError, header, index, start);

                    // Load next pointer and increment ptrs.
                    ui8_ptr = reinterpret_cast<const uint64_t*>(ptrs[index]);


                    ui8 = *ui8_ptr;

                WriteUV8:   //< Manual do while loop to optimize loop exit.
                    {

                        if (--bufferSpace == 0)
                            return errors ()->report (BufferUnderflowError, header, index, start);

                        ui1 = ((byte)ui8) & 0x7f;
                        ui8 = ui8 >> 7;
                        if (++stop >= end) stop -= l_slotSize;
                        if (ui8 == 0)
                        {
                            *stop = ui1 | 0x80;
                            break;
                        }
                        *stop = ui1;

                        goto WriteUV8;
                    }
                    #else
                case UV8:
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR1:  //< _W_r_i_t_e__A_r_r_a_y________________________________

                    #if USING_AR1
                           //! Load next pointer and increment ptrs.
                    ui1_ptr = reinterpret_cast<const byte*>(ptrs[index]);

                    ui1 = *ui1_ptr;

                    //! Copy the book into memory.
                    if ((start > stop) && (start + ui1 >= end))
                    {
                        //! Calculate upper chunk size.
                        temp = end - stop;
                        temp -= temp;

                        memcpy (stop, ui2_ptr, temp);
                        memcpy (begin, ui2_ptr + temp, (size_t)temp);
                        stop = begin + temp;
                        break;
                    }
                    memcpy (stop, ui2_ptr, (size_t)temp);
                    stop += temp;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR2:  //< _W_r_i_t_e__A_r_r_a_y_2______________________________

                    #if USING_AR2
                           //! Load next pointer and increment ptrs.
                    ui2_ptr = reinterpret_cast<const uint16_t*>(ptrs[index]);

                    ui2 = *ui2_ptr;

                    //! Copy the book into memory.
                    if ((start > stop) && (start + ui2 >= end))
                    {
                        //! Calculate upper chunk size.
                        temp = end - stop;
                        temp -= temp;

                        memcpy (stop, ui2_ptr, temp);
                        memcpy (begin, ui2_ptr + temp, temp);
                        stop = begin + temp;
                        break;
                    }
                    memcpy (stop, ui2_ptr, temp);
                    stop += temp;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR4:  //< _W_r_i_t_e__A_r_r_a_y_4______________________________

                    #if USING_AR2
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case AR8:  //< _W_r_i_t_e__A_r_r_a_y_8______________________________

                    #if USING_AR8
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case ESC: //< _W_r_i_t_e__E_s_c_a_p_e_S_e_q_u_e_n_c_e_______________
                    #if _UsingESC_
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case BK8:  //< _W_r_i_t_e__6_4_-_B_i_t__B_o_o_k_____________________

                    # if USING_BK2
                    ui4_ptr = reinterpret_cast<const uint32_t*>(ptrs[index]);
                    if (ui4_ptr == nullptr) break;  //! This isn't an error because the user might want to pass in a nullptr.

                                                    //! To save stack space, we have to manually load the book header and calculate size.

                                                    //! Load numKeys.
                    ui4 = *ui4_ptr;
                    temp = ui4; //! Store temp variable

                                //! Load bufferSize.
                    ui4 = *(ui4_ptr + sizeof (uint16_t));
                    temp += ui4;    //! Add the stored temp variable.

                                    //! Check the number of pages and buffered number of pages is valid.
                    if (temp >= MaxNumPagesBook8 || (temp & 0x7) != 0)
                        return errors ()->report (InvalidNumBookPagesError, header, index, start);

                    //! Load collisionsSize.
                    ui4 = *(ui4_ptr + 2 * sizeof (uint16_t));
                    collisionsSize = static_cast<uint32_t>(ui4);    //! put aside till after we calc page indexes size.

                                                                    //! Calculate indexed size
                    temp *= (sizeof (uint32_t) + sizeof (uint32_t) + (collisionsSize > 0) *(sizeof (byte) + sizeof (hash32_t)));
                    temp += collisionsSize;

                    //! Load headerSize.
                    ui4 = *(ui4_ptr + 2 * sizeof (uint16_t) + sizeof (uint32_t));
                    temp += ui4;
                    ui4_ptr += sizeof (uint32_t);

                    //! Load dataSize.
                    ui8_ptr = reinterpret_cast<uint64_t*>(ui4_ptr);
                    ui8 = *(ui8_ptr + 2 * sizeof (uint16_t) + 2 * sizeof (uint32_t));
                    temp += ui8;
                    ui8_ptr += sizeof (uint64_t);

                    //! add the size of the header and we're done.
                    temp += sizeof (Book8);

                    if (bufferSpace < temp)
                        return errors ()->report (BufferUnderflowError, header, index, start);
                    bufferSpace -= (uint_t)temp;

                    //! Now we can copy the book into memory.
                    if ((start > stop) && (start + ui1 >= end))
                    {
                        //! Calculate upper chunk size.
                        temp = end - stop;
                        temp -= temp;

                        memcpy (stop, ui4_ptr, temp);
                        memcpy (begin, ui4_ptr + temp, temp);
                        stop = begin + temp;
                        break;
                    }
                    memcpy (stop, ui4_ptr, temp);
                    stop += temp;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case BK4:  //< _W_r_i_t_e__B_o_o_k_4________________________________

                    # if USING_BK4
                    ui2_ptr = reinterpret_cast<const uint16_t*>(ptrs[index]);
                    if (ui2_ptr == nullptr) break;  //! This isn't an error because the user might want to pass in a nullptr.

                                                    //! To save stack space, we have to manually load the book header and calculate size.

                                                    //! Load numKeys.
                    ui2 = *ui2_ptr;
                    temp = ui2; //! Store temp variable

                                //! Load bufferSize.
                    ui2 = *(ui2_ptr + sizeof (uint16_t));
                    temp += ui2;    //! Add the stored temp variable.

                                    //! Check the number of pages and buffered number of pages is valid.
                    if (temp >= MaxNumPagesBook4 || (temp & 0x7) != 0)
                        return errors ()->report (InvalidNumBookPagesError, header, index, start);

                    //! Load collisionsSize.
                    ui2 = *(ui2_ptr + 2 * sizeof (uint16_t));
                    collisionsSize = static_cast<uint16_t>(ui2);    //! put aside till after we calc page indexes size.

                                                                    //! Calculate indexed size
                    temp *= (sizeof (uint16_t) + sizeof (uint16_t) + (collisionsSize > 0) *(sizeof (uint16_t) + sizeof (hash32_t)));
                    temp += collisionsSize;

                    //! Load headerSize.
                    ui2 = *(ui2_ptr + 3 * sizeof (uint16_t));
                    temp += ui2;
                    ui2_ptr += sizeof (uint16_t);

                    //! Load dataSize.
                    ui2 = *(ui2_ptr + 4 * sizeof (uint16_t));
                    temp += ui2;
                    ui2_ptr += sizeof (uint16_t);

                    //! add the size of the header and we're done.
                    temp += sizeof (Book4);

                    if (bufferSpace < temp)
                        return errors ()->report (BufferUnderflowError, header, index, start);
                    bufferSpace -= (uint_t)temp;

                    //! Copy the book into memory.
                    if ((start > stop) && (start + ui1 >= end))
                    {
                        //! Calculate upper chunk size.
                        temp = end - stop;
                        temp -= temp;

                        memcpy (stop, ui4_ptr, temp);
                        memcpy (begin, ui4_ptr + temp, temp);
                        stop = begin + temp;
                        break;
                    }
                    memcpy (stop, ui4_ptr, temp);
                    stop += temp;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case BK2:  //< _W_r_i_t_e__B_o_o_k_2________________________________

                    # if USING_BK2
                    ui2_ptr = reinterpret_cast<const uint16_t*>(ptrs[index]);
                    if (ui2_ptr == nullptr) break;  //! This isn't an error because the user might want to pass in a nullptr.

                                                    //! To save stack space, we have to manually load the book header and calculate size.

                                                    //! Load numKeys.
                    ui2 = *ui2_ptr;
                    temp = ui2; //! Store temp variable

                                //! Load bufferSize.
                    ui2 = *(ui2_ptr + sizeof (byte));
                    temp += ui2;    //! Add the stored temp variable.

                                    //! Check the number of pages and buffered number of pages is valid.
                    if (temp >= MaxNumPagesBook2 || (temp & 0x7) != 0)
                        return errors ()->report (InvalidNumBookPagesError, header, index, start);

                    //! Load collisionsSize.
                    ui2 = *(ui2_ptr + 2 * sizeof (byte));
                    collisionsSize = static_cast<uint16_t>(ui2);    //! put aside till after we calc page indexes size.

                                                                    //! Calculate index sizes
                    temp *= (sizeof (uint16_t) + sizeof (uint16_t) + (collisionsSize > 0)*(sizeof (byte) + sizeof (hash32_t)));
                    temp += collisionsSize;

                    //! Load headerSize.
                    ui2 = *(ui2_ptr + 2 * sizeof (byte) + sizeof (uint16_t));
                    temp += ui2;
                    ui2_ptr += sizeof (uint16_t);

                    //! Load dataSize.
                    ui2 = *(ui2_ptr + 2 * sizeof (byte) + 2 * sizeof (uint16_t));
                    temp += ui2;
                    ui2_ptr += sizeof (uint16_t);

                    temp += sizeof (Book2);   //! add the size of the header and we're done.

                    if (bufferSpace < temp)
                        return errors ()->report (BufferUnderflowError, header, index, start);
                    bufferSpace -= (uint_t)temp;

                    //! Copy the book into memory.
                    if ((start > stop) && (start + ui1 >= end))
                    {
                        //! Calculate upper chunk size.
                        temp = end - stop;
                        temp -= temp;

                        memcpy (stop, ui2_ptr, temp);
                        memcpy (begin, ui2_ptr + temp, temp);
                        stop = begin + temp;
                        break;
                    }
                    memcpy (stop, ui2_ptr, temp);
                    stop += temp;
                    break;
                    #else
                    return errors ()->report (UnsupportedTypeError, header, index, start);
                    #endif

                case US:
                    break;

                default:
                    return errors ()->report (ReadInvalidTypeError, header, index, start);
            }
        }

        txStop = static_cast<uint_t>(stop - begin);
        return 0;
        #else
        byte* start = getTxBaseAddress () + getRxStart ();
        return errors ()->report (UnsupportedTypeError, header, 0, start);
        #endif
    }

    void print()
    /*< Prints out the terminal to the debug console. */
    {
        byte* beginRx = getTxBaseAddress (),
            *endRx = beginRx + slotSize;
        byte* cursorRx = beginRx + txStart,
            *startRx = cursorRx,
            *stopRx = beginRx + txStop;

        byte* beginTx = getTxBaseAddress () + slotSize,
            *endTx = beginTx + slotSize;
        byte* cursorTx = beginTx + txStart,
            *startTx = cursorTx,
            *stopTx = beginTx + txStop;

        printLine ('_');
        printf ("| Terminal: %p\n"
            "| rxState: %u txState %u\n"
            "| stackHeight: %u numVerifying: %u stackSize: %u\n slotSize: %u",
            "| txStart: %u  txStop: %u txHash: %x\n"
            "| rxStart: %u  rxStop: %u rxHash: %x\n",
            this,
            rxState, txState,
            stackHeight, numVerifying, stackSize, slotSize,
            diff (this, startRx), diff (this, startRx), txHash,
            diff (this, startTx), diff (this, stopTx)); rxHash,
            putchar ('\n');
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        putchar ('\n');
        putchar ('|');
        for (int i = 0; i < 79; ++i) putchar ('_');
        putchar ('\n');

        printMemory (this, sizeof (Terminal) + stackSize * (2 * sizeof (void*)) + slotSize + slotSize);

        printLine ('_');
        printf ("sizeof (Terminal) + stackSize * (2 * sizeof(void*)) + slotSize + slotSize: %u\n",
            sizeof (Terminal) + stackSize * (2 * sizeof (void*)) + slotSize + slotSize);
    }

    private:

    NONCOPYABLE(Terminal)
    
    volatile byte rxState,                  //< The rx streaming state.
        txState,                            //< The tx streaming state.
        lastRxState;                        //< The last Rx state.
    byte stackHeight,                       //< The number of IObjcts on the stack.
        stackSize,                          //< The stack buffer size.
        numVerifying,                       //< The height of the headers and cursors stacks.
        statesSize,                         //< The size of the state stack. ? What is the size of the state stack compared to the stackSize?
        numStates;                          //< The number of states on the state stack.
    uint_t bytesLeft,                       //< Countdown counter for POD type being parsed.
        slotSize,                           //< The size of the rx and tx buffers.
        txOpen,                             //< The index the main program is reading from.
        rxRead;                             //< The index the main program is reading from.
    volatile uint_t rxStart,                //< The start of the Rx buffer data.
        rxStop,                             //< The stop of the Rx buffer data.
        txStart,                            //< The start of the Tx buffer data.
        txStop;                             //< The stop of the Tx buffer data.
    hash32_t txHash,                        //< The tx hash for receiver to verify data integrity.
        rxHash;                             //< The rx hash for verifying incoming data integrity.
    volatile const uint_t* scanHeader;      //< Pointer to the current header being verified.
    IDevice* device;                        //< The current device in the ESC being verified.
    Door* origin;                           //< The door to the system of origin.
    const uint_t* headers;                  //< The first header being scanned.

    byte* getTxBaseAddress()
    /*< The base of the io. */
    {
        return reinterpret_cast<byte*>(this) + sizeof (Terminal) + stackSize * (2 * sizeof (void*));
    }

    byte* getTxStartAddress ()
    /*< Gets the start of the TX buffer. */
    {
        return reinterpret_cast<byte*>(this) + sizeof (Terminal) + stackSize * (2 * sizeof (void*)) + txStart;
    }

    ticket_t pushScanHeader(const uint_t* header)
    /*< Selects the given member. */
    {
        uint16_t l_numVerifying = numVerifying;
        if (l_numVerifying >= stackSize)
        {
            return errors ()->report (StackOverflowError, header, 0, getTxStartAddress ());
        }
        uint_t** scanHeaders_ptr = reinterpret_cast<uint_t**>(this) + (sizeof (Terminal) / sizeof (uint_t**)) +
            l_numVerifying;
        uint_t* ptr = const_cast<uint_t*>(scanHeader);
        *scanHeaders_ptr = ptr;
        scanHeader = header;
        return 0;
    }

    ticket_t pushHeader(const uint_t* header)
    /*< Pushes a header onto the scan stack.*/
    {
        if (stackHeight >= stackSize)
        {
            // Big error! Stack overflow!
            return errors ()->report (StackOverflowError, header, 0, getTxStartAddress ());
        }

        //if(dc == nullptr) return noDevceSelectedError();



        return 0;
    }

    void scanNextType ()
    /*< Scans the next type scanHeader type. */
    {
        if (scanHeader == nullptr)
        {
            enterState (RxScanningArgsState);
            return;
        }

        uint_t type = *scanHeader;
        if (type == NIL)     //! Done scanning args.
        {
            popScanHeader ();
            return;
        }
        ++scanHeader;
        rxState = lastRxState;
        //type = *scanHeader;
    }

    byte* getStateStackBase_ptr ()
    /*< Gets the base address of the state stack. */
    {
        return reinterpret_cast<byte*>(this) + sizeof (Terminal);
    }

    const uint_t* getHeaderStackBase_ptr ()
    /*< Gets the base address of the header stack. */
    {
        return reinterpret_cast<const uint_t*>(reinterpret_cast<byte*>(this) + sizeof (Terminal) + stackHeight);
    }

    IDevice** getDeviceStackBase_ptr ()
    /*< Gets the base address of the device stack. */
    {
        return reinterpret_cast<IDevice**>(reinterpret_cast<byte*>(this) + sizeof (Terminal) + stackHeight * stackSize * sizeof (const uint_t*));
    }

    ticket_t popScanHeader()
    /*< Pops a header off the scan stack. */
    {
        if (numVerifying == 0)
        {
            return errors ()->report (TooManyPopsError);
        }
        --numVerifying;
        uint_t** scanHeaders_ptr = reinterpret_cast<uint_t**>(this) + (sizeof (Terminal) / sizeof (uint_t**)) +
            numVerifying;
        exitState ();
        scanHeader = *scanHeaders_ptr;
        return 0;
    }

    ticket_t enterState(byte newState)
    /*< Pushes the newState onto the verifier stack. */
    {
        if (newState > RxNumStates) return errors ()->report (InvalidStateError);
        if (stackHeight >= stackSize) return errors ()->report (StackOverflowError);
        getStateStackBase_ptr ()[stackHeight++] = rxState;
        rxState = newState;
        return 0;
    }
    //{
    //    if (newState >= RxNumStates) return errors ()->report (StackOverflowError);
    //    getStateStackBase_ptr ()[numStates++] = newState;
    //    return 0;
    //}

    ticket_t exitState()
    /*< Exits the current state. */
    {
        if (stackHeight == 0) return errors ()->report (TooManyPopsError);
        rxState = getStateStackBase_ptr ()[--stackHeight];
        return 0;
    }

    void closeExpression()
    /*< Closes the current expression and cues it for execution. */
    {
        #if DEBUG
        printf ("\r\n[FF]\r\n");
        #endif
    }

    void cancelExpression()
    /*< Cancels the current expression. */
    {
        #if DEBUG
        printf ("\r\n[CAN]\r\n\n");
        #endif

        //stopAddress = txOpen;
    }

    void scrubExpression()
    /*< Cancels the current expression and writes zeros to the buffer. */
    {
        //! Erase the buffer by writing zeros to it.

        uint_t l_slotSize = slotSize;

        byte* begin = getTxBaseAddress (),
            *end = begin + l_slotSize,
            *start = begin + txStart,
            *stop = begin + txStop;

        uint_t bufferSpace = calcRingBufferSpace (start, stop, l_slotSize);

        if (start == stop) return; //< Nothing to do.
        if (start > stop)
        {
            memset (start, '\0', end - start);
            memset (begin, '\0', start - begin);
            return;
        }
        memset (start, '\0', stop - start);
    }
};

inline Terminal* createTerminal(uint_t terminalSize, uint16_t stackSize)
/*< Creates a Terminal with the given buffer and stack size. */
{
    try
    {
        byte* buffer = new byte[terminalSize];
        #if DEBUG_CHINESEROOM
        memset (buffer, '\0', terminalSize);
        #endif
        Terminal* t = new(buffer) Terminal ();
        return t->init (terminalSize, stackSize);
    }
    catch (...)
    {
        return nullptr;
    }
}

inline void destroy(Terminal* t)
/*< Destroys the given Terminal. */
{
    if (t == nullptr) return;
    delete[] reinterpret_cast<byte*>(t);
}

}       //< namespace _

#endif  //< CHINESEROOM_TERMINAL_H
