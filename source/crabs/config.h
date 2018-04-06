/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/config.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_CRABS_CONFIG
#define HEADER_FOR_CRABS_CONFIG

// @todo Check all values of assembly_settings.inl, store them as an enum, 
// then #undef them.

#define BARE_METAL      1   //< Bare metal (i.e. No OS) OS type macro.
#define BARE_METAL_MBED 2   //< mbed bare-metal OS type macro.
#define MBED_OS         3   //< Arduino bare-metal OS type macro.
#define ARDUINO         4   //< mbed OS type macro.
#define MINGW32         5   //< Win32 OS type macro.
#define WINDOWS         6   //< Slots Universal OS type macro.
#define ANDROID         7   //< Android OS type macro.
#define LINUX           8   //< Linux OS type macro.
#define OSX             9   //< OSX OS type macro.
#define IOS             10  //< iOS OS type macro.

#define X86             1   //< 32-bit x86 processor family macro.
#define X64             2   //< 64-bit x86 processor family macro.
#define ARM8            3   //< ARM8 processor family macro.
#define ARM16           4   //< ARM16 processor family macro.
#define ARM32           5   //< ARM32 processor family macro.
#define ARM64           6   //< ARM64 processor family macro.

// Stupid dumb-face Big-Endian nonsense.

#define LITTLE          1
#define BIG             2   //< Don't ask me what they where thinking.

#define YES             1   //< Logical yes.
#define NO              0   //< Logical no.

// Executable assembly type macro.
#define EXECECUTABLE                1

// Statically linked library assembly type macro.
#define STATICALlY_LINKED_LIBRARY   2

// Dynamically linked library assembly type macro.
#define DYNAMICALLY_LINKED_LIBRARY  3

// Dynamically linked library assembly type macro.
#define SINGLE_DLL                  4

#define LARGEST_INT                 2147483647  //< Use this for 32-bit CPUs

#include <assembly.h>       //< Inline config stuff for your project.

typedef const char* string_ptr;

#if CRABS_MAX_ERRORS < 0
#error MAX_ERRORS must be greater than 0
#elif CRABS_MAX_ERRORS <= 255
typedef uint8_t ticket_t;
#elif MAX_ERRORS <= 65535
typedef uint16_t ticket_t;
#else
typedef uint32_t ticket_t;
#endif

#if CRABS_MAX_PARAMS < 0
#error MAX_ERRORS must be greater than 0
#endif

#if CRABS_TEXT_SIZE_DEFAULT < 0
#error MAX_ERRORS must be greater than 0
#endif

#if COM_TIMEOUT_TICKS < 0
#error MAX_ERRORS must be greater than 0
#endif

// Big-Endian nonsense
#define LITTLE   1 
#define BIG      2 //< Don't ask me what they where thinking.

// Executable assembly type macro.
#define EXECECUTABLE               1
// Statically linked library assembly type macro.
#define STATICALlY_LINKED_LIBRARY  2
// Dynamically linked library assembly type macro.
#define DYNAMICALLY_LINKED_LIBRARY 3
// Dynamically linked library assembly type macro.
#define SINGLE_DLL                 4

namespace _ {
enum {
    kNo              = 0,                 //< Script no/false value.
    kYes             = 1,                 //< Script yes/true value.
    //kMinFloorSize  = 256,               //< Min size of a room.
    //kMaxFloorSize  = 0x7FFFFFFC,        //< Max room size: 2GB - 7 bits.
    kMaxFloorsCount  = CRABS_MAX_WALLS,   //< Size of the Room Floor (buffer).
    kSlotSizeMin     = 128,               //< Min size of a Slot - 1.
    kMaxErrors       = CRABS_MAX_ERRORS,  //< Max errors before blowing up.
    kParamsMax       = CRABS_MAX_PARAMS,  //< Max number of parameters.
    kWordAddressMask = sizeof (void*) - 1,//< For masking the word address.
    kTimeoutMicroseconds = COM_TIMEOUT_TICKS, //< Timeout time in microseconds.
    kAddressLengthMax= CRABS_MAX_ADDRESS_LENGTH, //< Max address (ADR) length.
    kMinStackSize    = 1,                 //< Min Expr stack size.
    kOpNameLengthMax = CRABS_OP_MAX_NAME_LENGTH,
    // Max length of a Op description string.
    kOpDescriptionLengthMax = CRABS_OP_MAX_DECRABSION_LENGTH,
    // Max length of a Text.
    kCharCount        = CRABS_TEXT_LENGTH,
    // Number of bits to shift to multiply or divide to get num_words.
    kWordSizeShift   = (sizeof (void*) == 8) ? 3 : //< Shift left 3 to * by 8.
                       (sizeof (void*) == 4) ? 2 : //< Shift right 2 to / by 4.
                       1,
    // Max length of a float-to-string + 1.
    kkFloat32DigitsMax    = 4 + FLT_MANT_DIG - FLT_MIN_EXP,
    // Max length of a double-to-string + 1.
    kFloat64DigitsMax   = 4 + DBL_MANT_DIG - DBL_MIN_EXP,

