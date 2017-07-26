/** The Chinese Room
    @version 0.x
    @file    /.../assembly_settings.h
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
    
    @brief  This file contains the system configuration for ChineseRoom and  
            modules that use it.
    @desc   To modify this file, copy it into your project directory into the 
            source code root. This files gets #include <AssemblySettings.inl> in
            the config file. That means that the compiler will look in your 
            source code root before looking in this file. Please see config.h for 
            configuration details.
*/

#define PLATFORM                    MINGW

#define CPU_ACHITECTURE             X86

#define IS_LITTLE_ENDIAN            YES

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG                       YES

#ifndef DEBUG_CHINESEROOM
#define DEBUG_CHINESEROOM           YES
#endif //< DEBUG_CHINESEROOM

#define MEMORY_USE_LEVEL            2
/*< The memory usage level (MUL) 1-3.
    For constrained memory devices, use MUL 1. For most uses use MUL 2. For 
    applications that require 64-bit memory addresses, use MUL 3.
    @code
    | MUL | sizeof (uint_t) | #bits Unit |
    |:---:|:---------------:|:----------:|
    |  1  |      1 byte     |     5      |
    |  2  |      2 byte     |    10      |
    |  4  |      4 byte     |    16      |
    @endcode
*/

namespace _ {
enum
{
    kMaxErrors                  = 8,    //< Max num errors before blowing up.
    kDefaultTerminalSize        = 256,  //< Default Terminal buffer size.
    kDefaultHeaderBufferSize    = 32,   //< Default header buffer size.
    kDefaultDataBufferSize      = 32,   //< Default data buffer size.
    kHashSeed                   = 3,    //< Prime number hash seed.
    kInitStackHeight            = 8,    //< Initial stack height.
    kMaxAddressLength           = 10,   //< Max length of an GetAddress.
    kMaxNumParams               = 26,   //< Max number of parameters.
    kLogSize                    = 256,  //< Size of the log for this module.
    kMinRoomSize                = 256,  //< Min size of a room.
    kMaxRoomSize                = 0x80000000,   //< Max room size: 2GB.
    kMinTerminalSize            = 256,  //< Min size of a Terminal minus one.
};
}

typedef uint8_t error_t;

#define ASSEMBLY_NAME           "Unnamed"

//< The size of a transmission block unit in bits (1KB/2^10 default).

/*> Sets any of the following macros to 0 or non-zero to disable/enable 
    respectively. */

#define USING_1_BYTE_TYPES         YES
#define USING_2_BYTE_TYPES         YES
#define USING_4_BYTE_TYPES         YES
#define USING_8_BYTE_TYPES         YES
#define USING_VARINT2              YES
#define USING_VARINT4              YES
#define USING_VARINT8              YES
#define USING_AR1                  YES
#define USING_AR2                  YES
#define USING_AR4                  YES
#define USING_AR8                  YES
#define USING_BK2                  NO
#define USING_BK4                  NO
#define USING_BK8                  NO

#define USING_UTF                  NO
