/* Kabuki Toolkit
@version 0.x
@file    /kabuki/f2/tstr.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if SEAM >= SEAM_0_0_0__01
#ifndef INCLUDED_KABUKI_F2_UTFN
#define INCLUDED_KABUKI_F2_UTFN

#include "str.h"

#include "ascii.h"
#include "tbinary.h"
#include "test.h"
#include "tobj.h"
#include "tsocket.h"

#include "01/seam_header.inl"

namespace _ {

template <typename Char = char>
const Char* StringEmpty() {
  static const Char kString[] = {'\0'};
  return kString;
}

template <typename Char = char>
const Char* StringNL() {
  static const Char kString[] = {'\n'};
  return kString;
}

template <typename Char = char>
const Char* StringError() {
  static const Char kString[] = {'\n', 'E', 'r', 'r', 'o', 'r', ':', '\0'};
  return kString;
}

/* Converts the given value to a printable char if it's non-printable. */
template <typename Char = char>
inline Char PrintableChar(Char value) {
  if (value < 32 || value == 127) return ' ';
  return value;
}

/* Sets the string to either the given value or empty string if input is
nil. */
template <typename Char = char>
inline Char* StringSet(Char* string) {
  if (!string) return StringEmpty<Char>();
  return string;
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

template <typename Char = char>
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

template <typename Char = char>
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

template <typename Char = char>
const Char* TextNumberStop(const Char* cursor, const Char* end) {
  ASSERT(cursor);
  ASSERT(cursor <= end);
  Char c = *cursor++;
  if (!c) return nullptr;
  if (c == '-') {  // It might be negative.
    if (cursor >= end) return nullptr;
    c = *cursor++;
  }
  if (!IsDigit<Char>(c)) return nullptr;
  while (c) {
    if (cursor >= end) return nullptr;
    if (!IsDigit<Char>(c)) return cursor - 1;
    c = *cursor++;
  }
  return cursor - 1;
}

template <typename Char = char>
const Char* TextNumberStop(const Char* cursor) {
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

template <typename Char = char>
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

template <typename Char = char>
Char* TextSkipChar(Char* cursor, Char skip_char) {
  return const_cast<Char*>(TextSkipChar<Char>(cursor, skip_char));
}

template <typename Char = char>
const Char* StringSkipSpaces(const Char* cursor) {
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
  query = StringSkipSpaces<Char>(query);

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

template <typename Char = char>
const Char* StringSkipSpaces(const Char* cursor, const Char* end) {
  if (!cursor) return nullptr;
  if (cursor > end) return nullptr;
  PRINTF("\nSkipping spaces: ");
  Char c = *cursor;
  while (IsWhitespace(c)) {
    PRINT('.');
    if (!c) return nullptr;
    if (++cursor >= end) return nullptr;
    c = *cursor;
  }
  return cursor;
}

template <typename Char = char>
const Char* StringEquals(const Char* text_a, const Char* text_b) {
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

template <typename Char = char>
const Char* StringEquals(const Char* cursor, const Char* end,
                         const Char* query) {
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

template <typename Char = char>
bool TextQualifies(const Char* cursor) {
  if (cursor == nullptr) return false;
  Char c = *cursor;
  while (c) {
    if (!IsWhitespace<Char>(c)) return true;
    c = *(++cursor);
  }
  return false;
}

template <typename Char = char>
bool TextQualifies(const Char* cursor, const Char* end) {
  if (!cursor) return false;
  if (cursor > end) return false;
  Char c = *cursor;
  while (c) {
    if (!IsWhitespace<Char>(c)) {
      // The text must end at or before the target_end.
      do {
        if (++cursor >= end) return false;
        c = *cursor;
        if (!IsWhitespace<Char>(c)) return true;
      } while (c);
      return true;
    }
    if (++cursor >= end) return false;
    c = *cursor;
  }
  return false;
}

template <typename Char = char>
const Char* TextSkipCharsInRange(const Char* cursor, Char lower_bounds,
                                 Char upper_bounds) {
  ASSERT(cursor);
  ASSERT(lower_bounds < upper_bounds);
  Char c = *cursor;
  while (c >= lower_bounds && c <= upper_bounds) c = *(++cursor);
  return cursor;
}

template <typename Char = char>
inline const Char* TextSkipNumbers(const Char* cursor) {
  return TextSkipCharsInRange<Char>(cursor, '0', '9');
}

/* Prints the given token aligned right the given column_count.
@return Nil if any of the pointers are nil or if column_count < 1, and a
pointer to the nil-term char upon success.
@param  cursor The begin of the buffer.
@param  end    The end of the buffer.
@param  token  The token to print.
@param  column_count The number of columns to align right to. */
template <typename Char = char>
Char* PrintRight(Char* cursor, Char* end, const Char* token, int column_count) {
  ASSERT(token);
  ASSERT(cursor < end);

  if (cursor == nullptr || column_count < 1) return nullptr;

  if (cursor + column_count > end) {
    PRINT("\nBuffer overflow!");
    return nullptr;
  }

  const Char* token_end = StringEnd<Char>(token);
  Char c;  //< Temp variable.
  if (token == token_end) return cursor;
  intptr_t length = token_end - token;

  PRINTF("\n\nPrinting \"%s\":%i aligned right %i columns", token, (int)length,
         column_count);

  // If the length is less than the column_count we need to print ".", "..",
  // "..." or nothing and chop off some of the token.
  intptr_t count = column_count - length;
  if (count < 0) {
    if (count > -3) {
      PRINTF("\n Wrote dots:\"");
      while (count++ < 0) {
        *cursor++ = '.';
        PRINT('.');
      }
      PRINT('\"');
      *cursor = 0;
      return cursor;
    }
    end = cursor + column_count;
    *end-- = 0;
    *end-- = '.';
    *end-- = '.';
    *end-- = '.';
    token_end = token + column_count - 4;
    PRINTF("\n Wrote with dots backwards:\"...");
    while (token_end >= token) {
      c = *token_end--;
      *end-- = c;
      PRINT(c);
    }
    c = *token_end--;
    *end-- = c;
    PRINT(c);
    PRINTF("\"");
    return cursor + column_count;
  }
  PRINTF("\ncursor:0x%p end:0x%p token:0x%p token_end:0x%p", cursor, end, token,
         token_end);
  // In order to keep the current cache lines we're going to print
  // backwards back from the token_end.
  PRINTF("\n Wrote backwards:\"");
  end = cursor + column_count;
  --token_end;  //< This is pointed at the nil-term char
  *end-- = 0;   //< and there is no need to load a 0.
  while (token_end >= token) {
    c = *token_end--;
    *end-- = c;
    PRINT(c);
  }
  while (end >= cursor) {
    *end-- = ' ';
    PRINT(' ');
  }
  PRINTF("\"\nWrote:\"%s\"", cursor);
  return cursor + column_count;
}

template <typename Char = char>
Char* PrintCenter(Char* cursor, Char* end, const Char* string,
                  int column_count) {
  ASSERT(cursor);
  ASSERT(cursor < end);

  // We need to leave at least one space to the left and right of
  int length = StringLength<Char>(string), offset;
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

template <typename Char = char>
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

template <typename Char = char>
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

template <typename Char = char>
Char* PrintSocket(Char* cursor, Char* end, const void* start,
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
  cursor = PrintRight<Char>(cursor, end, Utf8Text(8).String(), 8);
  *cursor++ = ' ';
  for (int i = 16; i <= 56; i += 8)
    cursor = PrintRight(cursor, end, Utf8Text(i).String(), 8);
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

template <typename Char = char>
Char Lowercase(Char c) {
  if ((c & 0x3f) == 3) return c - 64;
  return c;
}

/* A UTF-8, UTF-16, or UTF-32 Text token without any whitespace. */
template <typename Char = char>
class TToken {
 public:
  /* Default constructor does nothing. */
  TToken() { *string = 0; }

  /* Prints the value to the text buffer. */
  TToken(Char character) { Print<Char>(string, string + kSize, character); }

  /* Prints the value to the text buffer. */
  TToken(const Char* string) { Print<Char>(string, string + kSize, string); }

  /* Prints the value to the text buffer. */
  TToken(int32_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  TToken(uint32_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  TToken(int64_t value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  TToken(uint64_t value) { Print<Char>(string, string + kSize, value); }

#if SEAM >= SEAM_0_0_0__02
  /* Prints the value to the text buffer. */
  TToken(float value) { Print<Char>(string, string + kSize, value); }

  /* Prints the value to the text buffer. */
  TToken(double value) { Print<Char>(string, string + kSize, value); }
#endif
  /* Gets the number string. */
  const Char* String() { return string; }

 private:
  enum { kSize = 24 };

  Char string[kSize * sizeof(Char) + 1];  //< String buffer.
};

/* Utility class for printing hex with operator<<. */
template <typename Char = char>
class TCenter {
 public:
  /* Prints the value to the text buffer. */
  TCenter(const Char* string, int column_count)
      : string(StringSet<Char>(string)), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TCenter(int32_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TCenter(uint32_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TCenter(int64_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TCenter(uint64_t value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

#if SEAM >= SEAM_0_0_0__02
  /* Prints the value to the text buffer. */
  TCenter(float value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TCenter(double value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }
#endif

  /* Gets the number string. */
  const Char* String() { return string; }

  /* Gets the column_count. */
  int GetColumnCount() { return column_count; }

 private:
  const Char* string;   //< Pointer to the string.
  TToken<Char> number;  //< Pointer to a pointer to print.
  int column_count;     //< Number of columns to center.
};

/* Utility class for printing hex with operator<<. */
template <typename Char = char>
class TRight {
 public:
  /* Prints the value to the text buffer. */
  TRight(const Char* string, int column_count)
      : string_(StringSet<Char>(string)), column_count_(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TRight(int32_t value, int column_count)
      : string_(nullptr), number_(value), column_count_(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TRight(uint32_t value, int column_count)
      : string_(nullptr), number_(value), column_count_(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TRight(int64_t value, int column_count)
      : string_(nullptr), number_(value), column_count_(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TRight(uint64_t value, int column_count)
      : string_(nullptr), number_(value), column_count_(column_count) {
    // Nothing to do here!
  }

#if SEAM >= SEAM_0_0_0__02
  /* Prints the value to the text buffer. */
  TRight(float value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }

  /* Prints the value to the text buffer. */
  TRight(double value, int column_count)
      : string(nullptr), number(value), column_count(column_count) {
    // Nothing to do here!
  }
#endif
  /* Gets the number string. */
  const Char* String() { return string_; }

  /* Gets the column_count. */
  int GetColumnCount() { return column_count_; }

 private:
  const Char* string_;   //< Pointer to the string.
  TToken<Char> number_;  //< Pointer to a pointer to print.
  int column_count_;     //< Number of columns to center.
};

/* Utility class for printing a single Char token line with operator<<. */
template <typename Char = char>
struct API TLine {
  Char token;        //< Character to print.
  int column_count;  //< Column count.

  /* Constructor. */
  TLine(Char token, int column_count)
      : token(token), column_count(column_count) {}
};

/* Utility class for printing a string line with operator<<. */
template <typename Char = char>
struct API TLineString {
  const Char* string;  //< Character to print.
  int column_count;    //< Column count.

  /* Constructor. */
  TLineString(const Char* string, int column_count)
      : string(string), column_count(column_count) {}
};

/* Buffer destructor prints the buffer to the console without deleting the
buffer. */
template <typename Char = char>
void Console(uintptr_t* buffer) {
  if (!buffer) return;
  uintptr_t address = reinterpret_cast<uintptr_t>(buffer) + sizeof(SI);
  Print(reinterpret_cast<const char*>(address));
}

/* Buffer destructor prints the buffer to the console and deletes the
buffer. */
template <typename Char = char>
void COutAuto(uintptr_t* buffer) {
  if (!buffer) return;
  uintptr_t address = reinterpret_cast<uintptr_t>(buffer) + sizeof(SI);
  Print(reinterpret_cast<const char*>(address));
  delete[] buffer;
}

template <typename Char = char, typename Size = int>
inline void StrTerminate(CObj obj) {
  uintptr_t* begin = obj.begin;
  Size size = ObjSize<Size>(begin);
  if (size < (2 * sizeof(Char))) {
    PRINTF("\nSTR too small! %i", (int)size);
    return;
  }
  uintptr_t address =
      reinterpret_cast<uintptr_t>(begin) + size - 1 - sizeof(Size);
  *reinterpret_cast<Char*>(address) = 0;
}

template <typename Char = char, typename Size = int>
inline Char* StrStart(uintptr_t* begin) {
  ASSERT(begin);
  uintptr_t address = reinterpret_cast<uintptr_t>(begin) + sizeof(Size);
  return reinterpret_cast<Char*>(address);
}

template <typename Char = char, typename Size = int>
inline Char* StrStop(uintptr_t* begin) {
  ASSERT(begin);
  Size size = *reinterpret_cast<Size*>(begin);
  uintptr_t address = reinterpret_cast<uintptr_t>(begin) + sizeof(Size);
  Char* start = reinterpret_cast<Char*>(address);
  return reinterpret_cast<Char*>(address) + (size >> kWordBitCount);
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
    ASSERT(ObjSizeIsValid<intptr_t>(size, 8));
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

#if SEAM >= SEAM_0_0_0__02
  /* Prints the given value as hex. */
  inline TUtf& Hex(float value) {
    return Set(PrintHex<Char>(begin, end, value));
  }

  /* Prints the given value as hex. */
  inline TUtf& Hex(double value) {
    return Set(PrintHex<Char>(begin, end, value));
  }
#endif

  /* Prints the given pointer as hex. */
  inline TUtf& Hex(const void* value) {
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

#if SEAM >= SEAM_0_0_0__02
  /* Prints the given value as binary. */
  inline TUtf& Binary(float value) {
    return Set(Binary<Char>(begin, end, value));
  }

  /* Prints the given value as binary. */
  inline TUtf& Binary(double value) {
    return Set(Binary<Char>(begin, end, value));
  }
#endif
  /* Prints the given pointer as binary. */
  inline TUtf& Binary(const void* value) {
    return Set(Binary<Char>(begin, end, value));
  }
};

/* A string that is statically or dynamically allocated and can do "stuff"
upon destruct.

This class is designed to take advantage of the behavior of the C++ operator
overloads. When you have a string of overloads, the objects get destructed
in the opposite order then where called, which is a stack push pop operation.
For this reason the destructor is programmable.

# Statically Allocated Strings

If it is null then the memory is treated as statically allocated memory.

@code
Str<> (&Console) << "Hello world!";
@endcode

# Dynamic Allocated Strings

Strings that use dynamic memory use the DCOut destructor:

@code
Str<uint32_t> (&DCOut) << "Hello world!";
@endcode
*/
template <typename Char = char, typename Size = schar_t>
class TStr : public TUtf<Char> {
 public:
  /* Constructs the Utf& pointers to the buffer_. */
  TStr() : TUtf<Char>(), obj_() { Terminate(); }

  /* Constructs the Utf& pointers to the buffer_. */
  TStr(Destructor destructor) : obj_(destructor) { Terminate(); }

  /* Constructs the Utf& pointers to the buffer_. */
  TStr(intptr_t size, Destructor destructor = nullptr)
      : obj_(size, destructor) {
    Terminate();
  }

  /* Constructs the Utf& pointers to the buffer_. */
  TStr(intptr_t size, uintptr_t* buffer, Destructor destructor = nullptr)
      : TObj(size, buffer, destructor) {
    Terminate();
  }

  /* Gets the UTF. */
  TUtf<Char> Print() {
    Size size = ObjSize<Size>(obj_);
    ASSERT(((size - sizeof(Size)) & 7) == 0);
    Char* start_ptr = reinterpret_cast<Char*>(
        reinterpret_cast<uintptr_t>(obj_.begin) + sizeof(Size));
    return TUtf<Char>(start_ptr, start_ptr + (size >> BitShiftCount<Size>()));
  }

  /* Prints a char to the string.
  @param item The item to print.
  @return A UTF. */
  template <typename T>
  TUtf<Char> Print(T item) {
    uintptr_t begin = reinterpret_cast<uintptr_t>(obj_.Begin());
    Size size = *reinterpret_cast<Size*>(begin);
    ASSERT((size & 7) == 0);
    Char *cursor = reinterpret_cast<Char*>(begin + sizeof(Size)),
         *end = cursor + (size >> BitShiftCount<Size>()) - 1;
    cursor = Print<Char>(cursor, end, item);
    if (!cursor) {
      return TUtf<Char>(reinterpret_cast<Char*>(begin), end);
    }
    return TUtf<Char>(cursor, end);
  }

  /* Prints a char to the string.
  @return A UTF. */
  inline TUtf<Char> Print(Char c) { return Print<Char>(c); }

  /* Prints a char to the string.
  @return A UTF. */
  inline TUtf<Char> Print(const Char* string) {
    return Print<const Char*>(string);
  }

  /* Prints the given value.
  @return A UTF. */
  inline TUtf<Char> Print(int32_t value) { return Print<int32_t>(value); }

  /* Prints the given value.
  @return A UTF. */
  inline TUtf<Char> Print(uint32_t value) { return Print<uint32_t>(value); }

  /* Prints the given value.
  @return A UTF. */
  inline TUtf<Char> Print(int64_t value) { return Print<int64_t>(value); }

  /* Prints the given value.
  @return A UTF. */
  inline TUtf<Char> Print(uint64_t value) { return Print<uint64_t>(value); }

#if SEAM >= SEAM_0_0_0__02
  /* Prints the given value.
  @return A UTF. */
  inline TUtf<Char> Print(float value) { return Print<float>(value); }

  /* Prints the given value.
  @return A UTF. */
  inline TUtf<Char> Print(double value) { return Print<double>(value); }
#endif

  /* Returns the stop of the buffer. */
  inline Char* Stop() { return StrStop<Char, Size>(obj_.Begin()); }

  /* Returns the end of the buffer. */
  inline Char* End() { return ObjEnd<Char, Size>(obj_); }

  /* Writes a nil-term char at the end of the string. */
  inline void Terminate() { *obj_.End() = 0; }

  /* Gets the begin of the Console buffer. */
  inline TObj<Size>& Obj() { return obj_; }

 private:
  TObj<Size> obj_;  //< ASCII OBJ.
};

template <typename Char = char>
TUtf<Char> Print(Destructor* mh) {
  return TStr<Char>(mh);
}

}  // namespace _

#define COUT ::_::TStr<>(&Console).Print();

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, const Char* string) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, string));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, Char c) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, c));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint8_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, int16_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint16_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, int32_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint32_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, int64_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, uint64_t value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

#if SEAM >= SEAM_0_0_0__02
/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, float value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, double value) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, value));
}
#endif

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  item The item to write to print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, _::TCenter<Char> item) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, item));
}

/* Writes the given value to the print justified right.
@return The utf.
@param  utf The utf.
@param  item The item to print. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, _::TRight<Char> item) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, item));
}

/* Prints a line of the given column_count to the utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, _::TLine<Char> line) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, line));
}

/* Prints a line string of the given column_count to the utf. */
template <typename Char = char>
API _::TUtf<Char>& operator<<(_::TUtf<Char>& utf, _::TLineString<Char> line) {
  return utf.Set(_::Print<Char>(utf.begin, utf.end, line));
}

#include "01/seam_footer.inl"
#endif  //< #if INCLUDED_KABUKI_F2_TSTR
#endif  //< #if SEAM >= SEAM_0_0_0__01
