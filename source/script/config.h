/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/config.h
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

#ifndef HEADER_FOR_SCRIPT_CONFIG
#define HEADER_FOR_SCRIPT_CONFIG

// @todo Check all values of assembly_settings.inl, store them as an enum, 
// then #undef them.

#define BARE_METAL       1  //< Bare metal (i.e. No OS) OS type macro.
#define BARE_METAL_MBED  2  //< mbed bare-metal OS type macro.
#define MBED_OS          3  //< Arduino bare-metal OS type macro.
#define ARDUINO          4  //< mbed OS type macro.
#define MINGW32          5  //< Win32 OS type macro.
#define WINDOWS          6  //< Slots Universal OS type macro.
#define ANDROID          7  //< Android OS type macro.
#define LINUX            8  //< Linux OS type macro.
#define OSX              9  //< OSX OS type macro.
#define IOS              10 //< iOS OS type macro.


#define X86              11 //< x86 processor family macro.
#define X64              12 //< x64 processor family macro.
#define ARM8             13 //< ARM8 processor family macro.
#define ARM16            14 //< ARM16 processor family macro.
#define ARM32            15 //< ARM32 processor family macro.
#define ARM64            16 //< ARM64 processor family macro.

// Stupid dumb-face Big-Endian nonsense

#define LITTLE   1
#define BIG      2   //< Don't ask me what they where thinking.
                     //< "but I can read it easier"...
                     //< READ IT EASIER?!?!?!
                     //< CHANGE YOUR IDE/EDA TOOLS FOOL!!!
                     //< DIPRA DIRPA!!!
                     //< CLICK! BIG ENDIAN! BOOM!!!
                     //< CLICK! LITTLE! ENDIAN! BOOM!!!
                     //< YOUR AN IDIOT!!!
                     //< MAYBE YOU SHOULD WRITE THIS BIG ENDIAN CRAP CODE IF 
                     //< YOU LIKE IT SO MUCH!!! Just kidding. :-)

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

#define DEBUG_ALL                   54321
#define SCRIPT_BIN                  54322
#define SCRIPT_BOUT                 54323
#define SCRIPT_DISPLAY              54324
#define SCRIPT_DOOR                 54325
#define SCRIPT_EXPRESSION           54326
#define SCRIPT_LIBRARY              54327
#define SCRIPT_ROOM                 54328
#define SCRIPT_ARRAY                54329
#define SCRIPT_BAG                  54320
#define SCRIPT_BOOK                 54331
#define SCRIPT_STRAND               54332
#define SCRIPT_TEXT                 54333
#define SCRIPT_STACK                54334
#define SCRIPT_WALL                 54335

#include <assembly.h>       //< Inline config stuff for your project.

#if SCRIPT_DEBUG == DEBUG_ALL
#define DEBUG_SCRIPT_BIN 1
#define DEBUG_SCRIPT_BOUT 1
#define DEBUG_SCRIPT_DISPLAY 1
#define DEBUG_SCRIPT_DOOR 1
#define DEBUG_SCRIPT_EXPRESSION 1
#define DEBUG_SCRIPT_LIBRARY 1
#define DEBUG_SCRIPT_ROOM 1
#define DEBUG_SCRIPT_ARRAY 1
#define DEBUG_SCRIPT_BAG 1
#define DEBUG_SCRIPT_BOOK 1
#define DEBUG_SCRIPT_STRAND 1
#define DEBUG_SCRIPT_TEXT 1
#define DEBUG_SCRIPT_STACK 1
#define DEBUG_SCRIPT_WALL 1
#elif SCRIPT_DEBUG == SCRIPT_BIN
#define DEBUG_SCRIPT_BIN 1
#elif SCRIPT_DEBUG == SCRIPT_BOUT
#define DEBUG_SCRIPT_BOUT 1
#elif SCRIPT_DEBUG == SCRIPT_DISPLAY
#define DEBUG_SCRIPT_DISPLAY 1
#elif SCRIPT_DEBUG == SCRIPT_DOOR
#define DEBUG_SCRIPT_DOOR 1
#elif SCRIPT_DEBUG == SCRIPT_EXPRESSION
#define DEBUG_SCRIPT_EXPRESSION 1
#elif SCRIPT_DEBUG == SCRIPT_LIBRARY
#define DEBUG_SCRIPT_LIBRARY 1
#elif SCRIPT_DEBUG == SCRIPT_ROOM
#define DEBUG_SCRIPT_ROOM 1
#elif SCRIPT_DEBUG == SCRIPT_ARRAY
#define DEBUG_SCRIPT_ARRAY 1
#elif SCRIPT_DEBUG == SCRIPT_BAG
#define DEBUG_SCRIPT_BAG 1
#elif SCRIPT_DEBUG == SCRIPT_BOOK
#define DEBUG_SCRIPT_BOOK 1
#elif SCRIPT_DEBUG == SCRIPT_STRAND
#define DEBUG_SCRIPT_STRAND 1
#elif SCRIPT_DEBUG == SCRIPT_STACK
#define DEBUG_SCRIPT_STACK 1
#elif SCRIPT_DEBUG == SCRIPT_WALL
#define DEBUG_SCRIPT_WALL 1
#endif
#undef SCRIPT_DEBUG

