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

#include "tconsole.h"

//#include "test.h"

#include <conio.h>
#include <cstdio>

namespace _ {

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

void PrintLn(char first, char second) { Print('\n', first, second); }

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

void Print(int64_t value) { Printf(FORMAT_SI8, value); }

void Print(int32_t value) {
#if SEAM == SEAM_0_0_0
  Printf("%i", value);
#else
  Print((int64_t)value);
#endif
}

void Print(uint64_t value) {
#if SEAM == SEAM_0_0_0
  Printf(FORMAT_UI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  Print(buffer);
#endif
}

void Print(uint32_t value) {
#if SEAM == SEAM_0_0_0
  Printf("%u", value);
#else
  Print((uint64_t)value);
#endif
}

void Print(float value) {
#if SEAM < SEAM_0_0_3
  Printf("%f", value);
#else
  enum { kSize = 16 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  Print(buffer);
#endif
}

void Print(double value) {
#if SEAM < SEAM_0_0_3
  Printf("%f", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  Print<>(buffer, kSize, value);
  Print(buffer);
#endif
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
  Print("\n| ");
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

void PrintBinary(float value) {
  PrintBinaryUnsigned<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

void PrintBinary(double value) {
  PrintBinaryUnsigned<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

void PrintBinary(void* ptr) {
  return PrintBinaryUnsigned<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

void PrintHex(uint8_t value) { return PrintHex<uint8_t>(value); }

void PrintHex(int8_t value) { return PrintHex<uint8_t>((int8_t)value); }

void PrintHex(uint16_t value) { return PrintHex<uint16_t>(value); }

void PrintHex(int16_t value) { return PrintHex<uint16_t>((uint16_t)value); }

void PrintHex(uint32_t value) { return PrintHex<uint32_t>(value); }

void PrintHex(int32_t value) { return PrintHex<uint32_t>((uint32_t)value); }

void PrintHex(uint64_t value) { return PrintHex<uint64_t>(value); }

void PrintHex(int64_t value) { return PrintHex<uint64_t>((uint64_t)value); }

void PrintHex(float value) {
  return PrintHex<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

void PrintHex(double value) {
  return PrintHex<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

void PrintHex(void* ptr) {
  return PrintHex<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

int KeyboardRead() { return _getch(); }

void Pause() {
  Print("\nPress any key to continue...");
  while (KeyboardRead() < 0)
    ;
}

void Pause(const char* message) {
  Printf("\n\n%s", message);
  Pause();
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

inline COut COut::Print(char c) { Print(c); }

inline COut COut::Print(char first, char second) { Print(first, second); }

inline COut COut::Print(char first, char second, char third) {
  Print(first, second, third);
}

inline COut COut::Print(const char* string) { Print(string); }

inline COut COut::Print(int64_t value) { Print(value); }

inline COut COut::Print(uint64_t value) { Print(value); }

inline COut COut::Print(int32_t value) { Print(value); }

inline COut COut::Print(uint32_t value) { Print(value); }

inline COut COut::Print(float value) { Print(value); }

inline COut COut::Print(double value) { Print(value); }

COut COut::PrintLn(char c) { PrintLn(c); }

COut COut::PrintLn(char first, char second) { PrintLn(first, second); }

COut COut::PrintLn(const char* string) { PrintLn(string); }

COut COut::Printf(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

COut COut::PrintfLn(const char* format, ...) {
  Print();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

COut COut::Line(int width, char token, char first_token) {
  PrintLine(width, token, first_token);
}

COut COut::Heading(const char* heading, int line_count, int width, char token,
                   char first_token) {
  PrintHeading(heading, line_count, width, token, first_token);
}

COut COut::Binary(uint8_t value) { PrintBinary(value); }

COut COut::Binary(int8_t value) { PrintBinary(value); }

COut COut::Binary(uint16_t value) { PrintBinary(value); }

COut COut::Binary(int16_t value) { PrintBinary(value); }

COut COut::Binary(uint32_t value) { PrintBinary(value); }

COut COut::Binary(int32_t value) { PrintBinary(value); }

COut COut::Binary(uint64_t value) { PrintBinary(value); }

COut COut::Binary(int64_t value) { PrintBinary(value); }

COut COut::Hex(uint8_t value) { PrintHex(value); }

COut COut::Hex(int8_t value) { PrintHex(value); }

COut COut::Hex(uint16_t value) { PrintHex(value); }

COut COut::Hex(int16_t value) { PrintHex(value); }

COut COut::Hex(uint32_t value) { PrintHex(value); }

COut COut::Hex(int32_t value) { PrintHex(value); }

COut COut::Hex(uint64_t value) { PrintHex(value); }

COut COut::Hex(int64_t value) { PrintHex(value); }

inline COut COut::Hex(float value) { PrintHex(value); }

inline COut COut::Hex(double value) { PrintHex(value); }

COut COut::Pause() { Pause(); }

COut COut::Pause(const char* message) { Pause(message); }

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

CHex::CHex(void* ptr) { PrintHex(ptr); }

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

CBinary::CBinary(void* ptr) { PrintBinary(ptr); }

}  // namespace _
