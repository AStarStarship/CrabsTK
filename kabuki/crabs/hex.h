/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/hex.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>
#if SEAM >= SEAM_0_0_0
#ifndef INCLUDED_CRABS_HEX
#define INCLUDED_CRABS_HEX
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

/* Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToLowerCase(byte b);

/* Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToUpperCase(byte b);

/* Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToLowerCase(byte b);

/* Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToUpperCase(byte b);

/* Converts a single hex byte a byte.
@return Returns -1 if c is not a hex byte.
*/
KABUKI int HexToByte(byte hex_byte);

/* Converts a single byte into a two-byte hex representation.
@return Returns -1 if c is not a hex byte.
*/
KABUKI int HexToByte(uint16_t hex);

/* Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToLowerCase(byte b);

/* Converts a single byte a one-byte hex representation. */
KABUKI byte HexNibbleToUpperCase(byte b);

/* Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToLowerCase(byte b);

/* Converts a single byte a two-byte hex representation. */
KABUKI uint16_t HexByteToUpperCase(byte b);

/* Converts a single hex byte a byte.
    @return Returns -1 if c is not a hex byte. */
KABUKI int HexToByte(byte c);

/* Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex byte. */
KABUKI int HexToByte(uint16_t h);

}  // namespace _

#endif  //< INCLUDED_CRABS_HEX
#endif  //< #if SEAM >= SEAM_0_0_0
