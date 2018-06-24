/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_printer1.cc
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
// Dependencies:
#include "clock.h"
#include "hex.h"
#if USING_PRINTER
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PUTCHAR(c) putchar(c)
#define PRINT_HEADING \
  std::cout << '\n';  \
  for (int i = 80; i > 0; --i) std::cout << '-'
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#endif

namespace _ {

enum {
  kMaxDigitsFloat = 3 + FLT_MANT_DIG - FLT_MIN_EXP,
  kMaxDigitsDouble = 3 + DBL_MANT_DIG - DBL_MIN_EXP,
};

template <typename Char>
const Char* Empty() {
  static const Char kString[] = {'\0'};
  return kString;
}

template <typename Char>
const Char* NewLine() {
  static const Char kString[] = {'\n'};
  return kString;
}

template <typename Char>
const Char* ErrorHeader() {
  static const Char kString[] = {'\n', 'E', 'r', 'r', 'o', 'r', ':', '\0'};
  return kString;
}

template <typename Char>
inline bool IsWhitespace(Char character) {
  return character <= ' ';
}

template <typename Char>
inline Char PrintableChar(Char value) {
  if (value < 32 || value == 127) return ' ';
  return value;
}

template <typename Char>
inline const Char* TextEnd(const Char* text, Char delimiter = 0) {
  Char c = *text;
  while (c > delimiter) {
    c = *(++text);
  }
  return text;
}

template <typename Char>
I TextLength(const Char* text, Char delimiter = 0) {
  ASSERT(text);
  I count = 0;
  Char c = *text;
  while (c > delimiter) {
    I upper_bounds = 0;            //< Faster to make from instructions then
    upper_bounds = ~upper_bounds;  //< load from ROM.
    if (count == upper_bounds) return 0;
    ++count;
    c = *(++text);
  }
  return count;
}

template <typename Char>
Char* TextClone(const Char* text, Char delimiter = 0) {
  ASSERT(text);
  int length = TextLength<Char>(text, delimiter) + 1;
  if (length < 0) return nullptr;
  Char* clone = new Char[length];
  MemoryCopy(clone, clone + length, text, text + length + 1);
  return clone;
}

template <typename Char>
const Char* TextLineEnd(const Char* text, int column_count,
                        Char delimiter = 0) {
  Char c;
  // Scroll to the end of the line.
  c = *target++;
  while (c > delimiter) {
    if (column_count-- < 0) {  // We've reached the end.
                               // Scroll left till we hit whitespace (if any).
      while (!IsWhitespace<Char>(c)) {
        c = *(--target);
      }
      // Then scroll past the whitespace.
      while (IsWhitespace<Char>(c)) {
        c = *(--target);
      }
      return target;
      c = *target++;
    }
    c = *target++;
  }
  return target - 1;
}

template <typename Char>
const Char* TextLineEnd(const Char* text, const Char* text_end,
                        int column_count, Char delimiter = 0) {
  if (!text) {
    PRINTF("\nText buffer overflow!");
    return nullptr;
  }
  ASSERT(text < text_end)
  Char c;
  // Scroll to the end of the line.
  c = *text++;
  while (c > delimiter) {
    if (text_end > text) return nullptr;
    if (column_count-- < 0) {  // We've reached the end.

      // Scroll left till we hit whitespace (if any).
      while (!IsWhitespace<Char>(c)) c = *(--text);
      // Then scroll past the whitespace.
      while (IsWhitespace<Char>(c)) c = *(--text);
      return text;
      c = *text++;
    }
    c = *text++;
  }
  return text - 1;
}

template <typename Char>
const Char* TextNumberEnd(const Char* text, const Char* text_end,
                          Char delimiter = 0) {
  if (text > text_end) return nullptr;
  Char c = *text;
  if (c == '-') {  // It might be negative.
    if (text > text_end) return nullptr;
    c = *(++text);
    if (!IsDigit<Char>(c)) return text - 1;  //< it's not negative.
    c = *(++text);
  }
  while (isdigit(c)) {
    if (text > text_end) return nullptr;
    c = *(++text);
  }
  return text;
}

template <typename Char>
const Char* TextNumberEnd(const Char* text) {
  ASSERT(text);
  Char c = *text;
  if (c < '0' || c > '9') return nullptr;
  if (c == '-') {  // It might be negative.
    c = *(++text);
    if (!isdigit(c)) return text - 1;  //< it's not negative.
    c = *(++text);
  }
  while (IsDigit<Char>(c)) {
    if (c <= 0) return text;
    c = *(++text);
  }
  return text;
}

template <typename Char>
const Char* TextSkipChar(const Char* text, Char skip_char, Char delimiter = 0) {
  if (text == nullptr) return nullptr;
  Char c = *text, d;
  if (c != skip_char) return text;
  d = *text;
  // We know the first Char is a skip_char so just loop till c and d aren't
  // the skip_char.
  while (c == d) {
    c = d;
    d = *(++text);
  }
  return text;
}

template <typename Char>
const Char* TextSkipSpaces(const Char* text) {
  if (!text) return nullptr;
  Char c = *text;
  while (IsWhitespace<Char>(c)) {
    PUTCHAR('.');
    if (!c) return text;  //< This isn't an error as far as I can see.
    ++text;
    c = *text;
  }
  return text;
}

template <typename Char>
const Char* TextSkipSpaces(const Char* text, const Char* text_end) {
  if (!text) return nullptr;
  if (text > text_end) return nullptr;
  PRINTF("\nSkipping spaces: ");
  Char c = *text;
  while (IsWhitespace(c)) {
    PUTCHAR('.');
    if (!c) return nullptr;
    if (++text > text_end) return nullptr;
    c = *text;
  }
  return text;
}

template <typename Char>
const Char* TextEquals(const Char* text_a, const Char* text_b, Char delimiter) {
  ASSERT(text_a);
  ASSERT(text_b);
  PRINTF("\nComparing \"%s\" to \"%s\"", text_a, text_b);

  Char a = *text_a, b = *text_b;
  while (a > delimiter) {
    PUTCHAR(a);
    if (a != b) {  // Not a hit.
      PRINTF("\nBut it's not a hit");
      return nullptr;
    }
    if (b <= delimiter) {  // Hit!
      PRINTF("\nFound hit at 0x%p", text_a);
      return text_a;
    }
    a = *(++text_a);
    b = *(++text_b);
  }
  if (b > delimiter) {
    PRINTF("\nNot a hit: no nil-term Char found");
    return nullptr;
  }
  PRINTF("\nFound hit at 0x%p", text_a);
  return text_a;  //< Find hit!
}

template <typename Char>
const Char* TextEquals(const Char* text, const Char* text_end,
                       const Char* query, Char delimiter = 0) {
  if (!text) return nullptr;
  ASSERT(text < text_end)
  return nullptr;
  if (!string) {
    return nullptr;
  }
  PRINTF("\nComparing \"%s\" to \"%s\"", text, string);

  Char a = *text, b = *string;
  while (a > delimiter) {
    PUTCHAR(a);
    if (a != b) {  // Not a hit.
      PRINTF("\nBut it's not a hit");
      return nullptr;
    }
    if (b <= delimiter) {  // Hit!
      PRINTF("\nFound hit at 0x%p", text);
      return text;
    }
    if (text > text_end) {
      return nullptr;
    }
    a = *(++text);
    b = *(++string);
  }
  if (b > delimiter) {
    PRINTF("\nNot a hit: no nil-term Char found");
    return nullptr;
  }
  PRINTF("\nFound hit at 0x%p.", text);
  return text;
}

template <typename Char>
bool TextQualifies(const Char* text, Char delimiter) {
  if (text == nullptr) return false;
  Char c = *text;
  while (c > delimiter) {
    if (!IsWhitespace<Char>(c)) return true;
    c = *(++text);
  }
  return false;
}

template <typename Char>
bool TextQualifies(const Char* text, const Char* text_end, Char delimiter) {
  if (!text) return false;
  if (text > text_end) return false;
  Char c = *text;
  while (c > delimiter) {
    if (!IsWhitespace<Char>(c)) {
      // The text must end at or before the target_end.
      do {
        if (++text > text_end) return false;
        c = *text;
        if (!IsWhitespace<Char>(c)) return true;
      } while (c > delimiter);
      return true;
    }
    if (++text > text_end) return false;
    c = *text;
  }
  return false;
}

template <typename Char>
const Char* TextEquals(const Char* text_a, const Char* text_b) {
  text_a = TextSkipSpaces<Char>(text_a);
  if (!text_a) return nullptr;
  if (!text_b) return nullptr;

  Char a = *text_a, b = *text_b;
  int result;

  PRINTF("\nComparing \"%s\" to \"%s\"", text_a, text_b);

  // text SHOULD be a nil-terminated string without whitespace.
  while (b) {
    result = b - a;
    PRINTF("\nb - a = %i - %i = %i", b, a, result);
    if (result) {
      PRINTF(" is not a hit.");
      return nullptr;
    }
    if (!a) {
      PRINTF(" is a partial match but !a.");
      return nullptr;
    }
    ++text_a;
    ++text_b;
    a = *text_a;
    b = *text_b;
  }

  if (a && !IsWhitespace<Char>(a)) {
    PRINTF(" is only a partial match but found %s", (a ? "a" : "space"));
    return nullptr;
  }
  PRINTF(" is a match!");
  return text_a;
}

template <typename Char>
int TextCompare(const Char* text_a, const Char* text_b) {
  int a, b, result;
  if (!text_a) {
    if (!text_b) return 0;
    return *text_a;
  }
  if (!text_b) return 0 - *text_a;

  PRINTF("\nComparing \"%s\" to \"%s\"", text_a, text_b);
  a = *text_a;
  b = *text_b;
  if (!a) {
    if (!a) return 0;
    return b;
  }
  if (!b) {
    if (!a) return 0;  //< I like !t code rather than !c code. :-)
    return 0 - a;
  }
  // text SHOULD be a nil-terminated string without whitespace.
  while (b) {
    result = b - a;
    PRINTF("\nb - a = %i - %i = %i", b, a, result);
    if (result) {
      PRINTF(" is not a hit.");
      return result;
    }
    if (!a) {
      PRINTF(" is a partial match but !a.");
      return result;
    }
    ++text_a;
    ++text_b;
    a = *text_a;
    b = *text_b;
  }
  if (a && !IsWhitespace<Char>(a)) {
    PRINTF(" is only a partial match but found %s", (a ? "a" : "space"));
    return b - a;
  }
  return 0;
}

template <typename Char>
int TextCompare(const Char* text_a, const Char* text_end, const Char* text_b) {
  Char a = *text_a, b = *text_b;
  int result;

  if (!text_a) {
    if (!text_b) return 0;
    a = 0;
    b = *text_b;
    return b - a;
  }
  if (!text_b) {
    a = *text_a;
    b = 0;
    return b - a;
  }
  if (text_a > text_end) return *text_b;

  // Algorithm combines loops for better performance.
  a = *text_a;
  b = *text_b;
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
    if (++text_a > text_end) {
      PRINTF(" but buffer overflowed!");
      return result;
    }
    ++text_b;
    a = *text_a;
    b = *text_b;
  }
  if (a && !IsWhitespace<Char>(a)) {
    PRINTF(" is only a partial match but found %s", (a ? "a" : "space"));
    return b - a;
  }
  PRINTF(" is a match!");
  return 0;
}

