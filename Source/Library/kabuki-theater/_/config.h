/** The Chinese Room
    @version 0.x
    @file    /.../config.h
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

#ifndef CHINESEROOM_CONFIG_H
#define CHINESEROOM_CONFIG_H

#define CHINESEROOM_MAJOR_VERSION 0
#define CHINESEROOM_MINOR_VERSION 9

// @todo Check all values of assembly_settings.inl, store them as an enum, 
// then undef them.

// OS Types

#define BARE_METAL                  0
#define MBED                        1
#define MBED_OS                     2
#define ARDUINO                     3
#define MINGW                       4
#define WINDOWS                     5
#define ANDROID                     6
#define LINUX                       7
#define OSX                         8
#define IOS                         9

// Processor categories

#define X86                         1
#define X64                         2
#define ARM8                        3
#define ARM16                       4
#define ARM32                       5
#define ARM64                       6

// Stupid dumb face Big-Endian nonsense

#define LITTLE_ENDIAN               1
#define BIG_ENDIAN                  2

#define YES                         1
#define NO                          0

#if MAX_ERRORS < 0
#error MAX_ERRORS must be greater than 0
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

#include <assembly.inl>             //< Config stuff for your project.
#include <stdafx.h>                 //< Don't knock it till you've tried it.

namespace _ {
enum {
    kMinRoomSize = 256,                     //< Min size of a room.
    kMaxRoomSize = 0x7FFFFFFC,              //< Max room size: 2GB - 7 bits.
    kMinTerminalSize = 256,                 //< Min size of a Terminal - 1.
    kMaxErrors = MAX_ERRORS,                //< Max errors before blowing up.
    kMaxNumParams = MAX_NUM_PARAMS,         //< Max number of parameters.
    kMaxStringLength = MAX_STRING_LENGTH,   //< The max string length.
    kTimeoutMicroseconds = COM_TIMEOUT_TICKS
    //< The number of seconds before a timeout over a generic communication link.
};
}
#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS

#if USE_MORE_ROM
#define INLINE inline
#else
#define INLINE static
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
                sizeof (int) == 2 ? static_cast<int>(0xFFFF) : 0,
};

static const uintptr_t WordMask = sizeof (void*) - 1;

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
typedef int8_t int_t;                       //< Buffer signed index type.
typedef uint8_t uint_t;                     //< Buffer unsigned index type.
typedef int16_t dint_t;                     //< Buffer double-wide signed index type.
typedef uint16_t duint_t;                   //< Buffer double-wide unsigned index type.
typedef byte TBookIndex;                    //< The largest bit-depth book TIndex this system supports.
typedef uint16_t TBookHeader;               //< The largest bit-depth book THeader this system supports.
typedef uint16_t TBookDataOffset;           //< The largest bit-depth book TData this system supports.
typedef uint16_t TBookHash;                 //< The largest bit-depth book THash this system supports.

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
typedef uint16_t TBookIndex;                //< Book4 TIndex.
typedef uint16_t TBookHeader;               //< Book4 TBookHeader
typedef uint32_t TBookDataOffset;           //< Book4 TBookDataOffset
typedef hash32_t TBookHash;                 //< Book4 TBookHash

namespace _ {
enum {
    kUnitSize = 1 << 10                     //< The size of unit.
};
}

#elif MEMORY_USE_LEVEL == 3
typedef byte int_t;                         //< Buffer signed index type.
typedef int16_t uint_t;                     //< Buffer unsigned signed index type.
typedef int16_t dint_t;                     //< Buffer double-wide signed index type.
typedef uint64_t duint_t;                   //< Buffer double-wide unsigned index type.
typedef uint16_t TBookIndex;                //< Book8 TBookIndex
typedef uint32_t TBookHeader;               //< Book8 TBookHeader
typedef uint64_t TBookDataOffset;           //< Book8 TBookDataOffset
typedef hash64_t TBookHash;                 //< Book8 TBookHash

namespace _ {
enum {
    kUnitSize = 1 << 10                     //< The size of unit.
};
}
#else
#error  Invalid MEMORY_USE_LEVEL
#endif

typedef uint_t ticket_t;                    //< The size of the error ticket.

/** Macro declares a class to be non-copyable. */
#define NONCOPYABLE (ClassName)\
  ClassName (const ClassName&) = delete;\
  void operator= (const ClassName&) = delete;

#endif  //< CHINESEROOM_CONFIG_H
