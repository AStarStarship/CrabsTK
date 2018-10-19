/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_console.cc
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

#include "tconsole.h"

#include <conio.h>
#include <cstdio>
#include <iostream>

namespace _ {

const char* ArgsToString(int args_count, char** args) {
  if (args_count <= 1) return nullptr;
  if (args_count == 2) return args[1];
  for (int i = 2; i < args_count; ++i) {
    char* cursor = args[i];
    while (*cursor) cursor--;
    *cursor = ' ';
  }
  return args[1];
}

inline void Print(char c) { putchar(c); }

inline void Print(char first, char second) {
  Print(first);
  Print(second);
}

inline void Print(char first, char second, char third) {
  Print(first);
  Print(second);
  Print(third);
}

void PrintLn(char c) { Print('\n', c); }

void PrintLn(char first, char second) { return Print('\n', first, second); }

void Printf(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

void PrintfLn(const char* format, ...) {
  PrintLn();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

void Print(const char* string) { return Printf(string); }

void Print(const char16_t* string) { PrintString<char16_t>(string); }

void Print(const char32_t* string) { PrintString<char32_t>(string); }

void Print(const char* string, char delimiter) {
  Printf(string);
  return Print(delimiter);
}

void Print(const char* a, const char* b) { std::cout << a << b; }

void Print(const char* a, const char* b, const char* c) {
  std::cout << a << b << c;
}

void Print(uint64_t value) {
#if SEAM == SEAM_0_0_0__00
  return Printf(FORMAT_UI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

void Print(uint32_t value) {
#if SEAM == SEAM_0_0_0__00
  return Printf("%u", value);
#else
  return Print((uint64_t)value);
#endif
}

void Print(int64_t value) {
#if SEAM == SEAM_0_0_0__00
  return Printf(FORMAT_SI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

void Print(int32_t value) {
#if SEAM == SEAM_0_0_0__00
  return Printf("%i", value);
#else
  return Print((int64_t)value);
#endif
}

void Print(float value) {
#if SEAM == SEAM_0_0_0__00
  return Printf("%f", value);
#else
  enum { kSize = 16 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

void Print(double value) {
#if SEAM == SEAM_0_0_0__00
  return Printf("%f", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  return Print(buffer);
#endif
}

void PrintLn(const char* string) {
  Print('\n');
  return Print(string);
}

void PrintIndent(int count) {
  Print('\n');
  while (--count > 0) Print(' ');
}

void PrintLine(int width, char token, char first_token) {
  if (width < 1) Print(first_token);
  while (width-- > 0) Print(token);
}

void PrintHeading(const char* heading_a, const char* heading_b, int line_count,
                  int width, char token, char first_token) {
  if (line_count < 1 || width < 1)
    while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print("\n| ");
  Print(heading_a);
  Print(heading_b);
  Print('\n');
  PrintLine(width, token, '+');
  return Print('\n');
}

void PrintHeading(const char* heading, int line_count, int width, char token,
                  char first_token) {
  if (line_count < 1 || width < 1)
    while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print("\n| ");
  Print(heading);
  Print('\n');
  PrintLine(width, token, '+');
  return Print('\n');
}

template <typename UI>
void PrintBinaryUnsigned(UI value) {
  enum { kSize = sizeof(UI) * 8 };

  for (int i = kSize; i > 0; --i) {
    char c = (char)('0' + (value >> (kSize - 1)));
    Print(c);
    value = value << 1;
  }
}

template <typename SI, typename UI>
void PrintBinarySigned(SI value) {
  return PrintBinaryUnsigned<UI>((UI)value);
}

void PrintBinary(uint8_t value) { return PrintBinaryUnsigned<uint8_t>(value); }

void PrintBinary(int8_t value) {
  return PrintBinarySigned<int8_t, uint8_t>(value);
}

void PrintBinary(uint16_t value) {
  return PrintBinaryUnsigned<uint16_t>(value);
}

void PrintBinary(int16_t value) {
  return PrintBinarySigned<int16_t, uint16_t>(value);
}

void PrintBinary(uint32_t value) {
  return PrintBinaryUnsigned<uint32_t>(value);
}

void PrintBinary(int32_t value) {
  return PrintBinarySigned<int32_t, uint32_t>(value);
}

void PrintBinary(uint64_t value) {
  return PrintBinaryUnsigned<uint64_t>(value);
}

void PrintBinary(int64_t value) {
  return PrintBinarySigned<int64_t, uint64_t>(value);
}

void PrintBinary(float value) {
  return PrintBinaryUnsigned<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

void PrintBinary(double value) {
  return PrintBinaryUnsigned<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

void PrintBinary(const void* ptr) {
  return PrintBinaryUnsigned<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

void PrintHex(uint8_t value) { PrintHex<uint8_t>(value); }

void PrintHex(int8_t value) { PrintHex<uint8_t>((uint8_t)value); }

void PrintHex(uint16_t value) { PrintHex<uint16_t>(value); }

void PrintHex(int16_t value) { PrintHex<uint16_t>((uint16_t)value); }

void PrintHex(uint32_t value) { PrintHex<uint32_t>(value); }

void PrintHex(int32_t value) { PrintHex<uint32_t>((uint32_t)value); }

void PrintHex(uint64_t value) { PrintHex<uint64_t>(value); }

void PrintHex(int64_t value) { PrintHex<uint64_t>((uint64_t)value); }

void PrintHex(float value) {
  PrintHex<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

void PrintHex(double value) {
  PrintHex<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

void PrintHex(void* ptr) {
  PrintHex<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

int CInKey() { return _getch(); }

bool CInState(int vk_code) {
#if COMPILER == VISUAL_CPP

#elif COMPILER == GCC

#elif COMPILER == CLANG

#endif
  return false;
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

}  // namespace _
