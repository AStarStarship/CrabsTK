/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/script_itoa.h
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
#ifndef HEADER_FOR_CRABS_NUMBER
#define HEADER_FOR_CRABS_NUMBER
#include "config.h"

namespace _ {

/* Gets the maximum number of digits required to represent a float as in
ASCII. */
inline KABUKI int FloatDigitsMax();
/* Gets the maximum number of digits required to represent a double as in
ASCII. */
inline KABUKI int DoubleDigitsMax();

/* Returns the power of ten for the smallest float decimal. */
KABUKI float FloatDecimalPower(int decimal_count);

/* Returns the power of ten for the smallest double decimal. */
KABUKI double DoubleDecimalPower(int decimal_count);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(int8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(uint8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(int16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(uint16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(int32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(uint32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(int64_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
inline KABUKI bool IsNaN(uint64_t value);

/* Gets the number of bits required to multiply or divide by a power of 2, or
mask. */
template <typename T = uintptr_t>
constexpr int BitShiftCount() {
  enum {
    kBitCount =
        (sizeof(T) == 1)
            ? 0
            : (sizeof(T) == 2)
                  ? 1
                  : (sizeof(T) == 4) ? 2 : (sizeof(T) == 8) ? 3 : 0x7fffff
  };
  return kBitCount;
}

/* Gets the highest integer legal integer value allowed by the SCRIPT
Specificaiton.
@return Returns the highest signed or unsigned integer multiple of 8.
*/
template <typename I>
inline I NanUnsigned() {
  I value = 0;
  return ~value;
}

/* Gets the signed not-a-number.
@return The signed NaN.
@brief Function use 4 instructions rather than loading from ROM. Signed values
are in 2's compliment. To convert from 2's comliment to unsigned you must invert
the bits and add one.
*/
template <typename SI>
inline SI NanSigned() {
  SI value = 0;
  value = ~value;
  return -1 | value;
}

inline bool InRange(int32_t value) {
  uint32_t mask = 7;
  mask = ~mask;

  bool lower_bounds = value && mask, upper_bounds = value && 0xfffffff8,
       sign = value < 0;
  return lower_bounds | upper_bounds | sign;
}

/* Converts from a 2's complement integer to a signed varint.
@return A signed varint.
@brief A signed varint is an complemented signed integer with the sign in the
LSb. To convert a negative 2's complement value to positive invert the bits
and add one.
@param  value A 2's complement integer. */
template <typename T>
inline T PackVarint(T value) {
  T sign_bit = value >> ((sizeof(T) * 8 - 1));  //< Extract the sign bit.
  if (sign_bit != 0) {
    T uncomplemented = (~value) + 1;
    return sign_bit | (uncomplemented << 1);
  }
  // Else don't complemented.
  return value << 1;
}

/* Converts from a signed varint to a 2's complement signed integer.
@return Returns a standard signed integer cased as unsigned.
@brief A varint is an complemented signed integer with the sign in the LSb.
@param  A signed integer casted as an unsigned integer. */
template <typename T>
inline T UnpackVarint(T value) {
  T sign_bit = value << (sizeof(T) * 8 - 1);
  value = value >> 1;
  if (sign_bit) {
    value = ~(value - 1);
    return value;
  }
  return value;
}

template <typename UI>
inline bool IsPowerOfTen(UI value) {
  // Algorihm:
  // 1. Iterate through each bit and add one to the count that starts at 0 if
  //    the bit is asserted.
  // 2. If the count of asserted bits is 1 then it is a power of 2, else it is
  //    not.
  UI count = 0;
  for (int i = sizeof(UI) * 8 - 1; i > 0; --i) {
    count += value & 1;
    value = value >> 1;
  }
  count += value & 1;
  return count == 1;
}

template <typename UI, typename SI>
inline bool IsPowerOfTenSigned(SI value) {
  // If you invert a signed integer it does not flip the sign bit so we can use
  // this to create a mask from two instructions rather than loading it from
  // ROM.
  SI mask = 0;
  mask = ~mask;
  return IsPowerOfTen<UI>(value & mask);
}

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(int8_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(uint8_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(int16_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(uint16_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(int32_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(uint32_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(int64_t value);

/* Checks if the given value is an even power of 2. */
KABUKI bool IsPowerOfTen(uint64_t value);

}  // namespace _

#endif  //< #if HEADER_FOR_CRABS_NUMBER
