/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    kabuki-toolkit.git/kabuki/crabs/crabs_console.cc
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

#include <conio.h>
#include <cstdio>
#include "test.h"

namespace _ {

void Print(char c) { putchar(c); }

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

void Print(const char* string) { Printf(string); }

void Print(int32_t value) { Printf("%i", value); }

void Print(uint32_t value) { Printf("%u", value); }

void Print(int64_t value) {
#if COMPILER == VISUAL_CPP
  Printf("%I64i", value);
#else
  Printf("%llu", value);
#endif
}

void Print(uint64_t value) {
#if COMPILER == VISUAL_CPP
  Printf("%I64i", value);
#else
  Printf("%lld", value);
#endif
}

void Print(float value) { Printf("%f", value); }

void Print(double value) { Printf("%f", value); }

void PrintLn(char c) {
  Print('\n');
  Print(c);
}

void PrintLn(const char* string) {
  Print('\n');
  Print(string);
}

void PrintLine(int width, char token, char first_token) {
  if (width < 1) return;
  Print(first_token);
  while (width-- > 0) Print(token);
}

void PrintHeading(const char* heading, int line_count, int width, char token,
                  char first_token) {
  if (line_count < 1 || width < 1) return;
  while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print(heading);
  Print('\n');
  PrintLine(width, token, '+');
  Print('\n');
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

void PrintHex(uint8_t value) { return PrintHex<uint8_t>(value); }

void PrintHex(int8_t value) { return PrintHex<int8_t, uint8_t>(value); }

void PrintHex(uint16_t value) { return PrintHex<uint16_t>(value); }

void PrintHex(int16_t value) { return PrintHex<int16_t, uint16_t>(value); }

void PrintHex(uint32_t value) { return PrintHex<uint32_t>(value); }

void PrintHex(int32_t value) { return PrintHex<int32_t, uint32_t>(value); }

void PrintHex(uint64_t value) { return PrintHex<uint64_t>(value); }

void PrintHex(int64_t value) { return PrintHex<int64_t, uint64_t>(value); }

void PrintHex(float value) {
  uint32_t value_ui4;
  value_ui4 = *reinterpret_cast<uint32_t*>(&value);
  return PrintHex<uint32_t>(value);
}

int CInChar() { return _getch(); }

void Pause() {
  Print("\nPress any key to continue...");
  while (CInChar() < 0)
    ;
}

void Pause(const char* message) {
  Printf("\n\n%s", message);
  Pause();
}

bool AssertHandle(int line, const char* file) {
  Printf("\nError at line %d in \"%s\"", line, file);
  Pause();
  return true;
}

bool Assert(bool condition) { return !condition; }

bool AssertHandle(int line, const char* file, const char* message) {
#if CRABS_ASSERT_PRINT
  Printf("\n%s\n,TestCase failed at line %d in \"%s\"", line, file);
#endif
#if CRABS_ASSERT_FREEZE
  while (1)
    ;
#elif CRABS_ASSERT_PAUSE
  Pause();
#endif
}

}  // namespace _
