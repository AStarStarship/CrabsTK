/** The Chinese Room
    @version 0.x
    @file    /.../Print.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
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

#include "Config.h"

#include <iostream>
#include <math.h>
#include <stdio.h>

namespace _ {

inline void printChar(char c)
/*< Prints the given char to the stdout if it is printable, and prints SOH. */
{
    if ((c > 0 && c < ' ') || c == 127)
    {
        putchar (1);
        return;
    }
    putchar (c);
}

inline void printLine(char token = '-', int columnWidth = 80)
/*< Prints a line with the given token and number of columns to the debug
    stream. */
{
    putchar ('\n');
    for (int i = 0; i < lineWidth; ++i)
        putchar (token);
    putchar ('\n');
}

inline void printLines(int numRows = 10)
/*< Prints a vertical tab with the given number of rows. */
{
    putchar ('\r');
    for (int i = 0; i < numRows - 1; ++i)
        putchar ('\n');
}

inline void printMemory(void* address, size_t size)
/*< Prints out the contents of the adress to the debug stream. */
{
    printf ("|%i", 0);
    //! Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    putchar ('\n');
    putchar ('|');
    for (int i = 0; i < 65; ++i)
        putchar ('_');
    putchar ('\n');

    const char* chars = reinterpret_cast<const char*>(address);
    char* end = reinterpret_cast<char*>(address) + size;
    char temp;
    while (chars < end)
    {
        putchar ('|');
        for (int i = 0; i < 64; ++i)
        {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            printChar (temp);
            ++chars;
        }
        printf ("| 0x%p\n", reinterpret_cast<uintptr_t>(chars) - 64);
    }
    putchar ('|');
    for (int i = 0; i < 64; ++i)
        putchar ('_');
    printf ("| 0x%p", reinterpret_cast<uintptr_t>(chars) + size);
    putchar ('\n');
}

template<typename Type>
void printHex(Type value)
/*< */
{
    char buffer[sizeof (Type) * 2 + 1];
    sprintf_s (buffer, "%p", value);
    printf ("0x");

    int length = strlen (buffer);
    for (int i = 0; i < length; ++i)
        putchar ('0');
    std::cout << buffer;
}

inline char createKeyValueFormatString(char* s, byte columnWidth, char type)
/*< Creates the format string for a key-value pair where the key is right-aligned to the columnWidth. */
{
    char hundreds = (columnWidth / 100),
        decimal = (columnWidth % 10),
        tens = (columnWidth - hundreds - decimal) / 10;
    s[0] = '%';
    if (columnWidth <  10)
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
    else if (columnWidth < 100)
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

template<byte ColumnWidth>
void printHex(const char* header, void* value)
/*< */
{
    static char formatString[12],
        nullTermChar = createKeyValueFormatString (formatString, ColumnWidth, 'p');
    printf (formatString, header, value);
}

template<byte ColumnWidth, typename Type>
void printSignedHex(const char* header, Type value)
/*< */
{
    static char formatString[12],
        nullTermChar = createKeyValueFormatString (formatString, ColumnWidth, 'i');
    printf ("formatString: %s\n", formatString);
    printf (formatString, header, value);
}

template<byte ColumnWidth, typename Type>
void printUnsignedHex(const char* header, Type value)
/*< */
{
    static char formatString[12],
        nullTermChar = createKeyValueFormatString (formatString, ColumnWidth, 'u');
    printf (formatString, header, value);
}

inline void printNumberLine(int index)
/*< Prints an 80-char line of the number repeating with an underscore i.e. 1_1_... */
{
    putchar ('\n');
    enum { MaxBufferSize = (sizeof (int) == 2) ? 7 : (sizeof (int) == 4) ? 11 : 128 };
    char buffer[MaxBufferSize];
    sprintf (buffer, "%u", index);
    int length = strlen (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1))  printf ("%s_", buffer);
    for (int j = 0; j < lettersLeft; ++j)   putchar (buffer[j]);
    putchar ('\n');
}

inline void printStringLine(const char* s)
/*< Prints an 80-char line of the string repeating with an underscore i.e. s_s_... */
{
    printLine ();
    int length = strlen (s),
        i;

    for (i = 0; i < 80; i += (length + 1))  printf ("%s_", s);
    for (int j = 0; j < 80 % (length + 1); ++j)   putchar (s[j]);
    printLine ();
}

template<typename Type, const char* format>
Type printReturn(Type value)
/*< Prints the given valud to the console and prompts the user to press any key to continue. */
{
    printf (format, value);
    return value;
}

template<typename Type>
void printArray(const char* header, const char* format, Type* base_ptr, 
    Type numElements)
/*< Prints the array starting at the base_ptr with the given numElements with a
    header and given format. */
{
    printLine ();
    printf (header);
    printf (": numElements: ");
    printf ("%u\n", numElements);
    for (Type i = 0; i < numElements; ++i)
    {
        printf (format, i);
        printf (": ");
        printf (format, base_ptr[i]);
        putchar ('\n');
    }
    putchar ('\n');
    printLine ();
}

inline void printPause (const char* s)
/*< Prints an error message and pauses the system. */
{
    #if DEBUG
    printf ("\n%s\n", s);
    system ("PAUSE");
    #endif
}

inline void printNL ()
/*< Prints a new line. */
{
    putchar ('\n');
}

}       //< namespace _

#endif  //< CHINESEROOM_PRINT_H
