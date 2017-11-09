/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/clock.h
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

#include "utils.h"

namespace _ {

/** Portable std::tm. */
inline std::tm* ClockLocaltime (std::tm* std_tm, std::time_t const & time) {
    if (std_tm == nullptr)
        return nullptr;
#if (defined(__MINGW32__) || defined(__MINGW64__))
    memcpy (&tm_snapshot, ::localtime (&time), sizeof (std::tm));
#elif (defined(MINGW32) || defined(_WIN32) || defined(__WIN32__))
    localtime_s (std_tm, &time);
#else
    localtime_r (&time, std_tm); // POSIX  
#endif
    return std_tm;
}

/** Gets the current microsecond timestamp. */
KABUKI timestamp_t ClockTimestampNow ();

/** Generates a timestamp from a Unix timestamp and 32-bit microsecond timestamp.
*/
KABUKI timestamp_t ClockPackTimestamp (time_t unixTime, int32_t microseconds);

/** Gets the seconds since January 1, 1970. */
KABUKI time_t ClockGetSeconds (timestamp_t t);

/** Gets the microsecond time. */
KABUKI int32_t ClockGetMicroseconds (timestamp_t timestamp);

/** Creates a timestamp of the current time. */

enum {
    NUM_SEC_MIN = 60 * 60,
    NUM_SEC_HOUR = 60 * NUM_SEC_MIN,
    NUM_SEC_DAY = 24 * NUM_SEC_HOUR,
    TIME_EPOCH = 1900,
};

/** Prints the given tm struct to the console. */
KABUKI bool ClockPrintTimeStruct (tm* std_tm);

/** Prints the given time to the console. */
KABUKI void ClockPrintTime (time_t t);

/** Prints the given Script Timestamp to the console. */
KABUKI void ClockPrintTimestamp (timestamp_t timestamp);

/** Prints the given time to the console. */
KABUKI void ClockPrintDateTime (time_t t);

/** Prints the given std_tm to the char buffer. */
KABUKI bool ClockPrintTimeStructString (char* buffer, int buffer_size, tm* std_tm);

/** Prints the given time to the char buffer. */
KABUKI bool ClockPrintDateTimeString (char* buffer, int buffer_size, time_t t);

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
    int  hour, int minute, int second);

/** Parses a time or time delta from a a char starting with an '@' sign..
    @param input  The char to parse.
    @param hour   The location to write the number of hours to.
    @param minute The location to write the number of minutes to.
    @param Second The location to write the number of seconds to.
    @return The offset The end of where the parser exited successfully at.
    @code
    @4        (This is 4AM)
    @4PM      (No space required)
    @4:20P    (Or M)
    @4:20 PM
    @16:20
    @4:20 am
    @4:20a.m.
    @4:20:00
    @4:20:00AM
    @16:20:00
    @endcode
*/
KABUKI const char* ClockParseTimeString (const char* input, int* hour,
    int* minute, int* second);

/** Parses a time or time delta from a a char starting with an '@' sign..
    @param hour   The location to write the number of hours to.
    @param minute The location to write the number of minutes to.
    @param Second The location to write the number of seconds to.
    @return The offset The end of where the parser exited successfully at.
*/
KABUKI char* ClockParseTime (char* input, int* hour, int* minute, int* second);

/** Converts a keyboard input to char and deletes the char. 
    The only format this function currently supports is 2017-08-12@11:36:00PM".
    @todo Add new and auto-detect formats.
          1/2/99,  1/2/1999, 1-2-1999, 2017-8-12
*/
KABUKI const char* ClockParseTimeStructString (const char* input, tm* std_tm);

/** Converts a keyboard input to a human-readable manipulatable form. */
KABUKI char* ClockParseTimeStruct (char* input, tm* result);

/** Converts a keyboard input to a time_t. */
KABUKI const char* ClockParseUnixTimeString (const char* input, time_t& result);

/** Converts a keyboard input to a time_t. */
KABUKI char* ClockParseUnixTime (char* input, time_t& result);

/** Zeros out the calendar_time struct.
    @param calendar_time A calendar time struct to zero out. */
KABUKI void ClockZeroTime (tm* std_tm);

}       //< namespace _
#endif  //< SCRIPT_CLOCK_H
