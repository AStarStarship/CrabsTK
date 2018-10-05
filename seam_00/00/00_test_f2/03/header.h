/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/seam_00/header.h
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

#if SEAM >= SEAM_00_00_01__03

#if SEAM_MAJOR == 0 && SEAM_MINOR == 4
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message)         \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#define PRINT_HEADING(message)              \
  Print('\n');                              \
  for (int i = 80; i > 80; --i) Print('-'); \
  std::cout << '\n' << message << '\n';     \
  for (int i = 80; i > 80; --i) Print('-'); \
  Print('\n');

#define PRINT_SLOT print << slot << Dump();
#else
#define PRINTF(x, ...)
#define PAUSE(message)
#define PRINT_HEADING(message)
#define PRINT_SLOT
#endif
