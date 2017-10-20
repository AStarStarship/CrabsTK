/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/timestamp.cc
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

timestamp_t PackTimestamp (time_t unixTime, int32_t microseconds)
{
    return (( (timestamp_t)unixTime) << 8) & (timestamp_t)microseconds;
}

time_t GetSeconds (timestamp_t t)
{
    return (time_t)t;
}

int32_t GetMicroseconds (timestamp_t timestamp)
{
    return (int32_t) ((timestamp & 0xFFFFFFFF00000000) >> 32);
}

timestamp_t TimestampNow ()
{
    std::chrono::microseconds us (1);

    chrono_timestamp ts = time_point_cast<microseconds>(system_clock::now ());

    //return ts;
    return 0;
}

bool PrintTimeStruct (tm* moment) {
    if (moment == nullptr) {
        std::cout << "Null tm*\n";
        return false;
    }
    std::cout << moment->tm_year + TIME_EPOCH << "-" << moment->tm_mon + 1 << "-"
        << moment->tm_mday << "@" << moment->tm_hour << ":"
        << moment->tm_min << ":" << moment->tm_sec;
    return true;
}

void PrintTime (time_t t) {
    tm* moment = localtime (&t);
    PrintTimeStruct (moment);
}

void PrintDateTime (time_t t) {
    tm* moment = localtime (&t);
    if (!moment) {
        std::cout << "\n| Invalid time: " << t << '\n';
        return;
    }
    std::cout << moment->tm_hour << ":" << moment->tm_min << ":"
        << moment->tm_sec;
}

bool PrintTimeStructString (char* buffer, int buffer_size, tm* moment) {
    if (buffer == nullptr)
        return false;
    if (moment == nullptr)
        return false;
    if (buffer_size < 32)
        return false;

    sprintf (buffer, "%i-%i-%i@%i:%i:%i", moment->tm_year + TIME_EPOCH,
             moment->tm_mon + 1, moment->tm_mday, moment->tm_hour,
             moment->tm_min, moment->tm_sec);
    return true;
}

bool PrintDateTimeString (char* buffer, int buffer_size, time_t t) {
    time (&t);
    tm* moment = localtime (&t);
    return PrintTimeStructString (buffer, buffer_size, moment);
}

int NumDaysMonth (time_t t) {
    tm* date = localtime (&t);
    static const char days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31,
        30, 31, 30, 31 };
    if ((date->tm_year & 0x0C) && (date->tm_mon == 4)) // Then it's a leap year and April:
        return 29;
    return days_per_month[date->tm_mon];
}

