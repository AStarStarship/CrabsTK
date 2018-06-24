/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_text.cc
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
// Dependencies:
#include "utf8.h"
#include "type.h"
// End dependencies.
#if USING_PRINTER
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PUTCHAR(c) putchar(c)
#define PRINT_HEADING \
  std::cout << '\n';  \
  for (int i = 80; i > 0; --i) std::cout << '-'
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#endif

namespace _ {}  // namespace _
#undef PRINTF
#undef PUTCHAR
#undef PRINT_HEADING
#endif  //< USING_PRINTER
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
