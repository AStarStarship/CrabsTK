/* Kabuki Toolkit
@version 0.x
@file    ~/tests/test_seam_1_1.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
#include <stdio.h>
#include <random>
#include "../../kabuki/crabs/global.h"
#include "test_seam_0.h"

#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1

using namespace _;
using namespace std;

#if SEAM_MAJOR == 0 && SEAM_MINOR == 0
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message)         \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PAUSE(message)
#endif

TEST_GROUP(SEAM_0_1){void setup(){} void teardown(){PAUSE("\n")}};

TEST(SEAM_0_1, SEAM_0_1_0) {
  Printf("\n    Testing SEAM_0_0_1... ");

  // Setup C++1x random number generator.
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint32_t> distr;

  PRINTF("\n\nTesting float scan functions...\n");

  for (int i = 0; i < 1 << 20; ++i) {
    uint32_t value;
    float flt_expected, flt_found;
    do {
      value = distr(eng);
      flt_expected = static_cast<float>(value);
    } while (!IsFinite(flt_expected));
    char buffer[25];
    Print(buffer, buffer + 24, flt_expected);
    CHECK(scanf_s("%f", &flt_found));
    CHECK_EQUAL(value, flt_expected);
  }

  PRINTF("\n\nTesting float scan functions...\n");

  for (int i = 0; i < 1 << 20; ++i) {
    uint32_t value;
    float flt_expected, flt_found;
    do {
      value = distr(eng);
      flt_expected = static_cast<float>(value);
    } while (!IsFinite(flt_expected));
    char buffer[25];
    sprintf_s(buffer, 24, "%f", flt_expected);
    CHECK(Scan(buffer, flt_expected));
    CHECK_EQUAL(value, flt_expected);
  }

  PAUSE("Done testing SEAM_0_0_2! ({:-)-+=<");
}

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