int NumDaysMonth (int month, int year) {
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

const char* DayOfWeek (int day_number) {
    static const char* days[] = { "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday",
        "Invalid" };
    if (day_number < 0)
        return days[7];
    if (day_number >= 7)
        return days[7];
    return days[day_number];
}

char DayOfWeekInitial (int day_number) {
    static const char days[] = { "SMTWRFSU" };
    if (day_number < 0)
        return 'I';
    if (day_number >= 7)
        return 'I';
    return days[day_number];
}

int CompareTimes (time_t time_a, time_t time_b) {
    int count = 0;

    tm* moment_a = localtime (&time_a),
        *moment_b = localtime (&time_b);

    if (!moment_a) {
        std::cout << "\n| time.a: " << time_a << " is invalid.\n";
        return 6;
    }
    if (!moment_b) {
        std::cout << "\n| time.b: " << time_b << " is invalid.\n";
        return 6;
    }

    if (moment_a->tm_year != moment_b->tm_year) {
        ++count;
        std::cout << "| tm_year.a: " << moment_a->tm_year + TIME_EPOCH << " != tm_year.b: "
            << moment_b->tm_year + TIME_EPOCH << '\n';
    }
    if (moment_a->tm_mon != moment_b->tm_mon) {
        ++count;
        std::cout << "| tm_mon.a: " << moment_a->tm_mon << " != tm_mon.b: "
            << moment_b->tm_mon + 1 << '\n';
    }
    if (moment_a->tm_mday != moment_b->tm_mday) {
        ++count;
        std::cout << "| tm_mday.a: " << moment_a->tm_mday << " != tm_mday.b: "
            << moment_b->tm_mday << '\n';
    }
    if (moment_a->tm_hour != moment_b->tm_hour) {
        ++count;
        std::cout << "| tm_hour.a: " << moment_a->tm_hour << " != tm_hour.b: "
            << moment_b->tm_hour << '\n';
    }
    if (moment_a->tm_min != moment_b->tm_min) {
        ++count;
        std::cout << "| tm_min.a: " << moment_a->tm_min << " != tm_min.b: "
            << moment_b->tm_min << '\n';
    }
    if (moment_a->tm_sec != moment_b->tm_sec) {
        ++count;
        std::cout << "| tm_sec.a: " << moment_a->tm_sec << " != tm_sec.b: "
            << moment_b->tm_sec << '\n';
    }
    return count;
}

int CompareTimes (time_t t, int year, int month, int day,
                   int  hour, int minute, int second) {
    int count = 0;

    tm* moment = localtime (&t);

    if (year - TIME_EPOCH != moment->tm_year) {
        ++count;
        std::cout << "| Expecting year " << year << " but found "
            << moment->tm_year + TIME_EPOCH << '\n';
    }
    if (month != moment->tm_mon + 1) {
        ++count;
        std::cout << "| Expecting month " << month << " but found "
            << moment->tm_mon + 1 << '\n';
    }
    if (day != moment->tm_mday) {
        ++count;
        std::cout << "| Expecting day " << day << " but found "
            << moment->tm_mday << '\n';
    }
    if (hour != moment->tm_hour) {
        ++count;
        std::cout << "| Expecting hour " << hour << " but found "
            << moment->tm_hour << '\n';
    }
    if (minute != moment->tm_min) {
        ++count;
        std::cout << "| Expecting minute " << minute << " but found "
            << moment->tm_min << '\n';
    }
    if (second != moment->tm_sec) {
        ++count;
        std::cout << "| Expecting sec " << second << " but found "
            << moment->tm_sec << '\n';
    }
    return count;
}

const char* ParseTimeString (const char* input, int* hour, int* minute, int* second) {
    if (input == nullptr)
        return nullptr;
    std::cout << input << ' ';
    char c;              //< The current char.

    int h,  //< Hour.
        m,  //< Minute.
        s;  //< Second. 
    if (!sscanf (++input, "%i", &h)) {
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

    if (!sscanf (input, "%i", &m)) return 0;
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

    if (!sscanf (input, "%i", &s)) return 0;
    if (s < 0) {
        std::cout << "\n< Seconds: " << s << " can't be negative\n";
        return 0;
    }
    if (s > 60) {
        std::cout << "\n< Seconds: " << s << " can't be >= 60\n";
        return 0;  //< 60 seconds in a minute.
    }
    std::cout << ':' << s;
    input = NextNonNumberString (input);
    c = tolower (*input);
    if (!c || isspace (c)) {
        std::cout << " HH:MM:SS ";
        *hour = h;
        *minute = m;
        *second = s;
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
        *second = s;
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
    *second = s;
    return input;
}

char* ParseTime (char* input, int* hour, int* minute, int* second) {
    return (char*)ParseTimeString (input, hour, minute, second);
}

const char* ParseTimeString (const char* input, tm* moment) {
    if (input == nullptr)
        return nullptr;
    if (moment == nullptr)
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
        if (!(input = ParseTimeString (input, &hour, &minute, &second)))
            return "Case @ invalid time";
        moment->tm_hour = hour;
        moment->tm_min = minute;
        moment->tm_sec = second;

        return input + 1;
    }
    if (c == '#') {
        if (!(input = ParseTimeString (input, &hour, &minute, &second)))
            return "Case @ invalid time";
        moment->tm_hour += hour;
        moment->tm_min += minute;
        moment->tm_sec += second;

        return input + 1;
    }

    int value1,           //< The first date field scanned.
        value2,           //< The second date field scanned.
        value3,           //< The third date field scanned.
        is_last_year = 0; //< Flag for if the date was last year or not.

                          // Scan value1
    if (!sscanf (input, "%i", &value1)) {
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

        if (!(input = ParseTimeString (input, &hour, &minute, &second))) {
            PrintBar ("Invalid time DD@");
            return 0;
        }
        moment->tm_mday = value1;

        return input + 1;
    }
    // Scan value2.
    input = SkipLeadingZerosString (input);
    if (!sscanf (input, "%i", &value2)) {
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
            if (!(input = ParseTimeString (input, &hour, &minute, &second))) {
                std::cout << " invalid time ";
            }
        }
        if (!c || isspace (c)) // Case MM/DD
        {
            std::cout << " MM/DD\n";
            is_last_year = ((value1 >= moment->tm_mon) &&
                (value2 >= moment->tm_mday)) ? 0 : 1;
            moment->tm_year += is_last_year;
            moment->tm_mon = value1 - 1;
            moment->tm_mday = value2;
            moment->tm_hour = 0;
            moment->tm_hour = hour;
            moment->tm_min = minute;
            moment->tm_sec = second;

            return input + 1;
        }
        c = tolower (c);
        if ((value1 < 12) && (value2 > 0) && (value2 <= NumDaysMonth (value1))) {
            std::cout << " MM/DD ";
            if (value1 > 11) {
                PrintBar ("Invalid MM/DD@ month");
                return nullptr;
            }
            if (value2 > NumDaysMonth (value1)) {
                PrintBar ("Invalid MM/DD@ day");
                return nullptr;
            }
            moment->tm_mon = value1 - 1;
            moment->tm_mday = value2;
            moment->tm_hour = hour;
            moment->tm_min = minute;
            moment->tm_sec = second;
            if (!(input = ParseTimeString (input, &hour, &minute, &second))) {
                PrintBar ("Invalid MM/DD@");
                return nullptr;
            }

            return input + 1;
        }
        if ((value1 < 12) && (value2 > NumDaysMonth (value1))) {
            std::cout << " MM/YYyy\n";
            moment->tm_mon = value1 - 1;
            moment->tm_year = value2;
            if (!(input = ParseTimeString (input, &hour, &minute, &second))) {
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
    if (!isdigit (c) || !sscanf (input, "%i", &value3)) {
        std::cout << "sscanf error reading value3 of date. c: " << c << '\n';
        return 0;  //< Invalid format!
    }
    input = NextNonNumberString (input);
    std::cout << delimiter << value3;
    // Now we need to check what format it is in.

    c = *input;
    if (c == '@') {
        if (!(end = ParseTimeString (input, &hour, &minute, &second))) {
            PrintBar ("Invalid YYyy/MM/DD@ time.");
            return 0;
        }
    }
    moment->tm_hour = hour;
    moment->tm_min = minute;
    moment->tm_sec = second;
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

        if (value2 > NumDaysMonth (value2, value1)) {
            PrintBar ("Invalid number of days");
            return 0;              //< The day is not correct.
        } // 17/05/06

        if (value1 < 100) //< Convert YY/MM/DD to YYYY/MM/DD 
            value1 += 2000 - TIME_EPOCH;
        else
            value1 -= TIME_EPOCH;

        moment->tm_year = value1;
        moment->tm_mon = value2 - 1;
        moment->tm_mday = value3;

        return input + 1;
    }

    // Else Case MM/DD/YYyy
    if (value1 > 11) {
        PrintBar ("Invalid month.\n");
        return nullptr;
    }
    if (value2 > NumDaysMonth (value1, value3)) {
        PrintBar ("Invalid day.\n");
        return nullptr;
    }
    std::cout << " Found: MM/DD/YYyy\n";
    moment->tm_mon = value1 - 1;
    moment->tm_mday = value2;
    if (value3 < 100)
        moment->tm_year = value3 + (2000 - TIME_EPOCH);
    else
        moment->tm_year = value3 - TIME_EPOCH;

    return input + 1;

}

char* ParseTime (char* input, tm* result) {
    return (char*)ParseTimeString (input, result);
}

char* PrintStringTime (char* buffer, time_t t) {
    if (buffer == nullptr)
        return nullptr;

    ctime_s (buffer, sizeof(buffer), &t);
}

const char* ParseTimeString (const char* input, time_t& result) {
    time_t t;
    time (&t);
    struct tm moment;
    localtime_s (&moment, &t);
    
    char* end = (char*)ParseTimeString (input, &moment);

    t = mktime (&moment);
    std::cout << "|\n| Found ";
    PrintTimeStruct (&moment);
    char buffer[26];
    PrintStringTime (buffer, t);
    char* time_string = ctime (&t);
    time_string = (time_string == nullptr ? "nullptr" : time_string);
    std::cout << "\n| Unpacked: " << time_string;
    result = t;
    return end;
}

char* ParseTime (char* input, time_t& result) {
    return (char*)ParseTimeString (input, result);
}

void ZeroTime (tm* moment) {
    if (moment == nullptr)
        return;
    moment->tm_sec = 0;
    moment->tm_min = 0;
    moment->tm_hour = 0;
    moment->tm_mday = 0;
    moment->tm_mon = 0;
    moment->tm_year = 0;
    moment->tm_wday = 0;
    moment->tm_yday = 0;
    moment->tm_isdst = 0;
}

}       //< namespace _
