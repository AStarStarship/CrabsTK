/* Kabuki Toolkit @version 0.x
@file    /kabuki/0/01_test_f2/10/seam_ascii_list.h
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

#include <kabuki/f2/global.h>

#include "seam_header.inl"

namespace _ {

static const char* _0_0_0__09_ASCII_List(char* seam_log, char* seam_end,
                                         const char* args) {
#if SEAM >= SEAM_0_0_0__09
  DTEST_BEGIN;

  List<> list(36, 960);
  Assert(list.This())

      PRINTF("\nPushing items on to the List stack...\n")

          const int test_ints[] = {'1', '2', '3', '4'};

  const int list_test_count = 12;
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i);
    list.Push(SI4, &test_ints[i++]);
    COUT << '\n'
         << list << '\n'
         << "\n\n"
         << Socket(list.This(), list.This()->size);
  }
  const float test_floats[] = {9.0, 10.0, 11.0, 12.0};
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i);
    list.Push(FLT, &test_floats[i++]);
    PRINT('\n');
    PRINT(list);
    PRINT ('\n'');
    PRINT ("\n\n"');
    PRINT (Socket (list.This (), list.This ()->size));
  }

  const char* test_strings[] = {"Test", " 1, ", " 2, ", " 3"};
  for (int i = 0; i < 4;) {
    PRINTF("\ni%i:", i);
    list.Push(STR, test_strings[i++]);
    PRINT('\n');
    PRINT(list);
    PRINT('\n');
    PRINT("\n\n");
    PRINT(Socket(list.This(), list.This()->size));
  }

  for (int i = list_test_count - 1; i > 0; --i) list.Pop();

  DTEST_END;
#endif
  return nullptr;
}
}  // namespace _

#include "seam_footer.inl"
