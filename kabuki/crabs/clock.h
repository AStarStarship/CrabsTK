/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/clock.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_CLOCK
#define HEADER_FOR_CRABS_CLOCK
// Dependencies:
#include "memory.h"
// End dependencies.

namespace _ {

/* A time in seconds format.
    Data structure is identical to std::tm and may be used interchangeably.
*/
struct TimeSeconds {
  int second,            //< Second of the minute [0, 60].
      minute,            //< Minute of the hour [0, 59].
      hour,              //< Hour of the day [0, 23].
      month_day,         //< Day of the month [1, 31].
      month,             //< Months since December [0, 11].
      year,              //< Number of years since epoch. [1900, current]
      week_day,          //< Day of the week [0, 6].
      year_day,          //< Day since January first [0, 365].
      daylight_savings;  //< Flag for if it's daylight savings time (-1 if no).
};

/* A time in seconds format.
Data structure is identical to std::tm and may be used interchangeably.
*/
struct TimeMicroseconds {
  int microseconds;      //<
  int second,            //< Second of the minute [0, 60].
      minute,            //< Minute of the hour [0, 59].
      hour,              //< Hour of the day [0, 23].
      month_day,         //< Day of the month [1, 31].
      month,             //< Months since December [0, 11].
      year,              //< Number of years since epoch. [1900, current]
      week_day,          //< Day of the week [0, 6].
      year_day,          //< Day since January first [0, 365].
      daylight_savings;  //< Flag for if it's daylight savings time (-1 if no).
};

#if USING_PRINTER

/* Writes the given time to the text buffer. */
KABUKI char* PrintClock(time_t t, char* buffer, char* buffer_end);

#endif  //< #if USING_PRINTER

/* Creates a timestamp of the current time. */

enum {
  kSecondsPerMinute = 60,
  kSecondsPerHour = 60 * kSecondsPerMinute,
  kSecondsPerDay = 24 * kSecondsPerHour,
  kTimeEpoch = 1900,
};

/* Gets the current microsecond timestamp. */
inline time_us_t ClockNow() {
  // std::chrono::microseconds us (1);
  // chrono_timestamp ts = time_point_cast<microseconds>(system_clock::now ());
  return 0;
}

/* Portable std::tm. */
KABUKI std::tm* ClockTimeLocal(std::tm* std_tm, std::time_t const& time);

/* Generates a timestamp from a Unix timestamp and 32-bit microsecond
    timestamp. */
KABUKI time_us_t ClockPackTimestamp(time_t unixTime, int32_t microseconds);

/* Gets the seconds since January 1, 1970. */
KABUKI time_t ClockGetSeconds(time_us_t t);

/* Gets the microsecond time. */
KABUKI int32_t ClockGetMicroseconds(time_us_t timestamp);

/* Gets the number of days in a months.
    @todo Maybe get some open-source date utility? */
KABUKI int ClockNumDaysMonth(time_t t);

/* Gets the number of days in a months.
    @param month The month index 0-11.
    @param year   */
KABUKI int ClockNumDaysMonth(int month, int year = 1);

/* Gets the abbreviated day of the week char of the given day number 1-7. */
KABUKI const char* ClockDayOfWeek(int day_number);

/* Gets the abbreviated day of the week char of the given day number 1-7. */
KABUKI char ClockDayOfWeekInitial(int day_number);

/* Compares the two the time and prints the results. */
KABUKI int ClockCompare(time_t time_a, time_t time_b);

/* Compares the given time_t to the time and prints the results. */
KABUKI int ClockCompare(time_t t, int year, int month, int day, int hour,
                        int minute, int second);

/* Zeros out the calendar_time struct.
    @param calendar_time A calendar time struct to zero out. */
KABUKI void ClockZeroTime(tm& std_tm);

KABUKI time_t ClockTime(int year, int month, int day, int hour = 0,
                        int minute = 0, int second = 0);
}  // namespace _

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#endif  //< HEADER_FOR_CRABS_CLOCK