template <typename Char>
const Char* TextFind(const Char* text, const Char* query, Char delimiter = 0) {
  ASSERT(text);
  ASSERT(query);

  Char string = *target,  //< Current target Char.
      t = *query,         //< Current query Char.
      c = t;              //< The first Char of the query we're searching for.
  if (c <= delimiter)     //< We're not allowing empty queries.
    return nullptr;
  const Char *start_of_query, *begin = target;
  query = TextSkipSpaces(query);

  // Scroll through each Char and match it to the query Char.
  while (string) {
    if (string == c) {  // The first Char matches:
                        // Setup to compare the strings;
      start_of_query = target;
      begin = query;
      t = c;
      // check the rest of the Char:
      while (string == t) {
        string = *(++target);
        t = *(++begin);
        if (t == delimiter) {     // Once we've reached the delimiter
          return start_of_query;  // it's a match!
        }
        if (!string)  // We've reached the end of Char without a hit.
          return nullptr;
      }
    }
    // The Char did not match so repeat the process for each Char.
    string = *(++target);
    t = *(++begin);
  }
  // If we haven't found it by now it's not in the target.
  return nullptr;
}

template <typename Char>
Char* Print(Char* begin, Char* end, const Char* string) {
  ASSERT(begin);
  ASSERT(string);

  if (begin >= end) {
    return nullptr;
  }

  char c = *string++;
  while (c) {
    *begin++ = c;
    if (begin >= end) return nullptr;
    c = *string++;
  }
  *begin = 0;
  return begin;
}

template <typename Char>
Char* Print(Char* begin, Char* end, Char character) {
  ASSERT(begin)
  ASSERT(end);

  if (begin + 1 >= end) return nullptr;

  *begin++ = character;
  *begin = 0;
  return begin;
}

template <typename Char>
Char* PrintRight(Char* begin, Char* end, const Char* token, int column_count) {
  Char* start = begin;
  ASSERT(token);
  ASSERT(begin);

  if (begin >= end) return nullptr;
  if (begin + column_count > end) return nullptr;

  // PRINTF ("\n\nPrinting \"%s\" aligned right %i columns", token,
  // column_count)

  intptr_t length = 0;            //< Length of the token.
  const Char* token_end = token;  //< Address of the last Char.
  Char* begin;                    //< Print cursor pointer.
  Char c;                         //< Temp variable.

  // Find string length.
  while (*(++token_end))
    ;
  length = token_end - token;

  // PRINTF ("\n    Found length:%i", (int)length)

  if (token == token_end) {
    return begin;
  }

  *(begin + length) = 0;
  // If the length is less than the column_count we need to print ".", "..",
  // "..." or nothing and chop off some of the token.
  if (length > column_count) {
    // PRINTF ("\n Wrote with dots:\"")
    intptr_t num_dots = length - column_count;
    if (num_dots > 3) {
      num_dots = 3;
    }
    end = begin + column_count;
    begin = end - num_dots;
    while (begin < begin) {
      c = *token++;
      *begin++ = c;
      // PUTCHAR (c)
    }
    while (begin < end) {
      *begin++ = '.';
      // PUTCHAR ('.')
    }
    // PUTCHAR ('\"')
    *begin = 0;
    return begin;
  }
  // PRINTF ("\n Wrote:\"")
  begin = begin + column_count;
  end = begin - length;
  while (begin >= end) {
    c = *token_end--;
    *begin-- = c;
    // PUTCHAR (c)
  }
  // PUTCHAR ('\"')
  end = begin - column_count;
  // PRINTF ("\ncursor:0x%p text:0x%x delta:%i", cursor, text, text, cursor)
  while (begin >= begin) {
    *begin-- = ' ';
  }
  end = begin + column_count;
  return end;
}

template <typename Char>
Char* PrintCenter(Char* begin, Char* end, const Char* string,
                  int column_count) {
  ASSERT(begin);
  ASSERT(begin < end);

  // We need to leave at least one space to the left and right of
  int length = TextLength<char>(string), offset;
  PRINTF("\n\n    Printing \"%s\":%i column_count:%i", string, length,
         column_count);

  if (column_count < length) {
    offset = length - column_count;
    if (offset > 3) {
      offset = 3;
    }
    column_count -= offset;
    while (column_count-- > 0) {
      *begin++ = *string++;
    }
    while (offset-- > 0) {
      *begin++ = '.';
    }
    *begin = 0;
    return begin;
  }
  offset = (column_count - length) >> 1;  //< >> 1 to /2
  length = column_count - length - offset;
  PRINTF("\n    length:%i offset:%i", length, offset);

  while (length-- > 0) *begin++ = ' ';

  char c = *string++;
  while (c) {
    *begin++ = c;
    c = *string++;
  }
  while (offset-- > 0) *begin++ = ' ';
  *begin = 0;
  PRINTF("\n    Printed:\"%s\"", string);
  print.begin = begin;
  return begin;
}

