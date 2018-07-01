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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#include <cfloat>
#include "number.h"

namespace _ {

int FloatDigitsMax() { return 8; }  //< 3 + FLT_MANT_DIG - FLT_MIN_EXP;

float FloatDecimalPower(int decimal_count) {
  static const float kDecimals[] = {0.1f,       0.01f,      0.001f,
                                    0.0001f,    0.00001f,   0.000001f,
                                    0.0000001f, 0.00000001f};
  if (decimal_count < 1 || decimal_count > FloatDigitsMax()) return 0.0f;
  return kDecimals[decimal_count - 1];
}

int DoubleDigitsMax() { return 16; }  //< 3 + DBL_MANT_DIG - DBL_MIN_EXP;

double DoubleDecimalPower(int decimal_count) {
  static const double kDecimals[] = {
      0.1,
      0.01,
      0.001,
      0.0001,
      0.00001,
      0.000001,
      0.0000001,
      0.00000001,
      0.000000001,
      0.0000000001,
      0.00000000001,
      0.000000000001,
      0.0000000000001,
      0.00000000000001,
      0.000000000000001,
      0.0000000000000001,
  };
  if (decimal_count < 0 || decimal_count > DoubleDigitsMax()) return 0.0;
  return kDecimals[decimal_count];
}

bool IsNaNPositive(int8_t value) { return value > NanUnsigned<int8_t>(); }
bool IsNaNNegative(int8_t value) { return value > NanUnsigned<int8_t>(); }
bool IsNaN(int8_t value) {
  return (value > NanUnsigned<int8_t>()) && (value > NanSigned<int8_t>());
}

bool IsNaN(uint8_t value) { return value > NanUnsigned<uint8_t>(); }

bool IsNaN(int16_t value) { return value > NanUnsigned<int16_t>(); }

bool IsNaN(uint16_t value) { return value > NanUnsigned<uint16_t>(); }

bool IsNaN(int32_t value) { return value > NanUnsigned<int32_t>(); }

bool IsNaN(uint32_t value) { return value > NanUnsigned<uint32_t>(); }

bool IsNaN(int64_t value) { return value > NanUnsigned<int64_t>(); }

bool IsNaN(uint64_t value) { return value > NanUnsigned<uint64_t>(); }

bool IsPowerOfTen(int8_t value) {
  return IsPowerOfTenSigned<uint8_t, int8_t>(value);
}

bool IsPowerOfTen(uint8_t value) { return IsPowerOfTen<uint8_t>(value); }

bool IsPowerOfTen(int16_t value) {
  return IsPowerOfTenSigned<uint16_t, int16_t>(value);
}

bool IsPowerOfTen(uint16_t value) { return IsPowerOfTen<uint16_t>(value); }

bool IsPowerOfTen(int32_t value) {
  return IsPowerOfTenSigned<uint32_t, int32_t>(value);
}

bool IsPowerOfTen(uint32_t value) { return IsPowerOfTen<uint32_t>(value); }

bool IsPowerOfTen(int64_t value) {
  return IsPowerOfTenSigned<uint64_t, int64_t>(value);
}

bool IsPowerOfTen(uint64_t value) { return IsPowerOfTen<uint64_t>(value); }

}  // namespace _

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
