/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_clock.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>

#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
// Dependencies:
#include "clock.h"
#include "utf.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PUTCHAR(c) putchar(c)
#define PRINT_DATE(date)                                                  \
  printf("%i/%i/%i@%i:%i:%i", date.tm_year + kTimeEpoch, date.tm_mon + 1, \
         date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_DATE(date)
#endif

// using namespace std::chrono;

// using chrono_timestamp = std::chrono::time_point<std::chrono::system_clock,
//    std::chrono::microseconds>;
namespace _ {

#if USING_PRINTER

std::tm* ClockTimeLocal(std::tm* std_tm, std::time_t const& time) {
  if (!std_tm) return nullptr;
#if (defined(__MINGW32__) || defined(__MINGW64__))
  memcpy(&tm_snapshot, ::localtime(&time), sizeof(std::tm));
#elif (defined(_WIN32) || defined(__WIN32__))
  localtime_s(std_tm, &time);
#else  // POSIX
  localtime_r(&time, std_tm);
#endif
  return std_tm;
}

time_us_t ClockPackTimestamp(time_t unixTime, int32_t microseconds) {
  return (((time_us_t)unixTime) << 8) & (time_us_t)microseconds;
}

time_t ClockGetSeconds(time_us_t t) { return (time_t)t; }

int32_t ClockGetMicroseconds(time_us_t timestamp) {
  return (int32_t)((timestamp & 0xFFFFFFFF00000000) >> 32);
}

char* PrintClock(char* begin, char* end, tm* std_tm) {
  ASSERT(begin);
  ASSERT(std_tm);
  ASSERT(begin < end)

  begin = Print(begin, end, std_tm->tm_year + kTimeEpoch);
  if (!begin) {
    return nullptr;
  }
  *begin++ = '-';
  begin = Print(begin, end, std_tm->tm_mon + 1);
  if (!begin) {
    return nullptr;
  }
  *begin++ = '-';
  begin = Print(begin, end, std_tm->tm_mday);
  if (!begin) {
    return nullptr;
  }
  *begin++ = '@';
  begin = Print(begin, end, std_tm->tm_hour);
  if (!begin) {
    return nullptr;
  }
  *begin++ = ':';
  begin = Print(begin, end, std_tm->tm_min);
  if (!begin) {
    return nullptr;
  }
  *begin++ = ':';
  begin = Print(begin, end, std_tm->tm_sec);
  if (!begin) {
    return nullptr;
  }
  return begin;
}

int ClockNumDaysMonth(time_t t) {
  tm date;
  ClockTimeLocal(&date, t);
  static const char days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};
  if ((date.tm_year & 0x0C) && (date.tm_mon == 4)) {
    // Then it's a leap year and April:
    return 29;
  }
  return days_per_month[date.tm_mon];
}

int ClockNumDaysMonth(int month, int year) {
  if (month < 1) return 0;
  if (month > 12) return 0;
  static const char days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};
  if ((year & 0xC) && (month == 4))  // Then it's a leap year and April:
    return 29;
  return days_per_month[month];
}

const char* ClockDayOfWeek(int day_number) {
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
  if (day_number < 0) {
    return 'I';
  }
  if (day_number >= 7) {
    return 'I';
  }
  return days[day_number];
}

int ClockCompare(time_t time_a, time_t time_b) {
  tm date_a, date_b;

  ClockTimeLocal(&date_a, time_a);
  ClockTimeLocal(&date_b, time_b);

  PRINTF("\n    Expecting Date:");
  PRINT_DATE(date_a);
  PRINTF(" and found:");
  PRINT_DATE(date_b);

  if (date_a.tm_year != date_b.tm_year) {
    PRINTF("\n    tm_year.a:%i != tm_year.b:%i ", date_a.tm_year + kTimeEpoch,
           date_b.tm_year + kTimeEpoch);
    return 1;
  }
  if (date_a.tm_mon != date_b.tm_mon) {
    PRINTF("\n    tm_mon.a:%i != tm_mon.b:%i ", date_a.tm_mon,
           date_b.tm_mon + 1);
    return 2;
  }
  if (date_a.tm_mday != date_b.tm_mday) {
    PRINTF("\n    tm_mday.a:%i != tm_mday.b:%i ", date_a.tm_mday,
           date_b.tm_mday);
    return 3;
  }
  if (date_a.tm_hour != date_b.tm_hour) {
    PRINTF("\n    tm_hour.a:%i != tm_hour.b:%i ", date_a.tm_hour,
           date_b.tm_hour);
    return 4;
  }
  if (date_a.tm_min != date_b.tm_min) {
    PRINTF("\n    tm_min.a:%i != tm_min.b:%i", date_a.tm_min, date_b.tm_min);
    return 5;
  }
  if (date_a.tm_sec != date_b.tm_sec) {
    PRINTF("\n    tm_sec.a:%i != tm_sec.b:%i ", date_a.tm_sec, date_b.tm_sec);
    return 6;
  }
  return 0;
}