template <typename T, typename Char>
Char* PrintHex(Char* begin, Char* buffer_end, T value) {
  enum { kHexStringLengthSizeMax = sizeof(T) * 2 + 3 };

  ASSERT(begin);
  if (begin + kHexStringLengthSizeMax >= buffer_end) return nullptr;

  *begin++ = '0';
  *begin++ = 'x';
  for (int num_bits_shift = sizeof(T) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *begin++ = HexNibbleToUpperCase((uint8_t)(value >> num_bits_shift));
  }
  *begin = 0;
  return begin;
}

template <typename T, typename Char>
Char* PrintBinary(Char* begin, Char* end, T value) {
  char* begin = printer.begin;
  if (begin + sizeof(uint64_t) * 8 >= printer.end) {
    return nullptr;
  }

  for (int i = 0; i < 64; ++i) {
    *buffer++ = (char)('0' + (value >> 63));
    value = value << 1;
  }
  *buffer = delimiter;
  return buffer;
}

template <typename Char>
Char* PrintMemory(Char* begin, Char* end, const void* start, const void* stop) {
  ASSERT(start);
  ASSERT(begin);
  ASSERT(begin < end);

  Char* buffer_begin = begin;
  const Char *address_ptr = reinterpret_cast<const Char*>(start),
             *address_end_ptr = reinterpret_cast<const Char*>(stop);
  size_t size = address_end_ptr - address_ptr,
         num_rows = size / 64 + (size % 64 != 0) ? 1 : 0;

  intptr_t num_bytes = 81 * (num_rows + 2);
  if ((end - begin) <= num_bytes) {
    PRINTF("\n    ERROR: buffer overflow trying to fit %i in %i bytes!",
           (int)num_bytes, (int)(end - begin));
    return nullptr;
  }
  size += num_bytes;
  *begin++ = '\n';
  *begin++ = '|';

  //  columns
  *begin++ = '0';
  begin = PrintRight<Char>(begin, end, 8, 8);
  *begin++ = ' ';
  for (int i = 16; i <= 56; i += 8) begin = PrintRight(begin, end, i, 8);
  for (int j = 6; j > 0; --j) *begin++ = ' ';
  *begin++ = '|';
  *begin++ = '\n';
  *begin++ = '|';
  for (int j = 8; j > 0; --j) {
    *begin++ = '+';
    for (int k = 7; k > 0; --k) *begin++ = '-';
  }
  *begin++ = '|';
  *begin++ = ' ';

  begin = PrintHex<Char>(begin, end, address_ptr);

  PRINTF("\nBuffer space left:%i", (int)(end - begin));
  Char c;
  while (address_ptr < address_end_ptr) {
    *begin++ = '\n';
    *begin++ = '|';
    for (int i = 0; i < 64; ++i) {
      c = *address_ptr++;
      if (address_ptr > address_end_ptr)
        c = 'x';
      else if (!c || c == TAB)
        c = ' ';
      else if (c < ' ')
        c = DEL;
      *begin++ = c;
    }
    *begin++ = '|';
    *begin++ = ' ';
    begin = PrintHex<Char>(begin, end, address_ptr);
  }
  *begin++ = '\n';
  *begin++ = '|';
  for (int j = 8; j > 0; --j) {
    *begin++ = '+';
    for (int k = 7; k > 0; --k) {
      *begin++ = '-';
    }
  }
  *begin++ = '|';
  *begin++ = ' ';
  return PrintHex<Char>(begin, end, address_ptr + size);
}

template <typename Char>
Char* PrintClock(Char* begin, Char* end, time_t t) {
  if (begin == nullptr) {
    return nullptr;
  }
  if (begin > end) {
    return nullptr;
  }
  time(&t);
  TimeSeconds time_components;
  ClockTimeLocal(&time_components, t);
  return PrintClock<Char>(begin, end, &time_components);
}

template <typename UI, typename Char>
const Char* TextScanUnsigned(const Char* string, UI& result) {
  ASSERT(string);
  Char c = *string;
  if (c < '0' || c > '9') return nullptr;
  Char* cursor = TextNumberEnd(string);
  if (cursor == string) {
    result = (c + '0') * sign;
    return cursor;
  }
  UI power_of_ten = 10;
  c = *cursor--;
  UI value = (UI)c;
  while (cusor > string) {
    UI new_power_of_ten = power_of_ten * 10;
    // First check for unsigned multiply loop-around condition.
    if (new_power_of_ten < power_of_ten) return nullptr;
    power_of_ten = new_power_of_ten;
    value += power_of_ten * (*cursor--);
  }
  result = value;
}

template <typename SI, typename Char>
const Char* TextScanSigned(const Char* string, SI& result) {
  ASSERT(string);
  UI power_of_ten = 10;
  c = *cursor--;
  SI value = (SI)c;
  Char* cursor;
  if (c == '-') {
    ++string;
    sign = -1;
    cursor = TextNumberEnd<Char>(string);
    if (cursor == nullptr) return nullptr;
  } else {
    if (c < '0' || c > '9') return nullptr;
    Char c = *string;
    value = c;
  }
  while (cusor > string) {
    power_of_ten *= 10;
    value += power_of_ten * (*cursor--);
  }
  result = value * sign;
}

template <typename Char>
const Char* TextScan(const Char* string, float& result) {
  int32_t integer;
  const Char* cursor = TextScanIntger<Char>(string);
  if (integer == NaNSigned<int32_t>()) return nullptr;
  value result = (float)integer;
  Char c = *string;
  if (c == '.') {
    // Check for end of sentence contiion.
    c = *(++string);
    if (!IsDigit(c)) return result;

    float power_of_ten = 0.1;
    while (IsDigit<Char>(c)) {
      power_of_ten /= 10.0;
      result += ((float)c) * power_of_ten;
      c = *string++;
    }
  }
  // Check for exponent condition.
  if (ToLower<Char>(c) == 'e') {
    int32_t exponent;
    string = TextScanSigned<int32_t, Char>(cursor);
    if (!string) return cursor;
    return result*
  }
  return result;
}

template <typename Char>
const Char* TextScan(const Char* string, double& result) {
  result = 0.0;
}

