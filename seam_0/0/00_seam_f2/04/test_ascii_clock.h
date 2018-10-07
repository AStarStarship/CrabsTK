/* Kabuki Toolkit @version 0.x
@file    /seam_0/0/01_test_f2/00/00/seam.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include <kabuki/f2/global.h>

#include "header.h"

namespace _ {

static const char* Seam_00_00_00__04(char* seam_log, char* seam_end,
                                     const char* args) {
#if SEAM == SEAM_0_0_0__04
  if (!TestBegin(seam_log, seam_end, args)) return __FUNCTION__;

  PRINT_HEADING("\n\n    Testing TextScanTime...");

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
    result = TextScanTime(strings[i], t);
    // Assert (!ClockCompare (t, 2018, 8, 9, 0, 0, 0))
  }

  PRINTF("\n\n    Testing more valid input...\n");

  t = ClockTimeTMS(8, 9, 17, 4, 20);
  Print(buffer, buffer + kSize, t);
  result = TextScanTime(buffer, t_found);
  Assert(ClockCompare(t_found, t))

      t = ClockTimeTMS(2020, 4, 20, 4, 20);
  Print(buffer, buffer + kSize, t);
  result = TextScanTime(buffer, t_found);
  Assert(ClockCompare(t, t_found))

      t = ClockTimeTMS(1947, 12, 7, 23, 5, 7);
  Print(buffer, buffer + kSize, t);
  result = TextScanTime(buffer, t_found);
  Assert(ClockCompare(t, t_found));

  PRINT_HEADING("\nTesting invalid input...\n");
  TextScanTime("cat", t);

  TextScanTime("2017-30-40", t);

  PRINTF("\nDone testing date parsing utils! :-)\n");
#endif
  return nullptr;
}
}  // namespace _

#include "footer.h"
