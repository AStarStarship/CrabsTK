/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/text.h
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
#include <stdafx.h>
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
#ifndef HEADER_FOR_CRABS_PRINT_FLOATING_POINT
#define HEADER_FOR_CRABS_PRINT_FLOATING_POINT
// Dependencies:
#include "config.h"
// End dependencies.
#if USING_PRINTER

namespace _ {

/* Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
template <typename Char>
Char* Print(Char* begin, Char* end, float value) {
  // @todo Replace with GrisuX algorithm that uses the Script itoa Algorithm.
  intptr_t buffer_size = end - begin;
  char* buffer = new char[buffer_size];
  int count = sprintf_s(buffer, buffer_size, "%f", value);
  if (count < 0) {
    *begin = 0;
    delete buffer;
    return nullptr;
  }
  Char* buffer_cursor = buffer;
  for (; count > 0; --count) *begin++ = *buffer_cursor++;
  *begin = 0;
  delete buffer;
  return begin;
}

/* Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
template <typename Char>
Char* Print(Char* begin, Char* end, double value) {
  // Right now we're going to enforce there be enough room to write any
  // int32_t.
  intptr_t buffer_size = end - begin;
  char* buffer = new char[buffer_size];
  int count = sprintf_s(buffer, buffer_size, "%f", value);
  if (count < 0) {
    *begin = 0;
    delete buffer;
    return nullptr;
  }
  Char* buffer_cursor = buffer;
  for (; count > 0; --count) *begin++ = *buffer_cursor++;
  *begin = 0;
  return begin;
}

}  // namespace _

#endif  //< HEADER_FOR_CRABS_PRINT_FLOATING_POINT
#endif  //< USING_PRINTER
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >=
