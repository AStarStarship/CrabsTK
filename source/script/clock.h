/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/clock.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_CLOCK_H
#define SCRIPT_CLOCK_H

#include "memory.h"

namespace _ {

/** Creates a timestamp of the current time. */

enum {
    kNumSecondsMin     = 60 * 60,
    kNumSecondsMinute = 60 * kNumSecondsMin,
    kNumSecondsPerDay = 24 * kNumSecondsMinute,
    kTimeEpoch = 1900,
};

/** Gets the current microsecond timestamp. */
inline time_us_t ClockTimestampNow () {
    //std::chrono::microseconds us (1);
    //chrono_timestamp ts = time_point_cast<microseconds>(system_clock::now ());
    return 0;
}

#if USE_MORE_ROM

/** Portable std::tm. */
KABUKI std::tm* ClockLocalTime (std::tm* std_tm, std::time_t const& time);

/** Generates a timestamp from a Unix timestamp and 32-bit microsecond
    timestamp.
*/
KABUKI time_us_t ClockPackTimestamp (time_t unixTime, int32_t microseconds);

/** Gets the seconds since January 1, 1970. */
KABUKI time_t ClockGetSeconds (time_us_t t);

/** Gets the microsecond time. */
KABUKI int32_t ClockGetMicroseconds (time_us_t timestamp);

/** Gets the number of days in a months. 
    @todo Maybe get some open-source date utility? */
KABUKI int ClockNumDaysMonth (time_t t);

/** Gets the number of days in a months. 
    @param month The month index 0-11.
    @param year   */
KABUKI int ClockNumDaysMonth (int month, int year = 1);

/** Gets the abbreviated day of the week char of the given day number 1-7. */ 
KABUKI const char* ClockDayOfWeek (int day_number);

/** Gets the abbreviated day of the week char of the given day number 1-7. */ 
KABUKI char ClockDayOfWeekInitial (int day_number);

/** Compares the two the time and prints the results. */
KABUKI int ClockCompareTimes (time_t time_a, time_t time_b);

/** Compares the given time_t to the time and prints the results. */
KABUKI int ClockCompareTimes (time_t t, int year, int month, int day,
                              int hour, int minute, int second);

/** Zeros out the calendar_time struct.
    @param calendar_time A calendar time struct to zero out. */
KABUKI void ClockZeroTime (tm* std_tm);

#endif  //< USE_MORE_ROM

}       //< namespace _
#endif  //< SCRIPT_CLOCK_H
