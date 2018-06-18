/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/library/kt/assembly.h
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

    @desc    This file contains the system configuration for ChineseRoom and
             applications and libraries that use it. To modify this file, copy
             it into your project directory into the source code root. This
             files gets #include <assembly.h> in the config file. That means
             that the compiler will look in your source code root before looking
             in this file. Please see config.h for configuration details.
*/

// Choose BARE_METAL, MBED, MBED_OS, ARDUINO, MINGW32, WINDOWS, ANDROID,
// LINUX, OSX, or IOS
#define CRABS_PLATFORM MBED

// Choose X86, X64, ARM8, ARM16, ARM32, or ARM64
#define CRABS_ACHITECTURE X64

#define WORD_SIZE 64

#define MEMORY_ALIGNMENT_REQUIRED NO

#define CRABS_ENDIANESS LITTLE

/* @brief The Memory Profiles 1-4 are used to signal that the system is
    memory constrained.
    For constrained memory devices, use CRABS_MEMORY_PROFILE 1. For most uses
   use CRABS_MEMORY_PROFILE 2. For applications that require 64-bit memory
   addresses, use CRABS_MEMORY_PROFILE 3.

    @code
    | RUL | sizeof (uint_t) | #bits Unit |
    |:---:|:---------------:|:----------:|
    |  1  |      1 byte     |     5      |
    |  2  |      2 byte     |    10      |
    |  4  |      4 byte     |    16      |
    @endcode
*/
#define CRABS_MEMORY_PROFILE 2

// Max errors before locking up.
#define CRABS_MAX_ERRORS 8

// Max number of parameters.
#define CRABS_MAX_PARAMS 26

// The max char length.
#define CRABS_TEXT_SIZE_DEFAULT 2 * 1024

// Max number of Slots.
#define MAX_NUM_SLOTS 256

// Number of seconds before a timeout over a generic communication link.
#define COM_TIMEOUT_TICKS 100

// Index width in bytes: 8, 16, or 32.
#define CRABS_CHAR_WIDTH 8

// Max number of walls in this Room
#define CRABS_MAX_WALLS 1024

// The size of the default Text<> () buffer in bytes.
#define CRABS_TEXT_LENGTH 2048

// Default init.
#define CRABS_STRAND_INIT_BUFFER_SIZE 256

// String that gets printed at the beginning of each new line.
#define NEW_LINE_HEADER "\n"

// Max Script Op name length in chars.
#define CRABS_OP_MAX_NAME_LENGTH 64

// Max Script Op description length in chars.
#define CRABS_OP_MAX_DECRABSION_LENGTH 1024

// Max Script address length.
#define CRABS_MAX_ADDRESS_LENGTH 26

#define CRABS_MAX_KEY_LENGTH 64

/* Native string width: 8 for UTF-8, 16 for UTF-16, and 32 for UTF-32.
    This macro dictates what Unicode format strings will be unpacked to when
    unpacked from a BIn. */
#define CRABS_NATIVE_STRING_WIDTH 8

// Size or init size of the Rx/Keyboard buffer.
#define CRABS_BUFFER_SIZE_RX 256

// Size or init size of the Tx/Console buffer.
#define CRABS_BUFFER_SIZE_TX 1024

// Sets any of the following macros to YES or NO.

#define USING_DYNAMIC_MEMORY YES
#define USING_CRABS_1_BYTE_TYPES YES
#define USING_CRABS_2_BYTE_TYPES YES
#define USING_CRABS_4_BYTE_TYPES YES
#define USING_CRABS_8_BYTE_TYPES YES
#define USING_CRABS_OBJ YES
#define USING_CRABS_ARRAY NO
#define USING_CRABS_BOOK NO
#define USING_CRABS_LIST NO
#define USING_CRABS_DIC NO
#define USING_PRINTER YES
#define USING_CRABS_EXCEPTIONS YES
