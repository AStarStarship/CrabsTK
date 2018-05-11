/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_clock.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>
#include "clock.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

#include "printer.h"
#include "text.h"
#include "scan.h"


#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_DATE(date)\
    printf ("%i/%i/%i@%i:%i:%i", date.tm_year + kTimeEpoch, date.tm_mon + 1,\
            date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_DATE(date)
#endif


//using namespace std::chrono;

//using chrono_timestamp = std::chrono::time_point<std::chrono::system_clock,
//    std::chrono::microseconds>;
namespace _ {

#if USING_PRINTER

std::tm* ClockLocalTime (std::tm* std_tm, std::time_t const& time) {
    if (!std_tm)
        return nullptr;
    #if (defined(__MINGW32__) || defined(__MINGW64__))
    memcpy (&tm_snapshot, ::localtime (&time), sizeof (std::tm));
    #elif (defined(_WIN32) || defined(__WIN32__))
    localtime_s (std_tm, &time);
    #else // POSIX
    localtime_r (&time, std_tm);
    #endif
    return std_tm;
}

/*
char* PrintClock (tm* std_tm, char* buffer, char* buffer_end) {
    return nullptr;
}

char* PrintClock (time_t t, char* buffer, char* buffer_end) {
    return nullptr;

}

const char* ClockScan (const char* input, int& hour, int& minute,
                              int& second) {
    return nullptr;

}

const char* ClockScan (tm& std_tm, const char* input) {
    return nullptr;

}

const char* ClockScan (time_t& result, const char* input) {
    return nullptr;

}

char* ClockScan (time_t& result, char* input) {
    return nullptr;

} */


const char* ClockScan (const char* input, int& hour, int& minute,
                       int& second) {
    if (input == nullptr)
        return nullptr;

    PRINTF ("\n\n    Scanning time:%s", input)
    char c;              //< The current char.
    int  h,  //< Hour.
         m,  //< Minute.
         s;  //< Second. 
    if (!Scan (h, ++input)) {
        PRINTF ("\nInvalid hour:%i", h)
        return 0;
    }
    input = TextSkipNumbers (input);
    if (h < 0) {
        PRINTF ("\nHours:%i can't be negative.", h)
        return 0;
    }
    if (h > 23) {
        PRINTF ("\nHours:%i can't be > 23.", h)
        return 0;
    }
    PRINTF ("%i", h)
    c = *input++;
    if (!c || IsWhitespace (c))  { // Case @HH
        PRINTF (" HH ")
        // Then it's a single number, so create a time_t for the current 
        // user-time hour..
        hour = h;
        return input;
    }
    c = tolower (c);
    if (c == 'a') { // 
        PRINTF ("\nCase @HHAm\n HHam ")

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !IsWhitespace (c)) {
            PRINTF ("\nInvalid am format.")
            return 0;
        }
        PRINTF (" @HHAM ")
        hour = h;
        return input;
    }
    if (c == 'p')
    {
        PRINTF (" Case @HHpm ")

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !IsWhitespace (c)) {
            PRINTF ("\ninvalid pm format.")
            return 0;
        }
        PRINTF ("< Case @HHPM %i:00:00\n",  h + 12);
        hour = h + 12;
        return input;
    }
    if (c != ':') {
        PRINTF ("\nExpecting ':'.");
        return 0;
    }

    PRINTF ("\nCases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and "
            "HH:MM:SSpm")

    if (!Scan (m, input)) return 0;
    input = TextSkipNumbers (input);
    if (m < 0) {
        PRINTF ("\nMinutes:%i can't be negative!", m)
        return 0;
    }
    if (m >= 60) {
        PRINTF ("\nMinutes:%i can't be >= 60!", m)
        return 0;    //< 60 minutes in an hour.
    }
    PRINTF (":%i", m)

    input = TextSkipNumbers (input);
    c = *input++;
    if (!c || IsWhitespace (c)) { // Case HH:MM
        PRINTF (" HH:MM ")
        hour = h;
        minute = m;
        return input;
    }
    c = tolower (c);
    if (c == 'a') { // Case HH::MM AM
        PRINTF (" HH:MMam ")
        c = *input++;
        if (tolower (c) == 'm') { //< The 'm' is optional.
            c = *input++;
        }
        if (c && !IsWhitespace (c)) { //< The space is not.
            return "Invalid am in HH::MM AM";
        }
        hour = h;
        minute = m;
        return input;
    }
    if (c == 'p')  { // Case HH:MM PM
        PRINTF (" HH:MMpm ")
        c = *input++;
        if (tolower (c) == 'm') { //< The 'm' is optional.
            c = *input++;
        }
        if (c && !IsWhitespace (c)) { //< The space is not.
            return "Invalid am in HH::MM PM";
        }
        hour = h + 12;
        minute = m;
        return input;
    }
    if (c != ':') return 0;

    PRINTF ("\n    Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm")

    if (!Scan (s, input))
        return 0;
    if (s < 0) {
        PRINTF ("\nSeconds:%i can't be negative!", s)
        return 0;
    }
    if (s > 60) {
        PRINTF ("\nSeconds:%i can't be >= 60!", s)
        return 0;
    }
    PRINTF (":%i", s)
    input = TextSkipNumbers (input);
    c = tolower (*input);
    if (!c || IsWhitespace (c)) {
        PRINTF (" HH:MM:SS ")
        hour = h;
        minute = m;
        second = s;
        return input;
    }
    if (c == 'a') {
        PRINTF (" HH:MM:SSam ")
        c = *input++;
        if (tolower (c) == 'm') { //< The 'm' is optional.
            c = *input++;
        }
        if (!c || !IsWhitespace (c)) {//< The space is not.
            return "Invalid am in HH::MM:SS AM";
        }
        hour = h;
        minute = m;
        second = s;
        return input;
    }
    if (c != 'p') {
        PRINTF ("\nExpecting a PM but found:%c", c)
        return 0; // Format error!
    }
    PRINTF (" HH:MM:SSpm ")
    c = tolower (*input++);
    if (c == 'm') { //< The 'm' is optional.
        c = *input++;
    }
    if (!c || !IsWhitespace (c)) { //< The space is not.
        PRINTF ("\nInvalid am in HH::MM:SS PM")
        return nullptr;
    }
    hour = h + 12;
    minute = m;
    second = s;
    return input;
}

