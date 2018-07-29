/* Kabuki Toolkit @version 0.x
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
#include <cfloat>
#include <cmath>
#include "number.h"
#include "tnumber.h"

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

bool IsNaN(float value) { return isnan(value); }

bool IsNaN(double value) { return isnan(value); }

bool IsFinite(float value) { return isfinite(value); }

bool IsFinite(double value) { return isfinite(value); }

bool IsInfinite(float value) { return isinf(value); }

bool IsInfinite(double value) { return isinf(value); }

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

/* Masks the lower bits using faster bit shifting.
@brief The algoirhm has you enter the highest bit rather than bit count because
it would introduct an extra instruction and you should do that manually if you
wish to do so.
@param value The value to mask.
@param mab The Most Significant bit, or one less than the number of bits to
mask off. */
template <typename UI>
inline UI ShiftLeftRight(UI value, int left_bits, int right_bits) {
  value = value << left_bits;
  return value >> right_bits;
}

/* Creates a mask with the given number of zeros in the MSb(s).
@param msb_zero_count The number of zeros in the Most Significant bits. */
template <typename UI>
inline UI CreateMaskLSb(UI value, UI msb_zero_count) {
  UI mask = 0;
  return (~mask) >> msb_zero_count;
}

/* Masks off the lower bits.
@param msb_zero_count The number of zeros in the Most Significant bits. */
template <typename UI>
inline UI MaskLSb(UI value, UI msb_zero_count) {
  return value & CreateMaskLSb<UI>(msb_zero_count);
}

/* Retruns 2^n.
@brief Funciton forces the compiler to create the mask without a LDR
instruciton. */
template <typename I>
inline I PowerOf2(I n) {
  I value = 1;
  return value << n;
}

/* Calculates the ceiling of the given value to the next highest integer.
@param value The value to take the ceiling of.
@return The ceiling to the next highest integer. */
template <typename Float, typename UI>
Float Ceiling(Float value) {
  enum {
    kFpBits = sizeof(Float) * 8 - 1,
    kExponentBitCount =
        (sizeof(Float) == 2)
            ? 5
            : (sizeof(Float) == 4)
                  ? 8
                  : (sizeof(Float) == 8) ? 11 : (sizeof(Float) == 16) ? 15 : 0,
    kExponentMask = ((1 << (kExponentBitCount - 1)) - 1),
    kMantissaBitCount = kFpBits - kExponentBitCount,
    kMantissaMSb = kMantissaBitCount - 1,
  };

  UI integer = *reinterpret_cast<UI*>(&value);

  // Extract sign, exponent and mantissa bias is removed from exponent.
  UI sign = integer >> kFpBits,
     exponent_mask = ShiftLeftRight<UI>(integer, 1, kMantissaBitCount),
     exponent = ((integer & 0x7fffffff) >> kMantissaMSb) - kExponentMask,
     mantissa = integer & 0x7fffff, comparator;

  if (exponent < 0) {  // value is in the open interval (-1, 1)
    if (value <= 0.0)
      return 0.0;
    else
      return 1.0;
  } else {
    // Mask the fractional part of the mantissa.
    UI mask = CreateMaskLSb<UI>(kMantissaMSb) >>
              exponent;  //< mask should be 0x7fffff

    if ((mantissa & mask) == 0)  // The fractional bits zero so it's an integer.
      return value;
    else {
      if (sign == 0) {
        // It's positive so add 1 to it before clearing the fractional bits.
        mantissa += ((UI)1) << (kMantissaMSb - exponent);

        comparator = PowerOf2<UI>(kFpBits);

        // Check for mantissa overflow...
        if (mantissa & comparator) {
          // The mantissa can only overflow if all the integer bits were
          // previously 1, so we can just clear out the mantissa and increment
          // the exponent.
          mantissa = 0;
          ++exponent;
        }
      }

      // Clear the fractional bits.
      mantissa &= ~mask;
    }
  }

  // Put sign, exponent and mantissa together again
  integer = (sign << kFpBits) | ((exponent + kExponentMask) << kMantissaMSb) |
            mantissa;

  return *reinterpret_cast<Float*>(&integer);
}

struct diy_fp {
  uint64_t f;
  int32_t e;
};

#define TEN9 1000000000

void digit_gen(diy_fp Mp, diy_fp delta, char* buffer, int* len, int* K) {
  uint32_t div;
  int32_t d, kappa;
  diy_fp one;
  one.f = ((uint64_t)1) << -Mp.e;
  one.e = Mp.e;
  uint32_t p1 = Mp.f >> -one.e;
  uint64_t p2 = Mp.f & (one.f - 1);
  *len = 0;
  kappa = 10;
  div = TEN9;
  while (kappa > 0) {
    d = p1 / div;
    if (d || *len) buffer[(*len)++] = '0' + d;
    p1 %= div;
    kappa--;
    div /= 10;
    if ((((uint64_t)p1) << -one.e) + p2 <= delta.f) {
      *K += kappa;
      return;
    }
  }
  do {
    p2 *= 10;
    d = p2 >> -one.e;
    if (d || *len) buffer[(*len)++] = '0' + d;
    p2 &= one.f - 1;
    kappa--;
    delta.f *= 10;
  } while (p2 > delta.f);
  *K += kappa;
}

float Ceiling(float value) { return Ceiling<float, uint32_t>(value); }

double Ceiling(double value) { return Ceiling<double, uint64_t>(value); }

union float_int {
  float f;
  int i;
};

inline float myceil(float x) {
  float_int val;
  val.f = x;

  // Extract sign, exponent and mantissa
  // Bias is removed from exponent
  int sign = val.i >> 31;
  int exponent = ((val.i & 0x7fffffff) >> 23) - 127;
  int mantissa = val.i & 0x7fffff;

  // Is the exponent less than zero?
  if (exponent < 0) {
    // In this case, x is in the open interval (-1, 1)
    if (x <= 0.0f)
      return 0.0f;
    else
      return 1.0f;
  } else {
    // Construct a bit mask that will mask off the
    // fractional part of the mantissa
    int mask = 0x7fffff >> exponent;

    // Is x already an integer (i.e. are all the
    // fractional bits zero?)
    if ((mantissa & mask) == 0)
      return x;
    else {
      // If x is positive, we need to add 1 to it
      // before clearing the fractional bits
      if (!sign) {
        mantissa += 1 << (23 - exponent);

        // Did the mantissa overflow?
        if (mantissa & 0x800000) {
          // The mantissa can only overflow if all the
          // integer bits were previously 1 -- so we can
          // just clear out the mantissa and increment
          // the exponent
          mantissa = 0;
          exponent++;
        }
      }

      // Clear the fractional bits
      mantissa &= ~mask;
    }
  }

  // Put sign, exponent and mantissa together again
  val.i = (sign << 31) | ((exponent + 127) << 23) | mantissa;

  return val.f;
}

}  // namespace _

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
