/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/utfn.h
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
#ifndef HEADER_FOR_CRABS_UTFN
#define HEADER_FOR_CRABS_UTFN
// Dependencies:
#include "assert.h"
#include "config.h"
#include "number.h"
#include "print_floating_point.h"
#include "print_integer.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#else
#define PRINTF(x, ...)
#define PRINT(c)
#endif

namespace _ {

template <typename Char>
const Char* StringEmpty() {
  static const Char kString[] = {'\0'};
  return kString;
}

template <typename Char>
const Char* StringNL() {
  static const Char kString[] = {'\n'};
  return kString;
}

template <typename Char>
const Char* StringError() {
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
inline const Char* TextEnd(const Char* cursor) {
  Char c = *cursor;
  while (c > 0) c = *(++cursor);
  return cursor;
}

template <typename Char, typename I = int>
I TextLength(const Char* cursor) {
  ASSERT(cursor);
  I count = 0;
  Char c = *cursor;
  while (c) {
    I upper_bounds = 0;            //< Faster to make from instructions then
    upper_bounds = ~upper_bounds;  //< load from ROM.
    if (count == upper_bounds) return 0;
    ++count;
    c = *(++cursor);
  }
  return count;
}

template <typename Char, typename I = int>
I TextSize(const Char* cursor) {
  ASSERT(cursor);
  I count = 0;
  Char c = *cursor;
  while (c) {
    I upper_bounds = 0;            //< Faster to make from instructions then
    upper_bounds = ~upper_bounds;  //< load from ROM.
    if (count == upper_bounds) return 0;
    ++count;
    c = *(++cursor);
  }
  return count;
}

template <typename Char>
Char* TextClone(const Char* begin) {
  ASSERT(begin);
  int length = TextLength<Char>(begin) + 1;
  if (length < 0) return nullptr;
  Char* clone = new Char[length];
  SocketCopy(clone, clone + length, begin, begin + length + 1);
  return clone;
}

template <typename Char>
const Char* TextLineEnd(const Char* cursor, int column_count) {
  Char c;
  // Scroll to the end of the line.
  c = *cursor++;
  while (c) {
    if (column_count-- < 0) {  // We've reached the end.
                               // Scroll left till we hit whitespace (if any).
      while (!IsWhitespace<Char>(c)) c = *(--cursor);
      // Then scroll past the whitespace.
      while (IsWhitespace<Char>(c)) c = *(--cursor);
      return cursor;
      c = *cursor++;
    }
    c = *cursor++;
  }
  return cursor - 1;
}

template <typename Char>
const Char* TextLineEnd(const Char* cursor, const Char* end, int column_count) {
  if (!cursor) {
    PRINTF("\nText buffer overflow!");
    return nullptr;
  }
  ASSERT(cursor < end);
  Char c;
  // Scroll to the end of the line.
  c = *cursor++;
  while (c) {
    if (end > cursor) return nullptr;
    if (column_count-- < 0) {  // We've reached the end.

      // Scroll left till we hit whitespace (if any).
      while (!IsWhitespace<Char>(c)) c = *(--cursor);
      // Then scroll past the whitespace.
      while (IsWhitespace<Char>(c)) c = *(--cursor);
      return cursor;
      c = *cursor++;
    }
    c = *cursor++;
  }
  return cursor - 1;
}

template <typename Char>
const Char* TextNumberEnd(const Char* cursor, const Char* end) {
  if (cursor > end) return nullptr;
  Char c = *cursor;
  if (c == '-') {  // It might be negative.
    if (cursor > end) return nullptr;
    c = *(++cursor);
    if (!IsDigit<Char>(c)) return cursor - 1;  //< it's not negative.
    c = *(++cursor);
  }
  while (IsDigit<Char>(c)) {
    if (cursor > end) return nullptr;
    c = *(++cursor);
  }
  return cursor;
}

template <typename Char>
bool IsDigit(Char c) {
  return (c >= '0') && (c <= '9');
}

template <typename Char>
const Char* TextNumberEnd(const Char* cursor) {
  ASSERT(cursor);
  Char c = *cursor;
  if (c < '0' || c > '9') return nullptr;
  if (c == '-') {  // It might be negative.
    c = *(++cursor);
    if (!IsDigit<Char>(c)) return cursor - 1;  //< it's not negative.
    c = *(++cursor);
  }
  while (IsDigit<Char>(c)) {
    if (c <= 0) return cursor;
    c = *(++cursor);
  }
  return cursor;
}

template <typename Char>
const Char* TextSkipChar(const Char* cursor, Char skip_char) {
  if (cursor == nullptr) return nullptr;
  Char c = *cursor, d;
  if (c != skip_char) return cursor;
  d = *cursor;
  // We know the first Char is a skip_char so just loop till c and d aren't
  // the skip_char.
  while (c == d) {
    c = d;
    d = *(++cursor);
  }
  return cursor;
}

template <typename Char>
Char* TextSkipChar(Char* cursor, Char skip_char) {
  return const_cast<Char*>(TextSkipChar<Char>(cursor, skip_char));
}

template <typename Char>
const Char* TextSkipSpaces(const Char* cursor) {
  if (!cursor) return nullptr;
  Char c = *cursor;
  while (IsWhitespace<Char>(c)) {
    PRINT('.');
    if (!c) return cursor;  //< This isn't an error as far as I can see.
    ++cursor;
    c = *cursor;
  }
  return cursor;
}

template <typename Char>
const Char* TextSkipSpaces(const Char* cursor, const Char* end) {
  if (!cursor) return nullptr;
  if (cursor > end) return nullptr;
  PRINTF("\nSkipping spaces: ");
  Char c = *cursor;
  while (IsWhitespace(c)) {
    PRINT('.');
    if (!c) return nullptr;
    if (++cursor > end) return nullptr;
    c = *cursor;
  }
  return cursor;
}

template <typename Char>
const Char* TextEquals(const Char* text_a, const Char* text_b) {
  ASSERT(text_a);
  ASSERT(text_b);
  PRINTF("\nComparing \"%s\" to \"%s\"", text_a, text_b);

  Char a = *text_a, b = *text_b;
  while (a) {
    PRINT(a);
    if (a != b) {  // Not a hit.
      PRINTF("\nBut it's not a hit");
      return nullptr;
    }
    if (b == 0) {  // Hit!
      PRINTF("\nFound hit at 0x%p", text_a);
      return text_a;
    }
    a = *(++text_a);
    b = *(++text_b);
  }
  if (b) {
    PRINTF("\nNot a hit: no nil-term Char found");
    return nullptr;
  }
  PRINTF("\nFound hit at 0x%p", text_a);
  return text_a;  //< Find hit!
}

template <typename Char>
const Char* TextEquals(const Char* cursor, const Char* end, const Char* query) {
  if (!cursor) return nullptr;
  ASSERT(cursor < end);
  if (!query) return nullptr;

  PRINTF("\nComparing \"%s\" to \"%s\"", cursor, query);

  Char a = *cursor, b = *query;
  while (a) {
    PRINT(a);
    if (a != b) {  // Not a hit.
      PRINTF("\nBut it's not a hit");
      return nullptr;
    }
    if (b == 0) {  // Hit!
      PRINTF("\nFound hit at 0x%p", cursor);
      return cursor;
    }
    if (cursor > end) {
      return nullptr;
    }
    a = *(++cursor);
    b = *(++query);
  }
  if (b) {
    PRINTF("\nNot a hit: no nil-term Char found");
    return nullptr;
  }
  PRINTF("\nFound hit at 0x%p.", cursor);
  return cursor;
}

template <typename Char>
bool TextQualifies(const Char* cursor) {
  if (cursor == nullptr) return false;
  Char c = *cursor;
  while (c) {
    if (!IsWhitespace<Char>(c)) return true;
    c = *(++cursor);
  }
  return false;
}

template <typename Char>
bool TextQualifies(const Char* cursor, const Char* end) {
  if (!cursor) return false;
  if (cursor > end) return false;
  Char c = *cursor;
  while (c) {
    if (!IsWhitespace<Char>(c)) {
      // The text must end at or before the target_end.
      do {
        if (++cursor > end) return false;
        c = *cursor;
        if (!IsWhitespace<Char>(c)) return true;
      } while (c);
      return true;
    }
    if (++cursor > end) return false;
    c = *cursor;
  }
  return false;
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
int TextCompare(const Char* cursor, const Char* end, const Char* query) {
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
    if (++cursor > end) {
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

template <typename Char>
const Char* TextFind(const Char* cursor, const Char* query) {
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
        if (t == 0) {             // Once we've reached the delimiter
          return start_of_query;  // it's a match!
        }
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

template <typename Char>
const Char* TextSkipCharsInRange(const Char* cursor, Char lower_bounds,
                                 Char upper_bounds) {
  ASSERT(cursor);
  ASSERT(lower_bounds < upper_bounds);
  Char c = *cursor;
  while (c >= lower_bounds && c <= upper_bounds) c = *(++cursor);
  return cursor;
}

template <typename Char>
inline const Char* TextSkipNumbers(const Char* cursor) {
  return TextSkipCharsInRange<Char>(cursor, '0', '9');
}

template <typename Char>
Char* Print(Char* cursor, Char* end, const Char* string) {
  ASSERT(cursor);
  ASSERT(string);

  if (cursor >= end) {
    return nullptr;
  }

  char c = *string++;
  while (c) {
    *cursor++ = c;
    if (cursor >= end) return nullptr;
    c = *string++;
  }
  *cursor = 0;
  return cursor;
}

template <typename Char>
Char* PrintChar(Char* cursor, Char* end, Char character) {
  ASSERT(cursor);
  ASSERT(end);

  if (cursor + 1 >= end) return nullptr;

  *cursor++ = character;
  *cursor = 0;
  return cursor;
}

template <typename Char>
Char* PrintRight(Char* cursor, Char* end, const Char* token, int column_count) {
  Char* start = cursor;
  ASSERT(token);
  ASSERT(cursor);

  if (cursor >= end) return nullptr;
  if (cursor + column_count > end) return nullptr;

  // PRINTF ("\n\nPrinting \"%s\" aligned right %i columns", token,
  // column_count)

  int length = TextLength<Char>(token);
  Char c;  //< Temp variable.
  if (length == 0) return cursor;

  // PRINTF ("\n    Found length:%i", (int)length)

  *(cursor + length) = 0;
  // If the length is less than the column_count we need to print ".", "..",
  // "..." or nothing and chop off some of the token.
  if (length > column_count) {
    // PRINTF ("\n Wrote with dots:\"")
    intptr_t num_dots = length - column_count;
    if (num_dots > 3) {
      num_dots = 3;
    }
    end = cursor + column_count;
    cursor = end - num_dots;
    while (cursor < cursor) {
      c = *token++;
      *cursor++ = c;
      // PRINT (c)
    }
    while (cursor < end) {
      *cursor++ = '.';
      // PRINT ('.')
    }
    // PRINT ('\"')
    *cursor = 0;
    return cursor;
  }
  // PRINTF ("\n Wrote:\"")
  cursor = cursor + column_count;
  end = cursor - length;
  while (cursor >= end) {
    c = *end--;
    *cursor-- = c;
    // PRINT (c)
  }
  // PRINT ('\"')
  end = cursor - column_count;
  // PRINTF ("\ncursor:0x%p text:0x%x delta:%i", cursor, text, text, cursor)
  while (cursor >= cursor) {
    *cursor-- = ' ';
  }
  end = cursor + column_count;
  return end;
}

template <typename Char>
Char* PrintCenter(Char* cursor, Char* end, const Char* string,
                  int column_count) {
  ASSERT(cursor);
  ASSERT(cursor < end);

  // We need to leave at least one space to the left and right of
  int length = TextLength<Char>(string), offset;
  PRINTF("\n\n    Printing \"%s\":%i column_count:%i", string, length,
         column_count);

  if (column_count < length) {
    offset = length - column_count;
    if (offset > 3) {
      offset = 3;
    }
    column_count -= offset;
    while (column_count-- > 0) {
      *cursor++ = *string++;
    }
    while (offset-- > 0) {
      *cursor++ = '.';
    }
    *cursor = 0;
    return cursor;
  }
  offset = (column_count - length) >> 1;  //< >> 1 to /2
  length = column_count - length - offset;
  PRINTF("\n    length:%i offset:%i", length, offset);

  while (length-- > 0) *cursor++ = ' ';

  char c = *string++;
  while (c) {
    *cursor++ = c;
    c = *string++;
  }
  while (offset-- > 0) *cursor++ = ' ';
  *cursor = 0;
  PRINTF("\n    Printed:\"%s\"", string);
  return cursor;
}

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

template <typename Char>
Char* PrintLine(Char* cursor, Char* end, Char token, int column_count) {
  ASSERT(cursor);
  ASSERT(cursor < end);
  int l_column_count = column_count;
  if (cursor + l_column_count + 1 >= end) return nullptr;

  *cursor++ = '\n';

  while (l_column_count-- > 0) *cursor++ = token;

  *cursor = 0;
  return cursor;
}

template <typename Char>
Char* PrintLineString(Char* cursor, Char* end, const Char* string,
                      int column_count) {
  ASSERT(cursor);
  ASSERT(cursor < end);
  if (cursor + column_count + 1 > end) return nullptr;

  while (column_count-- > 0) {
    char c = *string++;
    if (!c) {
      cursor = 0;
    }
    *cursor++ = c;
  }
  *cursor = 0;
  return cursor;
}

template <typename Char>
Char* PrintMemory(Char* cursor, Char* end, const void* start,
                  const void* stop) {
  ASSERT(start);
  ASSERT(cursor);
  ASSERT(cursor < end);

  Char* buffer_begin = cursor;
  const Char *address_ptr = reinterpret_cast<const Char*>(start),
             *address_end_ptr = reinterpret_cast<const Char*>(stop);
  size_t size = address_end_ptr - address_ptr,
         num_rows = size / 64 + (size % 64 != 0) ? 1 : 0;

  intptr_t num_bytes = 81 * (num_rows + 2);
  if ((end - cursor) <= num_bytes) {
    PRINTF("\n    ERROR: buffer overflow trying to fit %i in %i bytes!",
           (int)num_bytes, (int)(end - cursor));
    return nullptr;
  }
  size += num_bytes;
  *cursor++ = '\n';
  *cursor++ = '|';

  //  columns
  *cursor++ = '0';
  cursor = PrintRight<Char>(cursor, end, Utf8Text(8).GetString(), 8);
  *cursor++ = ' ';
  for (int i = 16; i <= 56; i += 8)
    cursor = PrintRight(cursor, end, Utf8Text(i).GetString(), 8);
  for (int j = 6; j > 0; --j) *cursor++ = ' ';
  *cursor++ = '|';
  *cursor++ = '\n';
  *cursor++ = '|';
  for (int j = 8; j > 0; --j) {
    *cursor++ = '+';
    for (int k = 7; k > 0; --k) *cursor++ = '-';
  }
  *cursor++ = '|';
  *cursor++ = ' ';

  cursor =
      PrintHex<Char>(cursor, end, reinterpret_cast<uintptr_t>(address_ptr));

  PRINTF("\nBuffer space left:%i", (int)(end - cursor));
  Char c;
  while (address_ptr < address_end_ptr) {
    *cursor++ = '\n';
    *cursor++ = '|';
    for (int i = 0; i < 64; ++i) {
      c = *address_ptr++;
      if (address_ptr > address_end_ptr)
        c = 'x';
      else if (!c || c == TAB)
        c = ' ';
      else if (c < ' ')
        c = DEL;
      *cursor++ = c;
    }
    *cursor++ = '|';
    *cursor++ = ' ';
    cursor =
        PrintHex<Char>(cursor, end, reinterpret_cast<uintptr_t>(address_ptr));
  }
  *cursor++ = '\n';
  *cursor++ = '|';
  for (int j = 8; j > 0; --j) {
    *cursor++ = '+';
    for (int k = 7; k > 0; --k) {
      *cursor++ = '-';
    }
  }
  *cursor++ = '|';
  *cursor++ = ' ';
  return PrintHex<Char>(cursor, end,
                        reinterpret_cast<uintptr_t>(address_ptr) + size);
}

template <typename Char, typename UI = uint>
const Char* TextScanUnsigned(const Char* begin, UI& result) {
  ASSERT(begin);
  Char c = *begin;
  if (c < '0' || c > '9') return nullptr;
  const Char* cursor = TextNumberEnd<Char>(begin);
  if (cursor == begin) {
    result = c + '0';
    return cursor + 1;
  }
  const Char* end = cursor;  // Store end to return.
  UI power_of_ten = 10;
  c = *cursor--;
  UI value = (UI)c;
  while (cursor > begin) {
    UI new_power_of_ten = power_of_ten * 10;
    // First check for unsigned multiply loop-around condition.
    if (new_power_of_ten < power_of_ten) return nullptr;
    power_of_ten = new_power_of_ten;
    value += power_of_ten * (*cursor--);
  }
  value += power_of_ten * (*cursor--);
  result = value;
  return end;
}

template <typename Char, typename SI = int>
const Char* TextScanSigned(const Char* string, SI& result) {
  ASSERT(string);
  SI sign,                //< Sign of the signed integer being parsed
      power_of_ten = 10;  //< Power of 10 of current character.
  Char c = *string;
  const Char* cursor;
  if (c == '-') {
    c = *(++string);
    sign = -1;
  } else {
    sign = 1;
  }
  if (c < '0' || c > '9') return nullptr;  //< We need at least one digit.
  cursor = TextNumberEnd<Char>(string);
  if (!cursor) return nullptr;
  if (cursor == string) {
    result = sign * (c - '0');
    return cursor + 1;
  }
  SI value = (SI)c;
  while (cursor > string) {
    power_of_ten *= 10;
    value += power_of_ten * (*cursor--);
  }
  value += power_of_ten * (*cursor--);
  result = value * sign;
  return cursor;
}

template <typename Char>
const Char* TextScan(const Char* string, float& result) {
  return nullptr;
  /*
  int32_t integer;
  const Char* cursor = TextScanSigned<Char>(string, integer);
  if (cursor == nullptr) return cursor;
  float float_result = *(float*)&integer;
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
  if (Lowercase<Char>(c) == 'e') {
  int32_t exponent;
  string = TextScanSigned<int32_t, Char>(cursor);
  if (!string) return cursor;
  return result;
  }
  return result;*/
}

template <typename Char>
const Char* TextScan(const Char* string, double& result) {
  return nullptr;
}

template <typename Char>
Char Lowercase(Char c) {
  if ((c & 0x3f) == 3) return c - 64;
  return c;
}

/* Utility class for printing strings.
This class only stores the end of buffer pointer and a pointer to the write
begin. It is up the user to store start of buffer pointer and if they would
like to replace the begin with the beginning of buffer pointer when they
are done printing.
*/
template <typename Char>
struct KABUKI Utf {
  Char *begin,  //< Write begin pointer.
      *end;     //< End of buffer pointer.

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Utf(Char* begin, size_t size)
      : begin(begin), end(end >> BitShiftCount<Char>()) {
    ASSERT(begin);
  }

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Utf(Char* begin, Char* end) {}

  /* Clones the other print. */
  Utf(const Utf& other)
      : begin(other.begin), end(other.end) {  // Nothing to do here!.
  }

  /* Sets the begin pointer to the new_pointer. */
  inline Utf& Set(Char* new_pointer) {
    if (!new_pointer) return *this;
    begin = new_pointer;
    return *this;
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(int8_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(uint8_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(int16_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(uint16_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(int32_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(uint32_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(int64_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(uint64_t value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(float value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline Utf& Hex(double value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given pointer as hex. */
  inline Utf& Hex(const void* pointer) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(int8_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(uint8_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(int16_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(uint16_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(int32_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(uint32_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(int64_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(uint64_t value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(float value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline Utf& Binary(double value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given pointer as binary. */
  inline Utf& Binary(const void* pointer) {
    return Set(Binary<Char>(begin, end, value));
  }
};

/* Utility class for printing numbers. */
template <typename Char>
class UtfText {
 public:
  /* Default constructor does nothing. */
  UtfText() { *string = 0; }

  /* Prints the value to the text buffer. */
  UtfText(Char character) { Print<Char>(string, string + kSize, character); }

  /* Prints the value to the text buffer. */
  UtfText(int32_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  UtfText(uint32_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  UtfText(int64_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  UtfText(uint64_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  UtfText(float value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  UtfText(double value) { Print<Char>(string, string + kSize, value); }

  /* Gets the number string. */
  const Char* GetString() { return string; }

 private:
  enum { kSize = 24 };

  Char string[kSize * sizeof(Char) + 1];  //< String buffer.
};

/* Utility class for printing hex with operator<<. */
template <typename Char>
class UtfCenter {
 public:
  /* Prints the value to the text buffer. */
  UtfCenter(const Char* string, int column_count)
      : string((string == nullptr) ? StringEmpty<Char>() : string),
        column_coun(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfCenter(int32_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfCenter(uint32_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfCenter(int64_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfCenter(uint64_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfCenter(float value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfCenter(double value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Gets the number string. */
  const Char* GetString() { return string; }

  /* Gets the column_count. */
  int GetColumnCount() { return column_count; }

 private:
  const Char* string;  //< Pointer to the string.
  UtfText number;      //< Pointer to a pointer to print.
  int column_count;    //< Number of columns to center.
};

/* Utility class for printing hex with operator<<. */
template <typename Char>
class UtfRight {
 public:
  /* Prints the value to the text buffer. */
  UtfRight(const Char* string, int column_count)
      : string(string == nullptr) ? TextEmpty<Char>() : string),
        column_coun(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfRight(int32_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfRight(uint32_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfRight(int64_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfRight(uint64_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfRight(float value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  UtfRight(double value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Gets the number string. */
  const Char* GetString() { return string; }

  /* Gets the column_count. */
  int GetColumnCount() { return colun_count; }

 private:
  const Char* string;  //< Pointer to the string.
  UtfText number;      //< Pointer to a pointer to print.
  int column_count;    //< Number of columns to center.
};

/* Utility class for printing a single Char token line with operator<<. */
template <typename Char>
struct KABUKI UtfLine {
  Char token;        //< Character to print.
  int column_count;  //< Column count.

  /* Constructor. */
  UtfLine(Char token, int column_count)
      : token(token), column_count(column_count) {}
};

/* Utility class for printing a string line with operator<<. */
template <typename Char>
struct KABUKI UtfLineString {
  const Char* string;  //< Character to print.
  int column_count;    //< Column count.

  /* Constructor. */
  UtfLineString(const Char* string, int column_count)
      : string(string), column_count(column_count) {}
};

/* Buffer destructor prints the buffer to the console without deleting the
 * buffer. */
template <typename Char>
void COut(uintptr_t* buffer) {
  if (!buffer) return;
  Print<Char>(reinterpret_cast<char_t*>(buffer));
}

/* Buffer destructor prints the buffer to the console and deletes the buffer. */
template <typename Char>
void DCOut(uintptr_t* buffer) {
  if (!buffer) return;
  Print<Char>(reinterpret_cast<char_t*>(buffer));
  delete[] buffer;
}

/* A string that is statically or dynamically allocated and can do "stuff"
upon destruct.
This class is designed to take advantage of the behavior of the C++ operator
overloads. When you have a string of overloads, the objects get destructed in
the oposite order then where called, which is a stack push pop operation. For
this reason the destructor is programmable.

# Statically Allocated Strings

If it is null then the memory is treated as statically allocated memory.

@code
Str<> (&COut) << "Hello world!";
@endcode

# Dynamic Allocated Strings

Strings that use dynamic memory use the DCOut destructor:

@code
Str<uint32_t> (&DCOut) << "Hello world!";
@endcode
* */
template <typename Char = char_t, typename SI = schar_t>
class Str : public Utf {
 public:
  /* Constructs the Utf& pointers to the buffer_. */
  Str(Destructor destructor)
      : ascii_obj_(kSize, buffer, destructor),
        utf(ascii_obj_.GetBegin() + sizeof(Char), ascii_obj_.GetEnd()) {
    *ascii_obj_.GetEnd() = 0;
  }

  /* Constructs the Utf& pointers to the buffer_. */
  Str(intptr_t size) : Obj(kSize) { *ascii_obj_.GetBuffer() = 0; }

  /* Constructs the Utf& pointers to the buffer_. */
  Str(intptr_t size, uintptr_t* buffer) : Obj(kSize, buffer) {
    *ascii_obj_.GetBuffer() = 0;
  }

  /* Gets the begin of the Console buffer. */
  inline Obj& Obj() { return ascii_obj_; }

 private:
  Obj<SI> ascii_obj_;  //< ASCII Object.
};

#define COUT ::_::Str<>(&COut)

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, const Char* string) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, string));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, Char c) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, c));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, uint8_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, int16_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, uint16_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, int32_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, uint32_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, int64_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, uint64_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, float value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, double value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  item The item to write to print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, _::UtfCenter item) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, item));
}

/* Writes the given value to the print jusified right.
@return The utf.
@param  utf The utf.
@param  item The item to print. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, _::UtfRight item) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, item));
}

/* Prints a line of the given column_count to the utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, _::UtfLine line) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, line));
}

/* Prints a line string of the given column_count to the utf. */
template <typename Char>
KABUKI _::Utf& operator<<(_::Utf& utf, _::UtfLineString line) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, line));
}

}  // namespace _
#undef PRINT
#undef PRINTF
#endif  //< #if HEADER_FOR_CRABS_UTFN
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
