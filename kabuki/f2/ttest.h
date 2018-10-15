/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/ttest.h
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

#ifndef INCLUDED_KABUKI_F2_TTEST
#define INCLUDED_KABUKI_F2_TTEST 1

#include "test.h"

#include "tconsole.h"

namespace _ {

template <TestCase... N>
const char* TestTree(char* seam_log, char* seam_end, const char* args) {
  static TestCase nodes[sizeof...(N)] = {N...};
  return TestTree(seam_log, seam_end, args, nodes, sizeof...(N));
}

template <TestCase... N>
int TestTree(char* seam_log, intptr_t seam_log_size, const char* args) {
  static TestCase nodes[sizeof...(N)] = {N...};
  return TestTree(seam_log, seam_log + seam_log_size - 1, args, nodes,
                  sizeof...(N));
}

template <typename Char>
bool Test(const Char* a, const Char* b) {
  // int result = StringCompare<Char>(a, b);
  // if (!result) return false;
  Print("\nERROR: Fix test code.");
  // Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return true;
}

}  // namespace _

#endif  //< INCLUDED_KABUKI_F2_TTEST