int ClockCompare(time_t t, int year, int month, int day, int hour, int minute,
                 int second) {
  tm date_a;
  ClockTimeLocal(&date_a, t);

  PRINTF("\n    Expecting %i/%i/%i@%i:%i:%i and found ", year, month, day, hour,
         minute, second);
  PRINT_DATE(date_a);

  if (year - kTimeEpoch != date_a.tm_year) {
    PRINTF("\n    Expecting year:%i but found:%i.", year,
           date_a.tm_year + kTimeEpoch);
    return 1;
  }
  if (month != date_a.tm_mon + 1) {
    PRINTF("\n    Expecting month:%i but found:%i.", month, date_a.tm_mon + 1);
    return 2;
  }
  if (day != date_a.tm_mday) {
    PRINTF("\n    Expecting day:%i but found:%i.", day, date_a.tm_mday);
    return 3;
  }
  if (hour != date_a.tm_hour) {
    PRINTF("\n    Expecting hour:%i but found:%i.", hour, date_a.tm_hour);
    return 4;
#endif
  }  // namespace _
  if (minute != date_a.tm_min) {
    PRINTF("\n    Expecting minute:%i but found:%i.", minute, date_a.tm_min);
    return 5;
  }
  if (second != date_a.tm_sec) {
    PRINTF("\n    Expecting second:%i but found:%i.", second, date_a.tm_sec);
    return 6;
  }
  return 0;
}

void ClockZeroTime(tm& std_tm) {
  std_tm.tm_sec = 0;
  std_tm.tm_min = 0;
  std_tm.tm_hour = 0;
  std_tm.tm_mday = 0;
  std_tm.tm_mon = 0;
  std_tm.tm_year = 0;
  std_tm.tm_wday = 0;
  std_tm.tm_yday = 0;
  std_tm.tm_isdst = 0;
}

time_t ClockTime(int year, int month, int day, int hour, int minute,
                 int second) {
  time_t t;
  time(&t);
  tm moment;
  ClockTimeLocal(&moment, t);
  moment.tm_year = year - kTimeEpoch;
  moment.tm_mon = month - 1;
  moment.tm_mday = day;
  moment.tm_hour = hour;
  moment.tm_min = minute;
  moment.tm_sec = second;

  // if (!PrintClock (moment, buffer, buffer_size)) {
  //    PRINTF ("\nError making timestamp")
  //    return 0;
  //}
  // PRINTF ("\n Creating test time: ")
  // PrintClock (moment);
  t = mktime(&moment);
  if (t < 0) {
    /// PRINTF ("\n Invalid time:" << t << '\n')
    return 0;
  } else {
    // PUTCHAR ('\n')
  }
  return t;
}

time_t TimeMake(TimeSeconds& time) {
  return mktime(reinterpret_cast<tm*>(&time));
}

time_t ClockSecondsAtStartOfMonth(uint month, uint is_leap_year)

    time_t ClockTime(uint year, uint month, uint day, uint hour, uint minute,
                     uint second) {
  static const time_t kSecondsAtStartOfMonth[12] = {

  };
  static const time_t kSecondsAtStartOfMonthLeapYear[12] = {};
  enum {
    kSecondsPerMinute = 60,
    kSecondsPerHour = 60 * 60,
    kSecondsPerDay = kMinutesPerHour * 24,
    k
  };
  return seconds + kSecondsPerHour * minutes + kSecondsPerHour * hours +
         kSecondsPerDay * (day - 1) + time_t t;
  time(&t);
  tm moment;
  ClockTimeLocal(&moment, t);
  moment.tm_year = year - kTimeEpoch;
  moment.tm_mon = month - 1;
  moment.tm_mday = day;
  moment.tm_hour = hour;
  moment.tm_min = minute;
  moment.tm_sec = second;

  t = mktime(&moment);
  return t;
}

}  // namespace _

#undef PRINTF
#undef PUTCHAR
#undef PRINT_DATE
#endif  //< SEAM_MAJOR == 0 && SEAM_MINOR >= 2
