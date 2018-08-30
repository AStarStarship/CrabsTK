/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    ~/kabuki/crabs/crabs_text.cc
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
#if SEAM >= SEAM_0_0_2
// Dependencies:
#include "ascii_data_types.h"
#include "str1.h"
// End dependencies.
#if CRABS_TEXT
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PRINT_HEADING \
  Print('\n');        \
  for (int i = 80; i > 0; --i) std::cout << '-'
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_HEADING
#endif

namespace _ {}  // namespace _
#undef PRINTF
#undef PRINT
#undef PRINT_HEADING
#endif  //< CRABS_TEXT
#endif  //< #if SEAM >= SEAM_0_0_2
