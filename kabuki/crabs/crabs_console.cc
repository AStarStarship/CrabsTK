/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_console.cc
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
#include <cstdio>
#include "assert.h"
#include "config.h"
#include "console.h"
#include "utfn.h"

namespace _ {

void Print(char_t c) { putchar(c); }

void PrintLn(char_t c) {
  Print('\n');
  Print(c);
}

void Print(const char_t* string) { Str<>(&COut) << string; }

void Print(int32_t value) { Str<>(&COut) << value; }

void Print(uint32_t value) { Str<>(&COut) << value; }

void Print(int64_t value) { Str<>(&COut) << value; }

void Print(uint64_t value) { Str<>(&COut) << value; }

void Print(float value) { Str<>(&COut) << value; }

void Print(double value) { Str<>(&COut) << value; }

void PrintLn(const char_t* string) {
  Print('\n');
  Print(string);
}

void Printf(const char_t* format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

void PrintfLn(const char_t* format, ...) {
  PrintLn('\n');
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

}  // namespace _