const char* ClockScan (tm& std_tm, const char* input) {
    if (!input) {
        return nullptr;
    }
    PRINTF ("\n    Scanning std::tm: %s"
            "\n    Scanning: ", input)

    input = TextSkipChar (input, '0');
    char c = *input,    //< The current char.
        delimiter;     //< The delimiter.
    const char* end;    //< Might not need

    int hour = 0,
        minute = 0,
        second = 0;

    if (c == '@') {
        if (!(input = ClockScan (input, hour, minute, second)))
            return "Case @ invalid time";
        std_tm.tm_hour = hour;
        std_tm.tm_min = minute;
        std_tm.tm_sec = second;

        return input + 1;
    }
    if (c == '#') {
        if (!(input = ClockScan (input, hour, minute, second)))
            return "Case @ invalid time";
        std_tm.tm_hour += hour;
        std_tm.tm_min += minute;
        std_tm.tm_sec += second;

        return input + 1;
    }

    int value1,           //< The first date field scanned.
        value2,           //< The second date field scanned.
        value3,           //< The third date field scanned.
        is_last_year = 0; //< Flag for if the date was last year or not.

                          // Scan value1
    if (!Scan (value1, input)) {
        PRINTF ("Scan error at value1")
        return 0;
    }
    if (value1 < 0) {
        PRINTF ("Dates can't be negative.")
        return 0;
    }
    input = TextSkipNumbers (input);
    delimiter = *input++;
    PRINTF ("%i%c", value1, delimiter)
    if (delimiter == '@') {
        PRINTF (" HH@ ")

        if (!(input = ClockScan (input, hour, minute, second))) {
            PRINTF ("Invalid time DD@")
            return 0;
        }
        std_tm.tm_mday = value1;

        return input + 1;
    }
    // Scan value2.
    input = TextSkipChar (input, '0');
    if (!Scan (value2, input)) {
        PRINTF ("Failed scanning value2 of date.")
        return 0;
    }
    if (value2 < 0) {
        PRINTF ("Day can't be negative.")
        return 0;  //< Invalid month and day.
    }
    PRINTF ("%i", value2)
    input = TextSkipNumbers (input);
    c = *input;
    if (c != delimiter) {
        PRINTF ("\n    Cases MM/DD and MM/YYyy")
        if (c == '@') {
            if (!(input = ClockScan (input, hour, minute, second))) {
                PRINTF (" invalid time ")
                return nullptr;
            }
        }
        if (!c || IsWhitespace (c)) {
            PRINTF ("\n    Format is MM/DD and year is %i", 
                    std_tm.tm_year + kTimeEpoch)
            //is_last_year = ((value1 >= std_tm.tm_mon) &&
            //    (value2 >= std_tm.tm_mday)) ? 0:1;
            std_tm.tm_year += is_last_year;
            std_tm.tm_mon = value1 - 1;
            std_tm.tm_mday = value2;
            std_tm.tm_hour = 0;
            std_tm.tm_hour = hour;
            std_tm.tm_min = minute;
            std_tm.tm_sec = second;

            return input + 1;
        }
        c = tolower (c);
        if ((value1 < 12) && (value2 > 0) &&
             (value2 <= ClockNumDaysMonth (value1))) {
            PRINTF (" MM/DD ")
            if (value1 > 11) {
                PRINTF ("\nInvalid MM/DD@ month")
                return nullptr;
            }
            if (value2 > ClockNumDaysMonth (value1)) {
                PRINTF ("\nInvalid MM/DD@ day")
                return nullptr;
            }
            std_tm.tm_mon  = value1 - 1;
            std_tm.tm_mday = value2;
            std_tm.tm_hour = hour;
            std_tm.tm_min  = minute;
            std_tm.tm_sec  = second;
            if (!(input = ClockScan (input, hour, minute, second))) {
                PRINTF ("\nInvalid MM/DD@")
                return nullptr;
            }

            return input + 1;
        }
        if ((value1 < 12) && (value2 > ClockNumDaysMonth (value1))) {
            PRINTF (" MM/YYyy\n")
            std_tm.tm_mon = value1 - 1;
            std_tm.tm_year = value2;
            if (!(input = ClockScan (input, hour, minute, second))) {
                PRINTF ("\nInvalid MM / YYYY@ time")
                return 0;
            }

            return input + 1;
        }
        PRINTF ("\nInvalid MM/DD or MM/YYyy format\n")
        return nullptr;
    }

    // Formats MM/DD/YYyy and YYyy/MM/DD

    input = TextSkipChar (++input, '0');
    c = *input;
    // Then there are 3 values and 2 delimiters.
    if (!isdigit (c) || !Scan (value3, input)) {
        PRINTF ("\n    SlotRead error reading value3 of date. %c: ", c)
        return 0;  //< Invalid format!
    }
    input = TextSkipNumbers (input);
    PRINTF ("%c%i", delimiter, value3)
    // Now we need to check what format it is in.

    c = *input;
    if (c == '@') {
        if (!(end = ClockScan (input, hour, minute, second))) {
            PRINTF ("Invalid YYyy/MM/DD@ time.")
            return 0;
        }
    }
    std_tm.tm_hour = hour;
    std_tm.tm_min = minute;
    std_tm.tm_sec = second;
    if (IsWhitespace (*(++input))) {
        PRINTF ("No date found.")
        return 0;
    }
    if (value1 > 11)  {//< 
        PRINTF ("\n    Case YYyy/MM/DD\n")
        if (value2 == 0 || value2 > 12) {
            PRINTF ("Invalid number of months")
            return 0;
        }

        if (value2 > ClockNumDaysMonth (value2, value1)) {
            PRINTF ("Invalid number of days")
            return 0;
        } // 17/05/06

        if (value1 < 100) {
            PRINTF ("\n    Case YY/MM/DD")
            value1 += 2000 - kTimeEpoch;
        }
        else {
            PRINTF ("\n    Case YYYY/MM/DD")
            value1 -= kTimeEpoch;
        }

        std_tm.tm_year = value1;
        std_tm.tm_mon = value2 - 1;
        std_tm.tm_mday = value3;

        return input + 1;
    }
    PRINTF ("\n    Cases MM/DD/YY and MM/DD/YYYY")

    if (value1 > 11) {
        PRINTF ("Invalid month.\n")
        return nullptr;
    }
    if (value2 > ClockNumDaysMonth (value1, value3)) {
        PRINTF ("Invalid day.\n");
        return nullptr;
    }
    std_tm.tm_mon = value1 - 1;
    std_tm.tm_mday = value2;
    if (value3 < 100) {
        PRINTF ("\n    Case MM/DD/YY")
        std_tm.tm_year = value3 + (2000 - kTimeEpoch);
    }
    else {
        PRINTF ("\n    Case MM/DD/YYYY")
        std_tm.tm_year = value3 - kTimeEpoch;
    }
    return input + 1;
}

