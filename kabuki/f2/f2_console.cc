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

#if SEAM >= SEAM_00_00_00__00_01
#include "tconsole.h"

#include <conio.h>
#include <cstdio>
#include <iostream>

namespace _ {

const char* ConsoleArgs(int args_count, char** args) {
  if (args_count <= 1) return false;
  if (args_count == 2) return true;
  for (int i = 2; i < args_count; ++i) {
    char* cursor = args[i];
    while (*cursor) cursor--;
    *cursor = ' ';
  }
  return args[1];
}

inline Console Print(char c) { putchar(c); }

inline Console Print(char first, char second) {
  Print(first);
  Print(second);
  return Console();
}

inline Console Print(char first, char second, char third) {
  Print(first);
  Print(second);
  Print(third);
  return Console();
}

Console PrintLn(char c) {
  Print('\n', c);
  return Console();
}

Console PrintLn(char first, char second) { return Print('\n', first, second); }

Console Printf(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return Console();
}

Console PrintfLn(const char* format, ...) {
  PrintLn();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return Console();
}

Console Print(const char* string) { return Printf(string); }

Console Print(const char* string, char delimiter) {
  Printf(string);
  return Print(delimiter);
}

Console Print(const char* a, const char* b) { std::cout << a << b; }

Console Print(const char* a, const char* b, const char* c) {
  std::cout << a << b << c;
}

Console Print(uint64_t value) {
#if SEAM == SEAM_00_00_00__00_01
  return Printf(FORMAT_UI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

Console Print(uint32_t value) {
#if SEAM == SEAM_00_00_00__00_01
  return Printf("%u", value);
#else
  return Print((uint64_t)value);
#endif
}

Console Print(int64_t value) {
#if SEAM == SEAM_00_00_00__00_01
  return Printf(FORMAT_SI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

Console Print(int32_t value) {
#if SEAM == SEAM_00_00_00__00_01
  return Printf("%i", value);
#else
  return Print((int64_t)value);
#endif
}

Console Print(float value) {
#if SEAM == SEAM_00_00_00__00_01
  return Printf("%f", value);
#else
  enum { kSize = 16 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

Console Print(double value) {
#if SEAM == SEAM_00_00_00__00_01
  return Printf("%f", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

Console PrintLn(const char* string) {
  Print('\n');
  return Print(string);
}

Console PrintIndent(int count) {
  Print('\n');
  while (--count > 0) Print(' ');
  return Console();
}

Console PrintLine(int width, char token, char first_token) {
  if (width < 1) return Console();
  Print(first_token);
  while (width-- > 0) Print(token);
  return Console();
}

Console PrintHeading(const char* heading_a, const char* heading_b,
                     int line_count, int width, char token, char first_token) {
  if (line_count < 1 || width < 1) return Console();
  while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print("\n| ");
  Print(heading_a);
  Print(heading_b);
  Print('\n');
  PrintLine(width, token, '+');
  return Print('\n');
}

Console PrintHeading(const char* heading, int line_count, int width, char token,
                     char first_token) {
  if (line_count < 1 || width < 1) return Console();
  while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print("\n| ");
  Print(heading);
  Print('\n');
  PrintLine(width, token, '+');
  return Print('\n');
}

template <typename UI>
Console PrintBinaryUnsigned(UI value) {
  enum { kSize = sizeof(UI) * 8 };

  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    Print(c);
    value = value << 1;
  }
  return Console();
}

template <typename SI, typename UI>
Console PrintBinarySigned(SI value) {
  return PrintBinaryUnsigned<UI>((UI)value);
}

Console PrintBinary(uint8_t value) {
  return PrintBinaryUnsigned<uint8_t>(value);
}

Console PrintBinary(int8_t value) {
  return PrintBinarySigned<int8_t, uint8_t>(value);
}

Console PrintBinary(uint16_t value) {
  return PrintBinaryUnsigned<uint16_t>(value);
}

Console PrintBinary(int16_t value) {
  return PrintBinarySigned<int16_t, uint16_t>(value);
}

Console PrintBinary(uint32_t value) {
  return PrintBinaryUnsigned<uint32_t>(value);
}

Console PrintBinary(int32_t value) {
  return PrintBinarySigned<int32_t, uint32_t>(value);
}

Console PrintBinary(uint64_t value) {
  return PrintBinaryUnsigned<uint64_t>(value);
}

Console PrintBinary(int64_t value) {
  return PrintBinarySigned<int64_t, uint64_t>(value);
}

Console PrintBinary(float value) {
  return PrintBinaryUnsigned<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

Console PrintBinary(double value) {
  return PrintBinaryUnsigned<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

Console PrintBinary(const void* ptr) {
  return PrintBinaryUnsigned<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

Console PrintHex(uint8_t value) { return PrintHex<uint8_t>(value); }

Console PrintHex(int8_t value) { return PrintHex<uint8_t>((int8_t)value); }

Console PrintHex(uint16_t value) { return PrintHex<uint16_t>(value); }

Console PrintHex(int16_t value) { return PrintHex<uint16_t>((uint16_t)value); }

Console PrintHex(uint32_t value) { return PrintHex<uint32_t>(value); }

Console PrintHex(int32_t value) { return PrintHex<uint32_t>((uint32_t)value); }

Console PrintHex(uint64_t value) { return PrintHex<uint64_t>(value); }

Console PrintHex(int64_t value) { return PrintHex<uint64_t>((uint64_t)value); }

Console PrintHex(float value) {
  return PrintHex<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

Console PrintHex(double value) {
  return PrintHex<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

Console PrintHex(void* ptr) {
  return PrintHex<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

int CInKey() { return _getch(); }

bool CInState(int vk_code) {
#if COMPILER == VISUAL_CPP

#elif COMPILER == GCC

#elif COMPILER == CLANG

#endif
}

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

Console::Console() {}

Console::Console(char c) { Print(c); }

Console::Console(char first, char second) { Print(first, second); }

Console::Console(char first, char second, char third) {
  Print(first, second, third);
}

Console::Console(const char* string) { Print(string); }

Console::Console(int64_t value) { Print(value); }

Console::Console(uint64_t value) { Print(value); }

Console::Console(int32_t value) { Print(value); }

Console::Console(uint32_t value) { Print(value); }

Console::Console(float value) { Print(value); }

Console::Console(double value) { Print(value); }

inline Console& Console::Print(char c) {
  Print(c);
  return *this;
}

inline Console& Console::Print(char first, char second) {
  Print(first, second);
  return *this;
}

inline Console& Console::Print(char first, char second, char third) {
  Print(first, second, third);
  return *this;
}

inline Console& Console::Print(const char* string) {
  Print(string);
  return *this;
}

inline Console& Console::Print(int64_t value) {
  Print(value);
  return *this;
}

inline Console& Console::Print(uint64_t value) {
  Print(value);
  return *this;
}

inline Console& Console::Print(int32_t value) {
  Print(value);
  return *this;
}

inline Console& Console::Print(uint32_t value) {
  Print(value);
  return *this;
}

inline Console& Console::Print(float value) {
  Print(value);
  return *this;
}

inline Console& Console::Print(double value) {
  Print(value);
  return *this;
}

Console& Console::PrintLn(char c) {
  PrintLn(c);
  return *this;
}

Console& Console::PrintLn(char first, char second) {
  return PrintLn(first, second);
  return *this;
}

Console& Console::PrintLn(const char* string) {
  return PrintLn(string);
  return *this;
}

Console& Console::Printf(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return *this;
}

Console& Console::PrintfLn(const char* format, ...) {
  Print();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  return *this;
}

Console& Console::Line(int width, char token, char first_token) {
  PrintLine(width, token, first_token);
  return *this;
}

Console& Console::Heading(const char* heading, int line_count, int width,
                          char token, char first_token) {
  PrintHeading(heading, line_count, width, token, first_token);
  return *this;
}

Console& Console::Binary(uint8_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(int8_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(uint16_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(int16_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(uint32_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(int32_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(uint64_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Binary(int64_t value) {
  PrintBinary(value);
  return *this;
}

Console& Console::Hex(uint8_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(int8_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(uint16_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(int16_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(uint32_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(int32_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(uint64_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(int64_t value) {
  PrintHex(value);
  return *this;
}

Console& Console::Hex(const void* pointer) {
  PrintHex(pointer);
  return *this;
}

inline Console& Console::Hex(float value) {
  PrintHex(value);
  return *this;
}

inline Console& Console::Hex(double value) {
  PrintHex(value);
  return *this;
}

Console& Console::Pause(const char* message) { Pause(message); }

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

_::Console& operator<<(_::Console& cout, const char* string) {
  _::Print(string);
  return cout;
}

_::Console& operator<<(_::Console& cout, char c) {
  _::Print(c);
  return cout;
}

_::Console& operator<<(_::Console& cout, uint8_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, int16_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, uint16_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, int32_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, uint32_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, int64_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, uint64_t value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, float value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, double value) {
  _::Print(value);
  return cout;
}

_::Console& operator<<(_::Console& cout, _::CHex& item) { return cout; }

_::Console& operator<<(_::Console& cout, _::CBinary& item) { return cout; }

#endif  //< #if SEAM >= SEAM_00_00_00__00_01
