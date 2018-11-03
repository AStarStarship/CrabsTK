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

#include <conio.h>
#include <cstdio>
#include <iostream>

#include "cconsole.h"

#include "cascii.h"
#include "tbinary.h"
#include "ttest.h"

#if SEAM == _0_0_0__00
#include "test_debug.inl"
#define PRINT_ARGS                                    \
  Printf("\nargs_count:%i args:%p", arg_count, args); \
  for (int i = 0; i < arg_count; ++i) Printf("\n%i:\"%s", i, args[i])
#else
#include "test_release.inl"
#define PRINT_ARGS
#endif

namespace _ {

const char* ArgsToString(int arg_count, char** args) {
  if (!args || arg_count <= 1) {
    PRINT("\n!args || arg_count <= 1");
    return "";
  }
  if (arg_count == 2) {
    PRINT("\narg_count == 2");
    return args[1];
  }
  PRINT_ARGS;
  char *begin = args[1], *end = args[arg_count - 1] - 1;
  while (end != begin) {
    char c = *end;
    if (!c) c = ' ';
    --end;
  }
  return begin;
}
#undef PRINT_ARGS
#include "test_footer.inl"

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
  if (!format) return;
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

void PrintfLn(const char* format, ...) {
  if (!format) return;
  PrintLn();
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

void Print(const char* string) { Printf(string); }

void Print(const char16_t* string) { PrintString<char16_t>(string); }

void Print(const char32_t* string) { PrintString<char32_t>(string); }

void Print(const char* string, char delimiter) {
  Printf(string);
  return Print(delimiter);
}

void Print(const char* a, const char* b) {
  if (!a || !b) return;
  Print(a);
  Print(b);
}

void Print(const char* a, const char* b, const char* c) {
  if (!a || !b || !c) return;
  Print(a);
  Print(b);
  Print(c);
}

void Print(uint64_t value) {
#if SEAM <= _0_0_0__01
  return Printf(FORMAT_UI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  PrintUnsigned<>(buffer, kSize - 1, value);
  Print(buffer);
#endif
}

void Print(uint32_t value) {
#if SEAM <= _0_0_0__01
  return Printf("%u", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  PrintUnsigned<uint32_t, char>(buffer, kSize - 1, value);
#endif
}

void Print(int64_t value) {
#if SEAM <= _0_0_0__01
  return Printf(FORMAT_SI8, value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  PrintSigned<int64_t>(buffer, kSize - 1, value);
  Print(buffer);
#endif
}

void Print(int32_t value) {
#if SEAM <= _0_0_0__01
  return Printf("%i", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  PrintSigned<int64_t>(buffer, kSize - 1, (int64_t)value);
#endif
}

void Print(float value) {
#if SEAM <= _0_0_0__12
  return Printf("%f", value);
#else
  enum { kSize = 16 };
  char buffer[kSize];
  PrintFloat<float, uint32_t, char>(buffer, kSize, value);
  Print(buffer);
#endif
}

void Print(double value) {
#if SEAM <= _0_0_0__12
  return Printf("%f", value);
#else
  enum { kSize = 24 };
  char buffer[kSize];
  PrintFloat<double, uint64_t, char>(buffer, kSize - 1, value);
  Print(buffer);
#endif
}

void PrintLn(const char* string) {
  Print('\n');
  Print(string);
}

void PrintIndent(int count) {
  Print('\n');
  while (--count > 0) Print(' ');
}

void PrintLine(int width, char token, char first_token) {
  Print('\n');
  if (width > 1) Print(first_token);
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
  PrintLine(width, token, '+');
  Print('\n');
}

void PrintHeading(const char* heading, int line_count, int width, char token,
                  char first_token) {
  if (line_count < 1 || width < 1) return;
  while (line_count-- > 0) Print('\n');
  PrintLine(width, token, '+');
  Print("\n| ");
  Print(heading);
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
  return PrintBinaryUnsigned<uint32_t>(*reinterpret_cast<uint32_t*>(&value));
}

void PrintBinary(double value) {
  return PrintBinaryUnsigned<uint64_t>(*reinterpret_cast<uint64_t*>(&value));
}

void PrintBinary(const void* ptr) {
  return PrintBinaryUnsigned<uintptr_t>(*reinterpret_cast<uintptr_t*>(&ptr));
}

/* Prints the following value to the console in Hex. */
template <typename UI>
void PrintHexConsole(UI value) {
  enum { kHexStringLengthSizeMax = sizeof(UI) * 2 + 3 };
  Print('0', 'x');
  for (int num_bits_shift = sizeof(UI) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4)
    Print(HexNibbleToUpperCase((uint8_t)(value >> num_bits_shift)));
}

void PrintHex(uint8_t value) { PrintHexConsole<uint8_t>(value); }

void PrintHex(int8_t value) { PrintHexConsole<uint8_t>((uint8_t)value); }

void PrintHex(uint16_t value) { PrintHexConsole<uint16_t>(value); }

void PrintHex(int16_t value) { PrintHexConsole<uint16_t>((uint16_t)value); }

void PrintHex(uint32_t value) { PrintHexConsole<uint32_t>(value); }

void PrintHex(int32_t value) { PrintHexConsole<uint32_t>((uint32_t)value); }

void PrintHex(uint64_t value) { PrintHexConsole<uint64_t>(value); }

void PrintHex(int64_t value) { PrintHexConsole<uint64_t>((uint64_t)value); }

void PrintHex(float value) {
  uint32_t f = *reinterpret_cast<uint32_t*>(&value);
  PrintHexConsole<uint32_t>(f);
}

void PrintHex(double value) {
  uint64_t f = *reinterpret_cast<uint64_t*>(&value);
  PrintHexConsole<uint64_t>(f);
}

void PrintHex(const void* ptr) {
  uintptr_t value = reinterpret_cast<uintptr_t>(ptr);
  PrintHexConsole<uintptr_t>(value);
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
  if (!format) return;
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

#if SEAM >= _0_0_0__02
namespace _ {

void PrintSocket(const char* begin, const char* end) {
  if (!begin || begin >= end) return;

  const char *address_ptr = reinterpret_cast<const char*>(begin),
             *address_end_ptr = reinterpret_cast<const char*>(end);
  size_t size = address_end_ptr - address_ptr,
         num_rows = size / 64 + (size % 64 != 0) ? 1 : 0;

  intptr_t num_bytes = 81 * (num_rows + 2);
  size += num_bytes;
  Print('\n');
  Print('|');

  //  columns
  Print('0');
  Printf("%8i", 8);
  Print(' ');
  for (int i = 16; i <= 56; i += 8) Printf("%8i", i);
  for (int j = 6; j > 0; --j) Print(' ');
  Print('|');
  Print('\n');
  Print('|');
  for (int j = 8; j > 0; --j) {
    Print('+');
    for (int k = 7; k > 0; --k) Print('-');
  }
  Print('|');
  Print(' ');

  PrintHex(address_ptr);

  char c;
  while (address_ptr < address_end_ptr) {
    Print('\n');
    Print('|');
    for (int i = 0; i < 64; ++i) {
      c = *address_ptr++;
      if (address_ptr > address_end_ptr)
        c = 'x';
      else if (!c || c == TAB)
        c = ' ';
      else if (c < ' ')
        c = DEL;
      Print(c);
    }
    Print('|');
    Print(' ');
    PrintHex(address_ptr);
  }
  Print('\n');
  Print('|');
  for (int j = 8; j > 0; --j) {
    Print('+');
    for (int k = 7; k > 0; --k) {
      Print('-');
    }
  }
  Print('|');
  Print(' ');
  PrintHex(address_ptr + size);
}

void PrintSocket(const void* begin, intptr_t size) {
  const char* begin_char = reinterpret_cast<const char*>(begin);
  return PrintSocket(begin_char, begin_char + size);
}

}  // namespace _
#undef PRINT_ARGS
#include "test_footer.inl"
#endif  //< #if SEAM >= _0_0_0__02
