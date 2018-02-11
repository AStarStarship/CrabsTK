/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_timestamp.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if USING_CRABS_CLOCK

#include "console.h"
#include "text.h"


//using namespace std::chrono;

//using chrono_timestamp = std::chrono::time_point<std::chrono::system_clock,
//    std::chrono::microseconds>;
namespace _ {

#if USING_CRABS_TEXT

std::tm* ClockLocalTime (std::tm* std_tm, std::time_t const& time) {
    if (!std_tm)
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

char* SlotWriteTime (char* buffer, char* buffer_end, tm* std_tm) {
    if (!buffer) {
        return nullptr;
    }
    if (buffer > buffer_end) {
        return nullptr;
    }
    if (!std_tm) {
        return nullptr;
    }

    buffer = Print (buffer, buffer_end, std_tm->tm_year + kTimeEpoch);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = '-';
    buffer = Print (buffer, buffer_end, std_tm->tm_mon + 1);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = '-';
    buffer = Print (buffer, buffer_end, std_tm->tm_mday);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = '@';
    buffer = Print (buffer, buffer_end, std_tm->tm_hour);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = ':';
    buffer = Print (buffer, buffer_end, std_tm->tm_min);
    if (!buffer) {
        return nullptr;
    }
    *buffer++ = ':';
    buffer = Print (buffer, buffer_end, std_tm->tm_sec);
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

int ClockCompareTimes (time_t time_a, time_t time_b) {
    int count = 0;

    tm moment_a,
        moment_b;

    ClockLocalTime (&moment_a, time_a);
    ClockLocalTime (&moment_b, time_b);

    if (moment_a.tm_year != moment_b.tm_year) {
        ++count;
        Print () << "\ntm_year.a: " << moment_a.tm_year + kTimeEpoch
               << " != tm_year.b: " << moment_b.tm_year + kTimeEpoch;
    }
    if (moment_a.tm_mon != moment_b.tm_mon) {
        ++count;
        Print () << "\ntm_mon.a: " << moment_a.tm_mon << " != tm_mon.b: "
               << moment_b.tm_mon + 1;
    }
    if (moment_a.tm_mday != moment_b.tm_mday) {
        ++count;
        Print () << "\ntm_mday.a: " << moment_a.tm_mday << " != tm_mday.b: "
               << moment_b.tm_mday;
    }
    if (moment_a.tm_hour != moment_b.tm_hour) {
        ++count;
        Print () << "\ntm_hour.a: " << moment_a.tm_hour << " != tm_hour.b: "
               << moment_b.tm_hour;
    }
    if (moment_a.tm_min != moment_b.tm_min) {
        ++count;
        Print () << "\ntm_min.a: " << moment_a.tm_min << " != tm_min.b: "
               << moment_b.tm_min;
    }
    if (moment_a.tm_sec != moment_b.tm_sec) {
        ++count;
        Print () << "\ntm_sec.a: " << moment_a.tm_sec << " != tm_sec.b: "
               << moment_b.tm_sec;
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
    #if CRABS_DEBUG
        Print () << "\nExpecting year:" << year << " but found "
               << std_tm.tm_year + kTimeEpoch;
   #endif
    }
    if (month != std_tm.tm_mon + 1) {
        ++count;
    #if CRABS_DEBUG
        Print () << "\nExpecting month:" << month << " but found "
               << std_tm.tm_mon + 1;
   #endif
    }
    if (day != std_tm.tm_mday) {
        ++count;
    #if CRABS_DEBUG
        Print () << "\nExpecting day:" << day << " but found "
               << std_tm.tm_mday;
   #endif
    }
    if (hour != std_tm.tm_hour) {
        ++count;
    #if CRABS_DEBUG
        Print () << "\nExpecting hour:" << hour << " but found "
               << std_tm.tm_hour;
   #endif
    }
    if (minute != std_tm.tm_min) {
        ++count;
    #if CRABS_DEBUG
        Print () << "\nExpecting minute:" << minute << " but found "
               << std_tm.tm_min;
   #endif
    }
    if (second != std_tm.tm_sec) {
        ++count;
    #if CRABS_DEBUG
        Print () << "\nExpecting second:" << second << " but found "
               << std_tm.tm_sec;
   #endif
    }
    return count;
}

const char* SlotReadTime (const char* input, int* hour, int* minute,
                                 int* second) {
    if (input == nullptr)
        return nullptr;
    #if CRABS_DEBUG
    Print () <<  input << ' ';
   #endif
    char c;              //< The current char.

    int h,  //< Hour.
        m,  //< Minute.
        s;  //< Second. 
    if (!Scan (++input, h)) {
    #if CRABS_DEBUG
        Print () << "\nInvalid hour: " << h;
   #endif
        return 0;
    }
    input = TextSkipNumbers (input);
    if (h < 0) {
    #if CRABS_DEBUG
        Print () << "\nHours: " << h << " can't be negative.";
   #endif
        return 0;
    }
    if (h > 23) {
    #if CRABS_DEBUG
        Print () << "\nHours: " << h << " can't be > 23.";
   #endif
        return 0;
    }
    #if CRABS_DEBUG
    Print () <<  h;
    #endif
    c = *input++;
    if (!c || IsWhitespace (c))  { // Case @HH
        #if CRABS_DEBUG
        Print () << " HH ";
        #endif
        // Then it's a single number, so create a time_t for the current 
        // user-time hour..
        *hour = h;
        return input;
    }
    c = tolower (c);
    if (c == 'a') { // Case @HHAm
        #if CRABS_DEBUG
        Print () << " HHam ";
        #endif

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !IsWhitespace (c)) {
            #if CRABS_DEBUG
            Print () << "\nInvalid am format.";
            #endif
            return 0;
        }
        // Case @HHAM
        *hour = h;
        return input;
    }
    if (c == 'p')  //< Case @HHpm
    {
        #if CRABS_DEBUG
        Print () << " HHpm ";
        #endif

        if (tolower (c = *input++) == 'm')
            c = *input++;
        if (c && !IsWhitespace (c)) {
            #if CRABS_DEBUG
            Print () << "\ninvalid pm format.";
            #endif
            return 0;
        }
        // Case @HHPM
        #if CRABS_DEBUG
        Print () << "< Case HHpm " << h + 12 << ":00:00\n";
        #endif
        *hour = h + 12;
        return input;
    }
    if (c != ':') {
        #if CRABS_DEBUG
        Print () << "\nExpecting ':'.";
        #endif
        return 0;
    }

    // Cases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm

    if (!Scan (input, m)) return 0;
    input = TextSkipNumbers (input);
    if (m < 0) {
        #if CRABS_DEBUG
        Print () << "\nMinutes: " << m << " can't be negative";
        #endif
        return 0;
    }
    if (m >= 60) {
        #if CRABS_DEBUG
        Print () << "\nMinutes: " << m << " can't be >= 60";
        #endif
        return 0;    //< 60 minutes in an hour.
    }
    #if CRABS_DEBUG
    Print () <<  ':' << m;
    #endif

    input = TextSkipNumbers (input);
    c = *input++;
    if (!c || IsWhitespace (c)) { // Case HH:MM
        #if CRABS_DEBUG
        Print () << " HH:MM ";
        #endif
        *hour = h;
        *minute = m;
        return input;
    }
    c = tolower (c);
    if (c == 'a') { // Case HH::MM AM
        #if CRABS_DEBUG
        Print () << " HH:MMam ";
        #endif
        c = *input++;
        if (tolower (c) == 'm') { //< The 'm' is optional.
            c = *input++;
        }
        if (c && !IsWhitespace (c)) { //< The space is not.
            return "Invalid am in HH::MM AM";
        }
        *hour = h;
        *minute = m;
        return input;
    }
    if (c == 'p')  { // Case HH:MM PM
        #if CRABS_DEBUG
        Print () << " HH:MMpm ";
        #endif
        c = *input++;
        if (tolower (c) == 'm') { //< The 'm' is optional.
            c = *input++;
        }
        if (c && !IsWhitespace (c)) { //< The space is not.
            return "Invalid am in HH::MM PM";
        }
        *hour = h + 12;
        *minute = m;
        return input;
    }
    if (c != ':') return 0;

    // Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm

    if (!Scan (input, s))
        return 0;
    if (s < 0) {
        #if CRABS_DEBUG
        Print () << "\nSeconds: " << s << " can't be negative";
        #endif
        return 0;
    }
    if (s > 60) {
        #if CRABS_DEBUG
        Print () << "\nSeconds: " << s << " can't be >= 60";
        #endif
        return 0;  //< 60 seconds in a minute.
    }
    #if CRABS_DEBUG
    Print () <<  ':' << s;
    #endif
    input = TextSkipNumbers (input);
    c = tolower (*input);
    if (!c || IsWhitespace (c)) {
    #if CRABS_DEBUG
        Print () << " HH:MM:SS ";
   #endif
        *hour = h;
        *minute = m;
        *second = s;
        return input;
    }
    if (c == 'a') {
        #if CRABS_DEBUG
        Print () << " HH:MM:SSam ";
        #endif
        c = *input++;
        if (tolower (c) == 'm') { //< The 'm' is optional.
            c = *input++;
        }
        if (!c || !IsWhitespace (c)) {//< The space is not.
            return "Invalid am in HH::MM:SS AM";
        }
        *hour = h;
        *minute = m;
        *second = s;
        return input;
    }
    if (c != 'p') {
        #if CRABS_DEBUG
        Print () << "\nExpecting a PM but found : " << c;
        #endif
        return 0; // Format error!
    }
    #if CRABS_DEBUG
    Print () << " HH:MM:SSpm ";
    #endif
    c = tolower (*input++);
    if (c == 'm') { //< The 'm' is optional.
        c = *input++;
    }
    if (!c || !IsWhitespace (c)) { //< The space is not.
        #if CRABS_DEBUG
        Print () << "\nInvalid am in HH::MM:SS PM";
        #endif
        return nullptr;
    }
    *hour = h + 12;
    *minute = m;
    *second = s;
    return input;
}

const char* SlotReadTime (const char* input, //char* buffer_end,
                            tm* std_tm) {
    if (input == nullptr)
        return nullptr;
    if (std_tm == nullptr)
        return nullptr;
    #if CRABS_DEBUG
    Print () << "\nParsing date: " << input
         << "\nScanning: ";
    #endif
    input = TextSkipZeros (input);
    char c = *input,    //< The current char.
        delimiter;     //< The delimiter.
    const char* end;    //< Might not need

    int hour = 0,
        minute = 0,
        second = 0;

    if (c == '@') {
        if (!(input = SlotReadTime (input, &hour, &minute,
                                            &second)))
            return "Case @ invalid time";
        std_tm->tm_hour = hour;
        std_tm->tm_min = minute;
        std_tm->tm_sec = second;

        return input + 1;
    }
    if (c == '#') {
        if (!(input = SlotReadTime (input, &hour, &minute,
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
    if (!Scan (input, value1)) {
        #if CRABS_DEBUG
        Print () << Heading ("Scan error at value1");
        #endif
        return 0;
    }
    if (value1 < 0) {
    #if CRABS_DEBUG
        Print () << Heading ("Dates can't be negative.");
   #endif
        return 0;
    }
    input = TextSkipNumbers (input);
    delimiter = *input++;
    #if CRABS_DEBUG
    Print () <<  value1 << delimiter;
    #endif
    if (delimiter == '@') {
        #if CRABS_DEBUG
        Print () << " HH@ ";
        #endif

        if (!(input = SlotReadTime (input, &hour, &minute,
                                            &second))) {
            #if CRABS_DEBUG
            Print () << Heading ("Invalid time DD@");
            #endif
            return 0;
        }
        std_tm->tm_mday = value1;

        return input + 1;
    }
    // Scan value2.
    input = TextSkipZeros (input);
    if (!Scan (input, value2)) {
        #if CRABS_DEBUG
        Print () << Heading ("Failed scanning value2 of date.");
        #endif
        return 0;
    }
    if (value2 < 0) {
        #if CRABS_DEBUG
        Print () << Heading ("Day can't be negative.");
        #endif
        return 0;  //< Invalid month and day.
    }
    #if CRABS_DEBUG
    Print () <<  value2;
    #endif
    input = TextSkipNumbers (input);
    c = *input;
    if (c != delimiter) // Cases MM/DD and MM/YYyy
    {
        if (c == '@') {
            if (!(input = SlotReadTime (input, &hour,
                                                &minute, &second))) {
                #if CRABS_DEBUG
                Print () << " invalid time ";
                #endif
                return nullptr;
            }
        }
        if (!c || IsWhitespace (c)) { // Case MM/DD
            #if CRABS_DEBUG
            Print () << " MM/DD\n";
            #endif
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
            #if CRABS_DEBUG
            Print () << " MM/DD ";
            #endif
            if (value1 > 11) {
                #if CRABS_DEBUG
                ConsoleDump (Text<> ().Heading ("Invalid MM/DD@ month"));
                #endif
                return nullptr;
            }
            if (value2 > ClockNumDaysMonth (value1)) {
                #if CRABS_DEBUG
                ConsoleDump (Text<> ().Heading ("Invalid MM/DD@ day"));
                #endif
                return nullptr;
            }
            std_tm->tm_mon  = value1 - 1;
            std_tm->tm_mday = value2;
            std_tm->tm_hour = hour;
            std_tm->tm_min  = minute;
            std_tm->tm_sec  = second;
            if (!(input = SlotReadTime (input, &hour, &minute, &second))) {
                #if CRABS_DEBUG
                ConsoleDump (Text<> ().Heading ("Invalid MM/DD@"));
                #endif
                return nullptr;
            }

            return input + 1;
        }
        if ((value1 < 12) && (value2 > ClockNumDaysMonth (value1))) {
            #if CRABS_DEBUG
            Print () << " MM/YYyy\n";
            #endif
            std_tm->tm_mon = value1 - 1;
            std_tm->tm_year = value2;
            if (!(input = SlotReadTime (input, &hour,
                                                &minute, &second))) {
                #if CRABS_DEBUG
                Print () << Heading ("Invalid MM/YYYY@ time");
                #endif
                return 0;
            }

            return input + 1;
        }
    #if CRABS_DEBUG
        Print () << "\nInvalid MM/DD or MM/YYyy format\n";
   #endif
        return nullptr;
    }

    // Formats MM/DD/YYyy and YYyy/MM/DD

    input = TextSkipZeros (++input);
    c = *input;
    // Then there are 3 values and 2 delimiters.
    if (!isdigit (c) || !Scan (input, value3)) {
    #if CRABS_DEBUG
        Print () << "SlotRead error reading value3 of date. c: " << c << '\n';
   #endif
        return 0;  //< Invalid format!
    }
    input = TextSkipNumbers (input);
    Print () <<  delimiter << value3;
    // Now we need to check what format it is in.

    c = *input;
    if (c == '@') {
        if (!(end = SlotReadTime (input, &hour, &minute,
                                          &second))) {
        #if CRABS_DEBUG
            Print () << Heading ("Invalid YYyy/MM/DD@ time.");
       #endif
            return 0;
        }
    }
    std_tm->tm_hour = hour;
    std_tm->tm_min = minute;
    std_tm->tm_sec = second;
    if (IsWhitespace (*(++input))) {
        #if CRABS_DEBUG
        Print () << Heading ("No date found.");
        #endif
        return 0;
    }
    if (value1 > 11)  //< Case YYyy/MM/DD
    {
        #if CRABS_DEBUG
        Print () << " YYyy/MM/DD\n";
        #endif
        if (value2 == 0 || value2 > 12) {
            #if CRABS_DEBUG
            Print () << Heading ("Invalid number of months");
            #endif
            return 0;              //< The day is not correct.
        }

        if (value2 > ClockNumDaysMonth (value2, value1)) {
        #if CRABS_DEBUG
            ConsoleDump (Text<> ().Heading ("Invalid number of days"));
       #endif
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
    #if CRABS_DEBUG
        ConsoleDump (Text<> ().Heading ("Invalid month.\n"));
   #endif
        return nullptr;
    }
    if (value2 > ClockNumDaysMonth (value1, value3)) {
    #if CRABS_DEBUG
        ConsoleDump (Text<> ().Heading ("Invalid day.\n"));
   #endif
        return nullptr;
    }
#if CRABS_DEBUG
    Print () << " Found: MM/DD/YYyy\n";
#endif
    std_tm->tm_mon = value1 - 1;
    std_tm->tm_mday = value2;
    if (value3 < 100)
        std_tm->tm_year = value3 + (2000 - kTimeEpoch);
    else
        std_tm->tm_year = value3 - kTimeEpoch;

    return input + 1;
}

char* SlotWriteTime (char* begin, char* end, time_t t) {
    if (begin == nullptr) {
        return nullptr;
    }
    if (begin > end) {
        return nullptr;
    }
    time (&t);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return SlotWriteTime (begin, end, &std_tm);
}

const char* SlotReadTime (const char* begin, time_t& result) {
    time_t t;
    time (&t);
    tm std_tm;
    ClockLocalTime (&std_tm, t);

    char* end = (char*)SlotReadTime (begin, &std_tm);

    t = mktime (&std_tm);
    //Print () << "\n\nFound ";
    //Print () << ClockPrintTimeStruct (&std_tm);
    char buffer[26];
    SlotWriteTime (buffer, buffer + 26, t);
    char time_string[26];
    SlotWriteTime (time_string, &time_string[0]  + 26, t);
    //Print () << "\nUnpacked: " << buffer;
    result = t;
    return end;
}

void ClockZeroTime (tm* std_tm) {
    if (std_tm == nullptr)
        return;
    std_tm->tm_sec   = 0;
    std_tm->tm_min   = 0;
    std_tm->tm_hour  = 0;
    std_tm->tm_mday  = 0;
    std_tm->tm_mon   = 0;
    std_tm->tm_year  = 0;
    std_tm->tm_wday  = 0;
    std_tm->tm_yday  = 0;
    std_tm->tm_isdst = 0;
}

#endif
}       //< namespace _

#undef DEBUG_CRABS_CLOCK
#endif  //< CRABS_SEAM >= 2
