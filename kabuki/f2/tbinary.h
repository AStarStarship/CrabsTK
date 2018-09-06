/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/tbinary.h
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
#include "binary.h"

#if SEAM == 1
#include <cstdio>
#include <cstring>

namespace _ {
inline void PrintPuffItoSDebug(uint64_t value) {
  enum { kSize = sizeof(uint64_t) * 8 };

  printf("\n    ");
  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    putchar(c);
    value = value << 1;
  }
  printf(
      "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "\n    6666555555555544444444443333333333222222222211111111110000000000"
      "\n    3210987654321098765432109876543210987654321098765432109876543210"
      "\n    |  |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |"
      "\n    2  1  1  1   1  1  1   1  1  1   1  0  0   0  0  0   0  0  0   0"
      "\n    0  9  8  7   6  5  4   3  2  1   0  9  8   7  6  5   4  3  2   1");
}

inline void PrintPuffItoSDebug(uint32_t value) {
  enum { kSize = sizeof(uint32_t) * 8 };

  printf("\n    ");
  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    putchar(c);
    value = value << 1;
  }

  printf(
      "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "\n    33222222222211111111110000000000"
      "\n    10987654321098765432109876543210"
      "\n    ||  |  |   |  |  |   |  |  |   |"
      "\n    |1  0  0   0  0  0   0  0  0   0"
      "\n    |0  9  8   7  6  5   4  3  2   1");
}

static char* buffer_begin = 0;

inline void PrintPrinted(char* cursor) {
  printf("\n    Printed \"%s\" leaving value:\"%s\"", buffer_begin, cursor);
  char* start = cursor;
  while (*cursor++)
    ;
  printf(":%i", (int)(cursor - start));
}

}  // namespace _

#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PRINTED PrintPrinted(begin, begin, value);

#define PRINT_BINARY \
  Print("\n    ");   \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define PRINT_HEADER                                    \
  buffer_begin = buffer;                                \
  for (int32_t i = 0; i < 10; ++i) *(cursor + i) = 'x'; \
  *(cursor + 21) = 0;                                   \
  char* begin = cursor;                                 \
  char buffer[256];                                     \
  sprintf_s(buffer, 256, "%u", value);                  \
  printf("Expecting %s:%u", buffer, (uint)strlen(buffer));
#define BEGIN_PUFF_ITOS_ALGORITHM \
  putchar('\n');                  \
  for (int32_t i = 80; i > 0; --i) putchar('-')
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_PRINTED ;
#define PRINT_HEADER
#define PRINT_HEADING
#endif

namespace _ {

template <typename Char, typename UI>
Char* PrintHex(Char* cursor, Char* end, UI value) {
  enum { kHexStringLengthSizeMax = sizeof(UI) * 2 + 3 };

  ASSERT(cursor);
  if (cursor + kHexStringLengthSizeMax >= end) return nullptr;

  *cursor++ = '0';
  *cursor++ = 'x';
  for (int num_bits_shift = sizeof(UI) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *cursor++ = HexNibbleToUpperCase((uint8_t)(value >> num_bits_shift));
  }
  *cursor = 0;
  return cursor;
}

template <typename Char, typename T>
Char* PrintBinary(Char* cursor, Char* end, T value) {
  if (cursor + sizeof(uint64_t) * 8 >= end) {
    return nullptr;
  }

  for (int i = 0; i < sizeof(T) * 8; ++i) {
    *cursor++ = (char)('0' + (value >> (sizeof(T) * 8 - 1)));
    value = value << 1;
  }
  *cursor = 0;
  return cursor;
}

/* Utility class for printing strings.
This class only stores the end of buffer pointer and a pointer to the write
begin. It is up the user to store start of buffer pointer and if they would
like to replace the begin with the beginning of buffer pointer when they
are done printing.
*/
template <typename Char = char>
struct TUtf {
  Char *begin,  //< Write begin pointer.
      *end;     //< End of buffer pointer.

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  TUtf(Char* begin, intptr_t size)
      : begin(begin), end(Ptr<Char>(begin, size - 1)) {
    ASSERT(begin);
    ASSERT(ObjSizeIsValid(size, 8));
  }

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  TUtf(Char* begin, Char* end) {}

