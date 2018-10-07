/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    //seams/0_0_benchmark_f2/itos/debug_itos_header.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once

#include <pch.h>

#if SEAM == SEAM_0_0_0

#ifndef INCLUDED_DEBUG_ITOS_HEADER
#define INCLUDED_DEBUG_ITOS_HEADER 1

#include "../../../kabuki/f2/console.h"

namespace _ {

template <typename UI>
inline void PrinttoSDebug(UI value) {
  enum { kSize = sizeof(UI) * 8 };

  const char* debug_table =
      sizeof(UI) == 8
          ? "\n    "
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "\n    "
            "6666555555555544444444443333333333222222222211111111110000000000"
            "\n    "
            "3210987654321098765432109876543210987654321098765432109876543210"
            "\n    |  |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |  |  "
            "|   |"
            "\n    2  1  1  1   1  1  1   1  1  1   1  0  0   0  0  0   0  0  "
            "0   0"
            "\n    0  9  8  7   6  5  4   3  2  1   0  9  8   7  6  5   4  3  "
            "2   1"
          : "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "\n    33222222222211111111110000000000"
            "\n    10987654321098765432109876543210"
            "\n    ||  |  |   |  |  |   |  |  |   |"
            "\n    |1  0  0   0  0  0   0  0  0   0"
            "\n    |0  9  8   7  6  5   4  3  2   1";

  PrintIndent(4);
  PrintBinary<char, UI>(value);
  Printf(debug_table);
}

static char* buffer_begin = 0;

template <typename UI>
inline void PrintPrinted(char* cursor) {
  static const char* format = (sizeof(UI) == 8) ? FORMAT_UI8 : "%i";

  Printf("\n    Printed \"%s\" leaving value:\"%s\"", buffer_begin, cursor);
  char* start = cursor;
  while (*cursor++)
    ;
  Print(':');
  Printf(format, cursor - start);
}

}  // namespace _
#endif  //< #ifndef INCLUDED_DEBUG_ITOS_HEADER

#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PRINTED PrintPrinted(begin, begin, value);

#define PRINT_BINARY \
  PrintIndent(4);    \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define PRINT_HEADER                                    \
  buffer_begin = buffer;                                \
  for (int32_t i = 0; i < 10; ++i) *(cursor + i) = 'x'; \
  *(cursor + 21) = 0;                                   \
  char* begin = cursor;                                 \
  char buffer[256];                                     \
  sprintf_s(buffer, 256, "%u", value);                  \
  Printf("Expecting %s:%u", buffer, StringLength<>(buffer));
#define BEGIN_PUFF_ITOS_ALGORITHM \
  putchar('\n');                  \
  for (int32_t i = 80; i > 0; --i) putchar('-')
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_PRINTED ;
#define PRINT_HEADER
#define PRINT_HEADING
#endif  //< #if SEAM >= SEAM_0_0_0
