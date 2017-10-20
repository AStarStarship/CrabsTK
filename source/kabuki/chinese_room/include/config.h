/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/config.h
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

#pragma once

#include <stdafx.h>

#ifndef KABUKI_TOOLKIT_CONFIG_H
#define KABUKI_TOOLKIT_CONFIG_H

#define CHINESE_ROOM_MAJOR_VERSION 0
#define CHINESE_ROOM_MINOR_VERSION 9

// @todo Check all values of assembly_settings.inl, store them as an enum, 
// then undef them.

// Bare metal (i.e. No OS) OS type macro.
#define BARE_METAL                  1
// mbed bare-metal OS type macro.
#define MBED                        2
// Arduino bare-metal OS type macro.
#define ARDUINO                     4
// mbed OS type macro.
#define MBED_OS                     3
// mbed OS type macro.
#define WIN32                       5
// Win32 OS type macro.
#define WINDOWS                     6
// Windows Universal OS type macro.
#define ANDROID                     7
// Android OS type macro.
#define LINUX                       8
// Linux OS type macro.
#define OSX                         9
// iOS OS type macro.
#define IOS                         10

// x86 processor type macro.
#define X86                         1
// x64 processor type macro.
#define X64                         2
// x86 processor type macro.
#define ARM8                        3
// ARM16 processor type macro.
#define ARM16                       4
// ARM32 processor type macro.
#define ARM32                       5
// ARM64 processor type macro.
#define ARM64                       6

// Stupid dumb-face Big-Endian nonsense

#define LITTLE_ENDIAN               1
#define BIG_ENDIAN                  2

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

#include <assembly.h>               //< Config stuff for your project.

#if CHAR_WIDTH == 8
typedef uint8_t index;
#undef CHAR_WIDTH
#elif CHAR_WIDTH == 16
#undef CHAR_WIDTH
typedef int16_t index;
#elif CHAR_WIDTH == 32
#undef CHAR_WIDTH
typedef int32_t index;
#else
#error CHAR_WIDTH is not 8, 16, or 32!
#endif

#if MAX_ERRORS < 0
#error MAX_ERRORS must be greater than 0
#elif MAX_ERRORS <= 255
typedef uint8_t ticket_t;
#elif MAX_ERRORS <= 65535
typedef uint16_t ticket_t;
#else
#error MAX_ERRORS isn't portable to 16-bit CPUs! Feel free to change this.
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
    kMinRoomSize     = 256,                 //< Min size of a room.
    kMaxRoomSize     = 0x7FFFFFFC,          //< Max room size: 2GB - 7 bits.
    kMinTerminalSize = 256,                 //< Min size of a Terminal - 1.
    kMaxErrors       = MAX_ERRORS,          //< Max errors before blowing up.
    kMaxNumParams    = MAX_NUM_PARAMS,      //< Max number of parameters.
    kMaxStringLength = MAX_STRING_LENGTH,   //< The max string length.
    kTimeoutMicroseconds = COM_TIMEOUT_TICKS,
    //< The number of seconds before a timeout over a generic communication link.
    kWordAddressMask = sizeof (void*) - 1,  //< For masking the word address.
};
}
#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS

#if USE_MORE_ROM
#define KABUKI // @todo Fix me!
#else
#define KABUKI
#endif

#if USING_BK2 || USING_BK4 || USING_BK8
#define USING_BOOKS                 YES
#endif

// Pre-compiled headers: comment out those you're not using.


typedef unsigned char byte;
typedef unsigned int uint;

typedef long long timestamp_t;              //< A 64-bit microsecond timestamp 
                                            //< in microseconds from Unix Epoch.
typedef unsigned char error_t;              //< An error (Might need to use 
                                            //< uint16_t, might not).

#define INVALID_POSITIVE_VALUE 0x7FFFFFFF

#if MAX_ERRORS < 0
#error Invalid MAX_ERRORS
#endif
#if MAX_ERRORS == INVALID_POSITIVE_VALUE
#error Invalid MAX_ERRORS
#endif
#undef MAX_ERRORS


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


typedef uint16_t hash16_t;                  //< Using unsigned 16-bit hash type.
typedef uint32_t hash32_t;                  //< Using unsigned 32-bit hash type.
typedef uint64_t hash64_t;                  //< Using unsigned 64-bit hash type.

enum {
    kLargest16BitPrime  = 65521,            //< The largest 16-bit prime number.
    kLargest32BitPrime = 4294967291,        //< The largest 32-bit prime number.
    kLargest64BitPrime = 18446744073709551557,  
                                            //< The largest 64-bit prime number.
};

static const char EmptyString[] = "\0";     //< The empty string.
static const char NewLineString[] = "\n\0"; //< A string with a single newline char.

/** The level will more code creating a larger binary. Use one 
    underscore to use more memory, and two underscores to use even more. */
#if RAM_USE_LEVEL == 1
typedef int8_t   int_t;                     //< Buffer signed index type.
typedef uint8_t  uint_t;                    //< Buffer unsigned index type.
typedef int16_t  dint_t;                    //< Buffer double-wide signed index type.
typedef uint16_t duint_t;                   //< Buffer double-wide unsigned index type.
typedef byte     index_t;                   //< The largest bit-depth bag index_t this system supports.
typedef uint16_t header_t;                  //< The largest bit-depth bag header_t this system supports.
typedef uint16_t offset_t;                  //< The largest bit-depth bag data_t this system supports.
typedef uint16_t hash_t;                    //< The largest bit-depth bag hash_t this system supports.

namespace _ {
enum {
    kUnitSize = 1 << 6                      //< The size of unit.
};
}

#elif RAM_USE_LEVEL == 2
typedef int16_t  int_t;                     //< Buffer signed index type.
typedef uint32_t uint_t;                    //< Buffer unsigned signed index type.
typedef int16_t  dint_t;                    //< Buffer double-wide signed index type.
typedef uint32_t duint_t;                   //< Buffer double-wide unsigned index type.
typedef uint16_t index_t;                   //< Bag4 index_t.
typedef uint16_t header_t;                  //< Bag4 header_t
typedef uint32_t offset_t;                  //< Bag4 offset_t
typedef hash32_t hash_t;                    //< Bag4 hash_t

namespace _ {
enum {
    kUnitSize = 1 << 10                     //< The size of unit.
};
}

#elif RAM_USE_LEVEL == 3
typedef byte int_t;                         //< Buffer signed index type.
typedef int16_t uint_t;                     //< Buffer unsigned signed index type.
typedef int16_t dint_t;                     //< Buffer double-wide signed index type.
typedef uint64_t duint_t;                   //< Buffer double-wide unsigned index type.
typedef uint16_t index_t;                   //< Bag8 index_t
typedef uint32_t header_t;                  //< Bag8 header_t
typedef uint64_t offset_t;                  //< Bag8 offset_t
typedef hash64_t hash_t;                    //< Bag8 hash_t

namespace _ {
enum {
    kUnitSize = 1 << 10                     //< The size of unit.
};
}
#else
#error  Invalid RAM_USE_LEVEL
#endif

/** Macro declares a class to be non-copyable. */
#define NONCOPYABLE (ClassName)\
  ClassName (const ClassName&)      = delete;\
  void operator= (const ClassName&) = delete;

#endif  //< KABUKI_TOOLKIT_CONFIG_H