const char* ClockScan (time_t& result, const char* begin) {
    time_t t;
    time (&t);
    tm std_tm;
    ClockLocalTime (&std_tm, t);

    const char* end = ClockScan (std_tm, begin);

    t = mktime (&std_tm);
    //PRINTF ("\n\nFound ";
    //PRINTF (ClockPrintTimeStruct (&std_tm);
    char buffer[26];
    PrintClock (t, buffer, buffer + 26);
    char time_string[26];
    PrintClock (t, time_string, &time_string[0]  + 26);
    //PRINTF ("\nUnpacked: " << buffer;
    result = t;
    return end;
}


time_us_t ClockPackTimestamp (time_t unixTime, int32_t microseconds) {
    return (((time_us_t)unixTime) << 8) & (time_us_t)microseconds;
}

time_t ClockGetSeconds (time_us_t t) {
    return (time_t)t;
}

int32_t ClockGetMicroseconds (time_us_t timestamp) {
    return (int32_t)((timestamp & 0xFFFFFFFF00000000) >> 32);
}

char* PrintClock (char* buffer, char* buffer_end, tm* std_tm) {
    if (!buffer) {
        return nullptr;
    }
    if (buffer > buffer_end) {
        return nullptr;
    }
    if (!std_tm) {
        return nullptr;
    }

    buffer = Print (std_tm->tm_year + kTimeEpoch, buffer, buffer_end);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = '-';
    buffer = Print (std_tm->tm_mon + 1, buffer, buffer_end);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = '-';
    buffer = Print (std_tm->tm_mday, buffer, buffer_end);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = '@';
    buffer = Print (std_tm->tm_hour, buffer, buffer_end);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = ':';
    buffer = Print (std_tm->tm_min, buffer, buffer_end);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = ':';
    buffer = Print (std_tm->tm_sec, buffer, buffer_end);
    if (!buffer) {
        return nullptr;
    }
    return buffer;
}

