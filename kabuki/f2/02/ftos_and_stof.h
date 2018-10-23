/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\f2\02\ftos_and_stof.h
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
#include <random>

using namespace std;

#include "seam_header.inl"

namespace _ {

const char* _0_0_0__02_FtoS_and_StoF(char* seam_log, char* seam_end,
                                     const char* args) {
#if SEAM >= SEAM_0_0_0__02
  DTEST_BEGIN;

  enum { kSize = 24 };
  char buffer[kSize + 1];

  // Setup C++1x random number generator.
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint64_t> distr;

  uint64_t value;
  double dbl_expected, dbl_found;

  PRINTF("\n\nTesting Float Ceiling<Float, UI> (Float)...\n");

  for (int i = 0; i < 1 << 20; ++i) {
    do {
      value = distr(eng);
      dbl_expected = static_cast<double>(value);
    } while (!IsFinite(dbl_expected));
    dbl_found = Ceiling(dbl_expected);
    dbl_expected = ceil(dbl_expected);
    Test(dbl_expected, dbl_found);
  }

  PRINTF("\n\nTesting float Char* Print<Char> (Char*, Char*, float&)...\n");

  for (int i = 0; i < 1 << 20; ++i) {
    do {
      value = distr(eng);
      dbl_expected = static_cast<double>(value);
    } while (!IsFinite(dbl_expected));
    Print<>(buffer, buffer + kSize, dbl_expected);
    Assert(scanf_s("%lf", &dbl_found));
    Test(value, dbl_expected);
  }

  PRINT(
      "\n\nTesting const Char* Scan<Char> (const Char*, const Char*, float&) "
      "functions...\n");

  for (int i = 0; i < 1 << 20; ++i) {
    do {
      value = distr(eng);
      dbl_expected = static_cast<double>(value);
    } while (!IsFinite(dbl_expected));
    char buffer[24];
    sprintf_s(buffer, kSize, "%f", dbl_expected);
    Assert(Scan(buffer, dbl_expected));
    Test(value, dbl_expected);
  }

  DTEST_END;
#endif
  return nullptr;
}
}  // namespace _
#include "seam_footer.inl"
