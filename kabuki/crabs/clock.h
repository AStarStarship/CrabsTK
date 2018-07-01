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
#include "config.h"
#include "print_integer.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_DATE(date)                                                    \
  Printf("%i/%i/%i@%i:%i:%i", date.year + kSecondsPerEpoch, date.month + 1, \
         date.day, date.hour, date.minute, date.second);
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_DATE(date)
#endif
;
namespace _ {

/* A time in seconds and optional microseconds format that is compatible with
the C++ stadard library.
Data structure is identical to std::tm with the exection that it has an
additional microseconds from start of second varable.
*/
struct KABUKI Clock {
  int second,  //< Second of the minute [0, 59].
      minute,  //< Minute of the hour [0, 59].
      hour,    //< Hour of the day [0, 23].
      day,     //< Day of the month [1, 31].
      month,   //< Months since December [0, 11].
      year;    //< Number of years since epoch [-1902, 1970] U [1970, 2038].

  /* Constructs a clock from the given 32-bit seconds timestamp. */
  Clock(Tms time);

  /* Constructs a clock from the given 64-bit seconds timestamp. */
  Clock(Tme time);

  /* Sets the ticks_max. */
  void SetTicks(uint32_t ticks);

  /* Sets the ticks_max. */
  void SetTicks(uint32_t ticks, uint32_t ticks_max);