  /* Clones the other print. */
  TUtf(const TUtf& other)
      : begin(other.begin), end(other.end) {  // Nothing to do here!.
  }

  /* Sets the begin pointer to the new_pointer. */
  inline TUtf& Set(Char* new_pointer) {
    if (!new_pointer) return *this;
    begin = new_pointer;
    return *this;
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(int8_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(uint8_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(int16_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(uint16_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(int32_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(uint32_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(int64_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(uint64_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(float value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(double value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given pointer as hex. */
  inline TUtf& Hex(const void* pointer) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(int8_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(uint8_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(int16_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(uint16_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(int32_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(uint32_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(int64_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(uint64_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(float value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(double value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given pointer as binary. */
  inline TUtf& Binary(const void* pointer) {
    return Set(Binary<Char>(begin, end, value));
  }
};

/* Prints a two decimals to the buffer.
If the SEAM == SEAM_0_0_0 (1), then this function will print debug data.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
inline Char* PrintDecimals(Char* buffer, uint16_t decimal_pair) {
  enum { kSizeBits = sizeof(Char) * 8 };
#if ALIGN_MEMORY
  *buffer = (Char)(digits >> kSizeBits);
  *(buffer + 1) = (Char)(decimal_pair & 0xff);
#else
  *buffer = decimal_pair;
#endif
  PRINT_PRINTED;
  return cursor;
}

/* Prints a single decimal to the buffer.
If the SEAM == SEAM_0_0_0 (1), then this function will print debug data.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
inline Char* PrintDecimal(Char* cursor, Char digit) {
  *reinterpret_cast<uint16_t*>(cursor) = '0' + digit;
  PRINT_PRINTED;
  return cursor;
}

/* Prints a single decimal to the buffer.
If the SEAM == SEAM_0_0_0 (1), then this function will print debug data.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
inline Char* PrintChar(Char* cursor, Char c) {
  *cursor++ = c;
  PRINT_PRINTED;
  return cursor;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char, typename DChar = uint16_t>
inline void PrintDecimals(Char* cursor, Char a, Char b) {
  DChar upper_bits = (DChar)('0' + b) << (sizeof(DChar) * 2);
  *reinterpret_cast<DChar*>(cursor) = ((DChar)('0' + a)) | upper_bits;
  PRINT_PRINTED;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char, typename DChar = uint16_t>
inline void PrintDecimals(Char* cursor, Char a, Char b, Char c) {
  enum { kSizeBits = sizeof(DChar) * 8 };
  DChar upper_bits = ((DChar)('0' + c)) << (2 * kSizeBits);
  upper_bits |= ((DChar)('0' + b)) << kSizeBits;
  *reinterpret_cast<DChar*>(cursor) = ((DChar)('0' + a)) | upper_bits;
  PRINT_PRINTED;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char, typename DChar = uint16_t>
inline void PrintDigits(Char* cursor, Char a, Char b) {
  DChar upper_bits = ((DChar)b) << (sizeof(DChar) * 2);
  *reinterpret_cast<DChar*>(cursor) = ((DChar)a) | upper_bits;
  PRINT_PRINTED;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
Char* PrintNil(Char* cursor) {
  *cursor = 0;
  return cursor;
}

template <typename UI, typename SI>
struct Int {
  SI value;  //< Signed integer value;

  Int(SI value) : value(value) {}

  template <typename UI>
  struct Unsigned {
    UI value;

    Unsigned(UI value) : value(value) {}

    /* Prints the give value to the given buffer as a Unicode string.
    @return Nil upon buffer overflow and a pointer to the nil-term Char upon
    success.
    @param  cursor The begining of the buffer.
    @param  end    The end address of the buffer. */
    template <typename Char = char, typename DChar = uint16_t,
              typename UI = uint64_t>
    static Char* Print(Char* cursor, Char* end, UI value) {
      BEGIN_PUFF_ITOS_ALGORITHM;

      if (!cursor || cursor >= end) return nullptr;

      Char digit;
      uint32_t value_ui4, scalar;
      uint16_t digits1and2, digits3and4, digits5and6, digits7and8;
      const uint16_t* lut =

          do {
        if (value < 10) {
          PRINT("\n    Range:[0, 9] length:1 ");
          if (cursor + 1 >= end) return nullptr;
          return PrintNil<Char>(PrintDecimal<Char>(cursor, (Char)value));
        } else if (value < 100) {
          PRINT("\n    Range:[10, 99] length:2 ");
          if (cursor + 2 >= end) return nullptr;
          PrintDecimals(cursor, kDigits00To99[value]);
          return PrintNil<Char>(cursor + 2);
        } else {
          if ((value >> 10) == 0) {
            if (cursor + 4 >= end) return nullptr;
            digits1and2 = (uint16_t)value;
            digits3and4 = 1000;
            if (digits1and2 >= digits3and4) {
              PRINT("\n    Range:[1000, 1023] length:4");
              digits1and2 -= digits3and4;
              PrintDecimals<Char, DChar>(cursor + 2,
                                         kDigits00To99[digits1and2]);
              PrintDigits<Char, DChar>(cursor + 4, '1', '0');
              PrintNil<Char>(cursor + 4);
            }
            PRINT("\n    Range:[100, 999] length:3");
            digits1and2 = (uint16_t)value;
            digits3and4 = 100;
            digit = (Char)(digits1and2 / digits3and4);
            digits1and2 -= digit * digits3and4;
            PrintDecimal<Char>(cursor, digit);
            PrintDecimals<Char, DChar>(cursor + 1, kDigits00To99[digits1and2]);
            return PrintNil<Char>(cursor + 3);
          } else if ((value >> 13) == 0) {
            digits5and6 = 10000;
            digits1and2 = (uint16_t)value;
            if (digits1and2 >= digits5and6) {
              if (cursor + 5 >= end) return nullptr;
              PRINT("\n    Range:[10000, 16383] length:5");
              cursor = PrintChar(cursor);
              digits1and2 -= digits5and6;
            } else {
              PRINT("\n    Range:[1024, 9999] length:4");
              if (cursor + 4 >= end) return nullptr;
            }
            digits5and6 = 100;
            digits3and4 = digits1and2 / digits5and6;
            digits1and2 -= digits3and4 * digits5and6;
            PrintDecimals<Char, DChar>(cursor, kDigits00To99[digits3and4]);
            PrintDecimals<Char, DChar>(cursor + 2, kDigits00To99[digits1and2]);
            return PrintNil<Char>(cursor + 4);
          } else if (value >> 14) {
            if (value >= 100000) {
              PRINT("\n    Range:[65536, 131071] length:6");
              goto Print6;
            }
            PRINT("\n    Range:[10000, 65535] length:5");
            if (cursor + 5 >= end) return nullptr;
            value_ui4 = (uint32_t)value;
            digits5and6 = 10000;
            digit = (uint8_t)(value_ui4 / digits5and6);
            value_ui4 -= digits5and6 * digit;
            PrintDecimal<Char>(cursor, digits);
            digits5and6 = 100;
            digits3and4 = ((uint16_t)value_ui4) / digits5and6;
            digits1and2 -= digits3and4 * digits5and6;
            PrintDecimals<Char, DChar>(cursor + 1, kDigits00To99[digits3and4]);
            PrintDecimals<Char, DChar>(cursor + 3, kDigits00To99[digits1and2]);
            return PrintNil<Char>(cursor + 5);
          } else {
            int jump;  //< Jump table index for lengths 6, 7, 8, and 9 or more.
            if ((value >> 20) == 0) {
              jump = 0;
              comparator = 1000000;
              if (value >= comparator) {
                PRINT("\n    Range:[100000, 1048575] length:7");
                if (cursor + 7 >= end) return nullptr;
                cursor = PrintChar(cursor);
                value -= comparator;
              } else {
                PRINT("\n    Range:[131072, 999999] length:6");
              Print6:
                if (cursor + 6 >= end) return nullptr;
              }
              value_ui4 = (uint32_t)value;
              scalar = 10000;
            } else if ((value >> 24) == 0) {
              jump = 1;
              comparator = 10000000;
              if (value_ui4 >= comparator) goto Print8;
              PRINT("\n    Range:[1048576, 9999999] length:7");
              if (cursor + 7 >= end) return nullptr;
              scalar = 10000;
            } else {
              PRINTF("\n    Printing 8 decimals...");
              if (cursor + 8 >= end) return nullptr;

              comparator = 100000000;  // 10^8
              scalar = value / comparator;
              value -= scalar * comparator;
              scalar = 10000;  // 10^4

              if (value < comparator) {
              Print8:
                jump = 2;
                value_ui4 = (uint32_t)value;
                digits5and6 = (uint16_t)(value_ui4 / scalar);
              } else {
                jump = 3;
                value_ui4 = (uint32_t)(value / comparator);
                digits5and6 = (uint16_t)(value_ui4 / scalar);
              }
              if (cursor + 8 >= end) return nullptr;
            }
            digits1and2 = value_ui4 - scalar * digits5and6;
            digits7and8 = digits5and6 / 100;
            digits3and4 = digits1and2 / 100;
            digits5and6 -= 100 * digits7and8;
            digits1and2 -= 100 * digits3and4;
            PrintDecimals<Char, DChar>(cursor + 2, kDigits00To99[digits5and6]);
            PrintDecimals<Char, DChar>(cursor + 4, kDigits00To99[digits3and4]);
            PrintDecimals<Char, DChar>(cursor + 6, kDigits00To99[digits1and2]);
            switch (jump) {
              case 0: {  // Length 6
                return PrintNil<Char>(cursor + 5);
              }
              case 1: {  // Length 7
                PrintDecimal<Char>(cursor + 5, (Char)digits7and8);
                return PrintNil<Char>(cursor + 6);
              }
              case 2: {  // Length 8
                PrintDecimals<Char, DChar>(cursor + 6,
                                           kDigits00To99[digits7and8]);
                return PrintNil<Char>(cursor + 7);
              }
              default: {  // 9 or more
                PrintDecimals<Char, DChar>(cursor + 7,
                                           kDigits00To99[digits7and8]);
                cursor += 8;
                PRINTF("\n    value is now %llu");
              }
            }
          }
        }
      }
      while (value > comparator)
        ;
    }

    /* Scans the given buffer for an unsigned integer (UI).
    @return Nil if there is no UI to scan.
    @param begin The beginning of the buffer.
    @param result The UI to write the scanned UI. */
    template <typename Char, typename UI = uint>
    const Char* Scan(const Char* begin, UI& result) {
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
  };
  /* Prints the give value to the given buffer as a Unicode string.
  @return Nil upon buffer overflow and a pointer to the nil-term Char upon
  success.
  @param  cursor The begining of the buffer.
  @param  end    The end address of the buffer. */
  template <typename Char = char, typename DChar = uint16_t,
            typename UI = uint64_t>
  inline static Char* Print(Char* cursor, Char* end, UI value) {
    return Unsigned::Print<Char, DChar>(cursor, end, value);
  }

  /* Writes the give value to the given buffer as an ASCII string.
  @return Nil upon buffer overflow and a pointer to the nil-term Char upon
  success.
  @param  print The Utf& to print to.
  @param value The value to write. */
  template <typename Char = char, typename DChar = uint16_t,
            typename UI = uint64_t, typename SI = int64_t>
  inline static Char* Print(Char* begin, Char* end, SI value) {
    if (value >= 0) return Print<Char, DChar>(begin, end, (UI)value);
    *begin++ = '-';
    return Print<Char, DChar, UI>(begin, end, (UI)(-value));
  }
  /* Scans the given buffer for an unsigned integer (UI).
  @return Nil if there is no UI to scan.
  @param begin The beginning of the buffer.
  @param result The UI to write the scanned UI. */
  template <typename Char, typename UI = uint>
  const Char* ScanUnsigned(const Char* begin, UI& result) {
    Unsigned::Scan<Char, UI>, begin, UI& result);
  }
};

/* Prints the give value to the given buffer as a Unicode string.
@return Nil upon buffer overflow and a pointer to the nil-term Char upon
success.
@param cursor The buffer to print to.
@param size   The size of the buffer to print to.
@param value  The value to print. */
template <typename Char = char, typename DChar = uint16_t,
          typename UI = uint64_t>
inline Char* Print(Char* cursor, intptr_t size, UI value) {
  return Int::Unsigned::Print<Char, DChar, UI>(cursor, cursor + size, value);
}

/* Writes the give value to the given buffer as an ASCII string.
@return Nil upon buffer overflow and a pointer to the nil-term Char upon
success.
@param  print The Utf& to print to.
@param value The value to write. */
template <typename Char = char, typename DChar = uint16_t,
          typename UI = uint64_t, typename SI = int64_t>
Char* Print(Char* begin, Char* end, SI value) {
  if (value >= 0) return Print<Char, DChar>(begin, end, (UI)value);
  *begin++ = '-';
  return Int::Print<Char, DChar, UI>(begin, end, (UI)(-value));
}

/* Writes the give value to the given buffer as an ASCII string.
@return Nil upon buffer overflow and a pointer to the nil-term Char upon
success.
@param  print The Utf& to print to.
@param value The value to write. */
template <typename Char = char, typename DChar = uint16_t,
          typename UI = uint64_t>
inline Char* Print(Char* begin, intptr_t size, UI value) {
  return Print<Char, DChar, UI>(begin, begin + size, value);
}

/* Checks if the given char is a digit of a number.
@return True if it is a digit. */
template <typename Char = char>
bool IsDigit(Char c) {
  return (c >= '0') && (c <= '9');
}

/* Scans the given buffer for an unsigned integer (UI).
@return Nil if there is no UI to scan.
@param begin The beginning of the buffer.
@param result The UI to write the scanned UI. */
template <typename Char, typename UI = uint>
inline const Char* ScanUnsigned(const Char* begin, UI& result) {
  return Int::Unsigned::ScanUnsgined<Char, UI>(begin, result);
}

/* Scans the given buffer for an Signed Integer (SI).
@return Nil if there is no UI to scan.
@param begin The beginning of the buffer.
@param result The SI to write the scanned SI. */
template <typename Char, typename SI = int32_t, typename UI = uint>
const Char* ScanSigned(const Char* begin, SI& result) {
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

/* A decimal number in floating-point format. */
template <typename Float, typename UI>
class Binary {
 public:
  enum {
    kSize = sizeof(Float),
    kSizeBits = kSize * 8,
    kStringLengthMax = 24,
    kExponentSizeBits =
        (sizeof(Float) == 2)
            ? 5
            : (sizeof(Float) == 4) ? 8 : (sizeof(Float) == 8) ? 11 : 15,
    kCoefficientSize = kSizeBits - kExponentSizeBits - 1,
    kExponentMaskUnshifted =
        (~((uint32_t)0)) >> (kSizeBits - kExponentSizeBits),
    kExponentBias = kExponentMaskUnshifted + kCoefficientSize,
    kExponentMin = -kExponentBias,
  };

  inline static UI Coefficient(UI decimal) {
    return (decimal << (kExponentSizeBits + 1)) >> (kExponentSizeBits + 1);
  }

  inline static UI Exponent(UI decimal) {
    return (decimal << (kExponentSizeBits + 1)) >> (kExponentSizeBits + 1);
  }

  /* Constructs an uninitized floating-point number. */
  Binary() {}

  /* Converts a Float to a Binary*/
  Binary(Float binary) {
    UI ui = *reinterpret_cast<UI*>(&binary);
    uint32_t biased_e = ui << 1;  //< Get rid of sign bit.
    // Get rid of the integral portion.
    biased_e = biased_e >> (kSizeBits - kExponentSizeBits);
    // Get rid of the sign and exponent.
    uint64_t coefficient = Coefficient<UI>(word);
    if (biased_e != 0) {
      f = coefficient + (((UI)1) << kExponentSizeBits);
      e = biased_e - kExponentBias;
    } else {
      f = coefficient;
      e = kExponentMin + 1;
    }
  }

  Binary(UI f, int32_t e) : f(f), e(e) {}

  template <typename Char = char, typename DChar = uint16_t>
  inline Char* Print(Char* begin, Char* end, Float value) {
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
      begin[0] = (f >> (sizeof(UI > *8 - 1))) ? '-' : '+';
      begin[1] = 'i';
      begin[2] = 'n';
      begin[3] = 'f';
      begin[4] = 0;
      return begin + 5;
    }

    if (value == 0) {
      return Print<Char, DChar>(begin, end, '0', '.', '0');
    }
    if (value < 0) {
      *begin++ = '-';
      value = -value;
    }
    int32_t length, k;
    char* cursor = Print<Char, DChar>(begin, end, value, &length, k);
    Standardize(begin, length, k);
    return cursor;
  }
  /* Non-working algoihrm DOES NOT converts a string-to-float.
  @return nil if there is no number to scan or pointer to the next char after
  the the end of the scanned number upon success.
  @brief Algorithm uses a 32-bit unsigned value to scan the floating-point
  number, which can only have 10 digits max, so the maximum floating-point
  number digit count we can scan is 9 digits long.*/
  template <typename Char = char>
  const Char* Scan(const Char* begin, Float& result) {
    ASSERT(begin);
    PRINTF("\n\nScanning float:%s", begin);

    enum {
      kCharCountMax = 9,  // < (1 + [p*log_10(2)], where p = 32
    };

    static const uint32_t kIEEE754Pow10[] = {
        10,      100,      1000,      10000,      100000,
        1000000, 10000000, 100000000, 1000000000, 1000000001};

    uint32_t integer,  //< Integer portion in Binary.
        sign,          //< Sign in Binary32 format.
        ui_value,      //< Unsigned value.
        pow_10;        //< Power of 10 for ocnverting integers.

    // Scan sign of number:

    if (*begin == '-') {
      sign = NaNSigned<uint32_t, uint32_t>();
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
    ui_value = (uint32_t)(c - '0');
    pow_10 = 1;

    while (cursor >= begin) {
      c = *cursor--;
      pow_10 *= 10;
      uint32_t new_value = ui_value + pow_10 * (c - '0');
      if (new_value < ui_value) return nullptr;
      ui_value = new_value;
      PRINTF("\nvalue:%u", (uint)ui_value);
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
    // We have to inline the ScanUnsigned here in order to detect if there
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
    ui_value = (uint32_t)(c - '0'), pow_10 = 1;

    // Then iterate through the rest in a loop.
    while (cursor >= begin) {
      c = *cursor--;
      pow_10 *= 10;
      uint32_t new_value = ui_value + pow_10 * (c - '0');
      if (new_value < ui_value) {
        PRINTF("\nUnsigned wrap-around!");
        return nullptr;
      }
      ui_value = new_value;
      PRINTF("\nFound integer_value:%u", (uint)ui_value);
    }
    PRINTF("\nFound integer_value:%u", (uint)ui_value);

    PRINTF("\nConverting bit_pattern backwarards:");

    // Convert decimals to base 2 by multiply in a loop the intger value is
    // greater than one then subtract the equivalent of one until the value
    // is zero.

    uint32_t one = kIEEE754Pow10[length - 1], bit_pattern = 0;
    PRINT('\n');
    int32_t bit_shift_count = 0;
    while ((ui_value != 0) && (++bit_shift_count < 24)) {
      ui_value = ui_value << 1;  //< << 1 to * 2
      if (ui_value >= one) {
        bit_pattern = (bit_pattern << 1) | 1;
        ui_value -= one;
        PRINT('1');
      } else {
        bit_pattern = bit_pattern << 1;
        PRINT('0');
      }
    }
    PRINTF("'b0");
    PRINT_FLOAT_BINARY(integer, ui_value, length);
    // Now check for the exponenet.

    ui_value |= integer << length;

    PRINTF("\nNormalizing bits...");

    if (c != 'e' && c != 'E') {
      PRINTF("\nNo \'e\' or \'E\' found.");
      result = sign | FloatNormalize<float, uint32_t>(integer);
      return end;
    }

    // @todo This is no doubt optimizable, not sure how much it would help
    // though.
    int32_t signed_value;
    begin = ScanSigned<Char, int32_t, uint32_t>(end, signed_value);
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

 private:
  UI f;
  int32_t e;

  Binary NormalizeBoundary() const {
    int = RSB(0);
#if defined(_MSC_VER) && defined(_M_AMD64)
    unsigned long index;  //< This is Microsoft's fault.
    _BitScanReverse64(&index, f);
    return Binary(f << (63 - index), e - (63 - index));
#else
    Binary res = *this;
    while (!(res.f & (kDpHiddenBit << 1))) {
      res.f <<= 1;
      --res.e;
    }
    res.f <<= (kDiySignificandSize - kCoefficientSize - 2);
    res.e = res.e - (kDiySignificandSize - kCoefficientSize - 2);
    return res;
#endif
  }
  /*
  static const uint64_t  // kDpExponentMask = 0x7FF0000000000000,
  kDpSignificandMask = 0x000FFFFFFFFFFFFF,
  kDpHiddenBit = 0x0010000000000000;*/

  inline void NormalizedBoundaries(Binary& m_minus, Binary& m_plus) const {
    UI l_f,   //< Local copy of f.
        l_e;  //< Local copy of e.
    Binary pl = Binary((l_f << 1) + 1, l_e - 1).NormalizeBoundary();
    const uint64_t kHiddenBit = ((uint64_t)1) << 52;  //< 0x0010000000000000
    Binary mi = (f == kHiddenBit) ? Binary((l_f << 2) - 1, e - 2)
                                  : Binary((l_f << 1) - 1, e - 1);
    mi.f <<= mi.e - pl.e;
    mi.e = pl.e;
    *m_plus = pl;
    *m_minus = mi;
  }

  /* Rounds the Grisu estimation closer to the inside of the squeeze. */
  template <typename Char>
  inline void Round(Char& lsd, uint64_t delta, uint64_t rest,
                    uint64_t ten_kappa, uint64_t wp_w) {
    while (rest < wp_w && (delta - rest) >= ten_kappa &&
           (rest + ten_kappa < wp_w ||  /// closer
            (wp_w - rest) > (rest + ten_kappa - wp_w))) {
      --lsd;
      rest += ten_kappa;
    }
  }

  /* Prints the integer portion of the floating-point number. */
  template <typename Char>
  inline Char* PrintDecimals(Char* cursor, Char* end, const Binary& w,
                             const Binary& m_plus, uint64_t delta, int32_t& k) {
    Binary one(((uint64_t)1) << -m_plus.e, m_plus.e), wp_w = m_plus - w;
    uint32_t pow_10, p_1 = static_cast<uint32_t>(m_plus.f >> -one.e);
    uint64_t p_2 = m_plus.f & (one.f - 1);

    if (p_1 < (pow_10 = 10)) {
      kappa = 1;
    } else if (p_1 < (pow_10 = 100)) {
      kappa = 2;
    } else {
      if ((value >> 10) == 0) {
        kappa = 3;
        pow_10 = 1000;
      } else if (!(value >> 13)) {
        kappa = 4;
        pow_10 = 10000;
      } else if (!(value >> 17)) {
        kappa = 5;
        pow_10 = 100000;
      } else if (!(value >> 20)) {
        kappa = 6;
        pow_10 = 1000000;
      } else if (!(value >> 24)) {
        kappa = 7;
        pow_10 = 10000000;
      } else if (!(value >> 27)) {
        kappa = 8;
        pow_10 = 100000000;
      } else if (!(value >> 30)) {
        kappa = 9;
        pow_10 = 1000000000;
      } else {
        kappa = 10;
        pow_10 = 10000000000;
      }
      if (p_1 >= pow_10) {
        ++kappa;
        pow_10 *= 10;
      }
    }
    while (kappa > 0) {
      uint32_t d;
      d = p_1 / pow_10;
      p_1 -= d * pow_10;

      if (cursor >= end) return nullptr;

      if (d) cursor = PrintDecimal<Char>(cursor, d);

      --kappa;
      UI tmp = (static_cast<uint64_t>(p_1) << -one.e) + p_2;

      if (tmp <= delta) {
        *k += kappa;
        Round(delta, tmp, IEEE754Pow10(kappa) << -one.e, wp_w.f);
        return;
      }
    }

    for (;;) {  // kappa = 0
      p_2 *= 10;
      delta *= 10;
      char d = static_cast<char>(p_2 >> -one.e);
      if (curosr >= end) return nullptr;
      if (d) *cursor++ = '0' + d;
      p_2 &= one.f - 1;
      --kappa;
      if (p_2 < delta) {
        *k += kappa;
        Round(delta, p_2, one.f, wp_w.f * IEEE754Pow10(-kappa));
        return;
      }
    }

    // Load integer pow_10 from the i-cache.
    switch (kappa) {
      case 1:
        d = p_1;
        p_1 = 0;
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

  inline Binary IEEE754Pow10(int32_t e, int32_t& k) {
    // int32_t k = static_cast<int32_t>(ceil((-61 - e) *
    // 0.30102999566398114))

    // + 374; dk must be positive to perform ceiling function on positive
    // values.
    Float dk = (-61 - e) * 0.30102999566398114 + 347;
    int32_t k = static_cast<int32_t>(dk);
    if (k != dk) ++k;

    uint32_t index = static_cast<uint32_t>((k >> 3) + 1);

    k = -(-348 + static_cast<int32_t>(index << 3));
    // decimal exponent no need lookup table.

    ASSERT(index < 87);

    // Save exponents pointer and offset to avoid creating base pointer again.
    uint16_t* exponents = &Exponents()[index];
    return Binary(IEEE754Pow10(exponents), *exponents);
  }

  template <typename Char>
  inline Char* Print(Char* begin, Char* end, Float value, int32_t& k) {
    const Binary v(value);
    Binary minus, plus;
    v.NormalizedBoundaries(&minus, &plus);

    const Binary c_mk = IEEE754Pow10(plus.e, k);

    const Binary W = v.Normalize() * c_mk;
    Binary w_plus = plus * c_mk, w_minus = minus * c_mk;
    w_minus.f++;
    w_plus.f--;
    return PrintDecimals<Char>(begin, end, W, w_plus, w_plus.f - w_minus.f, k);
  }

  template <typename Char = char, typename DChar = uint16_t>
  inline void Standardize(Char* begin, Char* end, int32_t length, int32_t k) {
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
      begin[length + offset] = 0;
    } else if (length == 1) {
      // 1e30
      begin[1] = 'e';
      Print<Char, DChar>(begin + 2, end, kk - 1);
    } else {
      // 1234e30 -> 1.234e33
      SocketMove(&begin[2], &begin[1], length - 1);
      begin[1] = '.';
      begin[length + 1] = 'e';
      Print<Char, DChar>(length + 2, end, kk - 1);
    }
  }
};

// using Binary16 = Binary<half, uint32_t>; //< Comming soon.
using Binary32 = Binary<float, uint32_t>;
using Binary64 = Binary<double, uint64_t>;
// using Binary128 = Binary<quad, uint128_t>;
//< Comming soon but not in Visual-C++ due to lack of 128-bit integer support.

}  // namespace _

#undef PRINT
#undef PRINTF
#undef PRINT_PRINTED
#undef PRINT_HEADER
#undef PRINT_HEADING
#undef BEGIN_PUFF_ITOS_ALGORITHM
#endif  //< #if INCLUDED_CRABS_TDECIMAL

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, const Char* string) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, string));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, Char c) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, c));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint8_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, int16_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint16_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, int32_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint32_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, int64_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint64_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, float value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
DLL _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, double value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}
