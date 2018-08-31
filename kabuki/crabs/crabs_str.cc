/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/crabs_str.cc
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
#if SEAM >= SEAM_0_0_1
// Dependencies:
#include "ascii.h"
#include "debug.h"
#include "hex.h"
#include "socket.h"
#include "tbinary.h"
#include "tstr.h"
// End dependencies.

#if CRABS_TEXT

namespace _ {

void COut(uintptr_t* buffer) { COut<char>(buffer); }

void COutAuto(uintptr_t* buffer) { COutAuto<char>(buffer); }

}  // namespace _

#endif  // #if CRABS_TEXT

#if USING_UTF8
#include "str1.h"
namespace _ {

const char* StringEmpty() { return StringEmpty<char>(); }

const char* StringNL() { return StringNL<char>(); }

const char* StringError() { return StringError<char>(); }

bool IsWhitespace(char character) { return IsWhitespace<char>(character); }

char PrintableChar(char value) { return PrintableChar<char>(value); }

const char* TextEnd(const char* text) { return TextEnd<char>(text); }

int TextLength(const char* text) { return TextLength<char, int>(text); }

const char* TextLineEnd(const char* text, int column_count) {
  return TextLineEnd<char>(text, column_count);
}

const char* TextLineEnd(const char* text, const char* text_end,
                        int column_count) {
  return TextLineEnd<char>(text, text_end, column_count);
}

const char* TextNumberStop(const char* text, const char* text_end) {
  return TextNumberStop<char>(text, text_end);
}

const char* TextNumberStop(const char* text) {
  return TextNumberStop<char>(text);
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

const char* TextEquals(const char* text_a, const char* text_b) {
  return TextEquals<char>(text_a, text_b);
}

const char* TextEquals(const char* text, const char* text_end,
                       const char* query) {
  return TextEquals<char>(text, text_end, query);
}

bool TextQualifies(const char* text) { return TextQualifies<char>(text); }

bool TextQualifies(const char* text, const char* text_end) {
  return TextQualifies<char>(text, text_end);
}

int TextCompare(const char* text_a, const char* text_b) {
  return TextCompare<char>(text_a, text_b);
}

int TextCompare(const char* text_a, const char* text_end, const char* text_b) {
  return TextCompare<char>(text_a, text_end, text_b);
}

const char* TextFind(const char* text, const char* query) {
  return TextFind<char>(text, query);
}

char* Print(char* begin, char* end, const char* string) {
  return Print<char>(begin, end, string);
}

char* Print(char* begin, char* end, char character) {
  return PrintChar<char>(begin, end, character);
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
  return PrintCenter<char>(begin, end, Utf8Text(character).String(),
                           column_count);
}

char* PrintCenter(char* begin, char* end, int32_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintCenter(char* begin, char* end, uint32_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintCenter(char* begin, char* end, uint64_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintCenter(char* begin, char* end, int64_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintCenter(char* begin, char* end, float value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintCenter(char* begin, char* end, double value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintRight(char* begin, char* end, const char* string, int column_count) {
  return PrintRight<char>(begin, end, string, column_count);
}

char* PrintRight(char* begin, char* end, char character, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(character).String(),
                          column_count);
}

char* PrintRight(char* begin, char* end, uint32_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintRight(char* begin, char* end, int32_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintRight(char* begin, char* end, uint64_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintRight(char* begin, char* end, int64_t value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintRight(char* begin, char* end, float value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintRight(char* begin, char* end, double value, int column_count) {
  return PrintRight<char>(begin, end, Utf8Text(value).String(), column_count);
}

char* PrintHex(char* begin, char* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintHex<char>(begin, end, ptr);
}

char* PrintHex(char* begin, char* end, uint8_t value) {
  return PrintHex<char, uint8_t>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int8_t value) {
  return PrintHex<char, uint8_t>(begin, end, (uint8_t)value);
}

char* PrintHex(char* begin, char* end, uint16_t value) {
  return PrintHex<char, uint16_t>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int16_t value) {
  return PrintHex<char, uint16_t>(begin, end, (uint16_t)value);
}

char* PrintHex(char* begin, char* end, uint32_t value) {
  return PrintHex<char, uint32_t>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int32_t value) {
  return PrintHex<char, uint32_t>(begin, end, (uint32_t)value);
}

char* PrintHex(char* begin, char* end, uint64_t value) {
  return PrintHex<char, uint64_t>(begin, end, value);
}

char* PrintHex(char* begin, char* end, int64_t value) {
  return PrintHex<char, uint64_t>(begin, end, (uint64_t)value);
}

char* PrintHex(char* begin, char* end, float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return PrintHex<char, uint32_t>(begin, end, ui);
}

char* PrintHex(char* begin, char* end, double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return PrintHex<char, uint64_t>(begin, end, ui);
}

char* PrintBinary(char* begin, char* end, const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return PrintBinary<char>(begin, end, ptr);
}

char* PrintBinary(char* begin, char* end, uint8_t value) {
  return PrintBinary<char, uint8_t>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int8_t value) {
  return PrintBinary<char, uint8_t>(begin, end, (uint8_t)value);
}

char* PrintBinary(char* begin, char* end, uint16_t value) {
  return PrintBinary<char, uint16_t>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int16_t value) {
  return PrintBinary<char, uint16_t>(begin, end, (uint16_t)value);
}

char* PrintBinary(char* begin, char* end, uint32_t value) {
  return PrintBinary<char, uint32_t>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int32_t value) {
  return PrintBinary<char, uint32_t>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, uint64_t value) {
  return PrintBinary<char, uint64_t>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, int64_t value) {
  return PrintBinary<char, uint64_t>(begin, end, value);
}

char* PrintBinary(char* begin, char* end, float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return PrintBinary<char, uint32_t>(begin, end, ui);
}

char* PrintBinary(char* begin, char* end, double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return PrintBinary<char, uint64_t>(begin, end, ui);
}

char* PrintMemory(char* begin, char* end, const void* start, size_t size) {
  return PrintMemory<char>(begin, end, start,
                           reinterpret_cast<const char*>(start) + size);
}

char* PrintMemory(char* begin, char* end, const void* start, const void* stop) {
  return PrintMemory<char>(begin, end, start, stop);
}

char* PrintLine(char* cursor, char* end, char token, int column_count) {
  return PrintLine<char>(cursor, end, token, column_count);
}

char* PrintLineString(char* cursor, char* end, const char* string,
                      int column_count) {
  return PrintLineString<char>(cursor, end, string, column_count);
}

const char* Scan(const char* string, int8_t& result) {
  return TextScanSigned<char, int8_t, uint8_t>(string, result);
}

const char* Scan(const char* string, uint8_t& result) {
  return TextScanUnsigned<char, uint8_t>(string, result);
}

const char* Scan(const char* string, int16_t& result) {
  return TextScanSigned<char, int16_t, uint16_t>(string, result);
}

const char* Scan(const char* string, uint16_t& result) {
  return TextScanUnsigned<char, uint16_t>(string, result);
}

const char* Scan(const char* string, int32_t& result) {
  return TextScanSigned<char, int32_t, uint32_t>(string, result);
}

const char* Scan(const char* string, uint32_t& result) {
  return TextScanUnsigned<char, uint32_t>(string, result);
}

const char* Scan(const char* string, int64_t& result) {
  return TextScanSigned<char, int64_t, uint64_t>(string, result);
}

const char* Scan(const char* string, uint64_t& result) {
  return TextScanUnsigned<char, uint64_t>(string, result);
}

const char* Scan(const char* string, float& result) {
  return Scan<char>(string, result);
}

const char* Scan(const char* string, double& result) {
  return Scan<char>(string, result);
}

Utf8::Utf8(char* begin, size_t buffer_size)
    : begin(begin), end(begin + buffer_size - 1) {
  ASSERT(begin);
  ASSERT(buffer_size);
}

Utf8::Utf8(char* begin, char* end) : begin(begin), end(end) {
  ASSERT(begin);
  ASSERT(begin < end);
}

Utf8::Utf8(const Utf8& other) : begin(other.begin), end(other.end) {
  ASSERT(begin);
  ASSERT(end);
}

Utf8& Utf8::Set(char* new_cursor) {
  begin = new_cursor;
  return *this;
}

Utf8& Utf8::Hex(uint8_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(int8_t value) { return Set(PrintHex<char>(begin, end, value)); }

Utf8& Utf8::Hex(uint16_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(int16_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(uint32_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(int32_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(uint64_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(int64_t value) {
  return Set(PrintHex<char>(begin, end, value));
}

Utf8& Utf8::Hex(float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintHex<char>(begin, end, ui));
}

Utf8& Utf8::Hex(double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return Set(PrintHex<char>(begin, end, ui));
}

Utf8& Utf8::Hex(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintHex<char>(begin, end, ptr));
}

Utf8& Utf8::Binary(uint8_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(int8_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(uint16_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(int16_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(uint32_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(int32_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(uint64_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(int64_t value) {
  return Set(PrintBinary<char>(begin, end, value));
}

Utf8& Utf8::Binary(float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintBinary<char>(begin, end, ui));
}

Utf8& Utf8::Binary(double value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintBinary<char>(begin, end, ui));
}

Utf8& Utf8::Binary(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintBinary<char>(begin, end, ptr));
}

Utf8Text::Utf8Text() {}

Utf8Text::Utf8Text(char character) {
  *string = character;
  *(string + 1) = 0;
}

Utf8Text::Utf8Text(int32_t value) {
  Print<char>(string, string + kSize - 1, value);
}

Utf8Text::Utf8Text(uint32_t value) {
  Print<char>(string, string + kSize - 1, value);
}

Utf8Text::Utf8Text(int64_t value) {
  Print<char>(string, string + kSize - 1, value);
}

Utf8Text::Utf8Text(uint64_t value) {
  Print<char>(string, string + kSize - 1, value);
}

Utf8Text::Utf8Text(float value) {
  Print<char>(string, string + kSize - 1, value);
}

Utf8Text::Utf8Text(double value) {
  Print<char>(string, string + kSize - 1, value);
}

const char* Utf8Text::String() { return string; }

Utf8Center::Utf8Center(const char* string, int column_count)
    : string(string), column_count(column_count) {}

Utf8Center::Utf8Center(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Center::Utf8Center(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Center::Utf8Center(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Center::Utf8Center(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Center::Utf8Center(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Center::Utf8Center(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char* Utf8Center::String() {
  return (string == nullptr) ? number.String() : string;
}

int Utf8Center::GetColumnCount() { return column_count; }

Utf8Right::Utf8Right(const char* string, int column_count)
    : string(string), column_count(column_count) {}

Utf8Right::Utf8Right(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Right::Utf8Right(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Right::Utf8Right(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Right::Utf8Right(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Right::Utf8Right(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf8Right::Utf8Right(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char* Utf8Right::String() {
  return (string == nullptr) ? number.String() : string;
}

int Utf8Right::GetColumnCount() { return column_count; }

Utf8Line::Utf8Line(char token, int column_count)
    : token(token), column_count(column_count) {
  // Nothing to do here. ({:-)-+=<
}

Utf8LineString::Utf8LineString(const char* string, int column_count)
    : string(string), column_count(column_count) {
  // Nothing to do here. ({:-)-+=<
}

void COutUtf8(uintptr_t* buffer) { return COut<char>(buffer); }

void COutAutoUtf8(uintptr_t* buffer) { return COutAuto<char>(buffer); }

}  // namespace _

_::Utf8& operator<<(_::Utf8& utf, const char* string) {
  return utf.Set(_::Print(utf.begin, utf.end, string));
}

_::Utf8& operator<<(_::Utf8& utf, char value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, uint8_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, int16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, uint16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, int32_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, uint32_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, int64_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, uint64_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, float value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, double value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf8& operator<<(_::Utf8& utf, _::Utf8Center item) {
  return utf.Set(
      _::PrintCenter(utf.begin, utf.end, item.String(), item.GetColumnCount()));
}

_::Utf8& operator<<(_::Utf8& utf, _::Utf8Right item) {
  return utf.Set(
      _::PrintRight(utf.begin, utf.end, item.String(), item.GetColumnCount()));
}

_::Utf8& operator<<(_::Utf8& utf, _::Utf8Line line) {
  return utf.Set(
      _::PrintLine(utf.begin, utf.end, line.token, line.column_count));
}

_::Utf8& operator<<(_::Utf8& utf, _::Utf8LineString line) {
  return utf.Set(
      _::PrintLineString(utf.begin, utf.end, line.string, line.column_count));
}

#endif  //< #if USING_UTF8

#if USING_UTF16
#include "str2.h"
namespace _ {

const char16_t* Empty() { return Empty<char16_t>(); }

const char16_t* NewLine() { return NewLine<char16_t>(); }

const char16_t* ErrorHeader() { return ErrorHeader<char16_t>(); }

bool IsWhitespace(char16_t character) {
  return IsWhitespace<char16_t>(character);
}

char16_t PrintableChar(char16_t value) {
  return PrintableChar<char16_t>(value);
}

const char16_t* TextEnd(const char16_t* text) {
  return TextEnd<char16_t>(text);
}

int TextLength(const char16_t* text) { return TextLength<char16_t, int>(text); }

char16_t* TextClone(const char16_t* text) { return TextClone<char16_t>(text); }

const char16_t* TextLineEnd(const char16_t* text, int column_count) {
  return TextLineEnd<char16_t>(text, column_count);
}

const char16_t* TextLineEnd(const char16_t* text, const char16_t* text_end,
                            int column_count) {
  return TextLineEnd<char16_t>(text, text_end, column_count);
}

const char16_t* TextNumberStop(const char16_t* text, const char16_t* text_end) {
  return TextNumberStop<char16_t>(text, text_end);
}

const char16_t* TextNumberStop(const char16_t* text) {
  return TextNumberStop<char16_t>(text);
}

const char16_t* TextSkipChar(const char16_t* text, char16_t skip_char) {
  return TextSkipChar<char16_t>(text, skip_char);
}

const char16_t* TextSkipSpaces(const char16_t* text) {
  return TextSkipSpaces<char16_t>(text);
}

const char16_t* TextSkipSpaces(const char16_t* text, const char16_t* text_end) {
  return TextSkipSpaces<char16_t>(text, text_end);
}

const char16_t* TextEquals(const char16_t* text_a, const char16_t* text_b) {
  return TextEquals<char16_t>(text_a, text_b);
}

const char16_t* TextEquals(const char16_t* text, const char16_t* text_end,
                           const char16_t* query) {
  return TextEquals<char16_t>(text, text_end, query);
}

bool TextQualifies(const char16_t* text) {
  return TextQualifies<char16_t>(text);
}

bool TextQualifies(const char16_t* text, const char16_t* text_end) {
  return TextQualifies<char16_t>(text, text_end);
}

int TextCompare(const char16_t* text_a, const char16_t* text_b) {
  return TextCompare<char16_t>(text_a, text_b);
}

int TextCompare(const char16_t* text_a, const char16_t* text_end,
                const char16_t* text_b) {
  return TextCompare<char16_t>(text_a, text_end, text_b);
}

const char16_t* TextFind(const char16_t* text, const char16_t* query) {
  return TextFind<char16_t>(text, query);
}

char16_t* Print(char16_t* begin, char16_t* end, const char16_t* string) {
  return Print<char16_t>(begin, end, string);
}

char16_t* Print(char16_t* begin, char16_t* end, char16_t character) {
  return PrintChar<char16_t>(begin, end, character);
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
  return PrintHex<char16_t, uint8_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int8_t value) {
  return PrintHex<char16_t, uint8_t>(begin, end, (uint8_t)value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint16_t value) {
  return PrintHex<char16_t, uint16_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int16_t value) {
  return PrintHex<char16_t, uint16_t>(begin, end, (uint16_t)value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint32_t value) {
  return PrintHex<char16_t, uint32_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int32_t value) {
  return PrintHex<char16_t, uint32_t>(begin, end, (uint32_t)value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, uint64_t value) {
  return PrintHex<char16_t, uint64_t>(begin, end, value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, int64_t value) {
  return PrintHex<char16_t, uint64_t>(begin, end, (uint64_t)value);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, float value) {
  uint32_t float_as_ui32 = *reinterpret_cast<uint32_t*>(&value);
  return PrintHex<char16_t, uint32_t>(begin, end, float_as_ui32);
}

char16_t* PrintHex(char16_t* begin, char16_t* end, double value) {
  uint64_t double_as_ui64 = *reinterpret_cast<uint64_t*>(&value);
  return PrintHex<char16_t, uint64_t>(begin, end, double_as_ui64);
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
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return PrintBinary<char16_t>(begin, end, ui);
}

char16_t* PrintBinary(char16_t* begin, char16_t* end, double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return PrintBinary<char16_t>(begin, end, ui);
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
}  // namespace _
}  // namespace _

const char16_t* Scan(const char16_t* string, int8_t& result) {
  return TextScanSigned<char16_t, int8_t>(string, result);
}

const char16_t* Scan(const char16_t* string, uint8_t& result) {
  return TextScanUnsigned<char16_t, uint8_t>(string, result);
}

const char16_t* Scan(const char16_t* string, int16_t& result) {
  return TextScanSigned<char16_t, int16_t>(string, result);
}

const char16_t* Scan(const char16_t* string, uint16_t& result) {
  return TextScanUnsigned<char16_t, uint16_t>(string, result);
}

const char16_t* Scan(const char16_t* string, int32_t& result) {
  return TextScanSigned<char16_t, int32_t>(string, result);
}

const char16_t* Scan(const char16_t* string, uint32_t& result) {
  return TextScanUnsigned<char16_t, uint32_t>(string, result);
}

const char16_t* Scan(const char16_t* string, int64_t& result) {
  return TextScanSigned<char16_t, int64_t>(string, result);
}

const char16_t* Scan(const char16_t* string, uint64_t& result) {
  return TextScanUnsigned<char16_t, uint64_t>(string, result);
}

const char16_t* Scan(const char16_t* string, float& result) {
  return Scan<char16_t>(string, result);
}

const char16_t* Scan(const char16_t* string, double& result) {
  return Scan<char16_t>(string, result);
}

void COutUtf16(uintptr_t* buffer) { return COut<char16_t>(buffer); }

void DCOutUtf16(uintptr_t* buffer) { return DCOut<char16_t>(buffer); }

Utf16::Utf16(char16_t* begin, size_t buffer_size)
    : begin(begin), end(begin + buffer_size - 1) {
  ASSERT(begin);
  ASSERT(buffer_size);
}

Utf16::Utf16(char16_t* begin, char16_t* end) : begin(begin), end(end) {
  ASSERT(begin);
  ASSERT(begin < end);
}

Utf16::Utf16(const Utf16& other) : begin(other.begin), end(other.end) {
  ASSERT(begin);
  ASSERT(end);
}

Utf16& Utf16::Set(char16_t* new_cursor) {
  begin = new_cursor;
  return *this;
}

Utf16& Utf16::Hex(uint8_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(int8_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(uint16_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(int16_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(uint32_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(int32_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(uint64_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(int64_t value) {
  return Set(PrintHex<char16_t>(begin, end, value));
}

Utf16& Utf16::Hex(float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintHex<char16_t>(begin, end, ui));
}

Utf16& Utf16::Hex(double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return Set(PrintHex<char16_t>(begin, end, ui));
}

Utf16& Utf16::Hex(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintHex<char16_t>(begin, end, ptr));
}

Utf16& Utf16::Binary(uint8_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(int8_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(uint16_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(int16_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(uint32_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(int32_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(uint64_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(int64_t value) {
  return Set(PrintBinary<char16_t>(begin, end, value));
}

Utf16& Utf16::Binary(float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintBinary<char16_t>(begin, end, ui));
}

Utf16& Utf16::Binary(double value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintBinary<char16_t>(begin, end, ui));
}

Utf16& Utf16::Binary(const void* pointer) {
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

Utf16Center::Utf16Center(const char16_t* string, int column_count)
    : string(string), column_count(column_count) {}

Utf16Center::Utf16Center(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Center::Utf16Center(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Center::Utf16Center(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Center::Utf16Center(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Center::Utf16Center(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Center::Utf16Center(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char16_t* Utf16Center::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Utf16Center::GetColumnCount() { return column_count; }

Utf16Right::Utf16Right(const char16_t* string, int column_count)
    : string(string), column_count(column_count) {}

Utf16Right::Utf16Right(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Right::Utf16Right(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Right::Utf16Right(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Right::Utf16Right(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Right::Utf16Right(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf16Right::Utf16Right(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char16_t* Utf16Right::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Utf16Right::GetColumnCount() { return column_count; }

}  // namespace _

_::Utf16& operator<<(_::Utf16& utf, const char16_t* string) {
  return utf.Set(_::Print(utf.begin, utf.end, string));
}

_::Utf16& operator<<(_::Utf16& utf, char16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, uint8_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, int16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, uint16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, int32_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, uint32_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, int64_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, uint64_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, float value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, double value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf16& operator<<(_::Utf16& utf, _::Utf16Center item) {
  utf.Set(_::PrintCenter(utf.begin, utf.end, item.GetString(),
                         item.GetColumnCount()));
}

_::Utf16& operator<<(_::Utf16& utf, _::Utf16Right item) {
  utf.Set(_::PrintRight(utf.begin, utf.end, item.GetString(),
                        item.GetColumnCount()));
}

#endif  //< #if USING_UTF16

#if USING_UTF32
#include "str4.h"
namespace _ {

const char32_t* Empty() { return Empty<char32_t>(); }

const char32_t* NewLine() { return NewLine<char32_t>(); }

const char32_t* ErrorHeader() { return ErrorHeader<char32_t>(); }

bool IsWhitespace(char32_t character) {
  return IsWhitespace<char32_t>(character);
}

char32_t PrintableChar(char32_t value) {
  return PrintableChar<char32_t>(value);
}

const char32_t* TextEnd(const char32_t* text) {
  return TextEnd<char32_t>(text);
}

int TextLength(const char32_t* text) { return TextLength<char32_t, int>(text); }

char32_t* TextClone(const char32_t* text) { return TextClone<char32_t>(text); }

const char32_t* TextLineEnd(const char32_t* text, int column_count) {
  return TextLineEnd<char32_t>(text, column_count);
}

const char32_t* TextLineEnd(const char32_t* text, const char32_t* text_end,
                            int column_count) {
  return TextLineEnd<char32_t>(text, text_end, column_count);
}

const char32_t* TextNumberStop(const char32_t* text, const char32_t* text_end) {
  return TextNumberStop<char32_t>(text, text_end);
}

const char32_t* TextNumberStop(const char32_t* text) {
  return TextNumberStop<char32_t>(text);
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

const char32_t* TextEquals(const char32_t* text_a, const char32_t* text_b) {
  return TextEquals<char32_t>(text_a, text_b);
}

const char32_t* TextEquals(const char32_t* cursor, const char32_t* end,
                           const char32_t* text_b) {
  return TextEquals<char32_t>(cursor, end, text_b);
}

bool TextQualifies(const char32_t* text) {
  return TextQualifies<char32_t>(text);
}

bool TextQualifies(const char32_t* text, const char32_t* text_end) {
  return TextQualifies<char32_t>(text, text_end);
}

int TextCompare(const char32_t* text_a, const char32_t* text_b) {
  return TextCompare<char32_t>(text_a, text_b);
}

int TextCompare(const char32_t* text_a, const char32_t* text_end,
                const char32_t* text_b) {
  return TextCompare<char32_t>(text_a, text_end, text_b);
}

const char32_t* TextFind(const char32_t* text, const char32_t* query) {
  return TextFind<char32_t>(text, query);
}

char32_t* Print(char32_t* begin, char32_t* end, const char32_t* string) {
  return Print<char32_t>(begin, end, string);
}

char32_t* Print(char32_t* begin, char32_t* end, char32_t character) {
  return PrintChar<char32_t>(begin, end, character);
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
  return PrintHex<char32_t, uintptr_t>(begin, end, ptr);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint8_t value) {
  return PrintHex<char32_t, uint8_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int8_t value) {
  return PrintHex<char32_t, uint8_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint16_t value) {
  return PrintHex<char32_t, uint16_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int16_t value) {
  return PrintHex<char32_t, uint16_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint32_t value) {
  return PrintHex<char32_t, uint32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int32_t value) {
  return PrintHex<char32_t, uint32_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, uint64_t value) {
  return PrintHex<char32_t, uint64_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, int64_t value) {
  return PrintHex<char32_t, uint64_t>(begin, end, value);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return PrintHex<char32_t, uint64_t>(begin, end, ui);
}

char32_t* PrintHex(char32_t* begin, char32_t* end, double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return PrintHex<char32_t, uint64_t>(begin, end, ui);
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
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return PrintBinary<char32_t, uint64_t>(begin, end, ui);
}

char32_t* PrintBinary(char32_t* begin, char32_t* end, double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return PrintBinary<char32_t, uint64_t>(begin, end, ui);
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

const char32_t* Scan(const char32_t* string, int8_t& result) {
  return TextScanSigned<char32_t, int8_t>(string, result);
}

const char32_t* Scan(const char32_t* string, uint8_t& result) {
  return TextScanUnsigned<char32_t, uint8_t>(string, result);
}

const char32_t* Scan(const char32_t* string, int16_t& result) {
  return TextScanSigned<char32_t, int16_t>(string, result);
}

const char32_t* Scan(const char32_t* string, uint16_t& result) {
  return TextScanUnsigned<char32_t, uint16_t>(string, result);
}

const char32_t* Scan(const char32_t* string, int32_t& result) {
  return TextScanSigned<char32_t, int32_t>(string, result);
}

const char32_t* Scan(const char32_t* string, uint32_t& result) {
  return TextScanUnsigned<char32_t, uint32_t>(string, result);
}

const char32_t* Scan(const char32_t* string, int64_t& result) {
  return TextScanSigned<char32_t, int64_t>(string, result);
}

const char32_t* Scan(const char32_t* string, uint64_t& result) {
  return TextScanUnsigned<char32_t, uint64_t>(string, result);
}

const char32_t* Scan(const char32_t* string, float& result) {
  return Scan<char32_t>(string, result);
}

const char32_t* Scan(const char32_t* string, double& result) {
  return Scan<char32_t>(string, result);
}

void COuUtf32(uintptr_t* buffer) { return COut<char32_t>(buffer); }

void COuUtf32(uintptr_t* buffer) { return DCOut<char32_t>(buffer); }

Utf32::Utf32(char32_t* begin, size_t buffer_size)
    : begin(begin), end(begin + buffer_size - 1) {
  ASSERT(begin);
  ASSERT(buffer_size);
}

Utf32::Utf32(char32_t* begin, char32_t* end) : begin(begin), end(end) {
  ASSERT(begin);
  ASSERT(begin < end);
}

Utf32::Utf32(const Utf32& other) : begin(other.begin), end(other.end) {
  ASSERT(begin);
  ASSERT(end);
}

Utf32& Utf32::Set(char32_t* new_cursor) {
  begin = new_cursor;
  return *this;
}

Utf32& Utf32::Hex(uint8_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(int8_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(uint16_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(int16_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(uint32_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(int32_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(uint64_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(int64_t value) {
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(double value) {
  uint64_t ui = *reinterpret_cast<uint64_t*>(&value);
  return Set(PrintHex<char32_t>(begin, end, value));
}

Utf32& Utf32::Hex(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintHex<char32_t>(begin, end, ptr));
}

Utf32& Utf32::Binary(uint8_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(int8_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(uint16_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(int16_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(uint32_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(int32_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(uint64_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(int64_t value) {
  return Set(PrintBinary<char32_t>(begin, end, value));
}

Utf32& Utf32::Binary(float value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintBinary<char32_t>(begin, end, ui));
}

Utf32& Utf32::Binary(double value) {
  uint32_t ui = *reinterpret_cast<uint32_t*>(&value);
  return Set(PrintBinary<char32_t>(begin, end, ui));
}

Utf32& Utf32::Binary(const void* pointer) {
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer);
  return Set(PrintBinary<char32_t>(begin, end, ptr));
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

Utf32Center::Utf32Center(const char32_t* string, int column_count)
    : string(string), column_count(column_count) {}

Utf32Center::Utf32Center(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Center::Utf32Center(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Center::Utf32Center(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Center::Utf32Center(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Center::Utf32Center(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Center::Utf32Center(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char32_t* Utf32Center::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Utf32Center::GetColumnCount() { return column_count; }

Utf32Right::Utf32Right(const char32_t* string, int column_count)
    : string(string), column_count(column_count) {}

Utf32Right::Utf32Right(int32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Right::Utf32Right(uint32_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Right::Utf32Right(int64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Right::Utf32Right(uint64_t value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Right::Utf32Right(float value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

Utf32Right::Utf32Right(double value, int column_count)
    : string(nullptr), number(value), column_count(column_count) {}

const char32_t* Utf32Right::GetString() {
  return (string == nullptr) ? number.GetString() : string;
}

int Utf32Right::GetColumnCount() { return column_count; }

}  // namespace _

_::Utf32& operator<<(_::Utf32& utf, const char32_t* string) {
  return utf.Set(_::Print(utf.begin, utf.end, string));
}

_::Utf32& operator<<(_::Utf32& utf, char32_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, uint8_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, int16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, uint16_t value) {
  return utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, int32_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, uint32_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, int64_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, uint64_t value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, float value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, double value) {
  utf.Set(_::Print(utf.begin, utf.end, value));
}

_::Utf32& operator<<(_::Utf32& utf, _::Utf32Center item) {
  utf.Set(_::PrintCenter(utf.begin, utf.end, item.GetString(),
                         item.GetColumnCount()));
}

_::Utf32& operator<<(_::Utf32& utf, _::Utf32Right item) {
  utf.Set(_::PrintRight(utf.begin, utf.end, item.GetString(),
                        item.GetColumnCount()));
}

#endif  //< #if USING_UTF32

#undef PRINTF
#undef PRINT
#undef PRINT_HEADING
#endif  //< #if SEAM >= SEAM_0_0_2
