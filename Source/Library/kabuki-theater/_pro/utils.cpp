/** Misc C++ Utils
    @file   utils.cpp
    @author Cale McCollough
    @brief  Various C++ utils shared between projects.
*/

#include "utils.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

namespace _pro {

char ReadChar (const char* header) {
    std::cout << header;
    char c = getchar ();
    std::cout << '\n';
    return c;
}

int ReadInt (const char* header) {
    int number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

float ReadFloat (const char* header) {
    float number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

char* ReadString (const char* header, int buffer_size) {
    if (buffer_size < 0)
        return nullptr;
    char* buffer = new char[buffer_size];
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');

    return buffer;
}

void ReadString (const char* header, char* buffer, int buffer_size) {
    if (buffer == nullptr)
        return;
    if (buffer_size < 0)
        return;
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');
}

char* CloneString (const char* string) {
    if (string == nullptr)
        string = "";
    size_t length = strlen (string);
    char* clone = new char[length + 1];
    strcpy (clone, string);
    return clone;
}

void PrintBar (const char* string) {
    std::cout << "\n| " << string << '\n';
}

void PrintLines (int num_lines) {
    for (int i = 0; i < num_lines; ++i)
        std::cout << '\n';
}

void PrintBreak (const char* header, char c, int num_lines, int console_width) {
    for (int i = 0; i < num_lines; ++i)
        std::cout << '\n';
    std::cout << header;
    int length = strlen (header);
    for (int i = 0; i < console_width - length; ++i)
        std::cout << c;
    std::cout << '\n';
}

void PrintCentered (const char* string, int width, bool is_last, char column_delimeter) {
    if (width < 1)
        return;
    std::cout << column_delimeter;
    int length = strlen (string),
        num_spaces;

    if (width < length) {
        char format[32];
        sprintf (format, "%%%i", is_last ? width - 2 : width - 1);
        std::cout << column_delimeter;
        printf (format, string);
        if (is_last)
            std::cout << column_delimeter << '\n';
    }
    num_spaces = width - length - 1 - (is_last ? 1 : 0);
    for (int i = 0; i < num_spaces / 2; ++i)
        std::cout << ' ';
    std::cout << string;
    for (int i = 0; i < num_spaces - (num_spaces / 2); ++i)
        std::cout << ' ';
    if (is_last)
        std::cout << column_delimeter << '\n';
}


void PrintColumnBreak (int num_columns, char column_delimeter, char break_char, int width) {
    int column_width = width / num_columns;
    for (int i = 0; i < num_columns - 1; ++i) {
        std::cout << column_delimeter;
        for (int j = 0; j < column_width - 1; ++j)
            std::cout << break_char;
    }
    std::cout << column_delimeter;
    for (int j = 0; j < column_width - 2; ++j)
        std::cout << break_char;
    std::cout << column_delimeter << '\n';
}

inline const char* FindEndOfRow (const char* string, int num_columns) {
    char c;
    // Scroll to the end of the line.
    while (c = *string++) {
        if (num_columns-- < 0) // We've reached the end.
        {
            // Scroll left till we hit whitespace (if any).
            while (!isspace (c))
                c = *(--string);
            // Then scroll past the whitespace.
            while (isspace (c));
            c = *(--string);
            return string;
        }
    }
    return --string;
}

void PrintPageRight (const char* string, int num_columns) {
    char c;  //< The current char.

    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (string == nullptr)
        return;
    do {
        const char* end_row = FindEndOfRow (string, num_columns);
        size_t row_length = end_row - string,
            num_left_spaces = num_columns - row_length,
            i;
        // Print left whitespace.
        for (i = num_left_spaces; i != 0; --i)
            std::cout << ' ';
        // Print string.
        for (i = row_length; i != 0; --i)
            std::cout << *string++;
        // Increment the string pointer and scroll past the whitespace
        string += row_length;
        while (isspace (c = *string++));
    } while (c);
}

void PrintPageCentered (const char* string, int num_columns) {
    char c;   //< The current char.
    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (string == nullptr)
        return;
    do {
        const char* end_row = FindEndOfRow (string, num_columns);
        size_t row_length = end_row - string,
            num_left_spaces = (num_columns / 2) - (row_length / 2),
            i;
        // Print left whitespace.
        for (i = num_left_spaces; i != 0; --i)
            std::cout << ' ';
        // Print string.
        for (i = row_length; i != 0; --i)
            std::cout << *string++;
        for (i = num_columns - num_left_spaces - row_length; i != 0; --i)
            std::cout << ' ';
        // Increment the string pointer and scroll past the whitespace
        string += row_length;
        while (isspace (c = *string++));
    } while (c);
}

void PrintPage (const char* string, int indentation, char bullet,
                int index, int tab_size, int num_columns) {
    num_columns -= 4;
    std::cout << "| ";
    int cursor; //< The column number of the cursor.
    char c = *string++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || string == nullptr)  //< It's an empty string.
    {
        for (int i = num_columns; i > 0; --i)
            std::cout << ' ';
        std::cout << "|\n";
        return;
    }

    // Make the string for the bullet.
    if (isdigit (bullet)) // Then we have to print a number bullet.
    {
        sprintf (&buffer[0], "%i", index);
        //< + 2 for "| " - 2 for the bullet offset.
        char format[16];
        sprintf (&format[1], "%%%us", indentation * tab_size);
        printf (format, buffer);
    } else if (isalpha (bullet)) // Then it's a lower case bullet.
    {
        cursor = 0;
        bullet += index % 26;  //< 
        for (; index > 0; index -= 26)
            buffer[cursor++] = bullet;
        buffer[cursor] = '\0';
    } else {
        buffer[0] = bullet;
        buffer[1] = 0;
    }
    // Prints the string.
    int num_spaces = tab_size * indentation;
    while (c) {
        for (cursor = num_spaces; cursor > 0; --cursor)
            std::cout << ' ';
        cursor = num_spaces;
        while (c && (cursor++ < num_columns)) {
            std::cout << c;
            c = *string++;
        }
        std::cout << " |\n";
    }
    std::cout << '\n';
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

const char* NextNonNumberString (const char* input) {
    char c = *input;
    if (c == '-')           // It might be negative.
    {
        c = *(++input);
        if (!isdigit (c))   // but it's not.
            return input - 1;
        c = *(++input);
    }
    while (isdigit (c))
        c = *(++input);
    return input;
}

char* NextNonNumber (char* input) {
    return (char*)NextNonNumberString (input);
}

const char* SkipLeadingZerosString (const char* input) {
    if (input == nullptr)
        return nullptr;
    char c = *input,
        d;
    if (c != '0')
        return input;
    d = *(++input);
    // We know the first char is a '0' so just loop till c and d aren't '0'.
    while (c == d) {
        c = d;
        d = *(++input);
    }
    if (!isdigit (d))       //< Special case for "...00" string.
        return input - 1;
    return input;
}

char* SkipLeadingZeros (char* input) {
    return (char*)SkipLeadingZerosString (input);
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

const char* ParseTimeString (const char* input, time_t& result) {
    time_t t;
    time (&t);
    tm* moment = localtime (&t);
    char* end = (char*)ParseTimeString (input, moment);

    t = mktime (moment);
    std::cout << "|\n| Found ";
    PrintDateTime (moment);
    char* time_string = ctime (&t);
    time_string = (time_string == nullptr ? "nullptr" : time_string);
    std::cout << "\n| Unpacked: " << time_string;
    result = t;
    return end;
}

char* ParseTime (char* input, time_t& result) {
    return (char*)ParseTimeString (input, result);
}

bool PrintDateTime (tm* moment) {
    if (moment == nullptr) {
        std::cout << "Null tm*\n";
        return false;
    }
    std::cout << moment->tm_year + TIME_EPOCH << "-" << moment->tm_mon + 1 << "-"
        << moment->tm_mday << "@" << moment->tm_hour << ":"
        << moment->tm_min << ":" << moment->tm_sec;
    return true;
}

bool PrintDateTimeString (char* buffer, int buffer_size, tm* moment) {
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
    return PrintDateTimeString (buffer, buffer_size, moment);
}

void PrintTime (time_t t) {
    tm* moment = localtime (&t);
    if (!moment) {
        std::cout << "\n| Invalid time: " << t << '\n';
        return;
    }
    std::cout << moment->tm_hour << ":" << moment->tm_min << ":"
        << moment->tm_sec;
}

void PrintDateTime (time_t t) {
    tm* moment = localtime (&t);
    PrintDateTime (moment);
}

const char* SkipSpacesString (const char* input) {
    if (input == nullptr)
        return 0;
    //std::cout << "\nSkipping spaces: ";
    char c = *input;
    while (c) {
        //std::cout << '.';
        if (!isspace (c))
            return input;
        c = *(++input);
    }
    return input;
}

char* SkipSpaces (char* input) {
    return (char*)SkipSpacesString (input);
}

const char* EndOfTokenString (const char* input) {
    if (input == nullptr)
        return nullptr;
    input = SkipSpacesString (input);

    char c = *input;
    while (c) {
        if (isspace (c))
            return input + 1;
        c = *(++input);
    }
    return input;
}

char* EndOfToken (char* input) {
    return (char*)EndOfTokenString (input);
}

const char* EndOfString (const char* input, char delimiter) {
    char c = *input;
    while (c) {
        if (c == delimiter)
            return input;
        c = *(++input);
    }
    return input;
}

char* EndOf (char* input, char delimiter) {
    return (char*)EndOfString (input, delimiter);
}

const char* CompareTokenString (const char* input, const char* token) {
    if (input == nullptr)
        return nullptr;
    if (token == nullptr)
        return nullptr;

    input = SkipSpacesString (input);
    token = SkipSpacesString (token);

    char a = *input,
        b = *token;
    if (b == 0) // Nothing to do.
        return nullptr;
    while (a && !isspace (a)) {
        //std::cout << "> a: " << a << " b: " << b << '\n';
        if (a != b) {
            return nullptr;
        }
        if (!b)
            return nullptr;
        a = *(++input);
        b = *(++token);
    }
    //std::cout << "> Query hit!\n";
    return input;
}

char* CompareToken (const char* input, const char* token) {
    return (char*)CompareTokenString (input, token);
}

char* ParseToken (char* input) {
    if (input == nullptr)
        return nullptr;

    input = SkipSpaces (input);

    char c = *input;
    while (c && !isspace (c)) {
        c = *(++input);
    }
    *input = 0;   //< Mark the null-term char.
    return input; //< This is the start of the next token or whitespace.
}

const char* CompareString (const char* input, const char* query, char delimiter) {
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    input = SkipSpacesString (input);
    query = SkipSpacesString (query);

    char s = *input,
        t = *query;
    if (s == 0)          // Nothing to do.
        return nullptr;
    if (t == 0)          // Nothing to do.
        return nullptr;
    if (t == delimiter) // Nothing to do.
        return nullptr;
    while (!isspace (s)) {
        if (s != t)
            return nullptr;
        if (t == delimiter) // Special case for null-term char.
            return nullptr;
        s = *(++input);
        t = *(++query);
    }
    return input; //< Query hit!
}

char* Compare (char* input, char* query, char delimiter) {
    return (char*)CompareString (input, query, delimiter);
}

const char* FindString (const char* input, const char* query,
                        char delimiter) {
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    char s = *input,    //< Current input char.
        t = *query,     //< Current query char.
        c = t;          //< The first char of the query we're searching for.
    if (c == delimiter) //< We're not allowing empty queries.
        return nullptr;
    const char* start_of_query,
        *cursor;

    query = SkipSpacesString (query);

    // Scroll through each char and match it to the query string.
    while (s) {
        if (s == c) // The first char matches:
        {
            // Setup to compare the strings;
            start_of_query = input;
            cursor = query;
            t = c;
            // check the rest of the string:
            while (s == t) {
                s = *(++input);
                t = *(++cursor);
                if (t == delimiter)        // Once we've reached the delimiter
                    return start_of_query;  // it's a match!
                if (!s) // We've reached the end of string without a hit.
                    return nullptr;
            }
        }
        // The string did not match so repeat the process for each char. 
        s = *(++input);
        t = *(++cursor);

    }
    // If we haven't found it by now it's not in the input.
    return nullptr;
}

char* Find (char* input, const char* query, char delimiter) {
    return (char*)FindString (input, query, delimiter);
}

const char* ParseString (const char* input, char* destination, int buffer_size,
                         char delimiter) {
    //std::cout << "> parse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n> ";
    if (input == nullptr) {
        std::cout << "| input == nullptr\n";
        return nullptr;
    }
    if (destination == nullptr) {
        std::cout << "| destination == nullptr\n";
        return nullptr;
    }
    if (buffer_size < 1) {
        std::cout << "| buffer_size < 1\n";
        return nullptr;
    }
    //std::cout << delimiter;
    char c = *input;       //< Current char from input.
    while (c) {
        if (c == delimiter) {
            //std::cout << delimiter << " parsed.\n";
            *destination = 0;
            if (delimiter == 0)
                return input;
            return input + 1;
        }
        //std::cout << c;
        if (buffer_size-- < 1) {
            std::cout << "\n| Buffer overflow!\n";
            return nullptr;
        }
        *destination++ = c;
        c = *(++input);
    }
    //std::cout << delimiter << " done.\n";
    *destination = 0;
    if (delimiter == 0)
        return input;
    return input + 1;
}

char* Parse (char* input, char* destination, int buffer_size, char delimiter) {
    return (char*)ParseString (input, destination, buffer_size, delimiter);
}

const char* ParseIntString (const char* input, int* value) {
    if (input == nullptr)
        return nullptr;
    if (value == nullptr)
        return nullptr;
    if (!sscanf (input, "%i", value))
        return nullptr;
    return EndOfTokenString (input);
}

char* ParseInt (char* input, int* value) {
    return (char*)ParseIntString (input, value);
}


const char* ParseFloatString (const char* input, float* value) {
    if (input == nullptr)
        return nullptr;
    if (value == nullptr)
        return nullptr;
    if (!sscanf (input, "%f", value))
        return nullptr;
    return EndOfTokenString (input);
}

char* ParseFloat (char* input, float* value) {
    return (char*)ParseFloatString (input, value);
}

int StringLength (const char* input, char delimiter) {
    if (input == nullptr)
        return -1;
    int length = 0;
    char c = *input;
    while (c) {
        if (c == delimiter)
            return length;
        ++length;
        c = *(++input);
    }
    return length;
}

bool IsToken (const char* input) {
    if (input == nullptr)
        return false;
    char c = *input;
    if (!isspace (c))
        return false;
    if (isdigit (c))
        return false;
    while (c) {
        if (isspace (c))
            return false;
        c = *(++input);
    }
    return true;
}

void ZeroTime (tm* date) {
    if (date == nullptr)
        return;
    date->tm_sec = 0;
    date->tm_min = 0;
    date->tm_hour = 0;
    date->tm_mday = 0;
    date->tm_mon = 0;
    date->tm_year = 0;
    date->tm_wday = 0;
    date->tm_yday = 0;
    date->tm_isdst = 0;
}

}       //< _pro

