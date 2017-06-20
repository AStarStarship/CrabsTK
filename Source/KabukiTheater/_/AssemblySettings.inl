/** The Chinese Room
    @version 0.x
    @file    /.../Library/ChineseRoom-Config.h
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
    
    @brief  This file contains the system configuration for ChineseRoom and apps 
            that use it.
    @desc   To modify this file, copy it into your project directory into the 
            source code root. This files gets #include <AssemblySettings.inl> in
            the config file. That means that the compiler will look in your 
            source code root before looking in this file.
*/

#define PLATFORM                    MINGW

#define CPU_ACHITECTURE             X86

#define ENDIANESS                   LITTLE_ENDIAN

#define DEBUG                       YES

#define WORD_SIZE                   64
//< Set to 16, 32, or 64 based on your CPU memory address word size.

#define MUL                         1
/*< The memory usage level 0-3
	@code
	| MUL | sizeof(uint_t) | #bits Unit | Dynamic Memory |
	|:---:|:---------------:|:----------:|:--------------:|
	|  0  |      1 byte     |     6      |       No       |
	|  1  |      1 byte     |     6      |       No       |
	|  2  |      2 byte     |    10      |       No       |
	|  3  |      2 byte     |    10      |       No       |
	|  4  |      4 byte     |    16      |       No       |
	|  5  |      2 byte     |    16      |       Yes      |
	|  6  |      4 byte     |    16      |       Yes      |
	|  7  |      8 byte     |    16      |       Yes      |
	@endcode
*/

#define BUFFER_UINT_SIZE            8
//< Set to 8, 16, 32, or 64 based to set the size of the offset variable size.

#define MAX_NUM_ERRORS              8
//< The default number of errors in an error list.

enum
{
    DefaultTerminalSize             = 255,  //< Default Terminal buffer size.
    DefaultHeaderBufferSize         = 32,   //< Default header buffer size.
    DefaultDataBufferSize           = 32,   //< Default data buffer size.
    HashSeed                        = 3,    //< The prime number hash seed.
    InitStackHeight                 = 8,    //< The initial stack height.
    MaxAddressLength                = 10,   //< The max length of an address.
    MaxNumParams                    = 26,   //< The max number of parameters.
};

#define ASSEMBLY_NAME "Demo"

#define MIN_ROOM_SIZE               128
#define MAX_ROOM_SIZE               4 * 1024 * 1024 * 1024 - 1

#define UNIT_BIT_DEPTH              10,
//< The size of a transmission block unit in bits(1KB/2^10 default).

/*> Sets any of the following macros to 0 or non-zero to disable/enable 
    resepctivly. */

#define USING_STX                  YES
#define USING_1_BYTE_TYPES         NO
#define USING_2_BYTE_TYPES         NO
#define USING_4_BYTE_TYPES         NO
#define USING_8_BYTE_TYPES         NO
#define USING_VARINT2              NO
#define USING_VARINT4              NO
#define USING_VARINT4              NO
#define USING_AR1                  NO
#define USING_AR2                  NO
#define USING_AR4                  NO
#define USING_AR8                  NO
#define USING_BK2                  NO
#define USING_BK4                  NO
#define USING_BK8                  NO
