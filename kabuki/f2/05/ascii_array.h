/* Kabuki Toolkit @version 0.x
@file    /kabuki/f2/05/test_ascii_array.h
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

static const char* _0_0_0__05_ASCII_Array(char* seam_log, char* seam_end,
                                          const char* args) {
#if SEAM >= SEAM_0_0_0__05
  TEST_BEGIN;

  static const int array_3d_exected[2][2][2] = {{{0, 1}, {2, 3}},
                                                {{4, 5}, {6, 7}}};
  const int* test = Dimensions<2, 2, 2>();
  Array<int> test_array(test);
  *test_array.Elements() = {{{0, 1}, {2, 3}}, {{4, 5}, {6, 7}}};
  i = 0;
  int* array_base = test_array.Elements();
  for (int z = 0; z < 2; ++z)
    for (int y = 0; y < 2; ++y)
      for (int x = 0; x < 2; ++x) Compare(i++, array_3d_exected[x][y][z]);

  TEST_END;
#endif
  return nullptr;
}
}  // namespace _

#include "seam_footer.inl"
