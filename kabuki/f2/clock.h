/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/clock.h
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

#if SEAM >= SEAM_0_0_0__05
#ifndef INCLUDED_KABUKI_F2_CLOCK
#define INCLUDED_KABUKI_F2_CLOCK

#include "config.h"

namespace _ {

/* A time in seconds and optional microseconds format that is compatible with
the C++ standard library.
Data structure is identical to std::tm with the execution that it has an
additional microseconds from start of second variable.
*/
struct API Clock {
  int second,  //< Second of the minute [0, 59].
      minute,  //< Minute of the hour [0, 59].
      hour,    //< Hour of the day [0, 23].
      day,     //< Day of the month [1, 31].
      month,   //< Months since December [0, 11].
      year;    //< Number of years since epoch [-1902, 1970] U [1970, 2038].

  /* Constructs a clock with the set to Epoch. */
  Clock();

  /* Constructs a clock from the given 32-bit seconds timestamp. */
  Clock(Tms time);

  /* Constructs a clock from the given 64-bit seconds timestamp. */
  Clock(Tme time);

  /* Sets the time to the given 32-bit TMS timestamp. */
  void SetTime(Tms t);

  /* Sets the time to the given 32-bit TMS timestamp. */
  void SetTime(Tme t);
};

/* A sub-second timestamp composed of a TMS and a UI4 tick.
  Operation of the Tss is intended for two scenarios:
  1. Processor has a real microsecond timer stored as a 24-bit value.
  2. Processor is an x86 and timer gets updated with a tread or OS.
  In the real microsecond timer scenario the processor will just
  take the unsigned in value and copy it to the UI4 member. In the
  case of the OS having a variable update tick period, the ticker will
  work best if the value gets incremented using the ++operator and you will
  need to use modulo updates_per_second unless the timer is set to 64 updates
  per second or some other power of 2 in which case bit masking is the
  tool of choice. For desktop operating systems other threads may hijack the
  OS scheduler.
*/
struct Tss {
  Tms seconds;     //< Seconds since epoch.
  uint32_t ticks;  //< Ticks since epoch.
};

enum ClockConstants {
  kClockEpochInit = 1970,                    //< Initial clock epoch.
  kSecondsPerMinute = 60,                    //< Number of seconds in an minute.
  kSecondsPerHour = 60 * kSecondsPerMinute,  //< Number of seconds in an hour.
  kSecondsPerDay = 24 * kSecondsPerHour,     //< Number of seconds in an day.
  kSecondsPerYear = kSecondsPerDay * 365,    //< Number of seconds in an year.
  // Number of seconds in a a leap year.
  kSecondsPerEpoch = 10 * kSecondsPerYear,  //< Number of seconds in an year.
  kDaysInJanuary = 31,                      //< Number of days in January.
  kDaysInFebruary = 28,                     //< Number of days in February.
  kDaysInMarch = 31,                        //< Number of days in March.
  kDaysInApril = 30,                        //< Number of days in April.
  kDaysInMay = 31,                          //< Number of days in May.
  kDaysInJune = 30,                         //< Number of days in June.
  kDaysInJuly = 31,                         //< Number of days in July.
  kDaysInAugust = 31,                       //< Number of days in August.
  kDaysInSeptember = 30,                    //< Number of days in September.
  kDaysInOctober = 31,                      //< Number of days in October.
  kDaysInNovember = 30,                     //< Number of days in November.
  kDaysInDecember = 31,                     //< Number of days in December.
};

/* Gets the 32-bit TMS clock epoch. */
API inline int16_t ClockEpoch();

/* Lookup table for converting from day-of-year to month. */
API inline const int16_t* ClockLastDayOfMonth();

/* Lookup table for converting from day-of-year to month. */
API inline const int16_t* ClockLastDayOfMonthLeapYear();

/* Returns which month the given day is in based on the year. */
API inline int MonthByDay(int day, int year);

/* Initializes the clock from the given timestamp. */
API Clock* ClockInit(Clock& clock, Tms time);

/* Initializes the clock from the given timestamp. */
API Clock* ClockInit(Clock& clock, Tme time);

/* Initializes the clock from the given 64-bit microsecond timestamp. */
API Tss& StopwatchInit(Tss& clock, Tms t, uint32_t ticks);

/* Initializes the clock from the given timestamp. */
API Clock* ClockInit(Clock& clock);

/* Gets the current microsecond timestamp. */
API inline Tme ClockNow();

/* Calculates the seconds from epoch from the clock and stores it to the result.
 */
API inline Tms ClockTMS(Clock& clock);

/* Calculates the seconds from epoch from the clock and stores it to the result.
 */
API inline Tme ClockTME(Clock& clock);

/* Gets the number of days in a months.
    @todo Maybe get some open-source date utility? */
API int ClockMonthDayCount(Tms t);

/* Gets the number of days in a months.
@param month The month index 0-11.
@param year   */
API int ClockMonthDayCount(int month, int year);

/* Gets the abbreviated day of the week char of the given day number 1-7. */
API const char* ClockWeekDay(int day_number);

/* Gets the abbreviated day of the week char of the given day number 1-7. */
API char ClockDayOfWeekInitial(int day_number);

/* Compares the two the time and prints the results. */
API int ClockCompare(Tms a, Tms b);

/* Compares the two the time and prints the results. */
API int ClockCompare(Tms a, Tms b);

/* Compares the two the time and prints the results. */
API int ClockCompare(Clock& clock, Clock& other);

/* Compares the given Tms to the time and prints the results. */
API int ClockCompare(Clock& clock, int year, int month, int day, int hour,
                     int minute, int second);

/* Zeros out the struct values.
    @param calendar_time A calendar time struct to zero out. */
API void ClockZeroTime(Clock& seconds);

/* Creates a 32-bit seconds timestamp.  */
API Tms ClockTimeTMS(int year, int month, int day, int hour = 0, int minute = 0,
                     int second = 0);

/* Creates a 64-bit seconds timestamp.  */
API Tme ClockTimeTME(int year, int month, int day, int hour = 0, int minute = 0,
                     int second = 0);

#if USING_UTF8
/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char* Print(char* begin, char* end, Clock& clock);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param t     The 64-bit stopwatch timestamp. */
API char* Print(char* begin, char* end, Tss& t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char* PrintTime(char* begin, char* end, Tms time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char* PrintTime(char* begin, char* end, Tme time);

/* Reads a time or time delta from a a char starting with an '@' sign.
@brief
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

@param input  The char to parse.
@param hour   The location to write the number of hours to.
@param minute The location to write the number of minutes to.
@param Second The location to write the number of seconds to. */
API const char* TextScanTime(const char* string, int& hour, int& minute,
                             int& second);

/* Converts a keyboard input to char and deletes the char.
@return Nil upon buffer failure or char directly after the end of the
timestamp upon success.
*/
API const char* Scan(const char* string, Clock& clock);

/* Converts a keyboard input to a Tss. */
API const char* Scan(const char* string, Tss& result);

/* Converts a keyboard input to a Tms. */
API const char* TextScanTime(const char* string, Tms& result);

/* Converts a keyboard input to a Tme. */
API const char* TextScanTime(const char* string, Tme& result);

#endif  //< #if USING_UTF8

#if USING_UTF16

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char16_t* Print(char16_t* begin, char16_t* end, Clock& clock);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char16_t* Print(char16_t* begin, char16_t* end, Tss& t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char16_t* PrintTime(char16_t* begin, char16_t* end, Tms time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char16_t* Print(char16_t* begin, char16_t* end, Tme time);

/* Reads a time or time delta from a a char16_t starting with an '@' sign.
@brief
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

@param input  The char16_t to parse.
@param hour   The location to write the number of hours to.
@param minute The location to write the number of minutes to.
@param Second The location to write the number of seconds to. */
API const char16_t* TextScanTime(const char16_t* string, int& hour, int& minute,
                                 int& second);

/* Converts a keyboard input to char16_t and deletes the char16_t.
@return Nil upon buffer failure or char16_t directly after the end of the
timestamp upon success.
*/
API const char16_t* Scan(const char16_t* string, Clock& time);

/* Converts a keyboard input to a Tss. */
API const char16_t* Scan(const char16_t* string, Tss& result);

/* Converts a keyboard input to a Tms. */
API const char16_t* TextScanTime(const char16_t* string, Tms& result);

/* Converts a keyboard input to a Tme. */
API const char16_t* TextScanTime(const char16_t* string, Tme& result);

#endif  //< #if USING_UTF16
#if USING_UTF32
/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char32_t* Print(char32_t* begin, char32_t* end, Clock& clock);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char32_t* Print(char32_t* begin, char32_t* end, Tss& time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char32_t* PrintTime(char32_t* begin, char32_t* end, Tms time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
API char32_t* PrintTime(char32_t* begin, char32_t* end, Tme time);

/* Reads a time or time delta from a a char starting with an '@' sign..
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
API const char32_t* TextScanTime(const char32_t* input, int& hour, int& minute,
                                 int& second);

/* Converts a keyboard input to char and deletes the char.
 */
API const char32_t* Scan(const char32_t* input, Clock& time);

/* Converts a keyboard input to a Tms. */
API const char32_t* Scan(const char32_t* input, Tss& result);

/* Converts a keyboard input to a Tms. */
API const char32_t* TextScanTime(const char32_t* input, Tms& result);

/* Converts a keyboard input to a Tms. */
API const char32_t* TextScanTime(const char32_t* input, Tme& result);

#endif  //< #if USING_UTF32
}  // namespace _

#undef PRINT
#undef PRINTF
#endif  //< #ifndef INCLUDED_KABUKI_F2_CLOCK
#endif  //< #if SEAM >= SEAM_0_0_0__05
