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
#include "../../kabuki/crabs/global.h"
#include "test_seam_0.h"

#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1

using namespace _;
using namespace std;

#if SEAM_MAJOR == 0 && SEAM_MINOR == 0
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

TEST_GROUP(SEAM_0_1){void setup(){} void teardown(){PRINT_PAUSE("\n")}};

TEST(SEAM_0_1, SEAM_0_1_0) { printf("\n    Testing SEAM_0_0... "); }

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 1
