/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/binary_64.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#ifndef INCLUDED_CRABS_BINARY
#define INCLUDED_CRABS_BINARY

#ifndef DLL
#define DLL
#endif

namespace _ {

/* Gets the maximum number of digits required to represent a float as in
ASCII. */
DLL inline int FloatDigitsMax();

/* Gets the maximum number of digits required to represent a double as in
ASCII. */
DLL inline int DoubleDigitsMax();

/* Returns the power of ten for the smallest float decimal. */
DLL float FloatDecimalPower(int decimal_count);

/* Returns the power of ten for the smallest double decimal. */
DLL double DoubleDecimalPower(int decimal_count);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(int8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(uint8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(int16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(uint16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(int32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(uint32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(int64_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(uint64_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(float value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
DLL inline bool IsNaN(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
DLL /*inline*/ bool IsFinite(float value);

/* Checks if the given value is not NaN or +/- Infinity. */
DLL inline bool IsFinite(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
DLL inline bool IsInfinite(float value);

/* Checks if the given value is not NaN or +/- Infinity. */
DLL inline bool IsInfinite(double value);

/* Returns the ceiling of of the given value to the next highest up integer. */
DLL float Ceiling(float value);

/* Returns the ceiling of of the given value to the next highest up integer. */
DLL double Ceiling(double value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int32_t MSbAsserted(uint8_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int32_t MSbAsserted(int8_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int32_t MSbAsserted(uint16_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int32_t MSbAsserted(int16_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int32_t MSbAsserted(uint32_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int32_t MSbAsserted(int32_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int64_t MSbAsserted(uint64_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
DLL int64_t MSbAsserted(int64_t value);

/* Converts a single uint8_t a one-uint8_t hex representation. */
DLL uint8_t HexNibbleToLowerCase(uint8_t b);

/* Converts a single uint8_t a one-uint8_t hex representation. */
DLL uint8_t HexNibbleToUpperCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
DLL uint16_t HexByteToLowerCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
DLL uint16_t HexByteToUpperCase(uint8_t b);

/* Converts a single hex uint8_t a uint8_t.
@return Returns -1 if c is not a hex uint8_t. */
DLL int HexToByte(uint8_t hex_byte);

/* Converts a single uint8_t into a two-uint8_t hex representation.
@return Returns -1 if c is not a hex uint8_t.
*/
DLL int HexToByte(uint16_t hex);

/* Converts a single uint8_t a one-uint8_t hex representation. */
DLL uint8_t HexNibbleToLowerCase(uint8_t b);

/* Converts a single uint8_t a one-uint8_t hex representation. */
DLL uint8_t HexNibbleToUpperCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
DLL uint16_t HexByteToLowerCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
DLL uint16_t HexByteToUpperCase(uint8_t b);

/* Converts a single hex uint8_t a uint8_t.
@return Returns -1 if c is not a hex uint8_t. */
DLL int HexToByte(uint8_t c);

/* Converts a single uint8_t into a two-uint8_t hex representation.
@return Returns -1 if c is not a hex uint8_t. */
DLL int HexToByte(uint16_t h);

}  // namespace _

#endif  //< #if INCLUDED_CRABS_BINARY
