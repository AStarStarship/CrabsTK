/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/module_config.h
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_CONFIG_H
#define SCRIPT_CONFIG_H

#define SCRIPT_MAJOR_VERSION 0
#define SCRIPT_MINOR_VERSION 9

// @todo Check all values of assembly_settings.inl, store them as an enum, 
// then #undef them.

#define BARE_METAL       1  //< Bare metal (i.e. No OS) OS type macro.
#define MBED_BARE_METAL  2  //< mbed bare-metal OS type macro.
#define MBED_OS          3  //< Arduino bare-metal OS type macro.
#define ARDUINO          4  //< mbed OS type macro.
#define MINGW32          5  //< Win32 OS type macro.
#define WINDOWS          6  //< Windows Universal OS type macro.
#define ANDROID          7  //< Android OS type macro.
#define LINUX            8  //< Linux OS type macro.
#define OSX              9  //< OSX OS type macro.
#define IOS              10 //< iOS OS type macro.
#define X86              11 //< x86 processor type macro.
#define X64              12 //< x64 processor type macro.
#define ARM8             13 //< ARM8 processor type macro.
#define ARM16            14 //< ARM16 processor type macro.
#define ARM32            15 //< ARM32 processor type macro.
#define ARM64            16 //< ARM64 processor type macro.

// Stupid dumb-face Big-Endian nonsense

#define ENDIAN_LITTLE   1
#define ENDIAN_BIG      2   //< Don't ask me what they where thinking.

// Executable assembly type macro.
#define EXECECUTABLE                1
// Statically linked library assembly type macro.
#define STATICALlY_LINKED_LIBRARY   2
// Dynamically linked library assembly type macro.
#define DYNAMICALLY_LINKED_LIBRARY  3
// Dynamically linked library assembly type macro.
#define SINGLE_DLL                  4

// Logical yes.
#define YES                         1
// Logical no.
#define NO                          0

#include <assembly.h>       //< Inline config stuff for your project.

typedef const char* string_ptr;

#if SCRIPT_CHAR_WIDTH == 8
typedef uint8_t char_t;
#elif SCRIPT_CHAR_WIDTH == 16
typedef int16_t char_t;
#elif SCRIPT_CHAR_WIDTH == 32
typedef int32_t char_t;
#else
#error SCRIPT_CHAR_WIDTH must be 8, 16, or 32!
#endif

#if MAX_ERRORS < 0
#error MAX_ERRORS must be greater than 0
#elif MAX_ERRORS <= 255
typedef uint8_t ticket_t;
#elif MAX_ERRORS <= 65535
typedef uint16_t ticket_t;
#else
typedef uint32_t ticket_t;
#endif

#if MAX_NUM_PARAMS < 0
#error MAX_ERRORS must be greater than 0
#endif

#if MAX_STRING_LENGTH < 0
#error MAX_ERRORS must be greater than 0
#endif

#if COM_TIMEOUT_TICKS < 0
#error MAX_ERRORS must be greater than 0
#endif

namespace _ {
enum {
    // I think these two are macros.
    //kMinFloorSize     = 256,            //< Min size of a room.
    //kMaxFloorSize     = 0x7FFFFFFC,     //< Max room size: 2GB - 7 bits.
    kFloorSize       = ROOM_FLOOR_SIZE,   //< Size of the Room Floor (buffer).
    kMinSlotSize     = 128,               //< Min size of a Slot - 1.
    kMaxErrors       = MAX_ERRORS,        //< Max errors before blowing up.
    kMaxNumParams    = MAX_NUM_PARAMS,    //< Max number of parameters.
    kMaxTextLength = MAX_STRING_LENGTH, //< Max char length.
    kTimeoutMicroseconds = COM_TIMEOUT_TICKS,
    //< The number of seconds before a timeout over a generic communication
    //< link.
    kWordAddressMask = sizeof (void*) - 1,   //< For masking the word address.
    kMaxAddresLength = 255,                  //< Max address (ADR) length.
    kMinStackSize    = 1,                    //< Min Expression stack size.
    kOperationMaxNameLength = OPERATION_MAX_NAME_LENGTH,
    kOperationMaxDescriptionLength = OPERATION_MAX_DESCRIPTION_LENGTH,
};
}
#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS
#undef SCRIPT_LOG_SIZE
#undef OPERATION_MAX_NAME_LENGTH
#undef OPERATION_MAX_DESCRIPTION_LENGTH

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
    numbers due to  the 0. In the SCRIPT Protocol, this number is used to mark
    invalid or corrupted data. If you are not using them and require the ROM
    space, it will not harm anything to comment them out.
    @code
    int32_t& example (float input);
    if (example () == NaN_SI4) blowUp ();
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
#if MEMORY_PROFILE == 1
typedef int8_t   int_t;     //< Buffer signed index type.
typedef uint8_t  uint_t;    //< Buffer unsigned index type.
typedef int16_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint16_t duint_t;   //< Buffer double-wide unsigned index type.
typedef uint8_t  index_t;   //< Largest bit-depth TIndex this system supports.
typedef uint16_t header_t;  //< Largest bit-depth THeader this system supports.
typedef uint16_t data_t;    //< Largest bit-depth TData this system supports.

#elif (MEMORY_PROFILE == 2) || (MEMORY_PROFILE == 3)
typedef int16_t  int_t;     //< Buffer signed index type.
typedef uint16_t uint_t;    //< Buffer unsigned signed index type.
typedef int32_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint32_t duint_t;   //< Buffer double-wide unsigned index type.
typedef int16_t  index_t;   //< Default TIndex size.
typedef uint16_t header_t;  //< Default TKey size.
typedef uint32_t data_t;    //< Default TData size.

#elif MEMORY_PROFILE == 5
typedef int32_t  int_t;     //< Buffer signed index type.
typedef uint32_t uint_t;    //< Buffer unsigned signed index type.
typedef int16_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint64_t duint_t;   //< Buffer double-wide unsigned index type.
typedef uint16_t index_t;   //< Default TIndex size.
typedef uint32_t header_t;  //< Default TKey size.
typedef uint64_t data_t;    //< Default TData size.

#else
#error  Invalid MEMORY_PROFILE
#endif

#if MEMORY_PROFILE >= 3 || SCRIPT_DEBUG
#define USE_MORE_ROM 3
#endif  //< MEMORY_PROFILE >= 3

/** Macro declares a class to be non-copyable. */
#define NONCOPYABLE (ClassName)\
  ClassName (const ClassName&) = delete;\
  void operator= (const ClassName&) = delete;
  //ClassName () = default //< Saw this once on the net but do we need it?

#endif  //< SCRIPT_CONFIG_H
