/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_timestamp.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
#include "../include/clock.h"
#include "../include/utils.h"

using chrono_timestamp = std::chrono::time_point<std::chrono::system_clock,
    std::chrono::microseconds>;
using namespace std::chrono;

namespace _ {

timestamp_t ClockPackTimestamp (time_t unixTime, int32_t microseconds)
{
    return (((timestamp_t)unixTime) << 8) & (timestamp_t)microseconds;
}

time_t ClockGetSeconds (timestamp_t t)
{
    return (time_t)t;
}

int32_t ClockGetMicroseconds (timestamp_t timestamp)
{
    return (int32_t)((timestamp & 0xFFFFFFFF00000000) >> 32);
}

timestamp_t ClockTimestampNow () {
    std::chrono::microseconds us (1);
    chrono_timestamp ts = time_point_cast<microseconds>(system_clock::now ());
    return 0;
}

bool ClockPrintTimeStruct (tm* std_tm) {
    if (std_tm == nullptr) {
        std::cout << "Null tm*\n";
        return false;
    }
    std::cout << std_tm->tm_year + TIME_EPOCH << "-" << std_tm->tm_mon + 1 << "-"
        << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec;
    return true;
}

bool ClockPrintTimeStruct (tm* std_tm, int32_t microseconds) {
    if (std_tm == nullptr) {
        std::cout << "Null tm*\n";
        return false;
    }
    std::cout << std_tm->tm_year + TIME_EPOCH << "-" << std_tm->tm_mon + 1 << "-"
        << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec << ":" << microseconds;
    return true;
}

void ClockPrintTime (time_t t) {
    tm std_tm;
    localtime_s (&std_tm, &t);
    ClockPrintTimeStruct (&std_tm);
}

void ClockPrintTimestamp (timestamp_t timestamp) {
    time_t t = ClockGetSeconds (timestamp);
    tm std_tm;
    localtime_s (&std_tm, &t);
    int32_t microseconds = ClockGetMicroseconds (timestamp);
    ClockPrintTimeStruct (&std_tm, microseconds);
}

void ClockPrintDateTime (time_t t) {
    tm std_tm;
    localtime_s (&std_tm, &t);
    std::cout << std_tm.tm_hour << ":" << std_tm.tm_min << ":"
        << std_tm.tm_sec;
}

bool ClockPrintTimeStructString (char* buffer, int buffer_size, tm* std_tm) {
    if (buffer == nullptr)
        return false;
    if (std_tm == nullptr)
        return false;
    if (buffer_size < 32)
        return false;

    sprintf_s (buffer, buffer_size, "%i-%i-%i@%i:%i:%i",
        std_tm->tm_year + TIME_EPOCH, std_tm->tm_mon + 1,
        std_tm->tm_mday, std_tm->tm_hour, std_tm->tm_min,
        std_tm->tm_sec);
    return true;
}

bool ClockPrintDateTimeString (char* buffer, int buffer_size, time_t t) {
    time (&t);
    tm std_tm;
    localtime_s (&std_tm, &t);
    return ClockPrintTimeStructString (buffer, buffer_size, &std_tm);
}

int ClockNumDaysMonth (time_t t) {
    tm date;
    localtime_s (&date, &t);
    static const char days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31,
        30, 31, 30, 31 };
    if ((date.tm_year & 0x0C) && (date.tm_mon == 4)) // Then it's a leap year and April:
        return 29;
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
        "Thursday", "Friday", "Saturday",
        "Invalid" };
    if (day_number < 0)
        return days[7];
    if (day_number >= 7)
        return days[7];
    return days[day_number];
}

char ClockDayOfWeekInitial (int day_number) {
    static const char days[] = { "SMTWRFSU" };
    if (day_number < 0)
        return 'I';
    if (day_number >= 7)
        return 'I';
    return days[day_number];
}

