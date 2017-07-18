/** The Chinese Room
    @version 0.x
    @file    /.../Print.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_PRINT_H
#define CHINESEROOM_PRINT_H

#include "config.h"

#if DEBUG

namespace _ {

/** Prints the given string center in the given width.
    If string is too small to fit in the width, function will print as much of
    the string as it has room for with a "..." If the given width is less than
    5, than
*/
inline void PrintCentered (const char* s, int width) {
    if (width < 2)
    {
        //? Not sure if this is an error.
        return;
    }
    // We need to leave at least one space to the left and right of 
    int length = strlen (s);
    if (length < width - 2)
    {
        // We need to write the ....
        if (length < 4)
        {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length)
            {
                std::cout << '\n';
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i)
        std::cout << '\n';
    printf (s);
    for (offset = width - length - offset; offset <= width; ++offset)
        std::cout << '\n';
}

/** Prints the given char to the stdout if it is printable, and prints SOH. */
static void PrintChar (char c) {
    if ((c > 0 && c < ' ') || c == 127)
    {
        putchar (1);
        return;
    }
    putchar (c);
}

/** Prints a line with the given token and number of columns to the debug
    stream. */
static void PrintLine (char token = '-', int column_width = 80) {
    std::cout << '\n';
    for (int i = 0; i < column_width; ++i)
        putchar (token);
    std::cout << '\n';
}

/** Prints a vertical tab with the given number of rows. */
static void PrintLines (int numRows = 10) {
    putchar ('\r');
    for (int i = 0; i < numRows - 1; ++i)
        std::cout << '\n';
}

/** Prints out the contents of the address to the debug stream. */
static void PrintMemory (void* address, size_t size) {
    printf ("|%i", 0);
    // Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    std::cout << '\n';
    putchar ('|');
    for (int i = 0; i < 65; ++i)
        putchar ('_');
    std::cout << '\n';

    const char* chars = reinterpret_cast<const char*> (address);
    char* end = reinterpret_cast<char*> (address) + size;
    char temp;
    while (chars < end)
    {
        putchar ('|');
        for (int i = 0; i < 64; ++i)
        {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            PrintChar (temp);
            ++chars;
        }
        printf ("| 0x%x\n", reinterpret_cast<uintptr_t> (chars) - 64);
    }
    putchar ('|');
    for (int i = 0; i < 64; ++i)
        putchar ('_');
    printf ("| 0x%x\n", reinterpret_cast<uintptr_t> (chars) + size);
}

/** */
template<typename Type>
void PrintHex (Type value) {
    char buffer[sizeof (Type) * 2 + 1];
    sprintf_s (buffer, "%x", &value);
    printf ("0x");

    int length = strlen (buffer);
    for (int i = 0; i < length; ++i)
        putchar ('0');
    std::cout << buffer;
}

/** Creates the format string for a key-value pair where the key is right-aligned to the column_width. */
inline char CreateKeyValueFormatString (char* s, char column_width, char type) {
    char hundreds = (column_width / 100),
        decimal = (column_width % 10),
        tens = (column_width - hundreds - decimal) / 10;
    s[0] = '%';
    if (column_width <  10)
    {
        s[1] = decimal + '0';
        s[2] = 's';
        s[3] = ':';
        s[4] = ' ';
        s[5] = '0';
        s[6] = 'x';
        s[7] = '%';
        s[8] = 'p';
        s[9] = '\n';
        return 0;
    }
    else if (column_width < 100)
    {
        s[1] = tens + '0';
        s[2] = decimal + '0';
        s[3] = 's';
        s[4] = ':';
        s[5] = ' ';
        s[6] = '0';
        s[7] = 'x';
        s[8] = '%';
        s[9] = 'p';
        s[10] = '\n';
        return 0;
    }
    s[1] = hundreds + '0';
    s[2] = tens + '0';
    s[3] = decimal + '0';
    s[4] = 's';
    s[5] = ':';
    s[6] = ' ';
    s[7] = '0';
    s[8] = 'x';
    s[9] = '%';
    s[10] = 'p';
    s[11] = '\n';
    return 0;
}

/** */
template<char ColumnWidth>
void PrintHex (const char* header, void* value) {
    static char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, ColumnWidth, 'p');
    printf (formatString, header, value);
}

/** */
template<char ColumnWidth, typename Type>
void PrintSignedHex (const char* header, Type value) {
    static char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, ColumnWidth, 'i');
    printf ("formatString: %s\n", formatString);
    printf (formatString, header, value);
}

/** */
template<char ColumnWidth, typename Type>
void PrintUnsignedHex (const char* header, Type value) {
    static char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, ColumnWidth, 'u');
    printf (formatString, header, value);
}

/** Prints an 80-char line of the number repeating with an underscore i.e. 1_1_... */
static void PrintNumberLine (int index) {
    std::cout << '\n';
    enum { MaxBufferSize = (sizeof (int) == 2) ? 7 : (sizeof (int) == 4) ? 11 : 128 };
    char buffer[MaxBufferSize];
    sprintf_s (buffer, MaxBufferSize, "%u", index);
    int length = strlen (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1))
        printf ("%s_", buffer);
    for (int j = 0; j < lettersLeft; ++j)
        putchar (buffer[j]);
    std::cout << '\n';
}

/** Prints an 80-char line of the string repeating with an underscore i.e. s_s_... */
static void PrintStringLine (const char* s) {
    PrintLine ();
    int length = strlen (s),
        i;

    for (i = 0; i < 80; i += (length + 1))
        printf ("%s_", s);
    for (int j = 0; j < 80 % (length + 1); ++j)
        putchar (s[j]);
    PrintLine ();
}

/** Prints the given value to the console and prompts the user to press any key to continue. */
template<typename Type, const char* format>
Type PrintReturn (Type value) {
    printf (format, value);
    return value;
}

/** Prints the array starting at the base_ptr with the given numElements with a
    header and given format. */
template<typename Type>
void PrintArray (const char* header, const char* format, Type* base_ptr, 
    Type numElements) {
    PrintLine ();
    std::cout << header << ": numElements: " << numElements << '\n';
    for (Type i = 0; i < numElements; ++i)
    {
        printf (format, i);
        std::cout << ": ";
        printf (format, base_ptr[i]);
        std::cout << '\n';
    }
    std::cout << '\n';
    PrintLine ();
}

/** Prints an error message and pauses the system. */
inline void PrintPause (const char* s) {
    #if DEBUG
    std::cout << "\n%s\n" << s;
    //system ("PAUSE");
    #endif
}

/** Prints a new line. */
inline void PrintNL () {
    std::cout << '\n';
}

inline void PrintLineBreak (const char* message, int top_bottom_margin, 
                            char c = '-', int num_columns = 80) {
    PrintLines (top_bottom_margin);
    std::cout << message;
    PrintLine  (c, num_columns);
}

}       //< namespace _
#endif  //< DEBUG
#endif  //< CHINESEROOM_PRINT_H
