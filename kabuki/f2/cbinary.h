/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/cbinary.h
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
#include <pch.h>

#ifndef INCLUDED_KABUKI_F2_BINARY
#define INCLUDED_KABUKI_F2_BINARY 1

#if SEAM >= _0_0_0__00
#if SEAM == _0_0_0__00
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif
namespace _ {

/* Lookup table of ASCII Char pairs for 00, 01, ..., 99. */
// API inline const uint16_t* BinaryDecimalsLUT ();
inline const uint16_t* BinaryDecimalsLUT();

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(int8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(uint8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(int16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(uint16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(int32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(uint32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(int64_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(uint64_t value);

/* Utility function for casting a sign to an unsigned in templates. */
inline uint8_t Unsigned(int8_t value);

/* Utility function for casting a sign to an unsigned in templates. */
inline uint16_t Unsigned(int16_t value);

/* Utility function for casting a sign to an unsigned in templates. */
inline uint32_t Unsigned(int32_t value);

/* Utility function for casting a sign to an unsigned in templates. */
inline uint64_t Unsigned(int64_t value);

/* Utility function multiplies the given value by -1. */
inline uint8_t Negative(int8_t value);

/* Utility function inverts the bits and adds one (i.e. multiplies by -1). */
inline uint8_t Negative(uint8_t value);

/* Utility function multiplies the given value by -1. */
inline uint16_t Negative(int16_t value);

/* Utility function inverts the bits and adds one (i.e. multiplies by -1). */
inline uint16_t Negative(uint16_t value);

/* Utility function multiplies the given value by -1. */
inline uint32_t Negative(int32_t value);

/* Utility function inverts the bits and adds one (i.e. multiplies by -1). */
inline uint32_t Negative(uint32_t value);

/* Utility function multiplies the given value by -1. */
inline uint64_t Negative(int64_t value);

/* Utility function inverts the bits and adds one (i.e. multiplies by -1). */
inline uint64_t Negative(uint64_t value);

/* Converts a byte a one-uint8_t hex representation. */
API char HexNibbleToLowerCase(uint8_t b);

/* Converts a byte a one-uint8_t hex representation. */
API char HexNibbleToUpperCase(uint8_t b);

/* Converts a byte a two-byte hex representation. */
API uint16_t HexByteToLowerCase(uint8_t b);

/* Converts a byte a two-byte hex representation. */
API uint16_t HexByteToUpperCase(uint8_t b);

/* Converts a single hex uint8_t a uint8_t.
@return Returns -1 if c is not a hex uint8_t. */
API int HexToByte(char hex_byte);

/* Converts a byte into a two-byte hex representation.
@return Returns -1 if c is not a hex uint8_t.
*/
API int HexToByte(uint16_t hex);

API inline int BinaryLength(uint32_t value);
API inline int BinaryLength(uint64_t value);

#endif  //< #if SEAM >= _0_0_0__00

#if SEAM >= _0_0_0__03
/* Gets the maximum number of digits required to represent a float as in
ASCII. */
API inline int FloatDigitsMax();

/* Gets the maximum number of digits required to represent a double as in
ASCII. */
API inline int DoubleDigitsMax();

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(float value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
API inline bool IsFinite(float value);

/* Checks if the given value is not NaN or +/- Infinity. */
API inline bool IsFinite(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
API inline bool IsInfinite(float value);

/* Checks if the given value is not NaN or +/- Infinity. */
API inline bool IsInfinite(double value);

/* Returns the ceiling of the given value to the next highest up integer. */
API float Ceiling(float value);

/* Returns the ceiling of the given value to the next highest up integer. */
API double Ceiling(double value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(uint8_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(int8_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(uint16_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(int16_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(uint32_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(int32_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(uint64_t value);

/* Gets the Most Significant Asserted Bit (MSbAsserted).
@return A negative number if value is zero and the highest bit. */
API int MSbAsserted(int64_t value);

/* IEEE754 Powers of 10 exponents LUT. */
API inline const int16_t* IEEE754Pow10E();

/* IEEE754 Powers of 10 integral portions LUT. */
API inline const uint64_t* IEEE754Pow10F();

/* Prints the given float to the buffer. */
API char* Print(char* cursor, char* end, float value);

/* Prints the given float to the buffer. */
API char* Print(char* cursor, char* end, double value);

#endif  //< #if SEAM >= _0_0_0__03
}  // namespace _

#endif  //< #ifndef INCLUDED_KABUKI_F2_BINARY
