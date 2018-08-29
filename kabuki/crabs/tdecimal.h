/* Kabuki Toolkit @version 0.x
@file    $kabuki-toolkit/kabuki/crabs/tdecimal.h
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
#ifndef INCLUDED_CRABS_TDECIMAL
#define INCLUDED_CRABS_TDECIMAL
#include "decimal.h"
#if SEAM == SEAM_0_0_0
//#include <intrin.h>
#include "debug.h"

namespace _ {
inline void PrintBinaryTable(uint64_t value) {
  enum { kSize = sizeof(uint64_t) * 8 };

  Print("\n    ");
  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    Print(c);
    value = value << 1;
  }
  Print(
      "\n    0000000000000000000000000000000000000000000000100100100111110000"
      "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "\n    6666555555555544444444443333333333222222222211111111110000000000"
      "\n    3210987654321098765432109876543210987654321098765432109876543210"
      "\n    |  |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |"
      "\n    2  1  1  1   1  1  1   1  1  1   1  0  0   0  0  0   0  0  0   0"
      "\n    0  9  8  7   6  5  4   3  2  1   0  9  8   7  6  5   4  3  2   1");
}
}  // namespace _

#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PRINTED                                                   \
  ;                                                                     \
  sprintf_s(buffer, 24, "%u", value);                                   \
  *end = 0;                                                             \
  Printf("\n    Printed \"%s\" leaving value:\"%s\":%u", begin, buffer, \
         (uint)strlen(buffer))
#define PRINT_BINARY \
  Print("\n    ");   \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define PRINT_HEADER                   \
  for (int32_t i = 0; i < 10; ++i) {   \
    *(cursor + i) = 'x';               \
  }                                    \
  *(cursor + 21) = 0;                  \
  Char* begin = cursor;                \
  Char buffer[256];                    \
  sprintf_s(buffer, 256, "%u", value); \
  Printf("Expecting %s:%u", buffer, (uint)strlen(buffer));
#define PRINT_HEADING \
  Print('\n');        \
  for (int32_t i = 80; i > 0; --i) Print('-')
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_PRINTED ;
#define PRINT_HEADER
#define PRINT_HEADING
#endif

namespace _ {

/** @ingroup Utf */

/* Writes the give value to the given buffer as an ASCII string.
@param  print The Utf& to print to.
@param value The value to write. */
template <typename Char = char>
Char* Print(Char* begin, Char* end, int32_t value) {
  if (value >= 0) return Print(begin, end, (uint32_t)value);
  *begin++ = '-';
  return Print<Char>(begin, end, (uint32_t)(value * -1));
}

