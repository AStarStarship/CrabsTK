/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/timestamp.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_TIMESTAMP_H
#define CHINESE_ROOM_TIMESTAMP_H

#include "utils.h"

namespace _ {

/** Generates a timestamp from a Unix timestamp and 32-bit microsecond timestamp. */
KABUKI timestamp_t PackTimestamp (time_t unixTime, int32_t microseconds);

/** Gets the seconds since January 1, 1970. */
KABUKI time_t GetSeconds (timestamp_t t);

/** Gets the microsecond time. */
KABUKI int32_t GetMicroseconds (timestamp_t timestamp);

/** Creates a timestamp of the current time. */
KABUKI timestamp_t TimestampNow ();

enum {
    NUM_SEC_MIN = 60 * 60,
    NUM_SEC_HOUR = 60 * NUM_SEC_MIN,
    NUM_SEC_DAY = 24 * NUM_SEC_HOUR,
    TIME_EPOCH = 1900,
};

/** Prints the given tm struct to the console. */
KABUKI bool PrintTimeStruct (tm* moment);

/** Prints the given time to the console. */
KABUKI void PrintTime (time_t t);

/** Prints the given time to the console. */
KABUKI void PrintDateTime (time_t t);

/** Prints the given moment to the string buffer. */
KABUKI bool PrintTimeStructString (char* buffer, int buffer_size, tm* moment);

/** Prints the given time to the string buffer. */
KABUKI bool PrintDateTimeString (char* buffer, int buffer_size, time_t t);

/** Gets the number of days in a months. 
    @todo Maybe get some open-source date utility? */
KABUKI int NumDaysMonth (time_t t);

/** Gets the number of days in a months. 
    @param month The month index 0-11.
    @param year   */
KABUKI int NumDaysMonth (int month, int year = 1);

/** Gets the abbreviated day of the week string of the given day number 1-7. */ 
KABUKI const char * DayOfWeek (int day_number);

/** Gets the abbreviated day of the week string of the given day number 1-7. */ 
KABUKI char DayOfWeekInitial (int day_number);

/** Compares the two the time and prints the results. */
KABUKI int CompareTimes (time_t time_a, time_t time_b);

/** Compares the given time_t to the time and prints the results. */
KABUKI int CompareTimes (time_t t, int year, int month, int day,
    int  hour, int minute, int second);

/** Parses a time or time delta from a a string starting with an '@' sign..
    @param input  The string to parse.
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
KABUKI const char * ParseTimeString (const char * input, int* hour, int* minute, int* second);

/** Parses a time or time delta from a a string starting with an '@' sign..
    @param hour   The location to write the number of hours to.
    @param minute The location to write the number of minutes to.
    @param Second The location to write the number of seconds to.
    @return The offset The end of where the parser exited successfully at.
*/
KABUKI char* ParseTime (char* input, int* hour, int* minute, int* second) {
    return (char*)ParseTimeString (input, hour, minute, second);
}

/** Converts a keyboard input to string and deletes the string. 
    The only format this function currently supports is 2017-08-12@11:36:00PM".
    @todo Add new and auto-detect formats.
          1/2/99,  1/2/1999, 1-2-1999, 2017-8-12
*/
KABUKI const char * ParseTimeString (const char * input, tm* moment);

/** Converts a keyboard input to a human-readable manipulatable form. */
KABUKI char* ParseTime (char* input, tm* result);

/** Converts a keyboard input to a time_t. */
KABUKI const char * ParseTimeString (const char * input, time_t& result);

/** Converts a keyboard input to a time_t. */
KABUKI char* ParseTime (char* input, time_t& result);

/** Zeros out the calendar_time struct.
    @param calendar_time A calendar time struct to zero out. */
KABUKI void ZeroTime (tm* moment);

}       //< namespace _

#endif  //< CHINESE_ROOM_TIMESTRAMP_H
