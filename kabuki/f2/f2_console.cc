/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/f2_console.cc
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

#if SEAM >= SEAM_0_0_0
#include "tconsole.h"

#include <conio.h>
#include <cstdio>

namespace _ {

inline COut Print(char c) { putchar(c); }

inline COut Print(char first, char second) {
  Print(first);
  Print(second);
  return COut();
}

inline COut Print(char first, char second, char third) {
  Print(first);
  Print(second);
  Print(third);
  return COut();
}

COut PrintLn(char c) {
  Print('\n', c);
  return COut();
}

COut PrintLn(char first, char second) { return Print('\n', first, second); }

COut Printf(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return COut();
}

COut PrintfLn(const char* format, ...) {
  PrintLn();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return COut();
}

COut Print(const char* string) { return Printf(string); }

COut Print(const char* string, char delimiter) {
  Printf(string);
  return Print(delimiter);
}

COut Print(uint64_t value) {
#if SEAM == SEAM_0_0_0
  return Printf(FORMAT_UI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

COut Print(uint32_t value) {
#if SEAM == SEAM_0_0_0
  return Printf("%u", value);
#else
  return Print((uint64_t)value);
#endif
}

COut Print(int64_t value) {
#if SEAM == SEAM_0_0_0
  return Printf(FORMAT_SI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

COut Print(int32_t value) {
#if SEAM == SEAM_0_0_0
  return Printf("%i", value);
#else
  return Print((int64_t)value);
#endif
}

COut Print(float value) {
#if SEAM == SEAM_0_0_0
  return Printf("%f", value);
#else
  enum { kSize = 16 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

COut Print(double value) {
#if SEAM == SEAM_0_0_0
  return Printf("%f", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

COut PrintLn(const char* string) {
  Print('\n');
  return Print(string);
}

COut PrintIndent(int count) {
  Print('\n');
  while (--count > 0) Print(' ');
  return COut();
}

COut PrintLine(int width, char token, char first_token) {
  if (width < 1) return COut();
  Print(first_token);
  while (width-- > 0) Print(token);
  return COut();
}

COut PrintHeading(const char* heading, int line_count, int width, char token,
                  char first_token) {
  if (line_count < 1 || width < 1) return COut();
  while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print("\n| ");
  Print(heading);
  Print('\n');
  PrintLine(width, token, '+');
  return Print('\n');
}

template <typename UI>
COut PrintBinaryUnsigned(UI value) {
  enum { kSize = sizeof(UI) * 8 };

  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    Print(c);
    value = value << 1;
  }
  return COut();
}

template <typename SI, typename UI>
COut PrintBinarySigned(SI value) {
  return PrintBinaryUnsigned<UI>((UI)value);
}

COut PrintBinary(uint8_t value) { return PrintBinaryUnsigned<uint8_t>(value); }

COut PrintBinary(int8_t value) {
  return PrintBinarySigned<int8_t, uint8_t>(value);
}

COut PrintBinary(uint16_t value) {
  return PrintBinaryUnsigned<uint16_t>(value);
}

COut PrintBinary(int16_t value) {
  return PrintBinarySigned<int16_t, uint16_t>(value);
}

COut PrintBinary(uint32_t value) {
  return PrintBinaryUnsigned<uint32_t>(value);
}

COut PrintBinary(int32_t value) {
  return PrintBinarySigned<int32_t, uint32_t>(value);
}

COut PrintBinary(uint64_t value) {
  return PrintBinaryUnsigned<uint64_t>(value);
}

COut PrintBinary(int64_t value) {
  return PrintBinarySigned<int64_t, uint64_t>(value);
}

COut PrintBinary(float value) {
  return PrintBinaryUnsigned<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

COut PrintBinary(double value) {
  return PrintBinaryUnsigned<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

COut PrintBinary(const void* ptr) {
  return PrintBinaryUnsigned<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

COut PrintHex(uint8_t value) { return PrintHex<uint8_t>(value); }

COut PrintHex(int8_t value) { return PrintHex<uint8_t>((int8_t)value); }

COut PrintHex(uint16_t value) { return PrintHex<uint16_t>(value); }

COut PrintHex(int16_t value) { return PrintHex<uint16_t>((uint16_t)value); }

COut PrintHex(uint32_t value) { return PrintHex<uint32_t>(value); }

COut PrintHex(int32_t value) { return PrintHex<uint32_t>((uint32_t)value); }

COut PrintHex(uint64_t value) { return PrintHex<uint64_t>(value); }

COut PrintHex(int64_t value) { return PrintHex<uint64_t>((uint64_t)value); }

COut PrintHex(float value) {
  return PrintHex<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

COut PrintHex(double value) {
  return PrintHex<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

COut PrintHex(void* ptr) {
  return PrintHex<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

int CInKey() { return _getch(); }

void Pause(const char* message) {
  if (!message) message = "";
  Printf("\n\n%s\nPress any key to continue...", message);
  while (CInKey() < 0)
    ;
}

void Pausef(const char* format, ...) {
  PrintLn();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);

  Pause("\nPress any key to continue...");
  while (CInKey() < 0)
    ;
}

COut::COut() {}

COut::COut(char c) { Print(c); }

COut::COut(char first, char second) { Print(first, second); }

COut::COut(char first, char second, char third) { Print(first, second, third); }

COut::COut(const char* string) { Print(string); }

COut::COut(int64_t value) { Print(value); }

COut::COut(uint64_t value) { Print(value); }

COut::COut(int32_t value) { Print(value); }

COut::COut(uint32_t value) { Print(value); }

COut::COut(float value) { Print(value); }

COut::COut(double value) { Print(value); }

inline COut& COut::Print(char c) {
  Print(c);
  return *this;
}

inline COut& COut::Print(char first, char second) {
  Print(first, second);
  return *this;
}

inline COut& COut::Print(char first, char second, char third) {
  Print(first, second, third);
  return *this;
}

inline COut& COut::Print(const char* string) {
  Print(string);
  return *this;
}

inline COut& COut::Print(int64_t value) {
  Print(value);
  return *this;
}

inline COut& COut::Print(uint64_t value) {
  Print(value);
  return *this;
}

inline COut& COut::Print(int32_t value) {
  Print(value);
  return *this;
}

inline COut& COut::Print(uint32_t value) {
  Print(value);
  return *this;
}

inline COut& COut::Print(float value) {
  Print(value);
  return *this;
}

inline COut& COut::Print(double value) {
  Print(value);
  return *this;
}

COut& COut::PrintLn(char c) {
  PrintLn(c);
  return *this;
}

COut& COut::PrintLn(char first, char second) {
  return PrintLn(first, second);
  return *this;
}

COut& COut::PrintLn(const char* string) {
  return PrintLn(string);
  return *this;
}

COut& COut::Printf(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return *this;
}

COut& COut::PrintfLn(const char* format, ...) {
  Print();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return *this;
}

COut& COut::Line(int width, char token, char first_token) {
  PrintLine(width, token, first_token);
  return *this;
}

COut& COut::Heading(const char* heading, int line_count, int width, char token,
                    char first_token) {
  PrintHeading(heading, line_count, width, token, first_token);
  return *this;
}

COut& COut::Binary(uint8_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(int8_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(uint16_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(int16_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(uint32_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(int32_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(uint64_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Binary(int64_t value) {
  PrintBinary(value);
  return *this;
}

COut& COut::Hex(uint8_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(int8_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(uint16_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(int16_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(uint32_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(int32_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(uint64_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(int64_t value) {
  PrintHex(value);
  return *this;
}

COut& COut::Hex(const void* pointer) {
  PrintHex(pointer);
  return *this;
}

inline COut& COut::Hex(float value) {
  PrintHex(value);
  return *this;
}

inline COut& COut::Hex(double value) {
  PrintHex(value);
  return *this;
}

COut& COut::Pause(const char* message) { Pause(message); }

CHex::CHex(const void* ptr) { PrintHex(ptr); }

CHex::CHex(uint8_t value) { PrintHex(value); }

CHex::CHex(int8_t value) { PrintHex(value); }

CHex::CHex(uint16_t value) { PrintHex(value); }

CHex::CHex(int16_t value) { PrintHex(value); }

CHex::CHex(uint32_t value) { PrintHex(value); }

CHex::CHex(int32_t value) { PrintHex(value); }

CHex::CHex(uint64_t value) { PrintHex(value); }

CHex::CHex(int64_t value) { PrintHex(value); }

CHex::CHex(float value) { PrintHex(value); }

CHex::CHex(double value) { PrintHex(value); }

CBinary::CBinary(uint8_t value) { PrintBinary(value); }

CBinary::CBinary(int8_t value) { PrintBinary(value); }

CBinary::CBinary(uint16_t value) { PrintBinary(value); }

CBinary::CBinary(int16_t value) { PrintBinary(value); }

CBinary::CBinary(uint32_t value) { PrintBinary(value); }

CBinary::CBinary(int32_t value) { PrintBinary(value); }

CBinary::CBinary(uint64_t value) { PrintBinary(value); }

CBinary::CBinary(int64_t value) { PrintBinary(value); }

CBinary::CBinary(float value) { PrintBinary(value); }

CBinary::CBinary(double value) { PrintBinary(value); }

CBinary::CBinary(const void* ptr) { PrintBinary(ptr); }

}  // namespace _

_::COut& operator<<(_::COut& cout, const char* string) {
  _::Print(string);
  return cout;
}

_::COut& operator<<(_::COut& cout, char c) {
  _::Print(c);
  return cout;
}

_::COut& operator<<(_::COut& cout, uint8_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, int16_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, uint16_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, int32_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, uint32_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, int64_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, uint64_t value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, float value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, double value) {
  _::Print(value);
  return cout;
}

_::COut& operator<<(_::COut& cout, _::CHex& item) { return cout; }

_::COut& operator<<(_::COut& cout, _::CBinary& item) { return cout; }

#endif  //< #if SEAM >= SEAM_0_0_0