template <typename Char>
const Char* TextScanTime(const Char* string, int& hour, int& minute,
                         int& second) {
  if (string == nullptr) return nullptr;

  PRINTF("\n\n    Scanning time:%s", string);
  Char c;  //< The current Char.
  int h,   //< Hour.
      m,   //< Minute.
      s;   //< Second.
  if (!Scan(h, ++string)) {
    PRINTF("\nInvalid hour:%i", h);
    return 0;
  }
  string = TextSkipNumbers(string);
  if (h < 0) {
    PRINTF("\nHours:%i can't be negative.", h);
    return 0;
  }
  if (h > 23) {
    PRINTF("\nHours:%i can't be > 23.", h);
    return 0;
  }
  PRINTF("%i", h)
  c = *string++;
  if (!c || IsWhitespace(c)) {  // Case @HH
    PRINTF(" HH ")
    // Then it's a single number, so create a time_t for the current
    // user-time hour..
    hour = h;
    return string;
  }
  c = tolower(c);
  if (c == 'a') {  //
    PRINTF("\nCase @HHAm\n HHam ");

    if (tolower(c = *string++) == 'm') c = *string++;
    if (c && !IsWhitespace(c)) {
      PRINTF("\nInvalid am format.");
      return 0;
    }
    PRINTF(" @HHAM ")
    hour = h;
    return string;
  }
  if (c == 'p') {
    PRINTF(" Case @HHpm ");

    if (tolower(c = *string++) == 'm') c = *string++;
    if (c && !IsWhitespace(c)) {
      PRINTF("\ninvalid pm format.");
      return 0;
    }
    PRINTF("< Case @HHPM %i:00:00\n", h + 12);
    hour = h + 12;
    return string;
  }
  if (c != ':') {
    PRINTF("\nExpecting ':'.");
    return 0;
  }

  PRINTF(
      "\nCases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and "
      "HH:MM:SSpm");

  if (!Scan(m, string)) return 0;
  string = TextSkipNumbers(string);
  if (m < 0) {
    PRINTF("\nMinutes:%i can't be negative!", m);
    return 0;
  }
  if (m >= 60) {
    PRINTF("\nMinutes:%i can't be >= 60!", m);
    return 0;  //< 60 minutes in an hour.
  }
  PRINTF(":%i", m)

  string = TextSkipNumbers(string);
  c = *string++;
  if (!c || IsWhitespace(c)) {  // Case HH:MM
    PRINTF(" HH:MM ");
    hour = h;
    minute = m;
    return string;
  }
  c = tolower(c);
  if (c == 'a') {  // Case HH::MM AM
    PRINTF(" HH:MMam ");
    c = *string++;
    if (tolower(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (c && !IsWhitespace(c)) {  //< The space is not.
      return "Invalid am in HH::MM AM";
    }
    hour = h;
    minute = m;
    return string;
  }
  if (c == 'p') {  // Case HH:MM PM
    PRINTF(" HH:MMpm ");
    c = *string++;
    if (tolower(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (c && !IsWhitespace(c)) {  //< The space is not.
      return "Invalid am in HH::MM PM";
    }
    hour = h + 12;
    minute = m;
    return string;
  }
  if (c != ':') return 0;

  PRINTF("\n    Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm");

  if (!TextScan<Char>(string, s)) return 0;
  if (s < 0) {
    PRINTF("\nSeconds:%i can't be negative!", s);
    return 0;
  }
  if (s > 60) {
    PRINTF("\nSeconds:%i can't be >= 60!", s);
    return 0;
  }
  PRINTF(":%i", s)
  string = TextSkipNumbers(string);
  c = ToLower<Char>(*string);
  if (!c || IsWhitespace<Char>(c)) {
    PRINTF(" HH:MM:SS ");
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c == 'a') {
    PRINTF(" HH:MM:SSam ");
    c = *string++;
    if (ToLower<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (!c || !IsWhitespace<Char>(c)) {  //< The space is not.
      return "Invalid am in HH::MM:SS AM";
    }
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c != 'p') {
    PRINTF("\nExpecting a PM but found:%c", c);
    return 0;  // Format error!
  }
  PRINTF(" HH:MM:SSpm ");
  c = ToLower<Char>(*string++);
  if (c == 'm') {  //< The 'm' is optional.
    c = *string++;
  }
  if (!c || !IsWhitespace<Char>(c)) {  //< The space is not.
    PRINTF("\nInvalid am in HH::MM:SS PM");
    return nullptr;
  }
  hour = h + 12;
  minute = m;
  second = s;
  return string;
}

template <typename Char>
const Char* TextScanTime(const Char* cursor, TimeSeconds& time) {
  ASSERT(cursor);
  PRINTF("\n    Scanning TimeSeconds: %s\n    Scanning: ", cursor);

  cursor = TextSkipChar(cursor, '0');
  Char c = *cursor,  //< The current Char.
      delimiter;     //< The delimiter.
  const Char* end;   //< Might not need

  int hour = 0, minute = 0, second = 0;

  if (c == '@') {
    if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second)))
      return "Case @ invalid time";
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = second;

    return cursor + 1;
  }
  if (c == '#') {
    if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second)))
      return "Case @ invalid time";
    time.tm_hour += hour;
    time.tm_min += minute;
    time.tm_sec += second;

    return cursor + 1;
  }

  int value1,            //< The first date field scanned.
      value2,            //< The second date field scanned.
      value3,            //< The third date field scanned.
      is_last_year = 0;  //< Flag for if the date was last year or not.

  // Scan value1
  if (!TextScan<Char>(value1, cursor)) {
    PRINTF("Scan error at value1");
    return 0;
  }
  if (value1 < 0) {
    PRINTF("Dates can't be negative.");
    return 0;
  }
  cursor = TextNumberEnd<Char>(cursor);
  delimiter = *cursor++;
  PRINTF("%i%c", value1, delimiter);
  if (delimiter == '@') {
    PRINTF(" HH@ ");

    if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
      PRINTF("Invalid time DD@");
      return 0;
    }
    time.tm_mday = value1;

    return cursor + 1;
  }
  // Scan value2.
  cursor = TextSkipChar<Char>(cursor, '0');
  if (!TextScan<Char>(value2, cursor)) {
    PRINTF("Failed scanning value2 of date.");
    return 0;
  }
  if (value2 < 0) {
    PRINTF("Day can't be negative.");
    return 0;  //< Invalid month and day.
  }
  PRINTF("%i", value2);
  cursor = TextNumberEnd<Char>(cursor);
  c = *cursor;
  if (c != delimiter) {
    PRINTF("\n    Cases MM/DD and MM/YYyy");
    if (c == '@') {
      if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
        PRINTF(" invalid time ");
        return nullptr;
      }
    }
    if (!c || IsWhitespace<Char>(c)) {
      PRINTF("\n    Format is MM/DD and year is %i", time.tm_year + kTimeEpoch);
      // is_last_year = ((value1 >= std_tm.tm_mon) &&
      //    (value2 >= std_tm.tm_mday)) ? 0:1;
      time.tm_year += is_last_year;
      time.tm_mon = value1 - 1;
      time.tm_mday = value2;
      time.tm_hour = 0;
      time.tm_hour = hour;
      time.tm_min = minute;
      time.tm_sec = second;

      return cursor + 1;
    }
    c = tolower(c);
    if ((value1 < 12) && (value2 > 0) &&
        (value2 <= ClockNumDaysMonth(value1))) {
      PRINTF(" MM/DD ");
      if (value1 > 11) {
        PRINTF("\nInvalid MM/DD@ month");
        return nullptr;
      }
      if (value2 > ClockNumDaysMonth(value1)) {
        PRINTF("\nInvalid MM/DD@ day");
        return nullptr;
      }
      time.tm_mon = value1 - 1;
      time.tm_mday = value2;
      time.tm_hour = hour;
      time.tm_min = minute;
      time.tm_sec = second;
      if (!(cursor = TextScanTime(cursor, hour, minute, second))) {
        PRINTF("\nInvalid MM/DD@");
        return nullptr;
      }

      return cursor + 1;
    }
    if ((value1 < 12) && (value2 > ClockNumDaysMonth<Char>(value1))) {
      PRINTF(" MM/YYyy\n");
      time.tm_mon = value1 - 1;
      time.tm_year = value2;
      if (!(cursor = TextScanTime(cursor, hour, minute, second))) {
        PRINTF("\nInvalid MM / YYYY@ time");
        return 0;
      }

      return cursor + 1;
    }
    PRINTF("\nInvalid MM/DD or MM/YYyy format\n");
    return nullptr;
  }

  // Formats MM/DD/YYyy and YYyy/MM/DD

  cursor = TextSkipChar<Char>(++cursor, '0');
  c = *cursor;
  // Then there are 3 values and 2 delimiters.
  if (!IsDigit<Char>(c) || !TextScan<Char>(value3, cursor)) {
    PRINTF("\n    SlotRead error reading value3 of date. %c: ", c);
    return 0;  //< Invalid format!
  }
  cursor = TextNumberEnd<Char>(cursor);
  PRINTF("%c%i", delimiter, value3);
  // Now we need to check what format it is in.

  c = *cursor;
  if (c == '@') {
    if (!(end = TextScanTime<Char>(cursor, hour, minute, second))) {
      PRINTF("Invalid YYyy/MM/DD@ time.");
      return 0;
    }
  }
  time.tm_hour = hour;
  time.tm_min = minute;
  time.tm_sec = second;
  if (IsWhitespace<Char>(*(++cursor))) {
    PRINTF("No date found.");
    return 0;
  }
  if (value1 > 11) {  //<
    PRINTF("\n    Case YYyy/MM/DD\n");
    if (value2 == 0 || value2 > 12) {
      PRINTF("Invalid number of months");
      return 0;
    }

    if (value2 > ClockNumDaysMonth<Char>(value2, value1)) {
      PRINTF("Invalid number of days");
      return 0;
    }  // 17/05/06

    if (value1 < 100) {
      PRINTF("\n    Case YY/MM/DD");
      value1 += 2000 - kTimeEpoch;
    } else {
      PRINTF("\n    Case YYYY/MM/DD");
      value1 -= kTimeEpoch;
    }

    time.tm_year = value1;
    time.tm_mon = value2 - 1;
    time.tm_mday = value3;

    return cursor + 1;
  }
  PRINTF("\n    Cases MM/DD/YY and MM/DD/YYYY");

  if (value1 > 11) {
    PRINTF("Invalid month.\n");
    return nullptr;
  }
  if (value2 > ClockNumDaysMonth<Char>(value1, value3)) {
    PRINTF("Invalid day.\n");
    return nullptr;
  }
  time.tm_mon = value1 - 1;
  time.tm_mday = value2;
  if (value3 < 100) {
    PRINTF("\n    Case MM/DD/YY");
    time.tm_year = value3 + (2000 - kTimeEpoch);
  } else {
    PRINTF("\n    Case MM/DD/YYYY");
    time.tm_year = value3 - kTimeEpoch;
  }
  return cursor + 1;
}

