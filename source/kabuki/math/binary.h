/** Kabuki Starship
    @version 0.9
    @file    /.../Source/_Math/Binary.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#pragma once

#include <KabukiTheater-Config.h>

namespace _Math {

inline void toBinary (char* Result, int8_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, byte Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, int16_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, uint16_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, int32_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, uint32_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, int64_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

inline void toBinary (char* Result, uint64_t Value);
/*< Converts the Value to ASCII binary string into the given buffer.
    @pre    The Result must be at least 9 bytes in length! 
*/

int8_t binaryToInt8 (string Value);
/*< Converts the given string into an int8_t. */

uint8_t binaryToUInt8 (string Value);
/*< Converts the given string into an uint8_t. */

int16_t binaryToInt16 (string Value);
/*< Converts the given string into an int16_t. */

uint16_t binaryToUInt16 (string Value);
/*< Converts the given string into an uint16_t. */

int32_t binaryToInt32 (string Value);
/*< Converts the given string into an int32_t. */

uint32_t binaryToUInt32 (string Value);
/*< Converts the given string into an uint32_t. */

int64_t binaryToInt64 (string Value);
/*< Converts the given string into an int64_t. */

uint64_t binaryToUInt64 (string Value);
/*< Converts the given string into an uint64_t. */

}   //<  _
