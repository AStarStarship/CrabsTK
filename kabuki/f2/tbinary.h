/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/tbinary.h
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

#ifndef INCLUDED_KABUKI_F2_TBINARY
#define INCLUDED_KABUKI_F2_TBINARY 1

#include "binary.h"

#include "00\header.h"
namespace _ {

inline void PrintDigits(char* cursor, uint16_t digits) {}

/* Scrolls over to the next double quote mark.
@warning This function is only safe to use on ROM strings with a nil-term
char. */
template <typename Char = char>
inline const Char* StringEnd(const Char* cursor, Char delimiter = 0) {
  ASSERT(cursor);
  while (*cursor++ != delimiter)
    ;
  return cursor - 1;
}

/* Gets the length of the given char.
@return  Returns -1 if the text char is nil.
@warning This function is only safe to use on ROM strings with a nil-term
char. */
template <typename Char, typename I = int>
I StringLength(const Char* cursor) {
  ASSERT(cursor);
  return (I)(StringEnd<Char>(cursor) - cursor);
}

/* Prints a Unicode string to the given buffer.
 @return Nil upon failure or a pointer to the nil-term Char upon success.
 @param  cursor    The beginning of the buffer.
 @param  end       The last byte in the buffer.
 @param  string    The string to print.
 @param  delimiter The delimiter to print (usually nil).
 @desc   This algorithm is designed to fail if the buffer is not a valid buffer
 with one or more bytes in it, or if string is nil. */
template <typename Char = char>
Char* Print(Char* cursor, Char* end, const Char* string, Char delimiter = 0) {
  ASSERT(cursor);
  ASSERT(string);

  if (cursor >= end) return nullptr;

  char c = *string++;
  while (c) {
    *cursor++ = c;
    if (cursor >= end) return nullptr;
    c = *string++;
  }
  *cursor = delimiter;
  return cursor;
}

/* Prints a Unicode string to the given buffer.
@return Nil upon failure or a pointer to the nil-term Char upon success.
@param  cursor    The beginning of the buffer.
@param  size      The size of the buffer in Char(s).
@param  string    The string to print.
@param  delimiter The delimiter to print (usually nil).
@desc   This algorithm is designed to fail if the buffer is not a valid buffer
with one or more bytes in it, or if string is nil. */
template <typename Char = char>
Char* Print(Char* cursor, intptr_t size, const Char* string,
            Char delimiter = 0) {
  return Print<Char>(cursor, cursor + size - 1, string, delimiter);
}

/* Prints a Unicode Char to the given buffer.
@return Nil upon failure or a pointer to the nil-term Char upon success.
@param  cursor    The beginning of the buffer.
@param  end       The last byte in the buffer.
@param  character The Char to print.
@desc   This algorithm is designed to fail if the buffer is not a valid buffer
with one or more bytes in it. */
template <typename Char = char>
Char* Print(Char* cursor, Char* end, Char character) {
  ASSERT(cursor);
  ASSERT(cursor < end);

  if (cursor + 1 >= end) return nullptr;

  *cursor++ = character;
  *cursor = 0;
  return cursor;
}

/* Prints a Unicode Char to the given buffer.
@return Nil upon failure or a pointer to the nil-term Char upon success.
@param  cursor    The beginning of the buffer.
@param  size      The size of the buffer in Char(s).
@param  string    The string to print.
@desc   This algorithm is designed to fail if the buffer is not a valid buffer
with one or more bytes in it. */
template <typename Char = char>
Char* Print(Char* cursor, intptr_t size, Char character) {
  return Print<Char>(cursor, cursor + size, character);
}

/* Prints a Unicode Char to the given buffer.
@return Nil upon failure or a pointer to the nil-term Char upon success.
@param  cursor    The beginning of the buffer.
@param  size      The size of the buffer in Char(s).
@param  character The Char to print.
@desc   This algorithm is designed to fail if the buffer is not a valid buffer
with one or more bytes in it. */
template <typename Char = char>
Char* PrintChar(Char* cursor, intptr_t size, Char character) {
  return PrintChar<Char>(cursor, cursor + size, character);
}

/* Prints a hex value to the Console. */
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

/* Checks if the given character is whitespace. */
template <typename Char = char>
inline bool IsWhitespace(Char character) {
  return character <= ' ';
}

template <typename Char = char>
int StringCompare(const Char* string_a, const Char* string_b,
                  Char delimiter = 0) {
  int a, b, result;
  if (!string_a) {
    if (!string_b) return 0;
    return *string_a;
  }
  if (!string_b) return 0 - *string_a;

  PRINTF("\nComparing \"%s\" to \"%s\"", string_a, string_b);
  a = *string_a;
  b = *string_b;
  if (!a) {
    if (!a) return 0;
    return b;
  }
  if (!b) {
    if (!a) return 0;  //< I like !t code rather than !c code. :-)
    return 0 - a;
  }
  // string_b SHOULD be a nil-terminated string without whitespace.
  while (b) {
    result = b - a;
    PRINTF("\nb - a = %i - %i = %i", b, a, result);
    if (result) {
      PRINTF(" is not a hit.");
      return result;
    }
    if (a <= (int)delimiter) {
      PRINTF(" is a partial match but a reached a delimiter first.");
      return result;
    }
    ++string_a;
    ++string_b;
    a = *string_a;
    b = *string_b;
  }
  if (a > (int)delimiter) {
    PRINTF(" is only a partial match but b reached a delimiter first.");
    return b - a;
  }
  return 0;
}

template <typename Char = char>
int StringCompare(const Char* cursor, const Char* end, const Char* query) {
  Char a = *cursor, b = *query;
  int result;

  if (!cursor) {
    if (!query) return 0;
    a = 0;
    b = *query;
    return b - a;
  }
  if (!query) {
    a = *cursor;
    b = 0;
    return b - a;
  }
  if (cursor > end) return *query;

  // Algorithm combines loops for better performance.
  a = *cursor;
  b = *query;
  if (!a) {
    if (!b) return 0;
    return b;
  }
  if (!b) {
    if (!a) return 0;
    return 0 - a;
  }
  // text SHOULD be a nil-terminated string without whitespace.
  while (b) {
    result = b - a;
    PRINTF("\nb - a = %c - %c = %i", b, a, result);
    if (result) {
      PRINTF(" is not a hit.");
      return result;
    }
    if (!a) {
      PRINTF(" is a partial match but !a.");
      return result;
    }
    if (++cursor >= end) {
      PRINTF(" but buffer overflowed!");
      return result;
    }
    ++query;
    a = *cursor;
    b = *query;
  }
  if (a && !IsWhitespace<Char>(a)) {
    PRINTF(" is only a partial match but found %s", (a ? "a" : "space"));
    return b - a;
  }
  PRINTF(" is a match!");
  return 0;
}

template <typename Char = char>
const Char* StringFind(const Char* cursor, const Char* query) {
  ASSERT(cursor);
  ASSERT(query);

  Char string = *cursor,  //< Current cursor Char.
      t = *query,         //< Current query Char.
      c = t;              //< The first Char of the query we're searching for.
  if (c == 0)             //< We're not allowing empty queries.
    return nullptr;
  const Char *start_of_query, *begin = cursor;
  query = TextSkipSpaces(query);

  // Scroll through each Char and match it to the query Char.
  while (string) {
    if (string == c) {  // The first Char matches:
                        // Setup to compare the strings;
      start_of_query = cursor;
      begin = query;
      t = c;
      // check the rest of the Char:
      while (string == t) {
        string = *(++cursor);
        t = *(++begin);
        if (t == 0)  // Once we've reached the delimiter it's a match!
          return start_of_query;
        if (!string)  // We've reached the end of Char without a hit.
          return nullptr;
      }
    }
    // The Char did not match so repeat the process for each Char.
    string = *(++cursor);
    t = *(++begin);
  }
  // If we haven't found it by now it's not in the cursor.
  return nullptr;
}

/* Prints a two decimals to the buffer.
If the SEAM == SEAM_0_0_0 (1), then this function will print debug data.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
inline Char* PrintDecimals(Char* buffer, uint16_t value) {
  enum { kSizeBits = sizeof(Char) * 8 };
#if ALIGN_MEMORY
  *buffer = (Char)(value >> kSizeBits);
  *(buffer + 1) = (Char)(value & 0xff);
#else
  *buffer = value;
#endif
  PRINT_PRINTED;
  return buffer;
}

/* Prints a single decimal to the buffer.
If the SEAM == SEAM_0_0_0 (1), then this function will print debug data.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
inline Char* PrintDecimal(Char* buffer, Char value) {
  *reinterpret_cast<uint16_t*>(buffer) = '0' + value;
  PRINT_PRINTED;
  return buffer;
}

/* Prints a single decimal to the buffer.
If the SEAM == SEAM_0_0_0 (1), then this function will print debug data.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
inline Char* PrintChar(Char* buffer, Char value) {
  *buffer++ = value;
  PRINT_PRINTED;
  return buffer;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char, typename DChar = uint16_t>
inline void PrintDecimals(Char* buffer, Char a, Char b) {
  DChar value = (DChar)('0' + b) << (sizeof(DChar) * 2);
  value |= ((DChar)('0' + a));
  *reinterpret_cast<DChar*>(buffer) = value;
  PRINT_PRINTED;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char, typename DChar = uint16_t>
inline void PrintDecimals(Char* buffer, Char a, Char b, Char c) {
  enum { kSizeBits = sizeof(DChar) * 8 };
  DChar value = ((DChar)('0' + c)) << (2 * kSizeBits);
  value |= ((DChar)('0' + b)) << kSizeBits;
  *reinterpret_cast<DChar*>(buffer) = ((DChar)('0' + a)) | value;
  PRINT_PRINTED;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char, typename DChar = uint16_t>
inline void PrintDigits(Char* buffer, Char a, Char b) {
  DChar value = ((DChar)b) << (sizeof(DChar) * 2);
  value |= ((DChar)a);
  *reinterpret_cast<DChar*>(buffer) = value;
  PRINT_PRINTED;
}

/* Prints two chars to the console.
@warning This function DOES NOT do any error checking! */
template <typename Char = char>
Char* PrintNil(Char* cursor) {
  *cursor = 0;
  return cursor;
}

/* Checks if the given char is a digit of a number.
@return True if it is a digit. */
template <typename Char = char>
bool IsDigit(Char c) {
  return (c >= '0') && (c <= '9');
}

/* Prints the give value to the given buffer as a Unicode string.
@return Nil upon buffer overflow and a pointer to the nil-term Char upon
success.
@param  cursor The beginning of the buffer.
@param  end    The end address of the buffer. */
template <typename UI = uint64_t, typename Char = char,
          typename DChar = uint16_t>
Char* Print(Char* cursor, Char* end, UI value) {
  BEGIN_ITOS_ALGORITHM;

  if (!cursor || cursor >= end) return nullptr;

  Char digit;
  uint32_t value_ui4, scalar, comparator;
  uint16_t digits1and2, digits3and4, digits5and6, digits7and8;
  const uint16_t* lut = IEEE754LUT();

  do {
    if (value < 10) {
      PRINT("\n    Range:[0, 9] length:1 ");
      if (cursor + 1 >= end) return nullptr;
      return PrintNil<Char>(PrintDecimal<Char>(cursor, (Char)value));
    } else if (value < 100) {
      PRINT("\n    Range:[10, 99] length:2 ");
      if (cursor + 2 >= end) return nullptr;
      PrintDecimals(cursor, lut[value]);
      return PrintNil<Char>(cursor + 2);
    } else {
      if ((value >> 10) == 0) {
        if (cursor + 4 >= end) return nullptr;
        digits1and2 = (uint16_t)value;
        digits3and4 = 1000;
        if (digits1and2 >= digits3and4) {
          PRINT("\n    Range:[1000, 1023] length:4");
          digits1and2 -= digits3and4;
          PrintDecimals<Char, DChar>(cursor + 2, lut[digits1and2]);
          PrintDigits<Char, DChar>(cursor + 4, '1', '0');
          PrintNil<Char>(cursor + 4);
        }
        PRINT("\n    Range:[100, 999] length:3");
        digits1and2 = (uint16_t)value;
        digits3and4 = 100;
        digit = (Char)(digits1and2 / digits3and4);
        digits1and2 -= digit * digits3and4;
        PrintDecimal<Char>(cursor, digit);
        PrintDecimals<Char, DChar>(cursor + 1, lut[digits1and2]);
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
        PrintDecimals<Char, DChar>(cursor, lut[digits3and4]);
        PrintDecimals<Char, DChar>(cursor + 2, lut[digits1and2]);
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
        PrintDecimal<Char>(cursor, digit);
        digits5and6 = 100;
        digits3and4 = ((uint16_t)value_ui4) / digits5and6;
        digits1and2 -= digits3and4 * digits5and6;
        PrintDecimals<Char, DChar>(cursor + 1, lut[digits3and4]);
        PrintDecimals<Char, DChar>(cursor + 3, lut[digits1and2]);
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
        PrintDecimals<Char, DChar>(cursor + 2, lut[digits5and6]);
        PrintDecimals<Char, DChar>(cursor + 4, lut[digits3and4]);
        PrintDecimals<Char, DChar>(cursor + 6, lut[digits1and2]);
        switch (jump) {
          case 0: {  // Length 6
            return PrintNil<Char>(cursor + 5);
          }
          case 1: {  // Length 7
            PrintDecimal<Char>(cursor + 5, (Char)digits7and8);
            return PrintNil<Char>(cursor + 6);
          }
          case 2: {  // Length 8
            PrintDecimals<Char, DChar>(cursor + 6, lut[digits7and8]);
            return PrintNil<Char>(cursor + 7);
          }
          default: {  // 9 or more
            PrintDecimals<Char, DChar>(cursor + 7, lut[digits7and8]);
            cursor += 8;
            PRINTF("\n    value is now %llu");
          }
        }
      }
    }
  } while (value > comparator);
  return nullptr;  //< Unreachable.
}

inline uint8_t Negative(uint8_t value) { return (uint8_t)(-((int8_t)value)); }

inline uint16_t Negative(uint16_t value) {
  return (uint16_t)(-((int16_t)value));
}

inline uint32_t Negative(uint32_t value) {
  return (uint32_t)(-((int32_t)value));
}

inline uint64_t Negative(uint64_t value) {
  return (uint64_t)(-((int64_t)value));
}

/* Writes the give value to the given buffer as an ASCII string.
@return Nil upon buffer overflow and a pointer to the nil-term Char upon
success.
@param  print The text formatter to print to.
@param value The value to write. */
template <typename SI = uint64_t, typename Char = char,
          typename DChar = uint16_t>
inline Char* Print(Char* buffer, Char* end, SI value) {
  if (value >= 0) return Print<Char, DChar>(buffer, end, Unsigned(value));
  *buffer++ = '-';
  return Print<UI, Char, DChar>(buffer, end, Negative(value));
}

/* Writes the give value to the given buffer as an ASCII string.
@return Nil upon buffer overflow and a pointer to the nil-term Char upon
success.
@param  print The text formatter to print to.
@param value The value to write. */
template <typename UI = uint64_t, typename SI = int64_t, typename Char = char,
          typename DChar = uint16_t>
inline Char* Print(Char* buffer, intptr_t size, SI value) {
  return Print<UI, SI, Char, DChar>(buffer, buffer + size, value);
}

/* Scans the given buffer for an unsigned integer (UI).
@return Nil if there is no UI to scan.
@param buffer The beginning of the buffer.
@param result The UI to write the scanned UI. */
template <typename UI, typename Char = char>
const Char* Scan(const Char* buffer, UI& result) {
  ASSERT(buffer);
  PRINTF("\nScanning unsigned value:%s", buffer);
  const Char* cursor = buffer;
  Char c = *cursor++;
  if (!IsDigit<Char>(c)) return nullptr;

  // Find length:
  c = *cursor++;
  while (IsDigit<Char>(c)) c = *cursor++;
  const Char* end = cursor;  // Store end to return.
  cursor -= 2;
  PRINTF("\nPointed at \'%c\' and found length:%i", *cursor,
         (int32_t)(cursor - buffer));

  c = *cursor--;
  UI value = (UI)(c - '0');
  UI pow_10 = 1;

  while (cursor >= buffer) {
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

/* Scans the given buffer for an Signed Integer (SI).
@return Nil if there is no UI to scan.
@param buffer The beginning of the buffer.
@param result The SI to write the scanned SI. */
template <typename SI = int32_t, typename UI = uint, typename Char>
const Char* ScanSigned(const Char* buffer, SI& result) {
  ASSERT(buffer);
  SI sign;
  const Char* cursor = buffer;
  Char c = *cursor++;
  if (c == '-') {
    PRINTF("\nScanning negative backwards:\"");
    c = *buffer++;
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
         (int32_t)(cursor - buffer));

  c = *cursor--;
  UI value = (UI)(c - '0');
  UI pow_10 = 1;

  while (cursor >= buffer) {
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

#if SEAM >= SEAM_0_0_0__01
#include "01/header.h"

/* Searches for the highest MSb asserted.
@return -1 */
template <typename UI>
int MSbAssertedReverse(UI value) {
  for (int i = sizeof(UI) * 8 - 1; i > 0; --i)
    if ((value >> i) != 0) return i;
  return -1;
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
    kMantissaSize = kSizeBits - kExponentSizeBits - 2,
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

  /* Constructs an uninitialized floating-point number. */
  Binary() {}

  /* Converts a Float to a Binary*/
  Binary(Float binary) {
    UI ui = *reinterpret_cast<UI*>(&binary);
    uint32_t biased_e = ui << 1;  //< Get rid of sign bit.
    // Get rid of the integral portion.
    biased_e = biased_e >> (kSizeBits - kExponentSizeBits);
    // Get rid of the sign and exponent.
    uint64_t coefficient = Coefficient<UI>(binary);
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
  static Char* Print(Char* buffer, Char* end, Float value) {
    // Not handling NaN and inf
    if (IsNaN(value)) {
      if (end - buffer < 4) return nullptr;
      buffer[0] = 'N';
      buffer[1] = 'a';
      buffer[2] = 'N';
      buffer[3] = 0;
      return buffer + 4;
    }
    if (IsInfinite(value)) {
      if (end - buffer < 4) return nullptr;
      buffer[0] = (f >> (sizeof(UI > *8 - 1))) ? '-' : '+';
      buffer[1] = 'i';
      buffer[2] = 'n';
      buffer[3] = 'f';
      buffer[4] = 0;
      return buffer + 5;
    }

    if (value == 0) {
      return Print<Char, DChar>(buffer, end, '0', '.', '0');
    }
    if (value < 0) {
      *buffer++ = '-';
      value = -value;
    }
    int32_t length, k;
    char* cursor = Print<Char, DChar>(buffer, end, value, &length, k);
    return Standardize(buffer, length, k);
  }
  template <typename UI = uintptr_t>
  inline UI NaNUnsigned() {
    UI nan = 0;
    return ~nan;
  }

  template <typename SI, typename UI>
  inline SI NaNSigned() {
    UI nan = 1;
    return (SI)(nan << (sizeof(UI) * 8 - 1));
  }

  /* Non-working algorithm DOES NOT converts a string-to-float.
  @return nil if there is no number to scan or pointer to the next char after
  the end of the scanned number upon success.
  @brief Algorithm uses a 32-bit unsigned value to scan the floating-point
  number, which can only have 10 digits max, so the maximum floating-point
  number digit count we can scan is 9 digits long.*/
  template <typename Char = char>
  const Char* Scan(const Char* buffer, Float& result) {
    ASSERT(buffer);
    PRINTF("\n\nScanning float:%s", buffer);

    enum {
      kCharCountMax = 9,  // < (1 + [p*log_10(2)], where p = 32
    };

    uint32_t integer,  //< Integer portion in Binary.
        sign,          //< Sign in Binary32 format.
        ui_value,      //< Unsigned value.
        pow_10;        //< Power of 10 for converting integers.

    // Scan sign of number:

    if (*buffer == '-') {
      sign = NaNSigned<uint32_t, uint32_t>();
      ++buffer;
    } else {
      sign = 0;
    }

    PRINTF("\nScanning integer portion:%i", static_cast<int32_t>(result));

    const Char* cursor = buffer;
    Char c = *cursor++;
    if (!IsDigit<Char>(c)) return nullptr;

    // Find length:
    c = *cursor++;
    while (IsDigit<Char>(c)) c = *cursor++;
    const Char* end = cursor;  // Store end to return.
    cursor -= 2;
    PRINTF("\nPointed at \'%c\' and found length:%i", *cursor,
           (int32_t)(cursor - buffer));

    c = *cursor--;
    ui_value = (uint32_t)(c - '0');
    pow_10 = 1;

    while (cursor >= buffer) {
      c = *cursor--;
      pow_10 *= 10;
      uint32_t new_value = ui_value + pow_10 * (c - '0');
      if (new_value < ui_value) return nullptr;
      ui_value = new_value;
      PRINTF("\nvalue:%u", (uint)ui_value);
    }

    // integer = unsigned_integer;

    PRINTF("\nfound %i and pointed at \'%c\'", integer, *end);

    // Numbers may start with a dot like .1, .2, ...
    if (*buffer == '.') goto ScanDecimals;

    if (*end != '.') {
      result = static_cast<float>(integer);
      PRINTF("\nFound value:%f", result);
      return end;
    }
    ++buffer;
  ScanDecimals:
    // We have to inline the ScanUnsigned here in order to detect if there
    // are too many decimals
    cursor = end;
    Char c = *cursor++;
    if (!IsDigit<Char>(c)) {
      PRINTF("Found a period.");
      return nullptr;
    }
    PRINTF("\nConverting decimals:\"%s\" with max length %i", buffer,
           kCharCountMax);

    // Find length
    c = *cursor++;
    while (IsDigit<Char>(c)) c = *cursor++;

    end = cursor;  // Store end to return.
    cursor -= 2;

    intptr_t length = cursor - buffer;
    PRINTF("\nPointed at \'%c\' and found length:%i", *cursor, (int32_t)length);

    if (length > kCharCountMax) {
      cursor = buffer + kCharCountMax;
      length = kCharCountMax;
    }

    // Manually load the first char.
    c = *cursor--;
    ui_value = (uint32_t)(c - '0'), pow_10 = 1;

    // Then iterate through the rest in a loop.
    while (cursor >= buffer) {
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

    PRINTF("\nConverting bit_pattern backwards:");

    // Convert decimals to base 2 by multiply in a loop the integer value is
    // greater than one then subtract the equivalent of one until the value
    // is zero.

    uint32_t one = IEEE754Pow10E()[length - 1], bit_pattern = 0;
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
    // Now check for the exponent.

    ui_value |= integer << length;

    PRINTF("\nNormalizing bits...");

    if (c != 'e' && c != 'E') {
      PRINTF("\nNo \'e\' or \'E\' found.");
      // ui_value = sign | FloatNormalize<float, uint32_t>(integer);
      // result = *reinterpret_cast<Float*>(&ui_value);
      return end;
    }

    // @todo This is no doubt optimization, not sure how much it would help
    // though.
    int32_t signed_value;
    buffer = ScanSigned<int32_t, uint32_t, Char>(end, signed_value);
    if (!buffer) {
      PRINTF("\nNo exponent found.");
      // result = reinterpret_cast(sign |);
      return end;
    }

    if (signed_value < -128 || signed_value > 127) {
      PRINTF("\nExponent out of range!");
      // result = result_flt;
      return end;
    }

    // We're finally done so store the result.
    // result = result_flt;

    return end;
  }

 private:
  UI f;
  int32_t e;

  Binary NormalizeBoundary() const {
    int = MSbAsserted(0);
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

  static const uint64_t  // kDpExponentMask = 0x7FF0000000000000,
      kDpSignificandMask = 0x000FFFFFFFFFFFFF,
      kDpHiddenBit = 0x0010000000000000;

  /* Normalizes the boundaries. */
  inline void NormalizedBoundaries(Binary& m_minus, Binary& m_plus) const {
    UI l_f,   //< Local copy of f.
        l_e;  //< Local copy of e.
    Binary pl = Binary((l_f << 1) + 1, l_e - 1).NormalizeBoundary();
    const uint64_t kHiddenBit = ((uint64_t)1)
                                << kMantissaSize;  //< 0x0010000000000000
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

  /* Prints the integer portion of the floating-point number.
  @return Nil upon failure or a pointer to the nil-term Char upon success. */
  template <typename Char>
  inline Char* PrintDecimals(Char* cursor, Char* end, const Binary& w,
                             const Binary& m_plus, uint64_t delta, int32_t& k) {
    Binary one(((uint64_t)1) << -m_plus.e, m_plus.e), wp_w = m_plus - w;
    uint32_t d, pow_10, p_1 = static_cast<uint32_t>(m_plus.f >> -one.e);
    uint64_t p_2 = m_plus.f & (one.f - 1);
    int kappa;
    if (p_1 < (pow_10 = 10)) {
      kappa = 1;
    } else if (p_1 < (pow_10 = 100)) {
      kappa = 2;
    } else {
      if ((p_1 >> 10) == 0) {
        kappa = 3;
        pow_10 = 1000;
      } else if (!(p_1 >> 13)) {
        kappa = 4;
        pow_10 = 10000;
      } else if (!(p_1 >> 17)) {
        kappa = 5;
        pow_10 = 100000;
      } else if (!(p_1 >> 20)) {
        kappa = 6;
        pow_10 = 1000000;
      } else if (!(p_1 >> 24)) {
        kappa = 7;
        pow_10 = 10000000;
      } else if (!(p_1 >> 27)) {
        kappa = 8;
        pow_10 = 100000000;
      } else if (!(p_1 >> 30)) {
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
      if (cursor >= end) return nullptr;
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
    uint16_t* exponents = &IEEE754Pow10E()[index];
    return Binary(IEEE754Pow10(exponents), *exponents);
  }

  template <typename Char>
  inline Char* Print(Char* buffer, Char* end, Float value, int32_t& k) {
    const Binary v(value);
    Binary minus, plus;
    v.NormalizedBoundaries(&minus, &plus);

    const Binary c_mk = IEEE754Pow10(plus.e, k);

    const Binary W = v.Normalize() * c_mk;
    Binary w_plus = plus * c_mk, w_minus = minus * c_mk;
    w_minus.f++;
    w_plus.f--;
    return PrintDecimals<Char>(buffer, end, W, w_plus, w_plus.f - w_minus.f, k);
  }

  template <typename Char = char, typename DChar = uint16_t>
  inline Char* Standardize(Char* buffer, Char* end, int32_t length, int32_t k) {
    const int32_t kk = length + k;  // 10^(kk-1) <= v < 10^kk
    Char* nil_term_char;
    if (length <= kk && kk <= 21) {  // 1234e7 -> 12340000000
      for (int32_t i = length; i < kk; i++) buffer[i] = '0';
      buffer[kk] = '.';
      buffer[kk + 1] = '0';
      nil_term_char = &buffer[kk + 2];
      *nil_term_char = '\0';
      return nil_term_char;
    } else if (0 < kk && kk <= 21) {  // 1234e-2 -> 12.34
      SocketMove(&buffer[kk + 1], &buffer[kk], length - kk);
      buffer[kk] = '.';
      nil_term_char = &buffer[length + 1];
      *nil_term_char = '\0';
      return nil_term_char;
    } else if (-6 < kk && kk <= 0) {  // 1234e-6 -> 0.001234
      const int32_t offset = 2 - kk;
      SocketMove(&buffer[offset], &buffer[0], length);
      buffer[0] = '0';
      buffer[1] = '.';
      for (int32_t i = 2; i < offset; i++) buffer[i] = '0';
      nil_term_char = &buffer[length + offset];
      *nil_term_char = 0;
      return nil_term_char;
    } else if (length == 1) {
      // 1e30
      buffer[1] = 'e';
      return Print<Char, DChar>(buffer + 2, end, kk - 1);
    }
    // else 1234e30 -> 1.234e33
    SocketMove(&buffer[2], &buffer[1], length - 1);
    buffer[1] = '.';
    buffer[length + 1] = 'e';
    return Print<Char, DChar>(length + 2, end, kk - 1);
  }
};

template <typename Float = double, typename UI = uint64_t, typename Char = char,
          typename DChar = uint16_t>
inline Char* Print(Char* buffer, Char* end, Float value) {
  // return Binary<Float, UI>::Print<Char, DChar>(buffer, end, value);
  return nullptr;
}

template <typename Float = double, typename UI = uint64_t, typename Char = char,
          typename DChar = uint16_t>
inline Char* Print(Char* buffer, intptr_t size, Float value) {
  // return Binary<Float, UI>::Print<Char, DChar>(buffer, buffer + size - 1,
  //                                             value);
  return nullptr;
}

// using Binary16 = Binary<half, uint32_t>; //< Coming soon.
using Binary32 = Binary<float, uint32_t>;
using Binary64 = Binary<double, uint64_t>;
// using Binary128 = Binary<quad, uint128_t>;
//< Coming soon but not in Visual-C++ due to lack of 128-bit integer support.

#include "01/footer.h"
#endif  //< #if SEAM >= SEAM_0_0_0__01

}  // namespace _
#endif  //< #if INCLUDED_KABUKI_F2_TBINARY