int ClockCompareTimes (time_t time_a, time_t time_b) {
    int count = 0;

    tm moment_a,
        moment_b;

    localtime_s (&moment_a, &time_a);
    localtime_s (&moment_b, &time_b);

    if (moment_a.tm_year != moment_b.tm_year) {
        ++count;
        std::cout << "| tm_year.a: " << moment_a.tm_year + TIME_EPOCH << " != tm_year.b: "
            << moment_b.tm_year + TIME_EPOCH << '\n';
    }
    if (moment_a.tm_mon != moment_b.tm_mon) {
        ++count;
        std::cout << "| tm_mon.a: " << moment_a.tm_mon << " != tm_mon.b: "
            << moment_b.tm_mon + 1 << '\n';
    }
    if (moment_a.tm_mday != moment_b.tm_mday) {
        ++count;
        std::cout << "| tm_mday.a: " << moment_a.tm_mday << " != tm_mday.b: "
            << moment_b.tm_mday << '\n';
    }
    if (moment_a.tm_hour != moment_b.tm_hour) {
        ++count;
        std::cout << "| tm_hour.a: " << moment_a.tm_hour << " != tm_hour.b: "
            << moment_b.tm_hour << '\n';
    }
    if (moment_a.tm_min != moment_b.tm_min) {
        ++count;
        std::cout << "| tm_min.a: " << moment_a.tm_min << " != tm_min.b: "
            << moment_b.tm_min << '\n';
    }
    if (moment_a.tm_sec != moment_b.tm_sec) {
        ++count;
        std::cout << "| tm_sec.a: " << moment_a.tm_sec << " != tm_sec.b: "
            << moment_b.tm_sec << '\n';
    }
    return count;
}

int ClockCompareTimes (time_t t, int year, int month, int day,
    int  hour, int minute, int second) {
    int count = 0;

    tm std_tm;
    localtime_s (&std_tm, &t);

    if (year - TIME_EPOCH != std_tm.tm_year) {
        ++count;
        std::cout << "| Expecting year " << year << " but found "
            << std_tm.tm_year + TIME_EPOCH << '\n';
    }
    if (month != std_tm.tm_mon + 1) {
        ++count;
        std::cout << "| Expecting month " << month << " but found "
            << std_tm.tm_mon + 1 << '\n';
    }
    if (day != std_tm.tm_mday) {
        ++count;
        std::cout << "| Expecting day " << day << " but found "
            << std_tm.tm_mday << '\n';
    }
    if (hour != std_tm.tm_hour) {
        ++count;
        std::cout << "| Expecting hour " << hour << " but found "
            << std_tm.tm_hour << '\n';
    }
    if (minute != std_tm.tm_min) {
        ++count;
        std::cout << "| Expecting minute " << minute << " but found "
            << std_tm.tm_min << '\n';
    }
    if (second != std_tm.tm_sec) {
        ++count;
        std::cout << "| Expecting sec " << second << " but found "
            << std_tm.tm_sec << '\n';
    }
    return count;
}

const char* ClockParseTimeString (const char* input, int* hour, int* minute, int* second) {
    if (input == nullptr)
        return nullptr;
    std::cout << input << ' ';
    char c;              //< The current char.

    int h,  //< Hour.
        m,  //< Minute.
        string;  //< Second. 
    if (!sscanf_s (++input, "%i", &h)) {
        std::cout << "< Invalid hour: " << h << "\n";
        return 0;
    }
    input = NextNonNumberString (input);
    if (h < 0) {
        std::cout << "< Hours: " << h << " can't be negative.\n";
        return 0;
    }
    if (h > 23) {
        std::cout << "< Hours: " << h << " can't be > 23.\n";
        return 0;
    }
    std::cout << h;
    c = *input++;
    if (!c || isspace (c))  // Case @HH
    {
        std::cout << " HH ";
        // Then it's a single number, so create a time_t for the current 
        // user-time hour..
        *hour = h;
        return input;
    }
    c = tolower (c);
    if (c == 'a') // Case @HHAm
    {
        std::cout << " HHam ";

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !isspace (c)) {
            PrintBar ("invalid am format.");
            return 0;
        }
        // Case @HHAM
        *hour = h;
        return input;
    }
    if (c == 'p')  //< Case @HHpm
    {
        std::cout << " HHpm ";

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !isspace (c)) {
            PrintBar ("invalid pm format.");
            return 0;
        }
        // Case @HHPM
        std::cout << "< Case HHpm " << h + 12 << ":00:00\n";
        *hour = h + 12;
        return input;
    }
    if (c != ':') {
        PrintBar ("Expecting ':'.");
        return 0;
    }

    // Cases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm

    if (!sscanf_s (input, "%i", &m)) return 0;
    input = NextNonNumberString (input);
    if (m < 0) {
        std::cout << "Minutes: " << m << " can't be negative\n";
        return 0;
    }
    if (m >= 60) {
        std::cout << "Minutes: " << m << " can't be >= 60\n";
        return 0;    //< 60 minutes in an hour.
    }
    std::cout << ':' << m;

    input = NextNonNumberString (input);
    c = *input++;
    if (!c || isspace (c)) // Case HH:MM
    {
        std::cout << " HH:MM ";
        *hour = h;
        *minute = m;
        return input;
    }
    c = tolower (c);
    if (c == 'a') // Case HH::MM AM
    {
        std::cout << " HH:MMam ";
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
        std::cout << " HH:MMpm ";
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

    if (!sscanf_s (input, "%i", &string)) return 0;
    if (string < 0) {
        std::cout << "\n< Seconds: " << string << " can't be negative\n";
        return 0;
    }
    if (string > 60) {
        std::cout << "\n< Seconds: " << string << " can't be >= 60\n";
        return 0;  //< 60 seconds in a minute.
    }
    std::cout << ':' << string;
    input = NextNonNumberString (input);
    c = tolower (*input);
    if (!c || isspace (c)) {
        std::cout << " HH:MM:SS ";
        *hour = h;
        *minute = m;
        *second = string;
        return input;
    }
    if (c == 'a') {
        std::cout << " HH:MM:SSam ";
        c = *input++;
        if (tolower (c) == 'm') //< The 'm' is optional.
            c = *input++;
        if (!c || !isspace (c))        //< The space is not.
            return "Invalid am in HH::MM:SS AM";
        *hour = h;
        *minute = m;
        *second = string;
        return input;
    }
    if (c != 'p') {
        std::cout << "\n< Expecting a PM but found : " << c << '\n';
        return 0; // Format error!
    }
    std::cout << " HH:MM:SSpm ";
    c = tolower (*input++);
    if (c == 'm') //< The 'm' is optional.
        c = *input++;
    if (!c || !isspace (c))        //< The space is not.
    {
        PrintBar ("Invalid am in HH::MM:SS PM");
        return nullptr;
    }
    *hour = h + 12;
    *minute = m;
    *second = string;
    return input;
}

