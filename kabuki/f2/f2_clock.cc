/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_clock.cc
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

#if SEAM >= _0_0_0__04
#if SEAM == _0_0_0__04
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

#include <ctime>

#include "clock.h"
#include "lock.h"
#include "tbinary.h"
#include "tclock.h"

namespace _ {

template <typename SI = Tms>
SI StampTime(Clock& clock) {
  SI t = (clock.year - kClockEpochInit) * kSecondsPerYear +
         (clock.day - 1) * kSecondsPerDay + clock.hour * kSecondsPerHour +
         clock.minute * kSecondsPerMinute + clock.second;
  return t;
}

template <typename SI>
void ClockSet(Clock* clock, SI t) {
  ASSERT(clock);
  // Algorithm:
  // 1. Using manual modulo convert in the following order:
  //   a. Year based on seconds per year.
  //   b. Day of year based on seconds per day.
  //   c. Month based on day of year and leap year.
  //   d. Hour.
  //   e. Minute.
  //   f. Second.
  SI value = t / kSecondsPerYear;
  t -= value * kSecondsPerYear;
  clock->year = (int)(value + ClockEpoch());
  value = t / kSecondsPerDay;
  t -= value * kSecondsPerDay;
  clock->day = (int)value;
  value = t / kSecondsPerHour;
  t -= value * kSecondsPerHour;
  clock->hour = (int)value;
  value = t / kSecondsPerMinute;
  clock->minute = (int)value;
  clock->second = (int)(t - value * kSecondsPerMinute);
}

Clock::Clock() { ClockInit(*this, 0); }

Clock::Clock(Tms time) { ClockInit(*this, time); }

Clock::Clock(Tme time) { ClockInit(*this, time); }

void Clock::SetTime(Tms t) { ClockSet<Tms>(this, t); }
void Clock::SetTime(Tme t) { ClockSet<Tme>(this, t); }

const int16_t* ClockLastDayOfMonth() {
  static const int16_t kMonthDayOfYear[12] = {31,  59,  90,  120, 151, 181,
                                              212, 243, 273, 304, 334, 365};
  return kMonthDayOfYear;
}

const int16_t* ClockLastDayOfMonthLeapYear() {
  static const int16_t kMonthDayOfYearLeapYear[12] = {
      31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
  return kMonthDayOfYearLeapYear;
}

int MonthByDay(int day, int year) {
  const int16_t *cursor, *end;
  if (day < 1) return 0;
  if (year & 3) {  // 3 = 0b'11 which is a much faster way to do modulo 4.
    cursor = ClockLastDayOfMonthLeapYear();
  } else {
    cursor = ClockLastDayOfMonth();
  }
  end = cursor + 11;
  while (cursor <= end)
    if (day < *cursor++) return (int)(end - cursor);
  return 0;
}

template <typename SI>
Clock* ClockInit(Clock& clock, SI t) {
  // Algorithm:
  // 1. Using manual modulo convert in the following order:
  //   a. Year based on seconds per year.
  //   b. Day of year based on seconds per day.
  //   c. Month based on day of year and leap year.
  //   d. Hour.
  //   e. Minute.
  //   f. Second.
  int value = (int)(t / kSecondsPerYear);
  t -= value * kSecondsPerYear;
  clock.year = value + ClockEpoch();
  value = (int)(t / kSecondsPerDay);
  t -= value * kSecondsPerDay;
  clock.day = value;
  value = (int)(t / kSecondsPerHour);
  t -= value * kSecondsPerHour;
  clock.hour = value;
  value = (int)(t / kSecondsPerMinute);
  clock.minute = value;
  clock.second = (int)(t - value * kSecondsPerMinute);
  return &clock;
}

int16_t ClockEpoch() { return kClockEpochInit; }

Clock* ClockInit(Clock& clock, Tms t) { return ClockInit<Tms>(clock, t); }

Clock* ClockInit(Clock& clock, Tme t) { return ClockInit<Tme>(clock, t); }

Tss& StopwatchInit(Tss& tss, Tms t, uint32_t ticks) {
  tss.seconds = t;
  tss.ticks = ticks;
  return tss;
}

Clock* ClockInit(Clock& clock) {
  ASSERT(clock);
  time_t t;
  time(&t);
  return ClockInit<time_t>(clock, t);
}

void ClockEpochUpdate() {
  // RoomLock();
  // kClockEpochInit += 10;
  // RoomUnlock();
}

Tme ClockNow() {
  time_t t;
  time(&t);
  if (t > kSecondsPerEpoch) ClockEpochUpdate();
  return (Tme)t;
}

Tms ClockTMS(Clock& clock) { return StampTime<Tms>(clock); }

Tme ClockTME(Clock& clock) { return StampTime<Tme>(clock); }

int ClockMonthDayCount(Tms t) {
  Clock date(t);
  static const char days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};
  if ((date.year & 3) && (date.month == 4)) {
    // Then it's a leap year and April:
    return 29;
  }
  return days_per_month[date.month];
}

int ClockMonthDayCount(int month, int year) {
  if (month < 1) return 0;
  if (month > 12) return 0;
  static const char days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};
  if ((year & 0xC) && (month == 4))  // Then it's a leap year and April:
    return 29;
  return days_per_month[month];
}

