/* Kabuki Toolkit
@file    ~/0_seam_tests/test_seam_0_.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <stdafx.h>
#if SEAM >= SEAM_0_0_03
// Dependencies:
#include "test_seam_0.h"

#if SEAM == SEAM_0_0_03
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message)         \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#define PRINT_HEADING(message)                   \
  std::cerr << "\n+";                            \
  for (int i = 79; i > 0; --i) std::cerr << '-'; \
  std::cerr << "| " << message << "\n+";         \
  for (int i = 79; i > 0; --i) std::cerr << '-';
#else
#define PRINTF(x, ...)
#define PAUSE(message)
#define PRINT_HEADING(message)
#endif

using namespace _;
const char* Test_0_0_03 (char* seam_log, char* seam_end, const char* args) {


  /*
  PRINTF ("\n\nTesting Dictionary...\n\n")
  PRINTF ("\n\nDone testing Dictionary!")

  PRINTF ("\n\nTesting Multimap...\n\n")

  int16_t index;

  Multimap2 multimap (8, kBufferSize);

  Assert (multimap != nullptr)

  index = multimap.Add (multimap, "D", (byte)0xFF);

  Compare (0, index)
  multimap.Print (multimap);
  Compare (0, index)
  index = multimap.Find (multimap, "D");
  Compare (0, index)
  PAUSE ("\n");
  index = multimap.Add (multimap, "C", (byte)0xFF);
  Compare (1, index)
  index = multimap.Find (multimap, "D");
  Compare (0, index)
  index = multimap.Find (multimap, "C");
  Compare (1, index)

  index = multimap.Add (multimap, "BIn", (byte)0xFF);
  Compare (2, index)
  index = multimap.Find (multimap, "D");
  Compare (0, index)
  index = multimap.Find (multimap, "C");
  Compare (1, index)
  index = multimap.Find (multimap, "BIn");
  Compare (2, index)

  index = multimap.Add (multimap, "A", (byte)0xFF);
  Compare (3, index)
  index = multimap.Find (multimap, "D");
  Compare (0, index)
  index = multimap.Find (multimap, "C");
  Compare (1, index)
  index = multimap.Find (multimap, "BIn");
  Compare (2, index)
  index = multimap.Find (multimap, "A");
  Compare (3, index)

  index = multimap.Add (multimap, "abc", (byte)0xFF);
  Compare (4, index)
  index = multimap.Find (multimap, "abc");
  Compare (4, index)

  index = multimap.Add (multimap, "bac", (byte)0xFF);
  Compare (5, index)
  index = multimap.Find (multimap, "abc");
  Compare (4, index)
  index = multimap.Find (multimap, "bac");
  Compare (5, index)

  index = multimap.Add (multimap, "cba", (byte)0xFF);
  Compare (6, index)
  index = multimap.Find (multimap, "abc");
  Compare (4, index)
  index = multimap.Find (multimap, "bac");
  Compare (5, index)
  index = multimap.Find (multimap, "cba");
  Compare (6, index)

  index = multimap.Add (multimap, "cab", (byte)0xFF);
  Compare (7, index)
  index = multimap.Find (multimap, "abc");
  Compare (4, index)
  index = multimap.Find (multimap, "bac");
  Compare (5, index)
  index = multimap.Find (multimap, "cba");
  Compare (6, index)
  index = multimap.Find (multimap, "cab");
  Compare (7, index)

  index = multimap.Add (multimap, "test", (byte)0xFF);
  Compare (index, -1)

  PAUSE ("\n\nDone Multimap!")*/

  PAUSE("\n\nDone Testing SEAM_1_3! ({:-)-+=<")
}

#undef PRINTF
#undef PAUSE
#undef PRINT_HEADING
#endif  //< #if SEAM_MAJOR >= 1 && SEAM_MINOR >= 3