char* ClockParseTime (char* input, int* hour, int* minute, int* second) {
    return (char*)ClockParseTimeString (input, hour, minute, second);
}

const char* ClockParseTimeStructString (const char* input, tm* std_tm) {
    if (input == nullptr)
        return nullptr;
    if (std_tm == nullptr)
        return nullptr;
    std::cout << "\n< Parsing date: " << input
        << "\n< Scanning: ";
    input = SkipLeadingZerosString (input);
    char c = *input,    //< The current char.
        delimiter;     //< The delimiter.
    const char* end;    //< Might not need

    int hour = 0,
        minute = 0,
        second = 0;

    if (c == '@') {
        if (!(input = ClockParseTimeString (input, &hour, &minute, &second)))
            return "Case @ invalid time";
        std_tm->tm_hour = hour;
        std_tm->tm_min = minute;
        std_tm->tm_sec = second;

        return input + 1;
    }
    if (c == '#') {
        if (!(input = ClockParseTimeString (input, &hour, &minute, &second)))
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
    if (!sscanf_s (input, "%i", &value1)) {
        PrintBar ("Scan error at value1");
        return 0;
    }
    if (value1 < 0) {
        PrintBar ("Dates can't be negative.");
        return 0;
    }
    input = NextNonNumberString (input);
    delimiter = *input++;
    //std::cout << " delimiter " << delimiter << ": ";
    std::cout << value1 << delimiter;
    if (delimiter == '@') {
        std::cout << " HH@ ";

        if (!(input = ClockParseTimeString (input, &hour, &minute, &second))) {
            PrintBar ("Invalid time DD@");
            return 0;
        }
        std_tm->tm_mday = value1;

        return input + 1;
    }
    // Scan value2.
    input = SkipLeadingZerosString (input);
    if (!sscanf_s (input, "%i", &value2)) {
        PrintBar ("Failed scanning value2 of date.");
        return 0;
    }
    if (value2 < 0) {
        PrintBar ("Day can't be negative.");
        return 0;  //< Invalid month and day.
    }
    std::cout << value2;
    input = NextNonNumberString (input);
    c = *input;
    if (c != delimiter) // Cases MM/DD and MM/YYyy
    {
        if (c == '@') {
            if (!(input = ClockParseTimeString (input, &hour, &minute, &second))) {
                std::cout << " invalid time ";
            }
        }
        if (!c || isspace (c)) // Case MM/DD
        {
            std::cout << " MM/DD\n";
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
        if ((value1 < 12) && (value2 > 0) && (value2 <= ClockNumDaysMonth (value1))) {
            std::cout << " MM/DD ";
            if (value1 > 11) {
                PrintBar ("Invalid MM/DD@ month");
                return nullptr;
            }
            if (value2 > ClockNumDaysMonth (value1)) {
                PrintBar ("Invalid MM/DD@ day");
                return nullptr;
            }
            std_tm->tm_mon = value1 - 1;
            std_tm->tm_mday = value2;
            std_tm->tm_hour = hour;
            std_tm->tm_min = minute;
            std_tm->tm_sec = second;
            if (!(input = ClockParseTimeString (input, &hour, &minute, &second))) {
                PrintBar ("Invalid MM/DD@");
                return nullptr;
            }

            return input + 1;
        }
        if ((value1 < 12) && (value2 > ClockNumDaysMonth (value1))) {
            std::cout << " MM/YYyy\n";
            std_tm->tm_mon = value1 - 1;
            std_tm->tm_year = value2;
            if (!(input = ClockParseTimeString (input, &hour, &minute, &second))) {
                PrintBar ("Invalid MM/YYYY@ time");
                return 0;
            }

            return input + 1;
        }
        std::cout << "\n< Invalid MM/DD or MM/YYyy format\n";
        return nullptr;
    }

    // Formats MM/DD/YYyy and YYyy/MM/DD

    input = SkipLeadingZerosString (++input);
    c = *input;
    // Then there are 3 values and 2 delimiters.
    if (!isdigit (c) || !sscanf_s (input, "%i", &value3)) {
        std::cout << "sscanf_s error reading value3 of date. c: " << c << '\n';
        return 0;  //< Invalid format!
    }
    input = NextNonNumberString (input);
    std::cout << delimiter << value3;
    // Now we need to check what format it is in.

    c = *input;
    if (c == '@') {
        if (!(end = ClockParseTimeString (input, &hour, &minute, &second))) {
            PrintBar ("Invalid YYyy/MM/DD@ time.");
            return 0;
        }
    }
    std_tm->tm_hour = hour;
    std_tm->tm_min = minute;
    std_tm->tm_sec = second;
    if (isspace (*(++input))) {
        PrintBar ("No date found.");
        return 0;
    }
    if (value1 > 11)  //< Case YYyy/MM/DD
    {
        std::cout << " YYyy/MM/DD\n";
        if (value2 == 0 || value2 > 12) {
            PrintBar ("Invalid number of months");
            return 0;              //< The day is not correct.
        }

        if (value2 > ClockNumDaysMonth (value2, value1)) {
            PrintBar ("Invalid number of days");
            return 0;              //< The day is not correct.
        } // 17/05/06

        if (value1 < 100) //< Convert YY/MM/DD to YYYY/MM/DD 
            value1 += 2000 - TIME_EPOCH;
        else
            value1 -= TIME_EPOCH;

        std_tm->tm_year = value1;
        std_tm->tm_mon = value2 - 1;
        std_tm->tm_mday = value3;

        return input + 1;
    }

    // Else Case MM/DD/YYyy
    if (value1 > 11) {
        PrintBar ("Invalid month.\n");
        return nullptr;
    }
    if (value2 > ClockNumDaysMonth (value1, value3)) {
        PrintBar ("Invalid day.\n");
        return nullptr;
    }
    std::cout << " Found: MM/DD/YYyy\n";
    std_tm->tm_mon = value1 - 1;
    std_tm->tm_mday = value2;
    if (value3 < 100)
        std_tm->tm_year = value3 + (2000 - TIME_EPOCH);
    else
        std_tm->tm_year = value3 - TIME_EPOCH;

    return input + 1;

}

char* ClockParseTimeStruct (char* input, tm* result) {
    return (char*)ClockParseTimeStructString (input, result);
}

char* ClockPrintStringTime (char* buffer, time_t t) {
    if (buffer == nullptr)
        return nullptr;

    ctime_s (buffer, sizeof (buffer), &t);
    return buffer;
}

const char* ClockParseUnixTimeString (const char* input, time_t& result) {
    time_t t;
    time (&t);
    struct tm std_tm;
    localtime_s (&std_tm, &t);

    char* end = (char*)ClockParseTimeStructString (input, &std_tm);

    t = mktime (&std_tm);
    std::cout << "|\n| Found ";
    ClockPrintTimeStruct (&std_tm);
    char buffer[26];
    ClockPrintStringTime (buffer, t);
    char time_string[26];
    errno_t error = ctime_s (time_string, 26, &t);
    std::cout << "\n| Unpacked: " << time_string;
    result = t;
    return end;
}

char* ClockParseUnixTime (char* input, time_t& result) {
    return (char*)ClockParseUnixTimeString (input, result);
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

}       //< namespace _