int ClockNumDaysMonth (time_t t) {
    tm date;
    ClockLocalTime (&date, t);
    static const char days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31,
                                             30, 31, 30, 31 };
    if ((date.tm_year & 0x0C) && (date.tm_mon == 4)) {
        // Then it's a leap year and April:
        return 29;
    }
    return days_per_month[date.tm_mon];
}

int ClockNumDaysMonth (int month, int year) {
    if (month < 1)
        return 0;
    if (month > 12)
        return 0;
    static const char days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31,
        30, 31, 30, 31 };
    if ((year & 0xC) && (month == 4)) // Then it's a leap year and April:
        return 29;
    return days_per_month[month];
}

const char* ClockDayOfWeek (int day_number) {
    static const char* days[] = { "Sunday", "Monday", "Tuesday", "Wednesday",
                                  "Thursday", "Friday", "Saturday" };
    static const char kInvalidText[] = "Invalid\0";
    if (day_number < 0) {
        return kInvalidText;
    }
    if (day_number >= 7) {
        return kInvalidText;
    }
    return days[day_number];
}

char ClockDayOfWeekInitial (int day_number) {
    static const char days[] = { "SMTWRFSU" };
    if (day_number < 0) {
        return 'I';
    }
    if (day_number >= 7) {
        return 'I';
    }
    return days[day_number];
}

int ClockCompare (time_t time_a, time_t time_b) {
    tm  date_a,
        date_b;

    ClockLocalTime (&date_a, time_a);
    ClockLocalTime (&date_b, time_b);

    PRINTF ("\n    Expecting Date:")
    PRINT_DATE (date_a)
    PRINTF (" and found:")
    PRINT_DATE (date_b)

    if (date_a.tm_year != date_b.tm_year) {
        PRINTF ("\n    tm_year.a:%i != tm_year.b:%i ",
                date_a.tm_year + kTimeEpoch, date_b.tm_year + kTimeEpoch)
        return 1;
    }
    if (date_a.tm_mon != date_b.tm_mon) {
        PRINTF ("\n    tm_mon.a:%i != tm_mon.b:%i ", date_a.tm_mon,
                date_b.tm_mon + 1)
        return 2;
    }
    if (date_a.tm_mday != date_b.tm_mday) {
        PRINTF ("\n    tm_mday.a:%i != tm_mday.b:%i ", date_a.tm_mday,
                date_b.tm_mday)
        return 3;
    }
    if (date_a.tm_hour != date_b.tm_hour) {
        PRINTF ("\n    tm_hour.a:%i != tm_hour.b:%i ", date_a.tm_hour,
                date_b.tm_hour)
        return 4;
    }
    if (date_a.tm_min != date_b.tm_min) {
        PRINTF ("\n    tm_min.a:%i != tm_min.b:%i", date_a.tm_min,
                date_b.tm_min)
        return 5;
    }
    if (date_a.tm_sec != date_b.tm_sec) {
        PRINTF ("\n    tm_sec.a:%i != tm_sec.b:%i ", date_a.tm_sec,
                date_b.tm_sec)
        return 6;
    }
    return 0;
}

