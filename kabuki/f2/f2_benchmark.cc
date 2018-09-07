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

template <typename UI = uint64_t, typename SI = int64_t,
          typename Float = double, typename Char = char>
class SeamMinor {
 public:
  SeamMinor() {}

  ErrorAssert* Run() {
    auto start = high_resolution_clock::now(),
         stop = high_resolution_clock::now();
    auto delta = duration_cast<milliseconds>(stop - start).count();

    const ErrorAssert* result = stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();

    return test_seam(const Char* args, int arg_count);
    ;
  }

 private:
  int seam,        //< SEAM
      seam_page,   //< SEAM_PAGE
      seam_major,  //< SEAM_MAJOR
      seam_minor;  //< SEAM_MINOR
  SeamMinorTest<UI, SI, Float, Char> test_seam;
  //< Minor seam unit tests.
};

template <typename UI = uint64_t, typename SI = int64_t,
          typename Float = double, typename Char = char>
class SeamMajor {
 public:
  SeamMajor() {}

  /* Inserts a new Minor Seam into this Major Seam*/
  const Char* Insert(SeamMinorTest<UI, SI, Float, Char>* new_test, int seam) {
    c(seam >= 0 && seam <= kSeamCount);
    SeamMinor<UI, SI, Float, Char>**cursor = &test[0],
                             **target = &test[seam - 1];
    SeamMinor<UI, SI, Float, Char>* current;
    bool run_tests = true;
    while (cursor < target) {
      run_tests = (*cursor++) == nullptr;
      if (!run_tests) break;
    }
    cursor = &tests[kSeamCount - 1];
    while (cursor > target) {
      run_tests = (*cursor--) == nullptr;
      if (!run_tests) break;
    }
    ASSERT(*target == nullptr, "Attempted to insert same target twice!")
    *target = new_test;
  }

 private:
  /* Array of unit tests for each minor seam.
  Please note that static memory initizes as all zeros. */
  static SeamMinor<UI, SI, Float, Char>* test[kSeamCount];
};

}  // namespace _

#endif  //< INCLUDED_F2_TTEST
