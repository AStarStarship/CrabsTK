/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/assembly.h
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
    
    @desc   This file contains the system configuration for ChineseRoom and  
            applications and libraries that use it. To modify this file, copy 
            it into your project directory into the source code root. This 
            files gets #include <assembly.h> in the config file. That means 
            that the compiler will look in your source code root before looking 
            in this file. Please see config.h for configuration details.
*/

// Choose BARE_METAL, MBED, MBED_OS, ARDUINO, WIN32, WINDOWS, ANDROID, 
// LINUX, OSX, or IOS
#define PLATFORM            WINDOWS

// Choose X86, X64, ARM8, ARM16, ARM32, or ARM64
#define CPU_ACHITECTURE     X86

#define IS_LITTLE_ENDIAN    YES

#define DEBUG               YES

#ifndef DEBUG_CHINESE_ROOM
#define DEBUG_CHINESE_ROOM   YES
#endif //< DEBUG_CHINESE_ROOM

/** @brief The RAM Use Levels 1-3 control how much RAM is used by adjusting the
           size of the uint_t type.
    For constrained memory devices, use RAM_USE_LEVEL 1. For most uses use
    RAM_USE_LEVEL 2. For applications that require 64-bit memory addresses, use
    RAM_USE_LEVEL 3.

    @code
    | RUL | sizeof (uint_t) | #bits Unit |
    |:---:|:---------------:|:----------:|
    |  1  |      1 byte     |     5      |
    |  2  |      2 byte     |    10      |
    |  4  |      4 byte     |    16      |
    @endcode
*/
#define RAM_USE_LEVEL       2       //< The RAM User Level 1-3.
#define USE_MORE_ROM        YES     //< Inlines functions or makes them static.
#define USING_CONSOLE       YES     //< Includes/excludes human-readable text.

#define MAX_ERRORS          8       //< Max errors before locking up.
#define MAX_NUM_PARAMS      26      //< Max number of parameters.
#define MAX_STRING_LENGTH   64 * 1024   //< The max string length.
#define COM_TIMEOUT_TICKS   100     //< The number of seconds before a timeout.
                                    //< over a generic communication link.
#define ASSEMBLY_NAME       "Unnamed"   //< Enter the name of your program or library.
#define CHAR_WIDTH          8       //< char_t width in bytes: 8, 16, or 32.

/*> Sets any of the following macros to YES or NO. */

#define USING_1_BYTE_TYPES  YES
#define USING_2_BYTE_TYPES  YES
#define USING_4_BYTE_TYPES  YES
#define USING_8_BYTE_TYPES  YES
#define USING_VARINT2       YES
#define USING_VARINT4       YES
#define USING_VARINT8       YES
#define USING_AR1           YES
#define USING_AR2           YES
#define USING_AR4           YES
#define USING_AR8           YES
#define USING_BK2           NO
#define USING_BK4           NO
#define USING_BK8           NO      //< Must use to enable file handling.

#define USING_UTF           NO      //< Leave YES unless you're trying to save ROM.