  /* Sets the time to the given 32-bit TMS timestamp. */
  void SetTime(Tms t);
};

/* A sub-second timestamp composed of a TMS and a UI4 tick.

  Operation ofthe Tss is intended for two scenarios.

  1. Processor has a real microsecond timer stored as a 24-bit value.
  2. Processor is an x86 and timer gets updated with a tread or OS.
  









  In the real microsecond timer scenario the processor will just
  take the unsigned in value and copy it to the UI4 member. In the
  case of the OS having a variable update tick period, the ticker will
  work best if the value gets inremented using the ++operator and you will
  need to use modulo updates_per_second unless the timer is set to 64 updates
  per second or some other power of 2 in which case bit masking is the
  tool of choice. For desktop operating systems other threads may hijack the
  OS scheduler.
*/
struct Tss {
  Tms seconds;     //< Seconds since epoch.
  uint32_t ticks;  //< Ticks since epoch.
};

enum {
  kClockEpochInit = 1970,                    //< Initial clock epoch.
  kSecondsPerMinute = 60,                    //< Numer of seconds in an minute.
  kSecondsPerHour = 60 * kSecondsPerMinute,  //< Numer of seconds in an hour.
  kSecondsPerDay = 24 * kSecondsPerHour,     //< Numer of seconds in an day.
  kSecondsPerYear = kSecondsPerDay * 365,    //< Numer of seconds in an year.
  // Numer of seconds in a a leap year.
  kSecondsPerEpoch = 10 * kSecondsPerYear,  //< Numer of seconds in an year.
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
inline KABUKI int16_t ClockEpoch();

/* Lookup table for converting from day-of-year to month. */
inline KABUKI const int16_t* ClockLastDayOfMonth();

/* Lookup table for converting from day-of-year to month. */
inline KABUKI const int16_t* ClockLastDayOfMonthLeapYear();

/* Returns which month the given day is in based on the year. */
inline KABUKI int MonthByDay(int day, int year);

/* Inializes the clock from the given timesamp. */
KABUKI Clock* ClockInit(Clock& clock, Tms time);

/* Inializes the clock from the given timesamp. */
KABUKI Clock* ClockInit(Clock& clock, Tme time);

/* Inializes the clock from the given 64-bit microsecond timesamp. */
KABUKI Tss& StopwatchInit(Tss& clock, Tms t, uint32_t ticks);

/* Inializes the clock from the given timesamp. */
KABUKI Clock* ClockInit(Clock& clock);

/* Gets the current microsecond timestamp. */
inline KABUKI Tme ClockNow();

/* Calculates the seconds from epoch from the clock and stores it to the result.
 */
inline KABUKI Tms ClockTMS(Clock& clock);

/* Calculates the seconds from epoch from the clock and stores it to the result.
 */
inline KABUKI Tme ClockTME(Clock& clock);

/* Gets the number of days in a months.
    @todo Maybe get some open-source date utility? */
KABUKI int ClockMonthDayCount(Tms t);

/* Gets the number of days in a months.
@param month The month index 0-11.
@param year   */
KABUKI int ClockMonthDayCount(int month, int year);

/* Gets the abbreviated day of the week char of the given day number 1-7. */
KABUKI const char* ClockWeekDay(int day_number);

/* Gets the abbreviated day of the week char of the given day number 1-7. */
KABUKI char ClockDayOfWeekInitial(int day_number);

/* Compares the two the time and prints the results. */
KABUKI int ClockCompare(Tms a, Tms b);

/* Compares the two the time and prints the results. */
KABUKI int ClockCompare(Tms a, Tms b);

/* Compares the two the time and prints the results. */
KABUKI int ClockCompare(Clock& clock, Clock& other);

/* Compares the given Tms to the time and prints the results. */
KABUKI int ClockCompare(Clock& clock, int year, int month, int day, int hour,
                        int minute, int second);

/* Zeros out the struct values.
    @param calendar_time A calendar time struct to zero out. */
KABUKI void ClockZeroTime(Clock& seconds);

/* Creates a 32-bit seconds timestamp.  */
KABUKI Tms ClockTimeTMS(int year, int month, int day, int hour = 0,
                        int minute = 0, int second = 0);

/* Creates a 64-bit seconds timestamp.  */
KABUKI Tme ClockTimeTME(int year, int month, int day, int hour = 0,
                        int minute = 0, int second = 0);

#if CRABS_UTF

template <typename Char>
Char* Print(Char* cursor, Char* end, Clock& clock) {
  // The way the print funcitons are setup, we return a nil-term char so we
  // don't have to check to write a single char in this
  ASSERT(cursor);
  ASSERT(cursor < end);
  ASSERT(clock);

  cursor = Print<Char>(cursor, end, clock.year + ClockEpoch());
  if (!cursor) return nullptr;
  *cursor++ = '-';
  cursor = Print<Char>(cursor, end, clock.month + 1);
  if (!cursor) return nullptr;
  *cursor++ = '-';
  cursor = Print<Char>(cursor, end, clock.day);
  if (!cursor) return nullptr;
  *cursor++ = '@';
  cursor = Print<Char>(cursor, end, clock.hour);
  if (!cursor) return nullptr;
  *cursor++ = ':';
  cursor = Print<Char>(cursor, end, clock.minute);
  if (!cursor) return nullptr;
  *cursor++ = ':';
  cursor = Print<Char>(cursor, end, clock.second);
  if (!cursor) return nullptr;
  return cursor;
}

template <typename Char>
Char* Print(Char* cursor, Char* end, Tss& t) {
  // The way the print funcitons are setup, we return a nil-term char so we
  // don't have to check to write a single char in this
  ASSERT(cursor);
  ASSERT(cursor < end);

  Clock clock(t.seconds);
  cursor = Print<Char>(cursor, end, clock);
  *cursor++ = ':';
  cursor = Print<Char>(cursor, end, clock.ticks);
  if (!cursor) return nullptr;
  return cursor;
}

template <typename Char>
const Char* TextScanTime(const Char* string, int& hour, int& minute,
                         int& second) {
  if (string == nullptr) return nullptr;

  PRINTF("\n\n    Scanning time:%s", string);
  Char c;  //< The current Char.
  int h,   //< Hour.
      m,   //< Minute.
      s;   //< Second.
  if (!TextScanSigned<Char>(++string, h)) {
    PRINTF("\nInvalid hour:%i", h);
    return nullptr;
  }
  string = TextSkipNumbers<Char>(string);
  if (h < 0) {
    PRINTF("\nHours:%i can't be negative.", h);
    return nullptr;
  }
  if (h > 23) {
    PRINTF("\nHours:%i can't be > 23.", h);
    return nullptr;
  }
  PRINT(h);
  c = *string++;
  if (!c || IsWhitespace<Char>(c)) {  // Case @HH
    PRINT(" HH ");
    // Then it's a single number, so create a Tms for the current
    // user-time hour..
    hour = h;
    return string;
  }
  c = Lowercase<Char>(c);
  if (c == 'a') {  //
    PRINT("\nCase @HHAm\n HHam ");

    if (Lowercase<Char>(c = *string++) == 'm') c = *string++;
    if (c && !IsWhitespace<Char>(c)) {
      PRINT("\nInvalid am format.");
      return nullptr;
    }
    PRINT(" @HHAM ");
    hour = h;
    return string;
  }
  if (c == 'p') {
    PRINT(" Case @HHpm ");
    c = *string++;
    if (Lowercase<Char>(c) == 'm') c = *string++;
    if (c && !IsWhitespace<Char>(c)) {
      PRINT("\ninvalid pm format.");
      return nullptr;
    }
    PRINTF("< Case @HHPM %i:00:00\n", h + 12);
    hour = h + 12;
    return string;
  }
  if (c != ':') {
    PRINTF("\nExpecting ':'.");
    return nullptr;
  }

  PRINTF(
      "\nCases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and "
      "HH:MM:SSpm");

  if (!TextScanSigned<Char>(string, m)) return nullptr;
  string = TextSkipNumbers<Char>(string);
  if (m < 0) {
    PRINTF("\nMinutes:%i can't be negative!", m);
    return nullptr;
  }
  if (m >= 60) {
    PRINTF("\nMinutes:%i can't be >= 60!", m);
    return nullptr;  //< 60 minutes in an hour.
  }
  PRINTF(":%i", m);

  string = TextSkipNumbers<Char>(string);
  c = *string++;
  if (!c || IsWhitespace<Char>(c)) {  // Case HH:MM
    PRINT(" HH:MM ");
    hour = h;
    minute = m;
    return string;
  }
  c = Lowercase<Char>(c);
  if (c == 'a') {
    PRINT(" HH:MMam ");
    c = *string++;
    if (Lowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (c && !IsWhitespace<Char>(c)) {  //< The space is not.
      PRINT("Invalid am in HH::MM AM");
      return nullptr;
    }
    hour = h;
    minute = m;
    return string;
  }
  if (c == 'p') {  // Case HH:MM PM
    PRINT(" HH:MMpm ");
    c = *string++;
    if (Lowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (c && !IsWhitespace<Char>(c)) {  //< The space is not.
      PRINT("Invalid am in HH::MM PM");
      return nullptr;
    }
    hour = h + 12;
    minute = m;
    return string;
  }
  if (c != ':') return nullptr;

  PRINT("\n    Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm");

  if (!TextScanSigned<Char>(string, s)) return nullptr;
  if (s < 0) {
    PRINTF("\nSeconds:%i can't be negative!", s);
    return nullptr;
  }
  if (s > 60) {
    PRINTF("\nSeconds:%i can't be >= 60!", s);
    return nullptr;
  }
  PRINTF(":%i", s);
  string = TextSkipNumbers<Char>(string);
  c = Lowercase<Char>(*string);
  if (!c || IsWhitespace<Char>(c)) {
    PRINTF(" HH:MM:SS ");
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c == 'a') {
    PRINT(" HH:MM:SSam ");
    c = *string++;
    if (Lowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (!c || !IsWhitespace<Char>(c)) {  //< The space is not.
      PRINT("\nInvalid am in HH::MM:SS AM");
      return nullptr;
    }
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c != 'p') {
    PRINTF("\nExpecting a PM but found:%c", c);
    return nullptr;  // Format error!
  }
  PRINTF(" HH:MM:SSpm ");
  c = Lowercase<Char>(*string++);
  if (c == 'm') {  //< The 'm' is optional.
    c = *string++;
  }
  if (!c || !IsWhitespace<Char>(c)) {  //< The space is not.
    PRINT("\nInvalid am in HH::MM:SS PM");
    return nullptr;
  }
  hour = h + 12;
  minute = m;
  second = s;
  return string;
}

template <typename Char>
const Char* TextScan(const Char* cursor, Clock& clock) {
  ASSERT(cursor);
  PRINTF("\n    Scanning Clock: %s\n    Scanning: ", cursor);

  cursor = TextSkipChar<Char>(cursor, '0');
  Char c = *cursor,  //< The current Char.
      delimiter;     //< The delimiter.
  const Char* end;   //< Might not need

  int hour = 0, minute = 0, second = 0;

  if (c == '@') {
    if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("\nCase @ invalid time");
      return nullptr;
    }
    clock.hour = hour;
    clock.minute = minute;
    clock.second = second;

    return cursor + 1;
  }
  if (c == '#') {
    if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("\nCase @ invalid time");
    }
    clock.hour += hour;
    clock.minute += minute;
    clock.second += second;

    return cursor + 1;
  }

  int value1,            //< The first date field scanned.
      value2,            //< The second date field scanned.
      value3,            //< The third date field scanned.
      is_last_year = 0;  //< Flag for if the date was last year or not.

  // Scan value1
  if (!TextScanSigned<Char>(cursor, value1)) {
    PRINT("Scan error at value1");
    return nullptr;
  }
  if (value1 < 0) {
    PRINT("Dates can't be negative.");
    return nullptr;
  }
  cursor = TextNumberEnd<Char>(cursor);
  delimiter = *cursor++;
  PRINTF("%i%c", value1);
  if (delimiter == '@') {
    PRINT(" HH@ ");

    if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("Invalid time DD@");
      return nullptr;
    }
    clock.day = value1;

    return cursor + 1;
  }
  // Scan value2.
  cursor = TextSkipChar<Char>(cursor, '0');
  if (!TextScanSigned<Char>(cursor, value2)) {
    PRINT("Failed scanning value2 of date.");
    return nullptr;
  }
  if (value2 < 0) {
    PRINT("Day can't be negative.");
    return nullptr;  //< Invalid month and day.
  }
  PRINTF("%i", value2);
  cursor = TextNumberEnd<Char>(cursor);
  c = *cursor;
  if (c != delimiter) {
    PRINT("\n    Cases MM/DD and MM/YYyy");
    if (c == '@') {
      if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
        PRINT(" invalid time ");
        return nullptr;
      }
    }
    if (!c || IsWhitespace<Char>(c)) {
      PRINTF("\n    Format is MM/DD and year is %i",
             clock.year + ClockEpochSeconds());
      // is_last_year = ((value1 >= std_tm.mon) &&
      //    (value2 >= std_tm.mday)) ? 0:1;
      clock.year += is_last_year;
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = 0;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;

      return cursor + 1;
    }
    c = Lowercase<Char>(c);
    if ((value1 < 12) && (value2 > 0) &&
        (value2 <= ClockMonthDayCount(value1))) {
      PRINT(" MM/DD ");
      if (value1 > 11) {
        PRINT("\nInvalid MM/DD@ month");
        return nullptr;
      }
      // We need to find out what year it is.
      Clock clock;
      ClockInit(clock);

      if (value2 > ClockMonthDayCount(clock->year - ClockEpochYear(), value1)) {
        PRINT("\nInvalid MM/DD@ day");
        return nullptr;
      }
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = hour;
      clock.min = minute;
      clock.second = second;
      if (!(cursor = TextScanTime(cursor, hour, minute, second))) {
        PRINT("\nInvalid MM/DD@");
        return nullptr;
      }

      return cursor + 1;
    }
    if ((value1 < 12) && (value2 > ClockMonthDayCount<Char>(value1))) {
      PRINT(" MM/YYyy\n");
      clock.mon = value1 - 1;
      clock.year = value2;
      if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
        PRINT("\nInvalid MM / YYYY@ time");
        return nullptr;
      }

      return cursor + 1;
    }
    PRINT("\nInvalid MM/DD or MM/YYyy format\n");
    return nullptr;
  }

