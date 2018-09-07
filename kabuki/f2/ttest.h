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

#include "test.h"

#include "tconsole.h"

namespace _ {

class SeamMajor {
 public:
  SeamMajor(SeamMinorTest** minor_seam, size_t minor_seam_count)
      : minor_seam(minor_seam), minor_seam_count(minor_seam_count) {}

  /* Runs a unit minor_seam with the given global parameter list.
  @return Nil upon success or an Assertion with debug info upon failure.
  @param  args      An array of string argumetns.
  @param  arg_count The number of arguments. */
  Assertion Run(const char* args, int arg_count, int seam_page,
                int seam_major) {
    enum { kSize = 128 };
    char buffer[kSize];
    char *cursor, *end = buffer + kSize - 1;
    cursor = Print(buffer, end, "Testing Major Seam ");
    cursor = Print(cursor, end, seam_major);
    PrintHeading(buffer);
    for (int seam_minor = 0; seam_minor < kSeamCount; seam_minor++) {
      cursor = Print(buffer, end, "Testing SEAM_");
      cursor = Print(cursor, end, seam_page);
      cursor = Print(cursor, end, '_');
      cursor = Print(cursor, end, seam_major);
      cursor = Print(cursor, end, '_');
      cursor = Print(cursor, end, seam_minor);
      PrintHeading(buffer);
      Assertion assertion = (*minor_seam[seam_minor])(args, arg_count);
      if (assertion.Failed()) return assertion;
      Printf("\n\nDone testing SEAM_%i_%i_%i", seam_page, seam_major,
             seam_minor);
    }
    Print("\n\nDone testing Major Seam");
    Print(seam_major);
    Print('\n', '\n');
    return Assertion();
  }

  template <const char... N>
  static inline SeamMinor MinorSeams(size_t& seam_count) {
    static SeamMinorTest* minor_seams[sizeof...(N)] = {N...};
    return SeamMajor(minor_seams, sizeof...(N));
  }

 private:
  /* Array of unit tests for each minor seam. */
  SeamMinorTest** minor_seam;  //< Array of function pointers.
  size_t minor_seam_count;     //< Minor seam count.
};

Assertion TestSEAM_0_0_0(const char* args, int arg_count) {
  return Assertion();
}

Assertion TestSEAM_0_0_1(const char* args, int arg_count) {
  return Assertion();
}

template <const char* kName, const char* kHeading = "">
int RunUnitTests(const char* args, int arg_count) {
  PrintHeading(kName);
  Print('\n');
  Print(kHeading);
  Print('\n');
  PrintLine();
  PrintLn();
  if (assertion.) {
    Print("\nUnit tests failed.");
    return APP_;
  }
  Pause("Completed running unit tests successsfully! :-)");
  return APP_EXIT_SUCCESS;
}

// static SeamMajor seam_1 =
//   SeamMajor::SeamMinor<&TestSEAM_0_0_0, &TestSEAM_0_0_1>();
}  // namespace _

#endif  //< INCLUDED_F2_TTEST