int ClockCompare (time_t t, int year, int month, int day,
                  int hour, int minute, int second) {
    tm  date_a;
    ClockLocalTime (&date_a, t);

    PRINTF ("\n    Expecting %i/%i/%i@%i:%i:%i and found ", year, month, day,
            hour, minute, second)
    PRINT_DATE (date_a)

    if (year - kTimeEpoch != date_a.tm_year) {
        PRINTF ("\n    Expecting year:%i but found:%i.", year,
                date_a.tm_year + kTimeEpoch)
        return 1;
    }
    if (month != date_a.tm_mon + 1) {
        PRINTF ("\n    Expecting month:%i but found:%i.", month,
                date_a.tm_mon + 1)
        return 2;
    }
    if (day != date_a.tm_mday) {
        PRINTF ("\n    Expecting day:%i but found:%i.", day,
                date_a.tm_mday)
        return 3;
    }
    if (hour != date_a.tm_hour) {
        PRINTF ("\n    Expecting hour:%i but found:%i.", hour,
                date_a.tm_hour)
        return 4;
   #endif
    }
    if (minute != date_a.tm_min) {
        PRINTF ("\n    Expecting minute:%i but found:%i.", minute,
                date_a.tm_min)
        return 5;
    }
    if (second != date_a.tm_sec) {
        PRINTF ("\n    Expecting second:%i but found:%i.", second,
                date_a.tm_sec)
        return 6;
    }
    return 0;
}

char* PrintClock (time_t t, char* buffer, char* end) {
    if (buffer == nullptr) {
        return nullptr;
    }
    if (buffer > end) {
        return nullptr;
    }
    time (&t);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return PrintClock (buffer, end, &std_tm);
}

void ClockZeroTime (tm& std_tm) {
    std_tm.tm_sec   = 0;
    std_tm.tm_min   = 0;
    std_tm.tm_hour  = 0;
    std_tm.tm_mday  = 0;
    std_tm.tm_mon   = 0;
    std_tm.tm_year  = 0;
    std_tm.tm_wday  = 0;
    std_tm.tm_yday  = 0;
    std_tm.tm_isdst = 0;
}

time_t ClockTime (int year, int month, int day, int  hour,
                 int minute, int second) {
    time_t t;
    time (&t);
    tm moment;
    ClockLocalTime (&moment, t);
    moment.tm_year = year - kTimeEpoch;
    moment.tm_mon  = month - 1;
    moment.tm_mday = day;
    moment.tm_hour = hour;
    moment.tm_min  = minute;
    moment.tm_sec  = second;

    //if (!PrintClock (moment, buffer, buffer_size)) {
    //    PRINTF ("\nError making timestamp")
    //    return 0;
    //}
   // PRINTF ("\n Creating test time: ")
    //PrintClock (moment);
    t = mktime (&moment);
    if (t < 0) {
        ///PRINTF ("\n Invalid time:" << t << '\n')
        return 0;
    } else {
        //PUTCHAR ('\n')
    }
    return t;
}

time_t ClockTime (uint year, uint month, uint day, uint hour, uint minute,
                  uint second) {
    time_t t;
    time (&t);
    tm moment;
    ClockLocalTime (&moment, t);
    moment.tm_year = year - kTimeEpoch;
    moment.tm_mon  = month - 1;
    moment.tm_mday = day;
    moment.tm_hour = hour;
    moment.tm_min  = minute;
    moment.tm_sec  = second;
    
    t = mktime (&moment);
    return t;
}

}       //< namespace _

#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 2
