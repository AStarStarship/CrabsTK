/* Kabuki Toolkit @version 0.x
@file    /kabuki/f2/0_0_0__04_clock.h
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

#include <cclock.h>

#if SEAM == _0_0_0__04
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

namespace _ {

static const char* _0_0_0__04_Clock(char* seam_log, char* seam_end,
                                    const char* args) {
#if SEAM >= _0_0_0__04
  TEST_BEGIN;

  PRINT_HEADING("\n\n    Testing StringScanTime...");

  Tms t, t_found;
  const char* result;

  // @note The following dates must be the current day to work right in order
  //       to auto-detect the year.
  const char* strings[] = {
      "8/9",
      "08/09",
      "8/9/18",
      "8/09/18",
      "8/9/2018",
      "8/09/2018",
      "8/09/2018",
      "08/9/2018",
      "8/09/2018@00",
      "8.09.2018@00AM",
      "8/09/2018@00:00",
      "8/09/18@00:0AM",
      "8/09/2018@00:00:00",
      "8/09/2018@00:00:00AM",
      "2018-08-09@00:00:00AM",
      "2018-08-09@00:00:00am",
      "2018-08-09@00:00:00A",
      "2018-08-09@00:00:00a ",
  };

  for (int i = 0; i < 18; ++i) {
    PRINT_LINE('-');
    PRINTF("\n    %i", i);
    Tms t = 0;
    result = StringScanTime(strings[i], t);
    // Assert (!ClockCompare (t, 2018, 8, 9, 0, 0, 0))
  }

  PRINTF("\n\n    Testing more valid input...\n");

  enum { kSize = 128 };
  char buffer[kSize];

  t = ClockTimeTMS(8, 9, 17, 4, 20);
  Print(buffer, buffer + kSize, t);
  result = StringScanTime(buffer, t_found);
  ASSERT(ClockCompare(t_found, t));

  t = ClockTimeTMS(2020, 4, 20, 4, 20);
  Print(buffer, buffer + kSize, t);
  result = StringScanTime(buffer, t_found);
  ASSERT(ClockCompare(t, t_found));

  t = ClockTimeTMS(1947, 12, 7, 23, 5, 7);
  Print(buffer, buffer + kSize, t);
  result = StringScanTime(buffer, t_found);
  ASSERT(ClockCompare(t, t_found));

  PRINT_HEADING("\nTesting invalid input...\n");
  StringScanTime("cat", t);

  StringScanTime("2017-30-40", t);

  PRINTF("\nDone testing date parsing utils! :-)\n");
#endif
  return nullptr;
}
}  // namespace _

#include "test_footer.inl"
