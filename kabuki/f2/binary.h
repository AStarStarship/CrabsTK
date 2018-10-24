/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/binary.h
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

namespace _ {

/* Lookup table of ASCII Char pairs for 00, 01, ..., 99. */
API inline const uint16_t* PuffDigitsLut() {
  static const uint16_t kDigits00To99[100] = {
      0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830,
      0x3930, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
      0x3831, 0x3931, 0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632,
      0x3732, 0x3832, 0x3932, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533,
      0x3633, 0x3733, 0x3833, 0x3933, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434,
      0x3534, 0x3634, 0x3734, 0x3834, 0x3934, 0x3035, 0x3135, 0x3235, 0x3335,
      0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x3036, 0x3136, 0x3236,
      0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x3037, 0x3137,
      0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x3038,
      0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
      0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839,
      0x3939,
  };
  return kDigits00To99;
}

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

/* Converts a single uint8_t a one-uint8_t hex representation. */
API uint8_t HexNibbleToLowerCase(uint8_t b);

/* Converts a single uint8_t a one-uint8_t hex representation. */
API uint8_t HexNibbleToUpperCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
API uint16_t HexByteToLowerCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
API uint16_t HexByteToUpperCase(uint8_t b);

/* Converts a single hex uint8_t a uint8_t.
@return Returns -1 if c is not a hex uint8_t. */
API int HexToByte(uint8_t hex_byte);

/* Converts a single uint8_t into a two-uint8_t hex representation.
@return Returns -1 if c is not a hex uint8_t.
*/
API int HexToByte(uint16_t hex);

/* Converts a single uint8_t a one-uint8_t hex representation. */
API uint8_t HexNibbleToLowerCase(uint8_t b);

/* Converts a single uint8_t a one-uint8_t hex representation. */
API uint8_t HexNibbleToUpperCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
API uint16_t HexByteToLowerCase(uint8_t b);

/* Converts a single uint8_t a two-uint8_t hex representation. */
API uint16_t HexByteToUpperCase(uint8_t b);

/* Converts a single hex uint8_t a uint8_t.
@return Returns -1 if c is not a hex uint8_t. */
API int HexToByte(uint8_t c);

/* Converts a single uint8_t into a two-uint8_t hex representation.
@return Returns -1 if c is not a hex uint8_t. */
API int HexToByte(uint16_t h);

#if SEAM >= SEAM_0_0_0__01
/* Gets the maximum number of digits required to represent a float as in
ASCII. */
API inline int FloatDigitsMax();

/* Gets the maximum number of digits required to represent a double as in
ASCII. */
API inline int DoubleDigitsMax();

/* Returns the power of ten for the smallest float decimal. */
API float FloatDecimalPower(int decimal_count);

/* Returns the power of ten for the smallest double decimal. */
API double DoubleDecimalPower(int decimal_count);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(float value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
API inline bool IsNaN(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
API /*inline*/ bool IsFinite(float value);

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

#endif  //< #if SEAM >= SEAM_0_0_0__01

#if SEAM >= SEAM_0_0_0__02

/* Prints the given float to the buffer. */
API char* PrintFloat(char* cursor, char* end, float value);

/* Prints the given float to the buffer. */
API char* PrintFloat(char* cursor, char* end, double value);

#endif  //< #if SEAM >= SEAM_0_0_0__02
}  // namespace _

#endif  //< #ifndef INCLUDED_KABUKI_F2_BINARY