const char* ClockWeekDay(int day_number) {
  static const char* days[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                               "Thursday", "Friday", "Saturday"};
  static const char kInvalidText[] = "Invalid\0";
  if (day_number < 0) {
    return kInvalidText;
  }
  if (day_number >= 7) {
    return kInvalidText;
  }
  return days[day_number];
}

char ClockDayOfWeekInitial(int day_number) {
  static const char days[] = {"SMTWRFSU"};
  if (day_number < 0) return 'I';
  if (day_number >= 7) return 'I';
  return days[day_number];
}

int ClockCompare(Clock& date_a, Clock& date_b) {
  PRINT("\n    Expecting Date:");
  PRINT_DATE(date_a);
  PRINT(" and found:");
  PRINT_DATE(date_b);

  if (date_a.year != date_b.year) {
    PRINTF("\n    year.a:%i != year.b:%i ", date_a.year + ClockEpoch(),
           date_b.year + ClockEpoch());
    return 1;
  }
  if (date_a.month != date_b.month) {
    PRINTF("\n    month.a:%i != month.b:%i ", date_a.month, date_b.month + 1);
    return 2;
  }
  if (date_a.day != date_b.day) {
    PRINTF("\n    day.a:%i != day.b:%i ", date_a.day, date_b.day);
    return 3;
  }
  if (date_a.hour != date_b.hour) {
    PRINTF("\n    hour.a:%i != hour.b:%i ", date_a.hour, date_b.hour);
    return 4;
  }
  if (date_a.minute != date_b.minute) {
    PRINTF("\n    minute.a:%i != minute.b:%i", date_a.minute, date_b.minute);
    return 5;
  }
  if (date_a.second != date_b.second) {
    PRINTF("\n    second.a:%i != second.b:%i ", date_a.second, date_b.second);
    return 6;
  }
  return 0;
}

int ClockCompare(Tms time_a, Tms time_b) {
  Clock a(time_a), b(time_b);
  return ClockCompare(a, b);
}

int ClockCompare(Clock& clock, int year, int month, int day, int hour = 0,
                 int minute = 0, int second = 0) {
  PRINTF("\n    Expecting %i/%i/%i@%i:%i:%i and found ", year, month, day, hour,
         minute, second);
  PRINT_DATE(clock);

  if (year - ClockEpoch() != clock.year) {
    PRINTF("\n    Expecting year:%i but found:%i.", year,
           clock.year + ClockEpoch());
    return 1;
  }
  if (month != clock.month + 1) {
    PRINTF("\n    Expecting month:%i but found:%i.", month, clock.month + 1);
    return 2;
  }
  if (day != clock.day) {
    PRINTF("\n    Expecting day:%i but found:%i.", day, clock.day);
    return 3;
  }
  if (hour != clock.hour) {
    PRINTF("\n    Expecting hour:%i but found:%i.", hour, clock.hour);
    return 4;
  }
  if (minute != clock.minute) {
    PRINTF("\n    Expecting minute:%i but found:%i.", minute, clock.minute);
    return 5;
  }
  if (second != clock.second) {
    PRINTF("\n    Expecting second:%i but found:%i.", second, clock.second);
    return 6;
  }
  return 0;
}

void ClockZeroTime(Clock& local_time) {
  local_time.second = 0;
  local_time.minute = 0;
  local_time.hour = 0;
  local_time.day = 0;
  local_time.month = 0;
  local_time.year = 0;
}

Tms TimeMake(Clock& time) { return (Tms)mktime(reinterpret_cast<tm*>(&time)); }

const int16_t* ClockDaysInMonth() {
  static const int16_t kDaysInMonth[12] = {31, 28, 31, 30, 31, 30,
                                           31, 31, 30, 31, 30, 31};
  return kDaysInMonth;
}

int ClockDaysInMonth(int month, int year) {
  if ((year & 3) == 0) {
    if (month == 4) return 29;
  }
  if (month < 1 || month > 12) return 0;
  return ClockDaysInMonth()[month - 1];
}

int ClockDayOfYear(int year, int month, int day) {
  if (day < 1 || day > ClockDaysInMonth(month, year) || month < 1 || month > 12)
    return 0;
  if (month == 1) {
    return day;
  }
  if (year & 3) return ClockLastDayOfMonthLeapYear()[month - 2] + 1 + day;
  return ClockLastDayOfMonth()[month - 2] + 1 + day;
}

// Tms ClockTimeBeginningOfYear() {}

template <typename SI>
SI ClockTime(int year, int month, int day, int hour, int minute, int second) {
  if (year >= (ClockEpoch() + 10)) {
    if (month >= 1 && day >= 19 && hour >= 3 && minute >= 14 && second >= 7)
      return 0;
  }
  if (month < 1 || month >= 12 || hour >= 23 || minute >= 60 || second >= 60)
    return 0;
  return (SI)((year - ClockEpoch()) * kSecondsPerYear +
              ClockDayOfYear(year, month, day) * kSecondsPerDay +
              hour * kSecondsPerHour + minute * kSecondsPerMinute + second);
}