    // Extra reserved memory at the end of BOut.
    kBOutOverflowSize = 32,

    // Size of the Display Out Slot.
    kSlotBufferSizeRx = CRABS_BUFFER_SIZE_RX,

    // Size of the KeyboardInBuffer.
    kSlotBufferSizeTx = CRABS_BUFFER_SIZE_TX,

    kWindowSizeMin = 512
};
}   //< namespace _

#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS
#undef CRABS_LOG_SIZE
#undef OPERATION_MAX_NAME_LENGTH
#undef OPERATION_MAX_DECRABSION_LENGTH
#undef CRABS_TEXT_LENGTH
#undef CRABS_BUFFER_SIZE_RX
#undef CRABS_BUFFER_SIZE_TX

#define KABUKI

typedef long word_t;

// Pre-compiled headers: comment out those you're not using.


typedef unsigned char byte;
typedef unsigned int uint;

typedef int64_t time_us_t; //< A 64-bit microseconds since epoch timestamp.

#if MAX_NUM_SLOTS <= 255
typedef byte slot_t;
enum {
    kMaxNumSlots = 0xff
};
#elif MAX_NUM_SLOTS <=65535
typedef uint16_t slot_t;
enum {
    kMaxNumSlots = 0xffff
};
#else
typedef uint32_t slot_t;
enum {
    kMaxNumSlots = 0xffffffff
};
#endif

/** Below are representations of not-a-numbers.
    
    With signed integers, there is one additional negative number than positive
    numbers due to  the 0. In the CRABS Protocol, this number is used to mark
    invalid or corrupted data. If you are not using them and require the ROM
    space, it will not harm anything to comment them out.

    @code
    #include <iostream>
    #define NaN_SI4 0xFFFFFFF
    void BlowUp () { PRINTF ("The sky is falling!"}
    if (-1 == NaN_SI4)
        BlowUp ();
    @endcode
*/

// int-sized not-a-number.
enum { NaNInt = sizeof (int) == 4 ? static_cast<int>(0xFFFFFFFF) :
                sizeof (int) == 2 ? static_cast<int>(0xFFFF)     : 0,
};


typedef uint16_t hash16_t;  		 //< Using unsigned 16-bit hash type.
typedef uint32_t hash32_t;  		 //< Using unsigned 32-bit hash type.
typedef uint64_t hash64_t;  		 //< Using unsigned 64-bit hash type.

enum {
    kLargest16BitPrime = 0xFFE1,     //< Largest 16-bit prime: 65521.
    kLargest32BitPrime = 0xFFFFFFFB, //< Largest 32-bit prime: 4294967291.
};

static const uint64_t kLargest64BitPrime = 0xFFFFFFFFFFFFFFC5;
//< Largest 64-bit prime: 18446744073709551557;

//< A char with a single newline char.

/** The level will more code creating a larger binary. Use one
    underscore to use more memory, and two underscores to use even more. */
#if CRABS_MEMORY_PROFILE == 1
typedef int8_t   int_t;     //< Buffer signed index type.
typedef uint8_t  uint_t;    //< Buffer unsigned index type.
typedef int16_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint16_t duint_t;   //< Buffer double-wide unsigned index type.
typedef uint8_t  index_t;   //< Largest bit-depth TIndex this system supports.
typedef uint16_t header_t;  //< Largest bit-depth THeader this system supports.
typedef uint16_t data_t;    //< Largest bit-depth TData this system supports.

#elif (CRABS_MEMORY_PROFILE == 2) || (CRABS_MEMORY_PROFILE == 3)
typedef int16_t  int_t;     //< Buffer signed index type.
typedef uint16_t uint_t;    //< Buffer unsigned signed index type.
typedef int32_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint32_t duint_t;   //< Buffer double-wide unsigned index type.
typedef int16_t  index_t;   //< Default TIndex size.
typedef uint16_t header_t;  //< Default TKey size.
typedef uint32_t data_t;    //< Default TData size.

#elif CRABS_MEMORY_PROFILE == 5
typedef int32_t  int_t;     //< Buffer signed index type.
typedef uint32_t uint_t;    //< Buffer unsigned signed index type.
typedef int16_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint64_t duint_t;   //< Buffer double-wide unsigned index type.
typedef uint16_t index_t;   //< Default TIndex size.
typedef uint32_t header_t;  //< Default TKey size.
typedef uint64_t data_t;    //< Default TData size.

#else
#error  Invalid CRABS_MEMORY_PROFILE
#endif

#if CRABS_MEMORY_PROFILE >= 3 || DEBUG
#define USING_TEXT_SCRIPT 3
#endif  //< CRABS_MEMORY_PROFILE >= 3

/** Macro declares a class to be non-copyable. */
#define NONCOPYABLE (ClassName)\
  ClassName (const ClassName&) = delete;\
  void operator= (const ClassName&) = delete;
  //ClassName () = default //< Saw this once on the net but do we need it?

#endif  //< HEADER_FOR_CRABS_CONFIG
