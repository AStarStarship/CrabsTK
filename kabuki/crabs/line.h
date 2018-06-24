/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/line.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_CRABS_LINE
#define HEADER_FOR_CRABS_LINE
// Dependencies:
#include "utf8.h"
// End dependencies.

namespace _ {

/* Utility class for printing a single char token line with operator<<. */
class KABUKI Line {
 public:
  char token;        //< Character to print.
  int column_count;  //< Column count.

  /* Constructor. */
  Line(char token, int column_count);

  /* Prints a string line of the char repeating. */
  Printer1& Print(Printer1& printer);
};

/* Utility class for printing a string line with operator<<. */
class KABUKI StringLine {
 public:
  const char* string;  //< Character to print.
  int column_count;    //< Column count.

  /* Constructor. */
  StringLine(const char* string, int column_count);

  /* Prints a string line of the char repeating. */
  Printer1& Print(Printer1& printer);
};

}  // namespace _

inline _::Printer1& operator<<(_::Printer1& printer, _::Line line) {
  return line.Print(printer);
}

inline _::Printer1& operator<<(_::Printer1& printer, _::StringLine line) {
  return line.Print(printer);
}

#endif  //< HEADER_FOR_CRABS_LINE
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 0