typedef const char* string_ptr;

#if SCRIPT_MAX_ERRORS < 0
#error MAX_ERRORS must be greater than 0
#elif SCRIPT_MAX_ERRORS <= 255
typedef uint8_t ticket_t;
#elif MAX_ERRORS <= 65535
typedef uint16_t ticket_t;
#else
typedef uint32_t ticket_t;
#endif

#if SCRIPT_MAX_PARAMS < 0
#error MAX_ERRORS must be greater than 0
#endif

#if SCRIPT_TEXT_SIZE_DEFAULT < 0
#error MAX_ERRORS must be greater than 0
#endif

#if COM_TIMEOUT_TICKS < 0
#error MAX_ERRORS must be greater than 0
#endif

// Stupid dumb-face Big-Endian nonsense
#define LITTLE   1
#define BIG      2   //< Don't ask me what they where thinking.

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
    kMajorVersion    = 0,                 //< Script version major.
    kMinorVersion    = 9,                 //< Script version minor.
    //kMinFloorSize  = 256,               //< Min size of a room.
    //kMaxFloorSize  = 0x7FFFFFFC,        //< Max room size: 2GB - 7 bits.
    kMaxFloorsCount  = SCRIPT_MAX_WALLS,  //< Size of the Room Floor (buffer).
    kMinSlotSize     = 128,               //< Min size of a Slot - 1.
    kMaxErrors       = SCRIPT_MAX_ERRORS, //< Max errors before blowing up.
    kParamsMax       = SCRIPT_MAX_PARAMS, //< Max number of parameters.
    kWordAddressMask = sizeof (void*) - 1,//< For masking the word address.
    kTimeoutMicroseconds = COM_TIMEOUT_TICKS, //< Timeout time in microseconds.
    kAddressLengthMax= SCRIPT_MAX_ADDRESS_LENGTH, //< Max address (ADR) length.
    kMinStackSize    = 1,                 //< Min Expr stack size.
    kOpNameLengthMax = SCRIPT_OP_MAX_NAME_LENGTH,
    // Max length of a Op description string.
    kOpDescriptionLengthMax = SCRIPT_OP_MAX_DESCRIPTION_LENGTH,
    // Max length of a Text.
    kTextSize        = SCRIPT_TEXT_LENGTH,
    // Number of bits to shift to multiply or divide to get num_words.
    kWordSizeShift   = (sizeof (void*) == 8) ? 3 : //< Shift left 3 to * by 8.
                       (sizeof (void*) == 4) ? 2 : //< Shift right 2 to / by 4.
                       1,
    kDoubleSizeMax   = 3 + DBL_MANT_DIG - DBL_MIN_EXP
};
}   //< namespace _

#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS
#undef SCRIPT_LOG_SIZE
#undef OPERATION_MAX_NAME_LENGTH
#undef OPERATION_MAX_DESCRIPTION_LENGTH
#undef SCRIPT_TEXT_LENGTH

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
    #include <iostream>
    #define NaN_SI4 0xFFFFFFF
    void BlowUp () { std::cout << "The sky is falling!"}
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
#if SCRIPT_MEMORY_PROFILE == 1
typedef int8_t   int_t;     //< Buffer signed index type.
typedef uint8_t  uint_t;    //< Buffer unsigned index type.
typedef int16_t  dint_t;    //< Buffer double-wide signed index type.
typedef uint16_t duint_t;   //< Buffer double-wide unsigned index type.
typedef uint8_t  index_t;   //< Largest bit-depth TIndex this system supports.
typedef uint16_t header_t;  //< Largest bit-depth THeader this system supports.
typedef uint16_t data_t;    //< Largest bit-depth TData this system supports.

#elif (SCRIPT_MEMORY_PROFILE == 2) || (SCRIPT_MEMORY_PROFILE == 3)
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

#if SCRIPT_MEMORY_PROFILE >= 3 || SCRIPT_DEBUG
#define USE_MORE_ROM 3
#endif  //< MEMORY_PROFILE >= 3

/** Macro declares a class to be non-copyable. */
#define NONCOPYABLE (ClassName)\
  ClassName (const ClassName&) = delete;\
  void operator= (const ClassName&) = delete;
  //ClassName () = default //< Saw this once on the net but do we need it?

#if SCRIPT_DEBUG
#define PRINT (MESSAGE)\
    Display (MESSAGE);

#define PRINT_LINE(MESSAGE)\
    std::cout << NEW_LINE_HEADER << MESSAGE;

#define PAUSE (MESSAGE)\
    std::cout << NEW_LINE_HEADER << MESSAGE; system ("PAUSE");

#define PRINTF(...) { printf(__VA_ARGS__); }

#else
#define PRINT(MESSAGE)

#define PRINT_LINE(MESSAGE)

#define COUT_PAUSE(MESSAGE)


#define PRINTF(...)

#endif  //< SCRIPT_DEBUG

#endif  //< HEADER_FOR_SCRIPT_CONFIG
