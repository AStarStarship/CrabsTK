/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/ttest.h
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
#include <stdafx.h>
#ifndef INCLUDED_F2_TTEST
#define INCLUDED_F2_TTEST

#include <chrono>
#include <random>
#include "test.h"

template <typename UI, typename SI, typename Float>
class SeamMinor {
 public:
  SeamMinor() {}

  ErrorAssert* Run() {
    auto start = high_resolution_clock::now(),
         stop = high_resolution_clock::now();
    auto delta = duration_cast<milliseconds>(stop - start).count();

    const ErrorAssert* result = test_seam(const char* args, int arg_count);
  }

 private:
  int seam,                                               //< SEAM
      seam_page,                                          //< SEAM_PAGE
      seam_major,                                         //< SEAM_MAJOR
      seam_minor;                                         //< SEAM_MINOR
  SeamMinorTest<uint32_t, int32_t, float> test_seam_32;   //< 32-bit unit tests.
  SeamMinorTest<uint64_t, int64_t, double> test_seam_64;  //< 64-bit unit tests.
};

template <>
class SeamMajor {
 public:
  SeamMajor() {}

  const char* Run() {}

 private:
  static SeamMinor test[SEAM_COUNT];  //< Array
}

}  // namespace _

#endif  //< INCLUDED_F2_TTEST
