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

_::TestResult Test_0_0_03 (const char* args) {
  int i = 0;  //< Shared looping variable.

  PRINT_HEADING("\nTesting Console ().Print ()... ");

  COUT << "Testing " << 1 << ", 2, " << -3;

  PRINT_HEADING("\nTesting SEAM_0_0_3... ");

  PRINTF("\n\nTesting ASCII OBJ Types");

  PRINTF("\n\nTesting Stack...\n\nPushing items on to the Stack...\n");

  Stack<> stack(8);

  for (i = 0; i <= 10; ++i) stack.Push(i);

  COUT << stack;

  PRINTF("\nPopping items off the Stack...\n");

  for (i = 10; i > 0; i--) Compare(i, stack.Pop())

  COUT << stack;

  PRINTF("\n\nDone testing Stack!")

  /*
  PRINTF ("\n\nTest Array...\n\n");
  static const int array_3d_exected[2][2][2] = { { { 0, 1 }, { 2, 3 } },
                                                 { { 4, 5 }, { 6, 7 } }
                                               };
  const int* test = Dimensions<2, 2, 2> ();
  Array<int> test_array (test);
  *test_array.Elements () = { { { 0, 1 }, { 2, 3 } },
                              { { 4, 5 }, { 6, 7 } }
                            };
  i = 0;
  int* array_base = test_array.Elements ();
  for (int z = 0; z < 2; ++z)
      for (int y = 0; y < 2; ++y)
          for (int x = 0; x < 2; ++x)
              Compare (i++, array_3d_exected[x][y][z])
  















  PAUSE ("\n\nDone Array!")
  */

  PRINT_HEADING("\n\nTest List...\n\n")

  List<> list(36, 960);
  Assert(list.This())

  PRINTF("\nPushing items on to the List stack...\n")

  const int test_ints[] = {'1', '2', '3', '4'};

  const int list_test_count = 12;
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i)
    list.Push(SI4, &test_ints[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
  }
  const float test_floats[] = {9.0, 10.0, 11.0, 12.0};
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i)
    list.Push(FLT, &test_floats[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
  }

  const char* test_strings[] = {"Test", " 1, ", " 2, ", " 3"};
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i)
    list.Push(STR, test_strings[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
    system("PAUSE");
  }

  for (int i = list_test_count - 1; i > 0; --i) list.Pop();

  PRINTF("\n\nDone testing List!")

  PRINTF("\n\nTesting Map...\n\n")

  Map<> map;

  PRINTF("\n\nDone testing Map!")

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