/* Writes the give value to the given buffer as an ASCII string.
@param  print The Utf& to print to.
@param end   The end address of the buffer. */
template <typename Char = char>
Char* Print(Char* cursor, Char* end, uint64_t value) {
  PRINT_HEADING;

  static const Char kMsbShift[] = {
      4, 7, 11, 14, 17, 21, 24, 27, 30,
  };

  if (!cursor) {
    return nullptr;
  }
  if (cursor >= end) {
    return nullptr;
  }

  uint16_t* text16;
  Char digit;
  uint32_t scalar;
  uint16_t digits1and2, digits3and4, digits5and6, digits7and8;
  uint32_t comparator = 100000000;

  PRINT_HEADER;

  while (value > comparator) {
    scalar = value / comparator;
    value -= scalar * comparator;

    PRINTF("\n    Print8:");
    scalar = 10000;
    digits5and6 = (uint16_t)(value / scalar);
    digits1and2 = value - scalar * digits5and6;
    digits7and8 = digits5and6 / 100;
    digits3and4 = digits1and2 / 100;
    digits5and6 -= 100 * digits7and8;
    digits1and2 -= 100 * digits3and4;
    *reinterpret_cast<uint16_t*>(cursor + 6) = kDigits00To99[digits1and2];
    PRINT_PRINTED;
    *reinterpret_cast<uint16_t*>(cursor + 4) = kDigits00To99[digits3and4];
    PRINT_PRINTED;
    *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits5and6];
    PRINT_PRINTED;
    *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits7and8];
    PRINT_PRINTED;
  }

  if (value < 10) {
    PRINT("\n    Range:[0, 9] length:1 ");
    if (cursor + 1 >= end) {
      return nullptr;
    }
    *cursor++ = '0' + (Char)value;
    PRINT_PRINTED;
    *cursor = 0;
    return cursor;
  }
  if (value < 100) {
    PRINT("\n    Range:[10, 99] length:2 ");
    if (cursor + 2 >= end) {
      return nullptr;
    }
    *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[value];
    PRINT_PRINTED;
    *(cursor + 2) = 0;
    return cursor + 2;
  }
  if (value >> 14) {
    if (value >> 27) {
      else if (value >> 24) {
        comparator = 100000000;
        if (value >= comparator) {
          PRINT("\n    Range:[100000000, 134217728] length:9");
          if (cursor + 9 >= end) {
            return nullptr;
          }
          *cursor++ = '1';
          PRINT_PRINTED;
          value -= comparator;
        }
        PRINT("\n    Range:[16777216, 9999999] length:8");
        if (cursor + 8 >= end) {
          return nullptr;
        }
      Print8:
        PRINTF("\n    Print8:");
        scalar = 10000;
        digits5and6 = (uint16_t)(value / scalar);
        digits1and2 = value - scalar * digits5and6;
        digits7and8 = digits5and6 / 100;
        digits3and4 = digits1and2 / 100;
        digits5and6 -= 100 * digits7and8;
        digits1and2 -= 100 * digits3and4;
        *reinterpret_cast<uint16_t*>(cursor + 6) = kDigits00To99[digits1and2];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor + 4) = kDigits00To99[digits3and4];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits5and6];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits7and8];
        PRINT_PRINTED;
        *(cursor + 8) = 0;
        return cursor + 8;
      }
      else if (value >> 20) {
        comparator = 10000000;
        if (value >= comparator) {
          PRINT("\n    Range:[10000000, 16777215] length:8");
          if (cursor + 8 >= end) {
            return nullptr;
          }
          *cursor++ = '1';
          PRINT_PRINTED;
          value -= comparator;
        } else {
          PRINT("\n    Range:[1048576, 9999999] length:7");
          if (cursor + 7 >= end) {
            return nullptr;
          }
        }
        scalar = 10000;
        digits5and6 = (uint16_t)(value / scalar);
        digits1and2 = value - scalar * digits5and6;
        digits7and8 = digits5and6 / 100;
        digits3and4 = digits1and2 / 100;
        digits5and6 -= 100 * digits7and8;
        digits1and2 -= 100 * digits3and4;
        *reinterpret_cast<uint16_t*>(cursor + 5) = kDigits00To99[digits1and2];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor + 3) = kDigits00To99[digits3and4];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor + 1) = kDigits00To99[digits5and6];
        PRINT_PRINTED;
        *cursor = (Char)digits7and8 + '0';
        *(cursor + 7) = 0;
        return cursor + 7;
      }
      else if (value >> 17) {
        comparator = 1000000;
        if (value >= comparator) {
          PRINT("\n    Range:[100000, 1048575] length:7");
          if (cursor + 7 >= end) {
            return nullptr;
          }
          *cursor++ = '1';
          PRINT_PRINTED;
          value -= comparator;
        } else {
          PRINT("\n    Range:[131072, 999999] length:6");
          if (cursor + 6 >= end) {
            return nullptr;
          }
        }
      Print6:
        scalar = 10000;
        digits5and6 = (uint16_t)(value / scalar);
        digits1and2 = value - scalar * digits5and6;
        digits7and8 = digits5and6 / 100;
        digits3and4 = digits1and2 / 100;
        digits5and6 -= 100 * digits7and8;
        digits1and2 -= 100 * digits3and4;
        text16 = reinterpret_cast<uint16_t*>(cursor + 6);
        *reinterpret_cast<uint16_t*>(cursor + 4) = kDigits00To99[digits1and2];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits3and4];
        PRINT_PRINTED;
        *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits5and6];
        PRINT_PRINTED;
        *(cursor + 6) = 0;
        return cursor + 6;
      }
      else {  // (value >> 14)
        if (value >= 100000) {
          PRINT("\n    Range:[65536, 131071] length:6");
          goto Print6;
        }
        PRINT("\n    Range:[10000, 65535] length:5");
        if (cursor + 5 >= end) {
          return nullptr;
        }
        digits5and6 = 10000;
        digit = (uint8_t)(value / digits5and6);
        value -= digits5and6 * digit;
        *cursor = digit + '0';
        PRINT_PRINTED;
        digits1and2 = (uint16_t)value;
        digits5and6 = 100;
        digits3and4 = digits1and2 / digits5and6;
        digits1and2 -= digits3and4 * digits5and6;
        *reinterpret_cast<uint16_t*>(cursor + 1) = kDigits00To99[digits3and4];
        PRINT_PRINTED;
        PRINTF("\n    digits1and2:%u", digits1and2);
        *reinterpret_cast<uint16_t*>(cursor + 3) = kDigits00To99[digits1and2];
        PRINT_PRINTED;
        *(cursor + 5) = 0;
        return cursor + 5;
      }
    }
    digits1and2 = (uint16_t)value;
    if (value >> 10) {
      digits5and6 = 10000;
      if (digits1and2 >= digits5and6) {
        if (cursor + 5 >= end) {
          return nullptr;
        }
        PRINT("\n    Range:[10000, 16383] length:5");
        *cursor++ = '1';
        PRINT_PRINTED;
        digits1and2 -= digits5and6;

      } else {
        PRINT("\n    Range:[1024, 9999] length:4");
        if (cursor + 4 >= end) {
          return nullptr;
        }
      }
      digits5and6 = 100;
      digits3and4 = digits1and2 / digits5and6;
      digits1and2 -= digits3and4 * digits5and6;
      *reinterpret_cast<uint16_t*>(cursor) = kDigits00To99[digits3and4];
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 2) = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *(cursor + 4) = 0;
      return cursor + 4;
    } else {
      if (cursor + 4 >= end) {
        return nullptr;
      }
      digits3and4 = 1000;
      if (digits1and2 >= digits3and4) {
        PRINT("\n    Range:[1000, 1023] length:4");
        digits1and2 -= digits3and4;
        text16 = reinterpret_cast<uint16_t*>(cursor + 2);
        *text16-- = kDigits00To99[digits1and2];
        PRINT_PRINTED;
        *text16 = (((uint16_t)'1') | (((uint16_t)'0') << 8));
        PRINT_PRINTED;
        *(cursor + 4) = 0;
        return cursor + 4;
      }
      PRINT("\n    Range:[100, 999] length:3");
      digits1and2 = (uint16_t)value;
      digits3and4 = 100;
      digit = (Char)(digits1and2 / digits3and4);
      digits1and2 -= digit * digits3and4;
      *cursor = digit + '0';
      PRINT_PRINTED;
      *reinterpret_cast<uint16_t*>(cursor + 1) = kDigits00To99[digits1and2];
      PRINT_PRINTED;
      *(cursor + 3) = 0;
      return cursor + 3;
    }
  }

  /* Writes the give value to the given buffer as an ASCII string.
  @param  print The Utf& to print to.
  @param value The value to write. */
  template <typename Char = char>
  Char* Print(Char * begin, Char * end, int64_t value) {
    if (value >= 0) return Print<Char>(begin, end, (uint64_t)value);
    *begin++ = '-';
    return Print<Char>(begin, end, (uint64_t)(value * -1));
  }

  /* Checks if the given char is a digit of a number.
  @return True if it is a digit. */
  template <typename Char = char>
  bool IsDigit(Char c) {
    return (c >= '0') && (c <= '9');
  }

  template <typename Char, typename UI = uint>
  const Char* TextScanUnsigned(const Char* begin, UI& result) {
    ASSERT(begin);
    PRINTF("\nScanning unsigned value:%s", begin);
    const Char* cursor = begin;
    Char c = *cursor++;
    if (!IsDigit<Char>(c)) return nullptr;

    // Find length:
    c = *cursor++;
    while (IsDigit<Char>(c)) c = *cursor++;
    const Char* end = cursor;  // Store end to return.
    cursor -= 2;
    PRINTF("\nPointed at \'%c\' and found length:%i", *cursor,
           (int32_t)(cursor - begin));

    c = *cursor--;
    UI value = (UI)(c - '0');
    UI pow_10 = 1;

    while (cursor >= begin) {
      c = *cursor--;
      pow_10 *= 10;
      UI new_value = value + pow_10 * (c - '0');
      if (new_value < value) return nullptr;
      value = new_value;
      PRINTF("\nvalue:%u", (uint)value);
    }
    PRINTF("\nvalue:%u", (uint)value);
    result = value;
    return end;
  }

  template <typename Char, typename SI = int32_t, typename UI = uint>
  const Char* TextScanSigned(const Char* begin, SI& result) {
    ASSERT(begin);
    SI sign;
    const Char* cursor = begin;
    Char c = *cursor++;
    if (c == '-') {
      PRINTF("\nScanning negative backwards:\"");
      c = *begin++;
      sign = -1;
    } else {
      PRINTF("\nScanning positive backwards:\"");
      sign = 1;
    }
    if (!IsDigit<Char>(c)) return nullptr;

    // Find length:
    c = *cursor++;
    while (IsDigit<Char>(c)) c = *cursor++;
    const Char* end = cursor;  // Store end to return.
    cursor -= 2;
    PRINTF("\nPointed at \'%c\' and found length:%i", *cursor,
           (int32_t)(cursor - begin));

    c = *cursor--;
    UI value = (UI)(c - '0');
    UI pow_10 = 1;

    while (cursor >= begin) {
      c = *cursor--;
      pow_10 *= 10;
      UI new_value = value + pow_10 * (c - '0');
      if (new_value < value) return nullptr;
      value = new_value;
      PRINTF("\nvalue:%u", (uint)value);
    }
    result = sign * value;
    return end;
  }

  template <typename Char>
  const Char* Scan(const Char* begin, uint8_t& result) {
    return TextScanUnsigned<Char, uint8_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, int8_t& result) {
    return TextScanSigned<Char, int8_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, uint16_t& result) {
    return TextScanUnsigned<Char, uint16_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, int16_t& result) {
    return TextScanSigned<Char, int16_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, uint32_t& result) {
    return TextScanUnsigned<Char, uint32_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, int32_t& result) {
    return TextScanSigned<Char, int32_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, uint64_t& result) {
    return TextScanUnsigned<Char, uint64_t>(begin, result);
  }

  template <typename Char>
  const Char* Scan(const Char* begin, int64_t& result) {
    return TextScanSigned<Char, int64_t>(begin, result);
  }

  template <typename Float, typename UI>
  inline UI Value(Float value) {
    return *reinterpret_cast<UI*>(&value);
  }

  /* A decimal number. */
  template <typename Float, typename UI>
  class Decimal {
   public:
    enum {
      kStringLengthMax = 24,
      kExponentSize =
          (sizeof(Float) == 2)
              ? 5
              : (sizeof(Float) == 4) ? 8 : (sizeof(Float) == 8) ? 11 : 15,
      kBitCount = sizeof(Float) * 8,
      kSignificandSize = kBitCount - kExponentSize - 1,
      kExponentMaskUnshifted = (~((uint32_t)0)) >> (32 - kExponentSize),
      kExponentBias = kExponentMaskUnshifted + kSignificandSize,
      kExponentMin = -kExponentBias,
    };

    /* Constructs an uninitized floating-point number. */
    Decimal() {}

    /* Converts a Float to a Decimal*/
    Decimal(Float binary) {
      UI ui = *reinterpret_cast<UI*>(&binary);
      uint32_t biased_e = ui << 1;  //< Get rid of sign bit.

      biased_e = biased_e >> ((sizeof(Float) * 8) - kExponentSize);
      uint64_t significand =
          ui << (kExponentSize + 1);  // Get rid of the sign and exponent.
      significand = significand >> (kExponentSize + 1);
      if (biased_e != 0) {
        f = significand + (((UI)1) << kExponentSize);
        e = biased_e - kExponentBias;
      } else {
        f = significand;
        e = kExponentMin + 1;
      }
    }

    Decimal(UI f, int32_t e) : f(f), e(e) {}

    inline char* Print(char* begin, char* end, Float value) {
      // Not handling NaN and inf
      if (IsNaN(value)) {
        if (end - begin < 4) return nullptr;
        begin[0] = 'N';
        begin[1] = 'a';
        begin[2] = 'N';
        begin[3] = 0;
        return begin + 4;
      }
      if (IsInfinite(value)) {
        if (end - begin < 4) return nullptr;
        begin[0] = 'I';
        begin[1] = 'n';
        begin[2] = 'f';
        begin[3] = 0;
        return begin + 4;
      }

      if (value == 0) {
        return PrintPacked(begin, end, '0', '.', '0');
      }
      if (value < 0) {
        *begin++ = '-';
        value = -value;
      }
      int32_t length, k;
      char* cursor = Puff(begin, end, value, &length, &k);
      Standardize(begin, length, k);
      return cursor;
    }

   private:
    UI f;
    int32_t e;

    Decimal NormalizeBoundary() const {
#if defined(_MSC_VER) && defined(_M_AMD64)
      unsigned long index;  //< This is Microsoft's fault.
      _BitScanReverse64(&index, f);
      return Decimal(f << (63 - index), e - (63 - index));
#else
      DiyFp res = *this;
      while (!(res.f & (kDpHiddenBit << 1))) {
        res.f <<= 1;
        --res.e;
      }
      res.f <<= (kDiySignificandSize - kSignificandSize - 2);
      res.e = res.e - (kDiySignificandSize - kSignificandSize - 2);
      return res;
#endif
    }
    /*
    static const uint64_t  // kDpExponentMask = 0x7FF0000000000000,
    kDpSignificandMask = 0x000FFFFFFFFFFFFF,
    kDpHiddenBit = 0x0010000000000000;*/

    inline void NormalizedBoundaries(Decimal& m_minus, Decimal& m_plus) const {
      UI l_f,   //< Local copy of f.
          l_e;  //< Local copy of e.
      Decimal pl = Decimal((l_f << 1) + 1, l_e - 1).NormalizeBoundary();
      const uint64_t kHiddenBit = ((uint64_t)1) << 52;  //< 0x0010000000000000
      Decimal mi = (f == kHiddenBit) ? Decimal((f << 2) - 1, e - 2)
                                     : Decimal((f << 1) - 1, e - 1);
      mi.f <<= mi.e - pl.e;
      mi.e = pl.e;
      *m_plus = pl;
      *m_minus = mi;
    }

    /* Rounds the Grisu estimation. */
    inline void GrisuRound(char* buffer, char* lsd, uint64_t delta,
                           uint64_t rest, uint64_t ten_kappa, uint64_t wp_w) {
      while (rest < wp_w && (delta - rest) >= ten_kappa &&
             (rest + ten_kappa < wp_w ||  /// closer
              (wp_w - rest) > (rest + ten_kappa - wp_w))) {
        *lsd = (*lsd) - 1;
        rest += ten_kappa;
      }
    }

    inline int32_t CountDecimalDigit32(uint32_t n) {}

    /* Prints the integer portion of the floating-point number. */
    inline char* DigitGen(char* cursor, char* end, const Decimal& w,
                          const Decimal& m_plus, uint64_t delta, int32_t* k) {
      const Decimal one(((uint64_t)1) << -m_plus.e, m_plus.e) const FP wp_w =
          Mp - W;
      uint32_t pow_10, p1 = static_cast<uint32_t>(Mp.f >> -one.e);
      uint64_t p2 = Mp.f & (one.f - 1);

      if (p1 < (pow_10 = 10))
        kappa = 1;
      else if (p1 < (pow_10 = 100))
        kappa = 2;
      else if (p1 < (pow_10 = 1000))
        kappa = 3;
      else if (p1 < (pow_10 = 10000))
        kappa = 4;
      else if (p1 < (pow_10 = 100000))
        kappa = 5;
      else if (p1 < (pow_10 = 1000000))
        kappa = 6;
      else if (p1 < (pow_10 = 10000000))
        kappa = 7;
      else if (p1 < (pow_10 = 100000000))
        kappa = 8;
      else if (p1 < (pow_10 = 1000000000))
        kappa = 9;
      else {
        pow_10 = 10000000000;
        kappa = 10;
      }

      while (kappa > 0) {
        uint32_t d;
        d = p1 / pow_10;
        p1 -= d * pow_10;

        if (d || (cursor != end)) *cursor++ = '0' + static_cast<char>(d);
        --kappa;
        uint64_t tmp = (static_cast<uint64_t>(p1) << -one.e) + p2;
        if (tmp <= delta) {
          *k += kappa;
          uint64_t pow_10 = PuffPow10(kappa);
          GrisuRound(cursor, end, delta, tmp, pow_10 << -one.e, wp_w.f);
          return;
        }
      }

      // kappa = 0
      for (;;) {
        p2 *= 10;
        delta *= 10;
        char d = static_cast<char>(p2 >> -one.e);
        if (d || (cursor != end)) *cursor++ = '0' + d;
        p2 &= one.f - 1;
        --kappa;
        if (p2 < delta) {
          *k += kappa;
          GrisuRound(cursor, end, delta, p2, one.f, wp_w.f * PuffPow10(-kappa));
          return;
        }
      }
      // Load pow_10 from the i-cache.
      switch (kappa) {
        case 1:
          d = p1;
          p1 = 0;
          break;
        case 2:
          pow_10 = 10;
          break;
        case 3:
          pow_10 = 100;
          break;
        case 4:
          pow_10 = 1000;
          break;
        case 5:
          pow_10 = 10000;
          break;
        case 6:
          pow_10 = 100000;
          break;
        case 7:
          pow_10 = 1000000;
          break;
        case 8:
          pow_10 = 10000000;
          break;
        case 9:
          pow_10 = 100000000;
          break;
        case 10:
          pow_10 = 1000000000;
          break;
      }
    }

    inline Decimal PuffCachedPower(int32_t e, int32_t* k) {
      // int32_t k = static_cast<int32_t>(ceil((-61 - e) * 0.30102999566398114))
      // + 374; dk must be positive, so can do ceiling in positive.
      Float dk = (-61 - e) * 0.30102999566398114 + 347;
      int32_t k = static_cast<int32_t>(dk);
      if (k != dk) ++k;

      unsigned index = static_cast<unsigned>((k >> 3) + 1);

      *k = -(-348 + static_cast<int32_t>(
                        index << 3));  // decimal exponent no need lookup table.

      ASSERT(index < 87 / sizeof(kCachedPowers_F[0]));

      uint16_t* exponents = &PuffExponents()[index];
      uint16_t exponent = *exponents;
      uint64_t pow_10 = PuffPow10(exponents);
      return Decimal(pow_10, exponents);
    }

    inline char* Puff(char* begin, char* end, Float value, int32_t* k) {
      const Decimal v(value);
      Decimal minus, plus;
      v.NormalizedBoundaries(&minus, &plus);

      const Decimal c_mk = PuffCachedPower(plus.e, k);

      const Decimal W = v.Normalize() * c_mk;
      Decimal Wp = plus * c_mk;
      Decimal Wm = minus * c_mk;
      Wm.f++;
      Wp.f--;
      return DigitGen(begin, end, W, Wp, Wp.f - Wm.f, k);
    }

    inline void WriteExponent(char* buffer, int32_t k) {
      if (k < 0) {
        *buffer++ = '-';
        k = -k;
      }
      const uint16_t* digits_lut;
      if (k >= 100) {
        *buffer++ = '0' + static_cast<char>(k / 100);
        k %= 100;
        const uint16_t* digits_lut = DigitsLut();
        const char* d = digits_lut + k;
        *buffer++ = d[0];
        *buffer++ = d[1];
      } else if (k >= 10) {
        digits_lut = DigitsLut();
        const char* d = digits_lut + k;
        *buffer++ = d[0];
        *buffer++ = d[1];
      } else
        *buffer++ = '0' + static_cast<char>(k);

      *buffer = '\0';
    }

    inline void Standardize(char* begin, int32_t length, int32_t k) {
      const int32_t kk = length + k;  // 10^(kk-1) <= v < 10^kk

      if (length <= kk && kk <= 21) {  // 1234e7 -> 12340000000
        for (int32_t i = length; i < kk; i++) begin[i] = '0';
        begin[kk] = '.';
        begin[kk + 1] = '0';
        begin[kk + 2] = '\0';
      } else if (0 < kk && kk <= 21) {  // 1234e-2 -> 12.34
        SocketMove(&begin[kk + 1], &begin[kk], length - kk);
        begin[kk] = '.';
        begin[length + 1] = '\0';
      } else if (-6 < kk && kk <= 0) {  // 1234e-6 -> 0.001234
        const int32_t offset = 2 - kk;
        SocketMove(&begin[offset], &begin[0], length);
        begin[0] = '0';
        begin[1] = '.';
        for (int32_t i = 2; i < offset; i++) begin[i] = '0';
        begin[length + offset] = '\0';
      } else if (length == 1) {
        // 1e30
        begin[1] = 'e';
        WriteExponent(kk - 1, &begin[2]);
      } else {
        // 1234e30 -> 1.234e33
        SocketMove(&begin[2], &begin[1], length - 1);
        begin[1] = '.';
        begin[length + 1] = 'e';
        WriteExponent(kk - 1, &begin[length + 2]);
      }
    }
  };

  /* Converts a string-to-float.
  @return nil if there is no number to scan or pointer to the next char after
  the the end of the scanned number upon success.
  @brief Algorithm uses a 32-bit unsigned value to scan the floating-point
  number, which can only have 10 digits max, so the maximum floating-point
  number digit count we can scan is 9 digits long.*/
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
      sign = NanSigned<uint32_t, uint32_t>();
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
           (int32_t)(cursor - begin));

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
    // We have to inline the TextScanUnsigned here in order to detect if there
    // are too many decimals
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
    PRINTF("\nPointed at \'%c\' and found length:%i", *cursor, (int32_t)length);

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
    int32_t bit_shift_count = 0;
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

}  // namespace _

#undef PRINT
#undef PRINTF
#undef PRINT_PRINTED
#undef PRINT_HEADER
#undef PRINT_HEADING
#endif  //< #if INCLUDED_CRABS_TDECIMAL