template <typename Char>
const Char* TextScanTime(const Char* begin, time_t& result) {
  time_t t;
  time(&t);
  tm std_tm;
  ClockTimeLocal(&std_tm, t);

  const Char* end = TextScanTime<Char>(std_tm, begin);

  t = mktime(&std_tm);
  // PRINTF ("\n\nFound ";
  // PRINTF (ClockPrintTimeStruct (&std_tm);
  Char buffer[26];
  PrintTime<Char>(buffer, buffer + 26, t);
  Char time_string[26];
  PrintTime<Char>(time_string, &time_string[0] + 26, t);
  // PRINTF ("\nUnpacked: " << buffer;
  result = t;
  return end;
}

}  // namespace _

#if USING_UTF8
#include "utf8.h"
namespace _ {

const char* Empty1() {
  static const char kString[] = {'\0'};
  return kString;
}

const char* NewLine1() {
  static const char kString[] = {'\n'};
  return kString;
}

const char* ErrorHeader1() {
  static const char kString[] = {'\n', 'E', 'r', 'r', 'o', 'r', ':', '\0'};
  return kString;
}

bool IsWhitespace(char character) { return IsWhitespace<char>(character); }

char Char(char value) { return PrintableChar<char>(value); }

const char* TextEnd(const char* text, char delimiter) {
  return TextEnd<char>(text, delimiter);
}

int TextLength(const char* text, char delimiter) {
  return TextLength<char>(text, delimiter);
}

char* TextClone(const char* text, char delimiter) {
  return TextClone<char>(text, delimiter);
}

const char* TextLineEnd(const char* text, int column_count, char delimiter) {
  return TextLineEnd<char>(text, column_count, delimiter);
}

const char* TextLineEnd(const char* text, const char* text_end,
                        int column_count, char delimiter) {
  return TextLineEnd<char>(text, text_end, column_count, delimiter);
}

const char* TextNumberEnd(const char* text, const char* text_end,
                          char delimiter) {
  return TextNumberEnd<char>(text, text_end, delimiter);
}

const char* TextNumberEnd(const char* text) {
  return TextNumberEnd<char>(text);
}

const char* TextSkipChar(const char* text, char skip_char) {
  return TextSkipChar<char>(text, skip_char);
}

const char* TextSkipSpaces(const char* text) {
  return TextSkipSpaces<char>(text);
}

const char* TextSkipSpaces(const char* text, const char* text_end) {
  return TextSkipSpaces<char>(text, text_end);
}

const char* TextEquals(const char* text_a, const char* text_b, char delimiter) {
  return TextEquals<char>(text_a, text_b, delimiter);
}

const char* TextEquals(const char* text, const char* text_end,
                       const char* query, char delimiter) {
  return TextEquals<char>(text, text_end, query, delimiter);
}

bool TextQualifies(const char* text, char delimiter) {
  return TextQualifies<char>(text, delimiter);
}

bool TextQualifies(const char* text, const char* text_end, char delimiter) {
  return TextQualifies<char>(text, text_end, delimiter);
}

const char* TextEquals(const char* text_a, const char* text_b) {
  return TextEquals<char>(text_a, text_b);
}

int TextCompare(const char* text_a, const char* text_b) {
  return TextCompare<char>(text_a, text_b);
}

int TextCompare(const char* text_a, const char* text_end, const char* text_b) {
  return TextCompare<char>(text_a, text_end, text_b);
}

const char* TextFind(const char* text, const char* query, char delimiter) {
  return TextFind<char>(text, query, delimiter);
}

char* Print(char* begin, char* end, const char* string) {
  return Print<char>(begin, end, string);
}

char* Print(char* begin, char* end, char character) {
  return Print<char>(begin, end, character);
}

char* Print(char* begin, char* end, uint32_t value) {
  return Print<char>(begin, end, value);
}

char* Print(char* begin, char* end, int32_t value) {
  return Print<char>(begin, end, value);
}

char* Print(char* begin, char* end, uint64_t value) {
  return Print<char>(begin, end, value);
}

char* Print(char* begin, char* end, int64_t value) {
  return Print<char>(begin, end, value);
}

char* Print(char* begin, char* end, float value) {
  return Print<char>(begin, end, value);
}

char* Print(char* begin, char* end, double value) {
  return Print<char>(begin, end, value);
}

char* PrintCenter(char* begin, char* end, const char* string,
                  int column_count) {
  return PrintCenter<char>(begin, end, string, column_count);
}

char* PrintCenter(char* begin, char* end, char character, int column_count) {
  return PrintCenter<char>(begin, end, Text1(character).GetString(),
                           column_count);
}

char* PrintCenter(char* begin, char* end, int32_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintCenter(char* begin, char* end, uint32_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintCenter(char* begin, char* end, uint64_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintCenter(char* begin, char* end, int64_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintCenter(char* begin, char* end, float value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintCenter(char* begin, char* end, double value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintRight(char* begin, char* end, const char* string, int column_count) {
  return PrintRight<char>(begin, end, string, column_count);
}

char* PrintRight(char* begin, char* end, char character, int column_count) {
  return PrintRight<char>(begin, end, Text1(character).GetString(),
                          column_count);
}

char* PrintRight(char* begin, char* end, uint32_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintRight(char* begin, char* end, int32_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintRight(char* begin, char* end, uint64_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintRight(char* begin, char* end, int64_t value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintRight(char* begin, char* end, float value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintRight(char* begin, char* end, double value, int column_count) {
  return PrintRight<char>(begin, end, Text1(value).GetString(), column_count);
}

char* PrintHex(char* begin, char* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintHex<char>(begin, end, ptr);
}

char* PrintHex(char* begin, char* end, uint8_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int8_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, uint16_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int16_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, uint32_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int32_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, uint64_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int64_t value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, float value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintHex(char* begin, char* end, double value) {
  return PrintHex<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintBinary<char>(begin, end, ptr);
}

char* PrintBinary(char* begin, char* end, uint8_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int8_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, uint16_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int16_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, uint32_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int32_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, uint64_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int64_t value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, float value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, double value) {
  return PrintBinary<char>(begin, end, value);
}

char* PrintMemory(char* begin, char* end, const void* start, size_t size) {
  return PrintMemory<char>(begin, end, start,
                           reinterpret_cast<const char*>(start) + size);
}

char* PrintMemory(char* begin, char* end, const void* start, const void* stop) {
  return PrintMemory<char>(begin, end, start, stop);
}
const char* TextScan(const char* string, int8_t& result) {
  return TextScanSigned<int8_t, char>(string, result);
}

const char* TextScanSigned(const char* string, uint8_t& result) {
  return TextScanUnsigned<uint8_t, char>(string, result);
}

const char* TextScanSigned(const char* string, int16_t& result) {
  return TextScanSigned<int16_t, char>(string, result);
}

const char* TextScanSigned(const char* string, uint16_t& result) {
  return TextScanUnsigned<uint16_t, char>(string, result);
}

const char* TextScanSigned(const char* string, int32_t& result) {
  return TextScanSigned<int32_t, char>(string, result);
}

const char* TextScanSigned(const char* string, uint32_t& result) {
  return TextScanUnsigned<uint32_t, char>(string, result);
}

const char* TextScanSigned(const char* string, int64_t& result) {
  return TextScanSigned<int64_t, char>(string, result);
}

const char* TextScanSigned(const char* string, uint64_t& result) {
  return TextScanUnsigned<uint64_t, char>(string, result);
}

const char* TextScan(const char* string, float& result) {
  return TextScan<char>(string, result);
}

const char* TextScan(const char* string, double& result) {
  return TextScan<char>(string, result);
}

const char* TextScanTime(const char* string, int& hour, int& minute,
                         int& second) {
  return TextScanTime<char>(string, hour, minute, second);
}

const char* TextScanTime(const char* string, TimeSeconds& time) {
  return TextScanTime<char>(string, time);
}

const char* TextScanTime(const char* string, time_t& result) {
  return TextScanTime<char>(string, result);
}

Printer1::Printer1(char* begin, size_t buffer_size)
    : begin(begin), end(begin + buffer_size - 1) {
  ASSERT(begin);
  ASSERT(buffer_size);
}

Printer1::Printer1(char* begin, char* end) : begin(begin), end(end) {
  ASSERT(begin);
  ASSERT(begin < end);
}

Printer1::Printer1(const Printer1& other) : begin(other.begin), end(other.end) {
  ASSERT(begin);
  ASSERT(end);
}

Printer1& Printer1::Set(char* new_cursor) {
  begin = new_cursor;
  return *this;
}

Printer1& Printer1::Hex(uint8_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(int8_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(uint16_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(int16_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(uint32_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(int32_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(uint64_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(int64_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(float value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(double value) {
  return Set(PrintHex<char>(begin, end, value));
}

Printer1& Printer1::Hex(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintHex<char>(begin, end, ptr));
}

Printer1& Printer1::Binary(uint8_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(int8_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(uint16_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(int16_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(uint32_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(int32_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(uint64_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(int64_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(float value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(double value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Printer1& Printer1::Binary(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintBinary<char>(begin, end, ptr));
}

Text1::Text1() {}

Text1::Text1(char character) {
  *string = character;
  *(string + 1) = 0;
}

Text1::Text1(int32_t value) { Print<char>(string, string + kSize - 1, value); }

Text1::Text1(uint32_t value) { Print<char>(string, string + kSize - 1, value); }

Text1::Text1(int64_t value) { Print<char>(string, string + kSize - 1, value); }

Text1::Text1(uint64_t value) { Print<char>(string, string + kSize - 1, value); }

Text1::Text1(float value) { Print<char>(string, string + kSize - 1, value); }

Text1::Text1(double value) { Print<char>(string, string + kSize - 1, value); }

const char* Text1::GetString() { return string; }

Center1::Center1(const char* string, int column_count)
    : string(string), column_count(column_count) {}

Center1::Center1(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center1::Center1(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center1::Center1(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center1::Center1(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center1::Center1(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center1::Center1(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char* Center1::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Center1::GetColumnCount() { return column_count; }

Right1::Right1(const char* string, int column_count)
    : string(string), column_count(column_count) {}

Right1::Right1(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right1::Right1(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right1::Right1(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right1::Right1(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right1::Right1(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right1::Right1(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char* Right1::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Right1::GetColumnCount() { return column_count; }

}  // namespace _

_::Printer1& operator<<(_::Printer1& printer, const char* string) {
  return printer.Set(_::Print(printer.begin, printer.end, string));
}

_::Printer1& operator<<(_::Printer1& printer, char value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, uint8_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, int16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, uint16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, int32_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, uint32_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, int64_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, uint64_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, float value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, double value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer1& operator<<(_::Printer1& printer, _::Center1 item) {
  printer.Set(_::PrintCenter(printer.begin, printer.end, item.GetString(),
                             item.GetColumnCount()));
}

_::Printer1& operator<<(_::Printer1& printer, _::Right1 item) {
  printer.Set(_::PrintRight(printer.begin, printer.end, item.GetString(),
                            item.GetColumnCount()));
}

#endif  //< #if USING_UTF8

#if USING_UTF16
#include "utf16.h"
namespace _ {

const char16_t* Empty2() {
  static const char16_t kString[] = {'\0'};
  return kString;
}

const char16_t* NewLine2() {
  static const char16_t kString[] = {'\n'};
  return kString;
}

const char16_t* ErrorHeader2() {
  static const char16_t kString[] = {'\n', 'E', 'r', 'r', 'o', 'r', ':', '\0'};
  return kString;
}

bool IsWhitespace(char16_t character) {
  return IsWhitespace<char16_t>(character);
}

char16_t Char(char16_t value) { return PrintableChar<char16_t>(value); }

const char16_t* TextEnd(const char16_t* text, char16_t delimiter) {
  return TextEnd<char16_t>(text, delimiter);
}

int TextLength(const char16_t* text, char16_t delimiter) {
  return TextLength<char16_t>(text, delimiter);
}

char16_t* TextClone(const char16_t* text, char16_t delimiter) {
  return TextClone<char16_t>(text, delimiter);
}

const char16_t* TextLineEnd(const char16_t* text, int column_count,
                            char16_t delimiter) {
  return TextLineEnd<char16_t>(text, column_count, delimiter);
}

const char16_t* TextLineEnd(const char16_t* text, const char16_t* text_end,
                            int column_count, char16_t delimiter) {
  return TextLineEnd<char16_t>(text, text_end, column_count, delimiter);
}

const char16_t* TextNumberEnd(const char16_t* text, const char16_t* text_end,
                              char16_t delimiter) {
  return TextNumberEnd<char16_t>(text, text_end, delimiter);
}

const char16_t* TextNumberEnd(const char16_t* text) {
  return TextNumberEnd<char16_t>(text);
}

const char16_t* TextSkipChar(const char16_t* text, char16_t skip_char,
                             char16_t delimiter) {
  return TextSkipChar<char16_t>(text, skip_char, delimiter);
}

const char16_t* TextSkipSpaces(const char16_t* text) {
  return TextSkipSpaces<char16_t>(text);
}

const char16_t* TextSkipSpaces(const char16_t* text, const char16_t* text_end) {
  return TextSkipSpaces<char16_t>(text, text_end);
}

const char16_t* TextEquals(const char16_t* text_a, const char16_t* text_b,
                           char16_t delimiter) {
  return TextEquals<char16_t>(text_a, text_b, delimiter);
}

const char16_t* TextEquals(const char16_t* text, const char16_t* text_end,
                           const char16_t* query, char16_t delimiter) {
  return TextEquals<char16_t>(text, text_end, query, delimiter);
}

bool TextQualifies(const char16_t* text, char16_t delimiter) {
  return TextQualifies<char16_t>(text, delimiter);
}

bool TextQualifies(const char16_t* text, const char16_t* text_end,
                   char16_t delimiter) {
  return TextQualifies<char16_t>(text, text_end, delimiter);
}

const char16_t* TextEquals(const char16_t* text_a, const char16_t* text_b) {
  return TextEquals<char16_t>(text_a, text_b);
}

int TextCompare(const char16_t* text_a, const char16_t* text_b) {
  return TextCompare<char16_t>(text_a, text_b);
}

int TextCompare(const char16_t* text_a, const char16_t* text_end,
                const char16_t* text_b) {
  return TextCompare<char16_t>(text_a, text_end, text_b);
}

const char16_t* TextFind(const char16_t* text, const char16_t* query,
                         char16_t delimiter) {
  return TextFind<char16_t>(text, query, delimiter);
}

char16_t* Print(char16_t* begin, char16_t* end, const char16_t* string) {
  return Print<char16_t>(begin, end, string);
}

char16_t* Print(char16_t* begin, char16_t* end, char16_t character) {
  return Print<char16_t>(begin, end, character);
}

char16_t* Print(char16_t* begin, char16_t* end, uint32_t value) {
  return Print<char16_t>(begin, end, value);
}

char16_t* Print(char16_t* begin, char16_t* end, int32_t value) {
  return Print<char16_t>(begin, end, value);
}

char16_t* Print(char16_t* begin, char16_t* end, uint64_t value) {
  return Print<char16_t>(begin, end, value);
}

char16_t* Print(char16_t* begin, char16_t* end, int64_t value) {
  return Print<char16_t>(begin, end, value);
}

char16_t* Print(char16_t* begin, char16_t* end, float value) {
  return Print<char16_t>(begin, end, value);
}

char16_t* Print(char16_t* begin, char16_t* end, double value) {
  return Print<char16_t>(begin, end, value);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, const char16_t* string,
                      int column_count) {
  return PrintCenter<char16_t>(begin, end, string, column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, char16_t character,
                      int column_count) {
  return PrintCenter<char16_t>(begin, end, Text2(character).GetString(),
                               column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, int32_t value,
                      int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, uint32_t value,
                      int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, uint64_t value,
                      int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, int64_t value,
                      int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, float value,
                      int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintCenter(char16_t* begin, char16_t* end, double value,
                      int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, const char16_t* string,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, string, column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, char16_t character,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(character).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, uint32_t value,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, int32_t value,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, uint64_t value,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, int64_t value,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, float value,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintRight(char16_t* begin, char16_t* end, double value,
                     int column_count) {
  return PrintRight<char16_t>(begin, end, Text2(value).GetString(),
                              column_count);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintHex<char16_t>(begin, end, ptr);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint8_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int8_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint16_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int16_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint32_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int32_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint64_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int64_t value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, float value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, double value) {
  return PrintHex<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintBinary<char16_t>(begin, end, ptr);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, uint8_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, int8_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, uint16_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, int16_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, uint32_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, int32_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, uint64_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, int64_t value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, float value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, double value) {
  return PrintBinary<char16_t>(begin, end, value);
}

char16_t* PrintMemory(char16_t* begin, char16_t* end, const void* start,
                      size_t size) {
  return PrintMemory<char16_t>(begin, end, start,
                               reinterpret_cast<const char16_t*>(start) + size);
}

char16_t* PrintMemory(char16_t* begin, char16_t* end, const void* start,
                      const void* stop) {
  return PrintMemory<char16_t>(begin, end, start, stop);
}

const char16_t* TextScan(const char16_t* string, int8_t& result) {
  return TextScanSigned<int8_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, uint8_t& result) {
  return TextScanUnsigned<uint8_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, int16_t& result) {
  return TextScanSigned<int16_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, uint16_t& result) {
  return TextScanUnsigned<uint16_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, int32_t& result) {
  return TextScanSigned<int32_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, uint32_t& result) {
  return TextScanUnsigned<uint32_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, int64_t& result) {
  return TextScanSigned<int64_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, uint64_t& result) {
  return TextScanUnsigned<uint64_t, char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, float& result) {
  return TextScan<char16_t>(string, result);
}

const char16_t* TextScan(const char16_t* string, double& result) {
  return TextScan<char16_t>(string, result);
}

const char16_t* TextScanTime(const char16_t* string, int& hour, int& minute,
                             int& second) {
  return TextScanTime<char16_t>(string, hour, minute, second);
}

const char16_t* TextScanTime(const char16_t* string, TimeSeconds& time) {
  return TextScanTime<char16_t>(string, time);
}

const char16_t* TextScanTime(const char16_t* string, time_t& result) {
  return TextScanTime<char16_t>(string, result);
}

Printer2::Printer2(char16_t* begin, size_t buffer_size)
    : begin(begin), end(begin + buffer_size - 1) {
  ASSERT(begin);
  ASSERT(buffer_size);
}

Printer2::Printer2(char16_t* begin, char16_t* end) : begin(begin), end(end) {
  ASSERT(begin);
  ASSERT(begin < end);
}

Printer2::Printer2(const Printer2& other) : begin(other.begin), end(other.end) {
  ASSERT(begin);
  ASSERT(end);
}

Printer2& Printer2::Set(char16_t* new_cursor) {
  begin = new_cursor;
  return *this;
}

Printer2& Printer2::Hex(uint8_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(int8_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(uint16_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(int16_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(uint32_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(int32_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(uint64_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(int64_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(float value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(double value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Printer2& Printer2::Hex(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintHex<char16_t>(begin, end, ptr));
}

Printer2& Printer2::Binary(uint8_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(int8_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(uint16_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(int16_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(uint32_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(int32_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(uint64_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(int64_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(float value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(double value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Printer2& Printer2::Binary(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintBinary<char16_t>(begin, end, ptr));
}

Text2::Text2() {}

Text2::Text2(char16_t character) {
  *string = character;
  *(string + 1) = 0;
}

Text2::Text2(int32_t value) {
  Print<char16_t>(string, string + kSize - 1, value);
}

Text2::Text2(uint32_t value) {
  Print<char16_t>(string, string + kSize - 1, value);
}

Text2::Text2(int64_t value) {
  Print<char16_t>(string, string + kSize - 1, value);
}

Text2::Text2(uint64_t value) {
  Print<char16_t>(string, string + kSize - 1, value);
}

Text2::Text2(float value) {
  Print<char16_t>(string, string + kSize - 1, value);
}

Text2::Text2(double value) {
  Print<char16_t>(string, string + kSize - 1, value);
}

const char16_t* Text2::GetString() { return string; }

Center2::Center2(const char16_t* string, int column_count)
    : string(string), column_count(column_count) {}

Center2::Center2(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center2::Center2(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center2::Center2(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center2::Center2(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center2::Center2(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center2::Center2(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char16_t* Center2::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Center2::GetColumnCount() { return column_count; }

Right2::Right2(const char16_t* string, int column_count)
    : string(string), column_count(column_count) {}

Right2::Right2(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right2::Right2(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right2::Right2(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right2::Right2(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right2::Right2(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right2::Right2(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char16_t* Right2::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Right2::GetColumnCount() { return column_count; }

}  // namespace _

_::Printer2& operator<<(_::Printer2& printer, const char16_t* string) {
  return printer.Set(_::Print(printer.begin, printer.end, string));
}

_::Printer2& operator<<(_::Printer2& printer, char16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, uint8_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, int16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, uint16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, int32_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, uint32_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, int64_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, uint64_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, float value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, double value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer2& operator<<(_::Printer2& printer, _::Center2 item) {
  printer.Set(_::PrintCenter(printer.begin, printer.end, item.GetString(),
                             item.GetColumnCount()));
}

_::Printer2& operator<<(_::Printer2& printer, _::Right2 item) {
  printer.Set(_::PrintRight(printer.begin, printer.end, item.GetString(),
                            item.GetColumnCount()));
}

#endif  //< #if USING_UTF16

#if USING_UTF32
#include "utf32.h"
namespace _ {

const char32_t* Empty4() {
  static const char32_t kString[] = {'\0'};
  return kString;
}

const char32_t* NewLine4() {
  static const char32_t kString[] = {'\n'};
  return kString;
}

const char32_t* ErrorHeader4() {
  static const char32_t kString[] = {'\n', 'E', 'r', 'r', 'o', 'r', ':', '\0'};
  return kString;
}

bool IsWhitespace(char32_t character) {
  return IsWhitespace<char32_t>(character);
}

char32_t Char(char32_t value) { return PrintableChar<char32_t>(value); }

const char32_t* TextEnd(const char32_t* text, char32_t delimiter) {
  return TextEnd<char32_t>(text, delimiter);
}

int TextLength(const char32_t* text, char32_t delimiter) {
  return TextLength<char32_t>(text, delimiter);
}

char32_t* TextClone(const char32_t* text, char32_t delimiter) {
  return TextClone<char32_t>(text, delimiter);
}

const char32_t* TextLineEnd(const char32_t* text, int column_count) {
  return TextLineEnd<char32_t>(text, column_count);
}

const char32_t* TextLineEnd(const char32_t* text, const char32_t* text_end,
                            int column_count) {
  return TextLineEnd<char32_t>(text, text_end, column_count);
}

const char32_t* TextNumberEnd(const char32_t* text, const char32_t* text_end) {
  return TextNumberEnd<char32_t>(text, text_end);
}

const char32_t* TextNumberEnd(const char32_t* text) {
  return TextNumberEnd<char32_t>(text);
}

const char32_t* TextSkipChar(const char32_t* text, char32_t skip_char) {
  return TextSkipChar<char32_t>(text, skip_char);
}

const char32_t* TextSkipSpaces(const char32_t* text) {
  return TextSkipSpaces<char32_t>(text);
}

const char32_t* TextSkipSpaces(const char32_t* text, const char32_t* text_end) {
  return TextSkipSpaces<char32_t>(text, text_end);
}

const char32_t* TextEquals(const char32_t* text_a, const char32_t* text_b,
                           char32_t delimiter) {
  return TextEquals<char32_t>(text_a, text_b);
}

bool TextQualifies(const char32_t* text, char32_t delimiter) {
  return TextQualifies<char32_t>(text, delimiter);
}

bool TextQualifies(const char32_t* text, const char32_t* text_end,
                   char32_t delimiter) {
  return TextQualifies<char32_t>(text, text_end, delimiter);
}

const char32_t* TextEquals(const char32_t* text_a, const char32_t* text_b) {
  return TextEquals<char32_t>(text_a, text_b);
}

int TextCompare(const char32_t* text_a, const char32_t* text_b) {
  return TextCompare<char32_t>(text_a, text_b);
}

int TextCompare(const char32_t* text_a, const char32_t* text_end,
                const char32_t* text_b) {
  return TextCompare<char32_t>(text_a, text_end, text_b);
}

const char32_t* TextFind(const char32_t* text, const char32_t* query,
                         char32_t delimiter) {
  return TextFind<char32_t>(text, query, delimiter);
}

char32_t* Print(char32_t* begin, char32_t* end, const char32_t* string) {
  return Print<char32_t>(begin, end, string);
}

char32_t* Print(char32_t* begin, char32_t* end, char32_t character) {
  return Print<char32_t>(begin, end, character);
}

char32_t* Print(char32_t* begin, char32_t* end, uint32_t value) {
  return Print<char32_t>(begin, end, value);
}

char32_t* Print(char32_t* begin, char32_t* end, int32_t value) {
  return Print<char32_t>(begin, end, value);
}

char32_t* Print(char32_t* begin, char32_t* end, uint64_t value) {
  return Print<char32_t>(begin, end, value);
}

char32_t* Print(char32_t* begin, char32_t* end, int64_t value) {
  return Print<char32_t>(begin, end, value);
}

char32_t* Print(char32_t* begin, char32_t* end, float value) {
  return Print<char32_t>(begin, end, value);
}

char32_t* Print(char32_t* begin, char32_t* end, double value) {
  return Print<char32_t>(begin, end, value);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, const char32_t* string,
                      int column_count) {
  return PrintCenter<char32_t>(begin, end, string, column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, char32_t character,
                      int column_count) {
  return PrintCenter<char32_t>(begin, end, Text4(character).GetString(),
                               column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, int32_t value,
                      int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, uint32_t value,
                      int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, uint64_t value,
                      int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, int64_t value,
                      int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, float value,
                      int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintCenter(char32_t* begin, char32_t* end, double value,
                      int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, const char32_t* string,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, string, column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, char32_t character,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(character).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, uint32_t value,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, int32_t value,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, uint64_t value,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, int64_t value,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, float value,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintRight(char32_t* begin, char32_t* end, double value,
                     int column_count) {
  return PrintRight<char32_t>(begin, end, Text4(value).GetString(),
                              column_count);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintHex<char32_t>(begin, end, ptr);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint8_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int8_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint16_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int16_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint32_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int32_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint64_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int64_t value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, float value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, double value) {
  return PrintHex<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintBinary<char32_t>(begin, end, ptr);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, uint8_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, int8_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, uint16_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, int16_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, uint32_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, int32_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, uint64_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, int64_t value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, float value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, double value) {
  return PrintBinary<char32_t>(begin, end, value);
}

char32_t* PrintMemory(char32_t* begin, char32_t* end, const void* start,
                      size_t size) {
  return PrintMemory<char32_t>(begin, end, start,
                               reinterpret_cast<const char32_t*>(start) + size);
}

char32_t* PrintMemory(char32_t* begin, char32_t* end, const void* start,
                      const void* stop) {
  return PrintMemory<char32_t>(begin, end, start, stop);
}
const char32_t* TextScan(const char32_t* string, int8_t& result) {
  return TextScanSigned<int8_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint8_t& result) {
  return TextScanUnsigned<uint8_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, int16_t& result) {
  return TextScanSigned<int16_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint16_t& result) {
  return TextScanUnsigned<uint16_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, int32_t& result) {
  return TextScanSigned<int32_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint32_t& result) {
  return TextScanUnsigned<uint32_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, int64_t& result) {
  return TextScanSigned<int64_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint64_t& result) {
  return TextScanUnsigned<uint64_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, float& result) {
  return TextScan<char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, double& result) {
  return TextScan<char32_t>(string, result);
}

Printer4::Printer4(char32_t* begin, size_t buffer_size)
    : begin(begin), end(begin + buffer_size - 1) {
  ASSERT(begin);
  ASSERT(buffer_size);
}

Printer4::Printer4(char32_t* begin, char32_t* end) : begin(begin), end(end) {
  ASSERT(begin);
  ASSERT(begin < end);
}

Printer4::Printer4(const Printer4& other) : begin(other.begin), end(other.end) {
  ASSERT(begin);
  ASSERT(end);
}

Printer4& Printer4::Set(char32_t* new_cursor) {
  begin = new_cursor;
  return *this;
}

Printer4& Printer4::Hex(uint8_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(int8_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(uint16_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(int16_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(uint32_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(int32_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(uint64_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(int64_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(float value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(double value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Printer4& Printer4::Hex(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintHex<char32_t>(begin, end, ptr));
}

Printer4& Printer4::Binary(uint8_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(int8_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(uint16_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(int16_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(uint32_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(int32_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(uint64_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(int64_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(float value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(double value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Printer4& Printer4::Binary(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintBinary<char32_t>(begin, end, ptr));
}
const char32_t* TextScan(const char32_t* string, int8_t& result) {
  return TextScanSigned<int8_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint8_t& result) {
  return TextScanUnsigned<uint8_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, int16_t& result) {
  return TextScanSigned<int16_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint16_t& result) {
  return TextScanUnsigned<uint16_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, int32_t& result) {
  return TextScanSigned<int32_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint32_t& result) {
  return TextScanUnsigned<uint32_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, int64_t& result) {
  return TextScanSigned<int64_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, uint64_t& result) {
  return TextScanUnsigned<uint64_t, char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, float& result) {
  return TextScan<char32_t>(string, result);
}

const char32_t* TextScan(const char32_t* string, double& result) {
  return TextScan<char32_t>(string, result);
}

const char* TextScanTime(const char* string, int& hour, int& minute,
                         int& second) {
  return TextScanTime<char>(string, hour, minute, second);
}

const char* TextScanTime(const char* string, TimeSeconds& time) {
  return TextScanTime<char>(string, time);
}

const char* TextScanTime(const char* string, time_t& result) {
  return TextScanTime<char>(string, result);
}

Text4::Text4() {}

Text4::Text4(char32_t character) {
  *string = character;
  *(string + 1) = 0;
}

Text4::Text4(int32_t value) {
  Print<char32_t>(string, string + kSize - 1, value);
}

Text4::Text4(uint32_t value) {
  Print<char32_t>(string, string + kSize - 1, value);
}

Text4::Text4(int64_t value) {
  Print<char32_t>(string, string + kSize - 1, value);
}

Text4::Text4(uint64_t value) {
  Print<char32_t>(string, string + kSize - 1, value);
}

Text4::Text4(float value) {
  Print<char32_t>(string, string + kSize - 1, value);
}

Text4::Text4(double value) {
  Print<char32_t>(string, string + kSize - 1, value);
}

const char32_t* Text4::GetString() { return string; }

Center4::Center4(const char32_t* string, int column_count)
    : string(string), column_count(column_count) {}

Center4::Center4(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center4::Center4(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center4::Center4(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center4::Center4(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center4::Center4(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Center4::Center4(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char32_t* Center4::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Center4::GetColumnCount() { return column_count; }

Right4::Right4(const char32_t* string, int column_count)
    : string(string), column_count(column_count) {}

Right4::Right4(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right4::Right4(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right4::Right4(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right4::Right4(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right4::Right4(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Right4::Right4(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char32_t* Right4::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Right4::GetColumnCount() { return column_count; }

}  // namespace _

_::Printer4& operator<<(_::Printer4& printer, const char32_t* string) {
  return printer.Set(_::Print(printer.begin, printer.end, string));
}

_::Printer4& operator<<(_::Printer4& printer, char32_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, uint8_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, int16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, uint16_t value) {
  return printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, int32_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, uint32_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, int64_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, uint64_t value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, float value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, double value) {
  printer.Set(_::Print(printer.begin, printer.end, value));
}

_::Printer4& operator<<(_::Printer4& printer, _::Center4 item) {
  printer.Set(_::PrintCenter(printer.begin, printer.end, item.GetString(),
                             item.GetColumnCount()));
}

_::Printer4& operator<<(_::Printer4& printer, _::Right4 item) {
  printer.Set(_::PrintRight(printer.begin, printer.end, item.GetString(),
                            item.GetColumnCount()));
}

#endif  //< #if USING_UTF32

#undef PRINTF
#undef PUTCHAR
#undef PRINT_HEADING
#endif  //< USING_PRINTER
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
