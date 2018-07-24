/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/text.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
#ifndef HEADER_FOR_CRABS_PRINT_FLOATING_POINT
#define HEADER_FOR_CRABS_PRINT_FLOATING_POINT
// Dependencies:
#include <stdio.h>
#include "debug.h"
#include "number.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 0
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#else
#define PRINT(item)
#define PRINTF(x, ...)
#endif
#if CRABS_TEXT

namespace _ {

struct FloatingPoint {
  uint64_t f;
  int e;
};

FloatingPoint Minus(FloatingPoint x, FloatingPoint y) {
  ASSERT(x.e == y.e && x.f >= y.f);
  FloatingPoint r = {x.f - y.f, x.e};
  return r;
}

FloatingPoint Multiply(FloatingPoint x, FloatingPoint y) {
  uint64_t a, b, c, d, ac, bc, ad, bd, tmp;
  FloatingPoint r;
  uint64_t M32 = 0xFFFFFFFF;

  a = x.f >> 32;
  b = x.f & M32;
  c = y.f >> 32;
  d = y.f & M32;
  ac = a * c;
  bc = b * c;
  ad = a * d;
  bd = b * d;
  tmp = (bd >> 32) + (ad & M32) + (bc & M32);
  tmp += 1U << 31;  // Round
  r.f = ac + (ad >> 32) + (bc >> 32) + (tmp >> 32);
  r.e = x.e + y.e + 64;
  return r;
}

int Ceiling(int a, float) { return 0; }

int k_comp(int e, int alpha, int gamma) {
  float x = 0.30102999f;
  // double x = 0.30102999566398114;
  x *= static_cast<float>(alpha - e + 63);  //< = x * 1/log_2(10)
  return static_cast<int>(Ceiling(x));
}

#define TEN7 10000000
void Cut(FloatingPoint D, uint32_t parts[3]) {
  parts[2] = (D.f % (TEN7 >> D.e)) << D.e;
  uint64_t tmp = D.f / (TEN7 >> D.e);
  parts[1] = tmp % TEN7;
  parts[0] = tmp / TEN7;
}

/*
void grisu(double v, char* buffer) {
  FloatingPoint w;
  uint32_t ps[3];
  int q = 64, alpha = 0, gamma = 3;

  w = normalize_floating_point(double2diy_fp(v));
  int mk = k_comp(w.e + q, alpha, gamma);
  FloatingPoint c_mk = cached_power(mk);
  FloatingPoint D = Multiply(w, c_mk);
  Cut(D, ps);
  sprintf(buffer, "%u%07u%07ue%d", ps[0], ps[1], ps[2], -mk);
}

int digit_gen_no_div(FloatingPoint D, char* buffer) {
  int i = 0,
      q = 64;
  FloatingPoint one;
  one.f = ((uint64_t)1) << -D.e;
  one.e = D.e;
  buffer[i++] = '0' + (D.f >> -one.e);  // division
  uint64_t f = D.f & (one.f - 1);       // modulo
  buffer[i++] = '.';
  while (-one.e > q - 5) {
    uint64_t tmp = (f << 2) & (one.f - 1);
    int d = f >> (-one.e - 3);
    d &= 6;
    f = f + tmp;
    d += f >> (-one.e - 1);
    buffer[i++] = '0' + d;
    one.e++;
    one.f >>= 1;
    f &= one.f - 1;
  }

  while (i < 19) {
    f *= 10;
    buffer[i++] = '0' + (f >> -one.e);
    f &= one.f - 1;
  }
  return i;
}*/

int digit_gen_mix(FloatingPoint d, char* buffer) {
  FloatingPoint one;
  one.f = ((uint64_t)1) << -d.e;
  one.e = d.e;
  uint32_t part1 = d.f >> -one.e;
  uint64_t f = d.f & (one.f - 1);
  int i = sprintf(buffer, "%u", part1);
  buffer[i++] = '.';
  while (i < 19) {
    f *= 10;
    buffer[i++] = '0' + (f >> -one.e);
    f &= one.f - 1;
  }
  return i;
}

#define TEN9 1000000000

void GenerateDigits(FloatingPoint mp, FloatingPoint delta, char* buffer,
                    int* length, int* k) {
  uint32_t div;
  int d, kappa;
  FloatingPoint one;
  one.f = ((uint64_t)1) << -mp.e;
  one.e = mp.e;
  uint32_t p1 = mp.f >> -one.e;
  uint64_t p2 = mp.f & (one.f - 1);
  *length = 0;
  kappa = 10;
  div = TEN9;
  while (kappa > 0) {
    d = p1 / div;
    if (d || *length) buffer[(*length)++] = '0' + d;
    p1 %= div;
    kappa--;
    div /= 10;
    if ((((uint64_t)p1) << -one.e) + p2 <= delta.f) {
      *k += kappa;
      return;
    }
  }
  do {
    p2 *= 10;
    d = p2 >> -one.e;
    if (d || *length) buffer[(*length)++] = '0' + d;
    p2 &= one.f - 1;
    kappa--;
    delta.f *= 10;
  } while (p2 > delta.f);
  *k += kappa;
}

/* Writes the give value to the given buffer as an ASCII string.
@warning This function is slow right now because it's using sprintf but GrisuX
coming soon.
@param begin Beginning address of the buffer.
@param end   The end address of the buffer.
@param value The value to write. */
template <typename Char = char>
Char* Print(Char* begin, Char* end, float value) {
  // @todo Replace with GrisuX algorithm that uses the Script itoa Algorithm.
  intptr_t buffer_size = end - begin;
  char* buffer = new char[buffer_size];
  int count = sprintf_s(buffer, buffer_size, "%f", value);
  if (count < 0) {
    *begin = 0;
    delete buffer;
    return nullptr;
  }
  char* buffer_cursor = buffer;
  for (; count > 0; --count) *begin++ = *buffer_cursor++;
  *begin = 0;
  delete buffer;
  return begin;
}

/* Writes the give value to the given buffer as an ASCII string.
@warning This function is slow right now because it's using
sprintf but GrisuX coming soon.
@param begin Beginning address of the buffer.
@param end   The end address of the buffer.
@param value The value to write. */
template <typename Char = char>
Char* Print(Char* cursor, Char* end, double value) {
  // Right now we're going to enforce there be enough room to write any
  // int32_t.
  intptr_t buffer_size = end - cursor;
  char* buffer = new char[buffer_size];
  int count = sprintf_s(buffer, buffer_size, "%f", value);
  if (count < 0) {
    *cursor = 0;
    delete buffer;
    return nullptr;
  }
  char* read = buffer;
  char c = *read++;
  while (c) {
    *cursor++ = c;
    c = *read++;
  }
  *cursor = 0;
  return cursor;
}

bool round_weed(char* buffer, int len, uint64_t wp_W, uint64_t Delta,
                uint64_t rest, uint64_t ten_kappa, uint64_t ulp) {
  uint64_t wp_Wup = wp_W - ulp;
  uint64_t wp_Wdown = wp_W + ulp;
  while (rest < wp_Wup && Delta - rest >= ten_kappa &&
         (rest + ten_kappa < wp_Wup ||
          wp_Wup - rest >= rest + ten_kappa - wp_Wup)) {
    buffer[len - 1]--;
    rest += ten_kappa;
  }
  if (rest < wp_Wdown && Delta - rest >= ten_kappa &&
      (rest + ten_kappa < wp_Wdown ||
       wp_Wdown - rest > rest + ten_kappa - wp_Wdown))
    return false;
  return 2 * ulp <= rest && rest <= Delta - 4 * ulp;
}

template <typename Float, typename UI>
Float FloatNormalize(UI sign, UI integer) {
  enum {
    kMantissaBitCount =
        (sizeof(Float) == 2)
            ? 4
            : (sizeof(Float) == 4)
                  ? 23
                  : (sizeof(Float) == 8) ? 52 : (sizeof(Float) == 16) ? 113 : 0,
    kExponentBitCount = sizeof(Float) * 8 - kMantissaBitCount - 1,
  };
  int bit_shift_count;
  return sign | (integer << bit_shift_count);
}

/* Converts a string-to-float.
@return nil if there is no number to scan or pointer to the next char after the
the end of the scanned number upon success.
@brief Algorithm uses a 32-bit unsigned value to scan the floating-point number,
which can only have 10 digits max, so the maximum floating-point number digit
count we can scan is 9 digits long.*/
template <typename Char = char>
const Char* Scan(const Char* begin, float& result) {
  ASSERT(begin);
  PRINTF("\n\nScanning float:%s", begin);

  enum {
    kCharCountMax = 9,  // < (1 + [p*log_10(2)], where p = 32
  };

  static const uint32_t kPow10[] = {
      10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

  uint32_t integer,    //< Integer portion in Binary.
      sign,            //< Sign in Binary32 format.
      unsigned_value,  //< Unsigned value.
      pow_10;          //< Power of 10 for ocnverting integers.

  // Scan sign of number:

  if (*begin == '-') {
    sign = 1 << (8 * sizeof(float) - 1);
    ++begin;
  } else {
    sign = 0;
  }

  PRINTF("\nScannign intger portion:%i", static_cast<int32_t>(result));

  const Char* cursor = begin;
  Char c = *cursor++;
  if (!IsDigit<Char>(c)) return nullptr;

  // Find length:
  c = *cursor++;
  while (IsDigit<Char>(c)) c = *cursor++;
  const Char* end = cursor;  // Store end to return.
  cursor -= 2;
  PRINTF("\nPointed at \'%c\' and found length:%i", *cursor,
         (int)(cursor - begin));

  c = *cursor--;
  unsigned_value = (uint32_t)(c - '0');
  pow_10 = 1;

  while (cursor >= begin) {
    c = *cursor--;
    pow_10 *= 10;
    uint32_t new_value = unsigned_value + pow_10 * (c - '0');
    if (new_value < unsigned_value) return nullptr;
    unsigned_value = new_value;
    PRINTF("\nvalue:%u", (uint)unsigned_value);
  }

  integer = unsigned_integer;

  PRINTF("\nfound %i and pointed at \'%c\'", integer, *end);

  // Numbers may start with a dot like .1, .2, ...
  if (*begin == '.') goto ScanDecimals;

  if (*end != '.') {
    result = static_cast<float>(integer);
    PRINTF("\nFound value:%f", result);
    return end;
  }
  ++begin;
ScanDecimals:
  // We have to inline the TextScanUnsigned here in order to detect if there are
  // too many decimals
  cursor = end;
  Char c = *cursor++;
  if (!IsDigit<Char>(c)) {
    PRINTF("Found a period.");
    return nullptr;
  }
  PRINTF("\nConverting decimals:\"%s\" with max length %i", begin,
         kCharCountMax);

  // Find length
  c = *cursor++;
  while (IsDigit<Char>(c)) c = *cursor++;

  end = cursor;  // Store end to return.
  cursor -= 2;

  intptr_t length = cursor - begin;
  PRINTF("\nPointed at \'%c\' and found length:%i", *cursor, (int)length);

  if (length > kCharCountMax) {
    cursor = begin + kCharCountMax;
    length = kCharCountMax;
  }

  // Manually load the first char.
  c = *cursor--;
  unsigned_value = (uint32_t)(c - '0'), pow_10 = 1;

  // Then iterate through the rest in a loop.
  while (cursor >= begin) {
    c = *cursor--;
    pow_10 *= 10;
    uint32_t new_value = unsigned_value + pow_10 * (c - '0');
    if (new_value < unsigned_value) {
      PRINTF("\nUnsigned wrap-around!");
      return nullptr;
    }
    unsigned_value = new_value;
    PRINTF("\nFound integer_value:%u", (uint)unsigned_value);
  }
  PRINTF("\nFound integer_value:%u", (uint)unsigned_value);

  PRINTF("\nConverting bit_pattern backwarards:");

  // Convert decimals to base 2 by multiply in a loop the intger value is
  // greater than one then subtract the equivalent of one until the value
  // is zero.

  uint32_t one = kPow10[length - 1], bit_pattern = 0;
  PRINT('\n');
  int bit_shift_count = 0;
  while ((unsigned_value != 0) && (++bit_shift_count < 24)) {
    unsigned_value = unsigned_value << 1;  //< << 1 to * 2
    if (unsigned_value >= one) {
      bit_pattern = (bit_pattern << 1) | 1;
      unsigned_value -= one;
      PRINT('1');
    } else {
      bit_pattern = bit_pattern << 1;
      PRINT('0');
    }
  }
  PRINTF("'b0");
  PRINT_FLOAT_BINARY(integer, unsigned_value, length);
  // Now check for the exponenet.

  unsigned_value |= integer << length;

  PRINTF("\nNormalizing bits...");

  if (c != 'e' && c != 'E') {
    PRINTF("\nNo \'e\' or \'E\' found.");
    result = sign | FloatNormalize<float, uint32_t>(integer);
    return end;
  }

  // @todo This is no doubt optimizable, not sure how much it would help
  // though.
  int32_t signed_value;
  begin = TextScanSigned<Char, int32_t, uint32_t>(end, signed_value);
  if (!begin) {
    PRINTF("\nNo exponent found.");
    // result = reinterpret_cast(sign |);
    return end;
  }

  if (signed_value < -128 || signed_value > 127) {
    PRINTF("\nExponent out of range!");
    result = result_flt;
    return end;
  }

  // We're finally done so store the result.
  result = result_flt;

  return end;
}

template <typename Char = char>
const Char* Scan(const Char* string, double& result) {
  ASSERT(string);

  enum {
    kCharCountMax = 17,  // < (1 + [p*log_10(2)], where p = 64
  };

  return nullptr;
}

}  // namespace _

#endif  //< HEADER_FOR_CRABS_PRINT_FLOATING_POINT
#endif  //< CRABS_TEXT
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >=
