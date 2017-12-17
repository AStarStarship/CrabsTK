/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/assembly.h
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

    @desc    This file contains the system configuration for ChineseRoom and
             applications and libraries that use it. To modify this file, copy
             it into your project directory into the source code root. This
             files gets #include <assembly.h> in the config file. That means
             that the compiler will look in your source code root before looking
             in this file. Please see config.h for configuration details.
*/

// Global debug macro
#define DEBUG        YES
#define SCRIPT_DEBUG YES

// Choose BARE_METAL, MBED, MBED_OS, ARDUINO, MINGW32, WINDOWS, ANDROID,
// LINUX, OSX, or IOS
#define SCRIPT_PLATFORM MBED

// Choose X86, X64, ARM8, ARM16, ARM32, or ARM64
#define SCRIPT_ACHITECTURE X86

#define SCRIPT_WORD_SIZE 32

#define SCRIPT_ENDIANESS LITTLE

/** @brief The Memory Profiles 1-4 are used to signal that the system is
    memory constrained.
    For constrained memory devices, use MEMORY_PROFILE 1. For most uses use
    MEMORY_PROFILE 2. For applications that require 64-bit memory addresses, use
    MEMORY_PROFILE 3.

    @code
    | RUL | sizeof (uint_t) | #bits Unit |
    |:---:|:---------------:|:----------:|
    |  1  |      1 byte     |     5      |
    |  2  |      2 byte     |    10      |
    |  4  |      4 byte     |    16      |
    @endcode
*/
#define SCRIPT_MEMORY_PROFILE 2

// Max errors before locking up.
#define SCRIPT_MAX_ERRORS 8

// Max number of parameters.
#define SCRIPT_MAX_PARAMS 26

// The max char length.
#define SCRIPT_TEXT_SIZE_MAX 2 * 1024

// Max number of Slots.
#define MAX_NUM_SLOTS 256

// Number of seconds before a timeout over a generic communication link.
#define COM_TIMEOUT_TICKS 100

// Index width in bytes: 8, 16, or 32.
#define SCRIPT_CHAR_WIDTH 8

// Max number of walls in this _::Room
#define SCRIPT_MAX_WALLS 1024

// 
#define SCRIPT_MAX_TEXT_LENGTH 

// String that gets printed at the beginning of each new line.
#define NEW_LINE_HEADER "\n| "

// Max Script Operation name length in chars.
#define SCRIPT_OPERATION_MAX_NAME_LENGTH 64

// Max Script Operation description length in chars.
#define SCRIPT_OPERATION_MAX_DESCRIPTION_LENGTH 1024

// Max Script address length.
#define SCRIPT_MAX_ADDRESS_LENGTH 26

// Sets any of the following macros to YES or NO.

#define SCRIPT_USING_1_BYTE_TYPES YES
#define SCRIPT_USING_2_BYTE_TYPES YES
#define SCRIPT_USING_4_BYTE_TYPES YES
#define SCRIPT_USING_8_BYTE_TYPES YES
#define SCRIPT_USING_VARINT2      YES
#define SCRIPT_USING_VARINT4      YES
#define SCRIPT_USING_VARINT8      YES
#define SCRIPT_USING_ARRAY        NO
#define SCRIPT_USING_BAG          NO
#define SCRIPT_USING_BOOK         NO
#define SCRIPT_USING_MAP          NO
#define SCRIPT_USING_LIST         NO
#define SCRIPT_USING_STRAND       YES
#define SCRIPT_USING_TEXT         YES
#define SCRIPT_USING_PRINT        YES
#define SCRIPT_USING_TOKEN        YES
#define SCRIPT_USING_KEYBOARD     YES
#define SCRIPT_USING_MIRROR       NO