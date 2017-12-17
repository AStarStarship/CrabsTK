/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/script_timestamp.cc
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

#include <stdafx.h>
#include "clock.h"
#include "text.h"

using namespace std;
//using namespace std::chrono;

//using chrono_timestamp = std::chrono::time_point<std::chrono::system_clock,
//    std::chrono::microseconds>;
namespace _ {

#if USE_MORE_ROM

std::tm* ClockLocalTime (std::tm* std_tm, std::time_t const& time) {
    if (std_tm == nullptr)
        return nullptr;
#if (defined(__MINGW32__) || defined(__MINGW64__))
    memcpy (&tm_snapshot, ::localtime (&time), sizeof (std::tm));
#elif (defined(_WIN32) || defined(__WIN32__))
    localtime_s (std_tm, &time);
#else
    localtime_r (&time, std_tm); // POSIX
#endif
    return std_tm;
}

time_us_t ClockPackTimestamp (time_t unixTime, int32_t microseconds)
{
    return (((time_us_t)unixTime) << 8) & (time_us_t)microseconds;
}

time_t ClockGetSeconds (time_us_t t)
{
    return (time_t)t;
}

int32_t ClockGetMicroseconds (time_us_t timestamp)
{
    return (int32_t)((timestamp & 0xFFFFFFFF00000000) >> 32);
}

char* StrandWriteTime (char* buffer, char* buffer_end, tm* std_tm) {
    if (buffer == nullptr) {
        return nullptr;
    }
    if (buffer_end <= buffer) {
        return nullptr;
    }
    if (std_tm == nullptr) {
        return nullptr;
    }

    buffer = StrandWrite (buffer, buffer_end, std_tm->tm_year + kTimeEpoch);
    if (buffer == nullptr) {
        return nullptr;
    }
    *buffer++ = '-';
    buffer = StrandWrite (buffer, buffer_end, std_tm->tm_mon + 1);
    if (buffer == nullptr) {
        return nullptr;
    }
    *buffer++ = '-';
    buffer = StrandWrite (buffer, buffer_end, std_tm->tm_mday);
    if (buffer == nullptr) {
        return nullptr;
    }
    *buffer++ = '@';
    buffer = StrandWrite (buffer, buffer_end, std_tm->tm_hour);
    if (buffer == nullptr) {
        return nullptr;
    }
    *buffer++ = ':';
    buffer = StrandWrite (buffer, buffer_end, std_tm->tm_min);
    if (buffer == nullptr) {
        return nullptr;
    }
    *buffer++ = ':';
    buffer = StrandWrite (buffer, buffer_end, std_tm->tm_sec);
    if (buffer == nullptr) {
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

int ClockCompareTimes (time_t time_a, time_t time_b) {
    int count = 0;

    tm moment_a,
        moment_b;

    ClockLocalTime (&moment_a, time_a);
    ClockLocalTime (&moment_b, time_b);

    if (moment_a.tm_year != moment_b.tm_year) {
        ++count;
        cout << "\n| tm_year.a: " << moment_a.tm_year + kTimeEpoch
             << " != tm_year.b: " << moment_b.tm_year + kTimeEpoch << '\n';
    }
    if (moment_a.tm_mon != moment_b.tm_mon) {
        ++count;
        cout << "\n| tm_mon.a: " << moment_a.tm_mon << " != tm_mon.b: "
            << moment_b.tm_mon + 1 << '\n';
    }
    if (moment_a.tm_mday != moment_b.tm_mday) {
        ++count;
        cout << "\n| tm_mday.a: " << moment_a.tm_mday << " != tm_mday.b: "
            << moment_b.tm_mday << '\n';
    }
    if (moment_a.tm_hour != moment_b.tm_hour) {
        ++count;
        cout << "\n| tm_hour.a: " << moment_a.tm_hour << " != tm_hour.b: "
            << moment_b.tm_hour << '\n';
    }
    if (moment_a.tm_min != moment_b.tm_min) {
        ++count;
        cout << "\n| tm_min.a: " << moment_a.tm_min << " != tm_min.b: "
            << moment_b.tm_min << '\n';
    }
    if (moment_a.tm_sec != moment_b.tm_sec) {
        ++count;
        cout << "\n| tm_sec.a: " << moment_a.tm_sec << " != tm_sec.b: "
            << moment_b.tm_sec << '\n';
    }
    return count;
}

int ClockCompareTimes (time_t t, int year, int month, int day,
                       int  hour, int minute, int second) {
    int count = 0;

    tm std_tm;
    ClockLocalTime (&std_tm, t);

    if (year - kTimeEpoch != std_tm.tm_year) {
        ++count;
        cout << "\n| Expecting year:" << year << " but found "
             << std_tm.tm_year + kTimeEpoch << '\n';
    }
    if (month != std_tm.tm_mon + 1) {
        ++count;
        cout << "\n| Expecting month:" << month << " but found "
             << std_tm.tm_mon + 1 << '\n';
    }
    if (day != std_tm.tm_mday) {
        ++count;
        cout << "\n| Expecting day:" << day << " but found "
            << std_tm.tm_mday << '\n';
    }
    if (hour != std_tm.tm_hour) {
        ++count;
        cout << "\n| Expecting hour:" << hour << " but found "
            << std_tm.tm_hour << '\n';
    }
    if (minute != std_tm.tm_min) {
        ++count;
        cout << "\n| Expecting minute:" << minute << " but found "
            << std_tm.tm_min << '\n';
    }
    if (second != std_tm.tm_sec) {
        ++count;
        cout << "\n| Expecting second:" << second << " but found "
            << std_tm.tm_sec << '\n';
    }
    return count;
}

const char* StrandReadTime (const char* input, int* hour, int* minute,
                                 int* second) {
    if (input == nullptr)
        return nullptr;
    cout <<  input << ' ';
    char c;              //< The current char.

    int h,  //< Hour.
        m,  //< Minute.
        s;  //< Second. 
    if (!StrandRead (++input, h)) {
        cout << "< Invalid hour: " << h << "\n";
        return 0;
    }
    input = StrandSkipNumbers (input);
    if (h < 0) {
        cout << "< Hours: " << h << " can't be negative.\n";
        return 0;
    }
    if (h > 23) {
        cout << "< Hours: " << h << " can't be > 23.\n";
        return 0;
    }
    cout <<  h;
    c = *input++;
    if (!c || isspace (c))  // Case @HH
    {
        cout << " HH ";
        // Then it's a single number, so create a time_t for the current 
        // user-time hour..
        *hour = h;
        return input;
    }
    c = tolower (c);
    if (c == 'a') // Case @HHAm
    {
        cout << " HHam ";

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !isspace (c)) {
            cout << "\n| Invalid am format.";
            return 0;
        }
        // Case @HHAM
        *hour = h;
        return input;
    }
    if (c == 'p')  //< Case @HHpm
    {
        cout << " HHpm ";

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !isspace (c)) {
            Print ("\n| invalid pm format.");
            return 0;
        }
        // Case @HHPM
        cout << "< Case HHpm " << h + 12 << ":00:00\n";
        *hour = h + 12;
        return input;
    }
    if (c != ':') {
        Print ("\n| Expecting ':'.");
        return 0;
    }

    // Cases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm

    if (!StrandRead (input, m)) return 0;
    input = StrandSkipNumbers (input);
    if (m < 0) {
        cout << "Minutes: " << m << " can't be negative\n";
        return 0;
    }
    if (m >= 60) {
        cout << "Minutes: " << m << " can't be >= 60\n";
        return 0;    //< 60 minutes in an hour.
    }
    cout <<  ':' << m;

    input = StrandSkipNumbers (input);
    c = *input++;
    if (!c || isspace (c)) // Case HH:MM
    {
        cout << " HH:MM ";
        *hour = h;
        *minute = m;
        return input;
    }
    c = tolower (c);
    if (c == 'a') // Case HH::MM AM
    {
        cout << " HH:MMam ";
        c = *input++;
        if (tolower (c) == 'm') //< The 'm' is optional.
            c = *input++;
        if (c && !isspace (c))  //< The space is not.
            return "Invalid am in HH::MM AM";
        *hour = h;
        *minute = m;
        return input;
    }
    if (c == 'p')  // Case HH:MM PM
    {
        cout << " HH:MMpm ";
        c = *input++;
        if (tolower (c) == 'm') //< The 'm' is optional.
            c = *input++;
        if (c && !isspace (c))        //< The space is not.
            return "Invalid am in HH::MM PM";
        *hour = h + 12;
        *minute = m;
        return input;
    }
    if (c != ':') return 0;

    // Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm

    if (!StrandRead (input, s))
        return 0;
    if (s < 0) {
        cout << "\n< Seconds: " << s << " can't be negative\n";
        return 0;
    }
    if (s > 60) {
        cout << "\n< Seconds: " << s << " can't be >= 60\n";
        return 0;  //< 60 seconds in a minute.
    }
    cout <<  ':' << s;
    input = StrandSkipNumbers (input);
    c = tolower (*input);
    if (!c || isspace (c)) {
        cout << " HH:MM:SS ";
        *hour = h;
        *minute = m;
        *second = s;
        return input;
    }
    if (c == 'a') {
        cout << " HH:MM:SSam ";
        c = *input++;
        if (tolower (c) == 'm') //< The 'm' is optional.
            c = *input++;
        if (!c || !isspace (c))        //< The space is not.
            return "Invalid am in HH::MM:SS AM";
        *hour = h;
        *minute = m;
        *second = s;
        return input;
    }
    if (c != 'p') {
        cout << "\n< Expecting a PM but found : " << c << '\n';
        return 0; // Format error!
    }
    cout << " HH:MM:SSpm ";
    c = tolower (*input++);
    if (c == 'm') //< The 'm' is optional.
        c = *input++;
    if (!c || !isspace (c))        //< The space is not.
    {
        Print ("\n| Invalid am in HH::MM:SS PM");
        return nullptr;
    }
    *hour = h + 12;
    *minute = m;
    *second = s;
    return input;
}

const char* StrandReadTime (const char* input, //char* buffer_end,
                                 tm* std_tm) {
    if (input == nullptr)
        return nullptr;
    if (std_tm == nullptr)
        return nullptr;
    cout << "\n< Parsing date: " << input
         << "\n< Scanning: ";
    input = StrandSkipZeros (input);
    char c = *input,    //< The current char.
        delimiter;     //< The delimiter.
    const char* end;    //< Might not need

    int hour = 0,
        minute = 0,
        second = 0;

    if (c == '@') {
        if (!(input = StrandReadTime (input, &hour, &minute,
                                            &second)))
            return "Case @ invalid time";
        std_tm->tm_hour = hour;
        std_tm->tm_min = minute;
        std_tm->tm_sec = second;

        return input + 1;
    }
    if (c == '#') {
        if (!(input = StrandReadTime (input, &hour, &minute,
                                            &second)))
            return "Case @ invalid time";
        std_tm->tm_hour += hour;
        std_tm->tm_min += minute;
        std_tm->tm_sec += second;

        return input + 1;
    }

    int value1,           //< The first date field scanned.
        value2,           //< The second date field scanned.
        value3,           //< The third date field scanned.
        is_last_year = 0; //< Flag for if the date was last year or not.

                          // Scan value1
    if (!StrandRead (input, value1)) {
        cout << Text ().Bar ("Scan error at value1");
        return 0;
    }
    if (value1 < 0) {
        cout << Text ().Bar ("Dates can't be negative.");
        return 0;
    }
    input = StrandSkipNumbers (input);
    delimiter = *input++;
    //cout << " delimiter " << delimiter << ": ";
    cout <<  value1 << delimiter;
    if (delimiter == '@') {
        cout << " HH@ ";

        if (!(input = StrandReadTime (input, &hour, &minute,
                                            &second))) {
            cout << Text ().Bar ("Invalid time DD@");
            return 0;
        }
        std_tm->tm_mday = value1;

        return input + 1;
    }
    // Scan value2.
    input = StrandSkipZeros (input);
    if (!StrandRead (input, value2)) {
        cout << Text ().Bar ("Failed scanning value2 of date.");
        return 0;
    }
    if (value2 < 0) {
        cout << Text ().Bar ("Day can't be negative.");
        return 0;  //< Invalid month and day.
    }
    cout <<  value2;
    input = StrandSkipNumbers (input);
    c = *input;
    if (c != delimiter) // Cases MM/DD and MM/YYyy
    {
        if (c == '@') {
            if (!(input = StrandReadTime (input, &hour,
                                                &minute, &second))) {
                cout << " invalid time ";
            }
        }
        if (!c || isspace (c)) // Case MM/DD
        {
            cout << " MM/DD\n";
            is_last_year = ((value1 >= std_tm->tm_mon) &&
                (value2 >= std_tm->tm_mday))?0:1;
            std_tm->tm_year += is_last_year;
            std_tm->tm_mon = value1 - 1;
            std_tm->tm_mday = value2;
            std_tm->tm_hour = 0;
            std_tm->tm_hour = hour;
            std_tm->tm_min = minute;
            std_tm->tm_sec = second;

            return input + 1;
        }
        c = tolower (c);
        if ((value1 < 12) && (value2 > 0) &&
             (value2 <= ClockNumDaysMonth (value1))) {
            cout << " MM/DD ";
            if (value1 > 11) {
                cout << Text ().Bar ("Invalid MM/DD@ month");
                return nullptr;
            }
            if (value2 > ClockNumDaysMonth (value1)) {
                cout << Text ().Bar ("Invalid MM/DD@ day");
                return nullptr;
            }
            std_tm->tm_mon = value1 - 1;
            std_tm->tm_mday = value2;
            std_tm->tm_hour = hour;
            std_tm->tm_min = minute;
            std_tm->tm_sec = second;
            if (!(input = StrandReadTime (input, &hour,
                                                &minute, &second))) {
                cout << Text ().Bar ("Invalid MM/DD@");
                return nullptr;
            }

            return input + 1;
        }
        if ((value1 < 12) && (value2 > ClockNumDaysMonth (value1))) {
            cout << " MM/YYyy\n";
            std_tm->tm_mon = value1 - 1;
            std_tm->tm_year = value2;
            if (!(input = StrandReadTime (input, &hour,
                                                &minute, &second))) {
                cout << Text ().Bar ("Invalid MM/YYYY@ time");
                return 0;
            }

            return input + 1;
        }
        cout << "\n< Invalid MM/DD or MM/YYyy format\n";
        return nullptr;
    }

    // Formats MM/DD/YYyy and YYyy/MM/DD

    input = StrandSkipZeros (++input);
    c = *input;
    // Then there are 3 values and 2 delimiters.
    if (!isdigit (c) || !StrandRead (input, value3)) {
        cout << "StrandRead error reading value3 of date. c: " << c << '\n';
        return 0;  //< Invalid format!
    }
    input = StrandSkipNumbers (input);
    cout <<  delimiter << value3;
    // Now we need to check what format it is in.

    c = *input;
    if (c == '@') {
        if (!(end = StrandReadTime (input, &hour, &minute,
                                          &second))) {
            cout << Text ().Bar ("Invalid YYyy/MM/DD@ time.");
            return 0;
        }
    }
    std_tm->tm_hour = hour;
    std_tm->tm_min = minute;
    std_tm->tm_sec = second;
    if (isspace (*(++input))) {
        cout << Text ().Bar ("No date found.");
        return 0;
    }
    if (value1 > 11)  //< Case YYyy/MM/DD
    {
        cout << " YYyy/MM/DD\n";
        if (value2 == 0 || value2 > 12) {
            cout << Text ().Bar ("Invalid number of months");
            return 0;              //< The day is not correct.
        }

        if (value2 > ClockNumDaysMonth (value2, value1)) {
            cout << Text ().Bar ("Invalid number of days");
            return 0;              //< The day is not correct.
        } // 17/05/06

        if (value1 < 100) //< Convert YY/MM/DD to YYYY/MM/DD 
            value1 += 2000 - kTimeEpoch;
        else
            value1 -= kTimeEpoch;

        std_tm->tm_year = value1;
        std_tm->tm_mon = value2 - 1;
        std_tm->tm_mday = value3;

        return input + 1;
    }

    // Else Case MM/DD/YYyy
    if (value1 > 11) {
        cout << Text ().Bar ("Invalid month.\n");
        return nullptr;
    }
    if (value2 > ClockNumDaysMonth (value1, value3)) {
        cout << Text ().Bar ("Invalid day.\n");
        return nullptr;
    }
    cout << " Found: MM/DD/YYyy\n";
    std_tm->tm_mon = value1 - 1;
    std_tm->tm_mday = value2;
    if (value3 < 100)
        std_tm->tm_year = value3 + (2000 - kTimeEpoch);
    else
        std_tm->tm_year = value3 - kTimeEpoch;

    return input + 1;

}

char* TextWriteTime (char* begin, char* end, time_t t) {
    if (begin == nullptr) {
        return nullptr;
    }
    if (begin > end) {
        return nullptr;
    }
    time (&t);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return StrandWriteTime (begin, end, &std_tm);
}

const char* StrandReadTime (const char* begin, time_t& result) {
    time_t t;
    time (&t);
    tm std_tm;
    ClockLocalTime (&std_tm, t);

    char* end = (char*)StrandReadTime (begin, &std_tm);

    t = mktime (&std_tm);
    //cout << "\n|\n| Found ";
    //cout << ClockPrintTimeStruct (&std_tm);
    char buffer[26];
    TextWriteTime (buffer, buffer + 26, t);
    char time_string[26];
    TextWriteTime (time_string, &time_string[0]  + 26, t);
    //cout << "\n| Unpacked: " << buffer;
    result = t;
    return end;
}

void ClockZeroTime (tm* std_tm) {
    if (std_tm == nullptr)
        return;
    std_tm->tm_sec = 0;
    std_tm->tm_min = 0;
    std_tm->tm_hour = 0;
    std_tm->tm_mday = 0;
    std_tm->tm_mon = 0;
    std_tm->tm_year = 0;
    std_tm->tm_wday = 0;
    std_tm->tm_yday = 0;
    std_tm->tm_isdst = 0;
}

#endif  //< USE_MORE_ROM
}       //< namespace _