  // Formats MM/DD/YYyy and YYyy/MM/DD

  cursor = TextSkipChar<Char>(++cursor, '0');
  c = *cursor;
  // Then there are 3 values and 2 delimiters.
  if (!IsDigit<Char>(c) || !TextScanSigned<Char>(cursor, value3)) {
    PRINTF("\n    SlotRead error reading value3 of date. %c: ", c);
    return nullptr;  //< Invalid format!
  }
  cursor = TextNumberEnd<Char>(cursor);
  PRINTF("%c%i", c, value3);
  // Now we need to check what format it is in.

  c = *cursor;
  if (c == '@') {
    if (!(end = TextScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("Invalid YYyy/MM/DD@ time.");
      return nullptr;
    }
  }
  clock.hour = hour;
  clock.minute = minute;
  clock.second = second;
  if (IsWhitespace<Char>(*(++cursor))) {
    PRINT("No date found.");
    return nullptr;
  }
  if (value1 > 11) {  //<
    PRINT("\n    Case YYyy/MM/DD\n");
    if (value2 == 0 || value2 > 12) {
      PRINTF("Invalid number of months");
      return nullptr;
    }

    if (value2 > ClockMonthDayCount<Char>(value2, value1)) {
      PRINT("Invalid number of days");
      return nullptr;
    }  // 17/05/06

    if (value1 < 100) {
      PRINT("\n    Case YY/MM/DD");
      value1 += 2000 - ClockEpochSeconds();
    } else {
      PRINT("\n    Case YYYY/MM/DD");
      value1 -= ClockEpochSeconds();
    }

    clock.year = value1;
    clock.month = value2 - 1;
    clock.day = value3;

    return cursor + 1;
  }
  PRINT("\n    Cases MM/DD/YY and MM/DD/YYYY");

  if (value1 > 11) {
    PRINT("Invalid month.\n");
    return nullptr;
  }
  if (value2 > ClockMonthDayCount<Char>(value1, value3)) {
    PRINT("Invalid day.\n");
    return nullptr;
  }
  clock.month = value1 - 1;
  clock.day = value2;
  if (value3 < 100) {
    PRINT("\n    Case MM/DD/YY");
    clock.year = value3 + (2000 - ClockEpochSeconds());
  } else {
    PRINT("\n    Case MM/DD/YYYY");
    clock.year = value3 - ClockEpochSeconds();
  }
  return cursor + 1;
}

template <typename Char, typename SI>
const Char* TextScanTime(const Char* begin, SI& result) {
  Clock clock;
  const Char* end = TextScanTime<Char>(begin, clock);
  result = ClockOffset(clock, result);
  return end;
}
#endif  // #if CRABS_UTF

#if USING_UTF8
/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char* Print(char* begin, char* end, Clock& clock);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param t     The 64-bit stopwatch timestamp. */
KABUKI char* Print(char* begin, char* end, Tss& t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char* PrintTime(char* begin, char* end, Tms time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char* PrintTime(char* begin, char* end, Tme time);

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
KABUKI const char* TextScanTime(const char* string, int& hour, int& minute,
                                int& second);

/* Converts a keyboard input to char and deletes the char.
@return Nil upon buffer failure or char directly after the end of the
timestamp upon success.
*/
KABUKI const char* TextScan(const char* string, Clock& clock);

/* Converts a keyboard input to a Tss. */
KABUKI const char* TextScan(const char* string, Tss& result);

/* Converts a keyboard input to a Tms. */
KABUKI const char* TextScanTime(const char* string, Tms& result);

/* Converts a keyboard input to a Tme. */
KABUKI const char* TextScanTime(const char* string, Tme& result);

#endif  //< #if USING_UTF8

#if USING_UTF16

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, Clock& clock);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, Tss& t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char16_t* PrintTime(char16_t* begin, char16_t* end, Tms time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, Tme time);

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
KABUKI const char16_t* TextScanTime(const char16_t* string, int& hour,
                                    int& minute, int& second);

/* Converts a keyboard input to char16_t and deletes the char16_t.
@return Nil upon buffer failure or char16_t directly after the end of the
timestamp upon success.
*/
KABUKI const char16_t* TextScan(const char16_t* string, Clock& time);

/* Converts a keyboard input to a Tss. */
KABUKI const char16_t* TextScan(const char16_t* string, Tss& result);

/* Converts a keyboard input to a Tms. */
KABUKI const char16_t* TextScanTime(const char16_t* string, Tms& result);

/* Converts a keyboard input to a Tme. */
KABUKI const char16_t* TextScanTime(const char16_t* string, Tme& result);

#endif  //< #if USING_UTF16
#if USING_UTF32
/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, Clock& clock);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, Tss& time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* PrintTime(char32_t* begin, char32_t* end, Tms time);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* PrintTime(char32_t* begin, char32_t* end, Tme time);

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
KABUKI const char32_t* TextScanTime(const char32_t* input, int& hour,
                                    int& minute, int& second);

/* Converts a keyboard input to char and deletes the char.
 */
KABUKI const char32_t* TextScan(const char32_t* input, Clock& time);

/* Converts a keyboard input to a Tms. */
KABUKI const char32_t* TextScan(const char32_t* input, Tss& result);

/* Converts a keyboard input to a Tms. */
KABUKI const char32_t* TextScanTime(const char32_t* input, Tms& result);

/* Converts a keyboard input to a Tms. */
KABUKI const char32_t* TextScanTime(const char32_t* input, Tme& result);

#endif  //< #if USING_UTF32
}  // namespace _

#undef PRINT
#undef PRINTF
#endif  //< #ifndef HEADER_FOR_CRABS_CLOCK
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
