/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\f2\0_0_0__03_ftos_and_stof.h
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

#include <cmath>
#include <cstdio>

#include "cbinary.h"
#include "crng.h"

using namespace std;

#if SEAM == _0_0_0__03
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

namespace _ {

const char* _0_0_0__03_FtoS_and_StoF(char* seam_log, char* seam_end,
                                     const char* args) {
#if SEAM >= _0_0_0__03
  TEST_BEGIN;

#if SEAM == _0_0_0__03
  enum { kTestCount = 1 << 15 };
#else
  enum { kTestCount = 1 << 20 };
#endif

  enum { kSize = 31 };
  char buffer[kSize + 1];

  uint64_t value;
  double dbl_expected, dbl_found;

  PRINTF("\n\nTesting Float Ceiling<Float, UI> (Float)...\n");

  for (int i = 0; i < kTestCount; ++i) {
    do {
      value = RandomUI8();
      dbl_expected = static_cast<double>(value);
    } while (!IsFinite(dbl_expected));
    dbl_found = ceil(dbl_expected);
    dbl_expected = Ceiling(dbl_expected);
    Test(dbl_expected, dbl_found);
  }

  PRINT(
      "\n\nTesting const Char* Scan<Char> (const Char*, const Char*, float&) "
      "functions...\n");

  for (int i = 0; i < kTestCount; ++i) {
    do {
      value = RandomUI8();
      dbl_expected = static_cast<double>(value);
    } while (!IsFinite(dbl_expected));
    sprintf_s(buffer, kSize, "%lf", dbl_expected);
    ASSERT(Scan(buffer, dbl_found));
    AVOW(dbl_expected, dbl_found);
  }

  PRINTF("\n\nTesting Char* Print<Char> (Char*, Char*, Float)...\n");

  for (int i = 0; i < kTestCount; ++i) {
    do {
      value = RandomUI8();
      dbl_expected = static_cast<double>(value);
    } while (!IsFinite(dbl_expected));
    Print(buffer, buffer + kSize, dbl_expected);
    int r = sscanf_s(buffer, "%lf", &dbl_found);
    ASSERT(r);
    AVOW(dbl_expected, dbl_found);
  }

  TEST_END;
#endif
  return nullptr;
}
}  // namespace _
#include "test_footer.inl"