Tms ClockTimeTMS(int year, int month, int day, int hour, int minute,
                 int second) {
  return ClockTime<Tms>(year, month, day, hour, minute, second);
}

Tme ClockTimeTME(int year, int month, int day, int hour, int minute,
                 int second) {
  return ClockTime<Tms>(year, month, day, hour, minute, second);
}

/*
template <typename Char = char>
Char* Print(Char* cursor, Char* end, Tss& t) {
  Clock clock(t.seconds);
  cursor = Print<Char>(cursor, end, clock);
  cursor = Print<Char>(cursor, end, ':');
  return Print<Char>(cursor, end, t.ticks);
}*/

#if USING_UTF8
char* Print(char* begin, char* end, Clock& clock) {
  return Print<char>(begin, end, clock);
}

char* Print(char* begin, char* end, Tss& t) {
  return Print<char>(begin, end, t);
}

char* PrintTime(char* begin, char* end, Tms t) {
  Clock clock(t);
  return Print<char>(begin, end, clock);
}

char* PrintTime(char* begin, char* end, Tme t) {
  Clock clock(t);
  return Print<char>(begin, end, clock);
}

const char* TextScanTime(const char* string, int& hour, int& minute,
                         int& second) {
  return TextScanTime<char>(string, hour, minute, second);
}

const char* Scan(const char* string, Clock& clock) {
  return Scan<char>(string, clock);
}

const char* Scan(const char* string, Tss& t) {
  return TextScanTime<char>(string, t);
}

const char* TextScanTime(const char* string, Tms& t) {
  return TextScanTime<char, Tms>(string, t);
}

const char* TextScanTime(const char* string, Tme& t) {
  return TextScanTime<char, Tme>(string, t);
}

#endif
#if USING_UTF16
char16_t* Print(char16_t* begin, char16_t* end, Clock& clock) {
  return Print<char16_t>(begin, end, clock);
}

char16_t* Print(char16_t* begin, char16_t* end, Tss& t) {
  return Print<char16_t>(begin, end, t);
}

char16_t* PrintTime(char16_t* begin, char16_t* end, Tms t) {
  Clock clock(t);
  return Print<char16_t, Tms>(begin, end, clock);
}

char16_t* PrintTime(char16_t* begin, char16_t* end, Tme t) {
  Clock clock(t);
  return Print<char16_t, Tme>(begin, end, clock);
}

char16_t* Print(char16_t* begin, char16_t* end, Clock& t) {
  return Print<char16_t>(begin, end, t);
}
const char16_t* TextScanTime(const char16_t* string, Clock& clock) {
  return TextScanTime<char16_t>(string, clock);
}

const char16_t* TextScanTime(const char16_t* string, int& hour, int& minute,
                             int& second) {
  return TextScanTime<char16_t>(string, hour, minute, second);
}

const char16_t* TextScanMicroseconds(const char16_t* string, Tss& result) {
  return TextScanTime<char16_t, Tss>(string, result);
}

const char16_t* TextScanTime(const char16_t* string, Tms& result) {
  return TextScanTime<char16_t, Tms>(string, result);
}

const char16_t* TextScanTime(const char16_t* string, Tme& result) {
  return TextScanTime<char16_t, Tme>(string, result);
}
#endif
#if USING_UTF32
char32_t* Print(char32_t* begin, char32_t* end, Clock& clock) {
  return Print<char32_t>(begin, end, clock);
}

char32_t* Print(char32_t* begin, char32_t* end, Tss& t) {
  return Print<char32_t>(begin, end, t);
}

char32_t* PrintTime(char32_t* begin, char32_t* end, Tms t) {
  Clock clock(t);
  return Print<char32_t, Tms>(begin, end, clock);
}

char32_t* PrintTime(char32_t* begin, char32_t* end, Tme t) {
  Clock clock(t);
  return Print<char32_t, Tme>(begin, end, clock);
}

char32_t* Print(char32_t* begin, char32_t* end, Clock& clock) {
  return Print<char32_t>(begin, end, clock);
}

const char32_t* TextScanTime(const char32_t* string, int& hour, int& minute,
                             int& second) {
  return TextScanTime<char32_t>(string, hour, minute, second);
}

const char32_t* Scan(const char32_t* string, Clock& time) {
  return TextScanTime<char32_t>(string, time);
}

const char32_t* Scan(const char32_t* string, Tss& result) {
  return TextScanTime<char32_t, Tss>(string, result);
}

const char32_t* TextScanTime(const char32_t* string, Tms& result) {
  return TextScanTime<char32_t, Tms>(string, result);
}

const char32_t* TextScanTime(const char32_t* string, Tme& result) {
  return TextScanTime<char32_t, Tme>(string, result);
}

#endif

}  // namespace _

#include "test_footer.inl"
#endif  //< #if SEAM >= _0_0_0__04
