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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
#ifndef HEADER_FOR_CRABS_NUMBER
#define HEADER_FOR_CRABS_NUMBER
#include "config.h"

namespace _ {

/* Gets the maximum number of digits required to represent a float as in
ASCII. */
KABUKI inline int FloatDigitsMax();
/* Gets the maximum number of digits required to represent a double as in
ASCII. */
KABUKI inline int DoubleDigitsMax();

/* Returns the power of ten for the smallest float decimal. */
KABUKI float FloatDecimalPower(int decimal_count);

/* Returns the power of ten for the smallest double decimal. */
KABUKI double DoubleDecimalPower(int decimal_count);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(int8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(uint8_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(int16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(uint16_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(int32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(uint32_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(int64_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(uint64_t value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(float value);

/* Checks if the given value is Not-a-Number.
@param  value The value to check.
@return True if the value is NaN and false otherwise. */
KABUKI inline bool IsNaN(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
KABUKI /*inline*/ bool IsFinite(float value);

/* Checks if the given value is not NaN or +/- Infinity. */
KABUKI inline bool IsFinite(double value);

/* Checks if the given value is not NaN or +/- Infinity. */
KABUKI inline bool IsInfinite(float value);

/* Checks if the given value is not NaN or +/- Infinity. */
KABUKI inline bool IsInfinite(double value);

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

/* Returns the ceiling of of the given value to the next highest up integer. */
KABUKI float Ceiling(float value);

/* Returns the ceiling of of the given value to the next highest up integer. */
KABUKI double Ceiling(double value);

}  // namespace _

#endif  //< #if HEADER_FOR_CRABS_NUMBER
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
