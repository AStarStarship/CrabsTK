/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    ~/kabuki/crabs/clock.h
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
#if SEAM >= SEAM_0_0_1
#ifndef INCLUDED_CRABS_TCLOCK
#define INCLUDED_CRABS_TCLOCK
// Dependencies:
#include "debug.h"
#include "tstr.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 2
#define PRINT(item)  // Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#else
#define PRINT(item)
#define PRINTF(x, ...)
#endif

namespace _ {

#if CRABS_TEXT

template <typename Char = char>
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

template <typename Char = char>
Char* Print(Char* cursor, Char* end, Tss& t) {
  // The way the print funcitons are setup, we return a nil-term char so we
  // don't have to check to write a single char in this
  ASSERT(cursor);
  ASSERT(cursor < end);

  Clock clock(t.seconds);
  cursor = Print<Char>(cursor, end, clock);
  *cursor++ = ':';
  cursor = Print<Char>(cursor, end, t.ticks);
  if (!cursor) return nullptr;
  return cursor;
}

template <typename Char = char>
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
    PRINTF("\nCase @HHPM %i:00:00", h + 12);
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

template <typename Char = char>
const Char* Scan(const Char* cursor, Clock& clock) {
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
  cursor = TextNumberStop<Char>(cursor);
  if (!cursor) return nullptr;
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
  cursor = TextNumberStop<Char>(cursor);
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
      PRINTF("\n    Format is MM/DD and year is %i", clock.year + ClockEpoch());
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

      if (value2 > ClockMonthDayCount(clock.year - ClockEpoch(), value1)) {
        PRINT("\nInvalid MM/DD@ day");
        return nullptr;
      }
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;
      if (!(cursor = TextScanTime(cursor, hour, minute, second))) {
        PRINT("\nInvalid MM/DD@");
        return nullptr;
      }

      return cursor + 1;
    }
    if ((value1 < 12) && (value2 > ClockMonthDayCount(value1))) {
      PRINT(" MM/YYyy");
      clock.month = value1 - 1;
      clock.year = value2;
      if (!(cursor = TextScanTime<Char>(cursor, hour, minute, second))) {
        PRINT("\nInvalid MM / YYYY@ time");
        return nullptr;
      }

      return cursor + 1;
    }
    PRINT("\nInvalid MM/DD or MM/YYyy format");
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
  cursor = TextNumberStop<Char>(cursor);
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
    PRINT("\n    Case YYyy/MM/DD");
    if (value2 == 0 || value2 > 12) {
      PRINTF("Invalid number of months");
      return nullptr;
    }

    if (value2 > ClockMonthDayCount(value2, value1)) {
      PRINT("Invalid number of days");
      return nullptr;
    }  // 17/05/06

    if (value1 < 100) {
      PRINT("\n    Case YY/MM/DD");
      value1 += 2000 - ClockEpoch();
    } else {
      PRINT("\n    Case YYYY/MM/DD");
      value1 -= ClockEpoch();
    }

    clock.year = value1;
    clock.month = value2 - 1;
    clock.day = value3;

    return cursor + 1;
  }
  PRINT("\n    Cases MM/DD/YY and MM/DD/YYYY");

  if (value1 > 11) {
    PRINT("\nInvalid month.");
    return nullptr;
  }
  if (value2 > ClockMonthDayCount(value1, value3)) {
    PRINT("\nInvalid day.");
    return nullptr;
  }
  clock.month = value1 - 1;
  clock.day = value2;
  if (value3 < 100) {
    PRINT("\n    Case MM/DD/YY");
    clock.year = value3 + (2000 - ClockEpoch());
  } else {
    PRINT("\n    Case MM/DD/YYYY");
    clock.year = value3 - ClockEpoch();
  }
  return cursor + 1;
}

template <typename Char, typename SI>
const Char* TextScanTime(const Char* begin, SI& result) {
  Clock clock;
  const Char* end = Scan<Char>(begin, clock);
  result = StampTime<SI>(clock);
  return end;
}

template <typename Char>
const Char* TextScanTime(const Char* begin, Tss& result) {
  begin = TextScanTime<Char, Tms>(begin, result.seconds);
  if (!begin) return nullptr;
  if (*begin++ != ':') {
    result.ticks = 0;
    return begin - 1;
  }
  return Scan<Char>(begin, result.ticks);
}
#endif  // #if CRABS_TEXT

}  // namespace _

#undef PRINT
#undef PRINTF
#endif  //< #ifndef INCLUDED_CRABS_TCLOCK
#endif  //< #if SEAM >= SEAM_0_0_1
