/** The Chinese Room
    @version 0.x
    @file    /.../Config.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 Cale McCollough <calemccollough.github.io>
        
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

#ifndef CHINESEROOM_CONFIG_H
#define CHINESEROOM_CONFIG_H

//! OS Types

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

//! Processor categories

#define X86                         1
#define X64                         2
#define ARM8                        3
#define ARM16                       4
#define ARM32                       5
#define ARM32                       6

//! Stupid dumb face Big-Endian nonsense

#define LITTLE_ENDIAN               1
#define BIG_ENDIAN                  2

#define YES                         1
#define NO                          0

#include "AssemblySettings.inl"

#define DEBUG_CHINESEROOM           1       //< Set to non-zero to debug your app.

#if USING_STX || USING_1_BYTE_TYPES || USING_2_BYTE_TYPES || \
    USING_4_BYTE_TYPES || USING_8_BYTE_TYPES || USING_VARINT2 || \
    USING_VARINT4 || USING_VARINT4 || USING_AR1 || USING_AR2 || USING_AR2 || \
    USING_AR8 || USING_BK4 || USING_AR8
#define USING_PARAMS                YES
#endif


static const char EmptyString[] = "\0";     //< The empty string.
static const char NewLineString[] = "\n\0"; //< The empty string.v

#include <cstdint>
#include <stdint.h>

typedef uint8_t byte;
typedef unsigned int uint;

/** The following numbers represent numbers that are not numbers.
    These numbers are useful for determining if a calculation went wrong based
    on it's address. They are not currently in use but look like this is use:
    @code
    int32_t& example(float input);
    if(example() == NaN_SI4) blowUp();
    @endcode
*/
enum { NaN  = ~(0) };                       //< Not-a-number as 32-bit signed or unsigned integer.

static const int8_t    NaN_SI1 = ~(0);      //< 8-bit  signed Not a number.
static const uint8_t   NaN_UI1 = ~(0);      //< 8-bit  unsigned Not a number.
static const int16_t   NaN_SI2 = ~(0);      //< 16-bit signed Not a number.
static const uint16_t  NaN_UI2 = ~(0);      //< 16-bit signed Not a number.
static const int32_t   NaN_SI4 = ~(0);      //< 32-bit signed Not a number.
static const uint32_t  NaN_UI4 = ~(0);      //< 32-bit signed Not a number.
static const int64_t   NaN_SI8 = ~(0);      //< 64-bit signed Not a number.
static const uint64_t  NaN_UI8 = ~(0);      //< 64-bit signed Not a number.
static const uintptr_t NaNWord = ~(0);      //< Word-sized unsigned Not a number.
static const intptr_t  NaNWInt = ~(0);      //< Word-sized signed Not a number.

typedef uint32_t hash32_t;                  //< Using unsigned 32-bit hash type.
typedef uint64_t hash64_t;                  //< Using unsigned 64-bit hash type.
typedef int64_t timestamp_t;                //< A 64-bit microsecond timestamp in microseconds from Unix Epoch.


/** The level will more code creating a larger binary. Use one 
    underscore to use more memory, and two underscores to use even more. */
#define _DynamicMUL_                4       //< Starting Dynamic Memory Level, which is MUL 4
#if MUL == 0
#define 
typedef int8_t int_t;                       //< Buffer signed index type.
typedef uint8_t uint_t;                     //< Buffer unsigned index type.
typedef byte BookIndexType;                 //< The largest bit-depth book TIndex this system supports.
typedef uint16_t BookHeaderType;            //< The largest bit-depth book THeader this system supports.
typedef uint16_t BookDataOffsetType;        //< The largest bit-depth book TData this system supports.
typedef uint16_t BookHashType;              //< The largest bit-depth book THash this system supports.

#elif MUL == 1
typedef int8_t int_t;                       //< Buffer signed index type.
typedef uint8_t uint_t;                     //< Buffer unsigned index type.
typedef byte BookIndexType;                 //< The largest bit-depth book TIndex this system supports.
typedef uint16_t BookHeaderType;            //< The largest bit-depth book THeader this system supports.
typedef uint16_t BookDataOffsetType;        //< The largest bit-depth book TData this system supports.
typedef uint16_t BookHashType;              //< The largest bit-depth book THash this system supports.

#elif MUL == 2
typedef byte int_t;                         //< Buffer signed index type.
typedef int16_t uint_t;                     //< Buffer unsigned signed index type.
typedef byte BookIndexType;
typedef uint16_t BookHeaderType;
typedef uint16_t BookDataOffsetType;
typedef uint16_t BookHashType;

#elif MUL == 3
typedef int32_t int_t;                      //< Buffer signed index type.
typedef uint32_t uint_t;                    //< Buffer unsigned signed index type.
typedef uint16_t BookIndexType;             //< Book4 TIndex.
typedef uint16_t BookHeaderType;
typedef uint32_t BookDataOffsetType;
typedef hash32_t BookHashType;

#elif MUL == 4                              //< Dynamic memory with 32-bit offsets.
typedef int32_t int_t;                      //< Buffer signed index type.
typedef uint32_t uint_t;                    //< Buffer unsigned signed index type.
typedef uint16_t BookIndexType;             //< Book4 TIndex.
typedef uint16_t BookHeaderType;
typedef uint32_t BookDataOffsetType;
typedef hash32_t BookHashType;

#elif MUL == 5                              //< Dynamic memory with 64-bit offsets.
typedef int64_t int_t;                      //< Buffer signed index type.
typedef uint64_t uint_t;                    //< Buffer unsigned index type.
typedef uint32_t BookIndexType;             //< Book8 TIndex.
typedef uint16_t BookHeaderType;
typedef uint32_t BookHeaderType;
typedef hash64_t BookHashType;

#else
#error  Invalid MUL
#endif

typedef uint_t ticket_t;                    //< The size of the error ticket.

/** Macro declares a class to be non-copyable. */
#define NONCOPYABLE(ClassName)\
  ClassName(const ClassName&) = delete;\
  void operator=(const ClassName&) = delete;


#define _Proc_(VarName, MemberName, InputParams, OutputParams, Description)\
  static const Member VarName = { MemberName, InputParams, OutputParams, Description };

#define DEVICE(VarName, MemberName, NumMembers, Description)\
  static const Member VarName = { MemberName, static_cast<const uint_t*>(NumMembers), 0, Description };

#if PLATFORM == MINGW
#define _VTableType_                _StandardVTable_
typedef int vtable_t;                       //< Type used to store the vtable.
#else
#define _VTableType_                _VTablePointerBeforeThis_
#endif

//! Precompiled headers

#include <time.h>
#include <iterator>
#include <locale>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <float.h>
#include <stdio.h>
#include <algorithm>
#include <locale>
#include <string>
#include <cstdlib>

#endif  //< CHINESEROOM_CONFIG_H
