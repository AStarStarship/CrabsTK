/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/script_itoa.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_CONSOLE
#define HEADER_FOR_CRABS_CONSOLE
// Dependencies:
#include "printer.h"
// End dependencies.

namespace _ {

/* Utility class for printing to the CRABS Output. */
template <size_t kSize = kBufferSizeDefault, typename Char = char>
class Console {
 public:
  /* Constructs the Printer& pointers to the buffer_. */
  Console() : out_(buffer_, kSize) { *buffer_ = 0; }

  /* Prints the buffer_. */
  ~Console() { std::cerr << buffer_; }

  /* Gets the begin of the Console buffer. */
  inline Char* GetBegin() { return buffer_; }

  /* Gets the Printer. */
  inline PrinterN<Char>& Out() { return out_; }

 private:
  PrinterN<Char> out_;                 //< Printer.
  char buffer_[kSize * sizeof(Char)];  //< Console buffer.
};

#define COUT ::Console<>().Out()

}  // namespace _

#endif  //< #if HEADER_FOR_CRABS_CONSOLE
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
