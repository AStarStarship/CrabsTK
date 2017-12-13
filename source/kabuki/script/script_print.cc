/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_print.cc
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

#include "print.h"
#if SCRIPT_USING_PRINT

#include "text.h"
#include "strand.h"
#include "memory.h"

using namespace std;

namespace _ {

void PrintLine (char token, const char* left_column_text, int length) {
    cout << '\n' << left_column_text;
    for (int i = 0; i < 79; ++i)
        cout << token;
}

void PrintLine (const char* string) {
    PrintLine ();
    int length = StrandLength (string),
        i;

    for (i = 0; i < 80; i += (length + 1))
        cout << string << '_';
    for (int j = 0; j < 80 % (length + 1); ++j)
        cout << string[j];
    PrintLine ();
}

void PrintLineBreak (const char* message, int top_bottom_margin,
                     char c, int num_columns) {
    PrintLines (top_bottom_margin);
    cout << "\n| " << message;
    PrintLine (c, "\n|", num_columns);
}

void PrintLines (int num_rows) {
    cout << '\r';
    for (int i = 0; i < num_rows - 1; ++i)
        cout << '\n';
}

void PrintDebugHex (const char* message, char value) {
#if DEBUG
    printf ("\n| %s '%c':0x%x", message, value, value);
#endif
}

void PrintError (const char* message, const char* end_string) {
#if DEBUG
    cout << TextErrorHeader () << message << end_string;
#endif
}

void PrintDebugPointer (const char* message, const void* address) {
#if DEBUG
    printf ("\n| Error at address 0x%p: %s", address, message);
#endif
}

void PrintHex (byte c) {
    uint16_t chars = MemoryByteToUpperCaseHex (c);
    putchar ((char)chars);
    putchar ((char)(chars >> 8));
    putchar (' ');
}

void PrintChar (char c) {
    if ((c > 0 && c < ' ') || c == 127) {
        cout << 1;
        return;
    }
    cout << c;
}

char PrintCreateKeyValueFormatText (char* string, char column_width, char type) {
    char hundreds = (column_width / 100),
        decimal = (column_width % 10),
        tens = (column_width - hundreds - decimal) / 10;
    string[0] = '%';
    if (column_width <  10) {
        string[1] = decimal + '0';
        string[2] = 's';
        string[3] = ':';
        string[4] = ' ';
        string[5] = '0';
        string[6] = 'x';
        string[7] = '%';
        string[8] = 'p';
        string[9] = '\n';
        return 0;
    } else if (column_width < 100) {
        string[1] = tens + '0';
        string[2] = decimal + '0';
        string[3] = 's';
        string[4] = ':';
        string[5] = ' ';
        string[6] = '0';
        string[7] = 'x';
        string[8] = '%';
        string[9] = 'p';
        string[10] = '\n';
        return 0;
    }
    string[1] = hundreds + '0';
    string[2] = tens + '0';
    string[3] = decimal + '0';
    string[4] = 's';
    string[5] = ':';
    string[6] = ' ';
    string[7] = '0';
    string[8] = 'x';
    string[9] = '%';
    string[10] = 'p';
    string[11] = '\n';
    return 0;
}

void PrintNumberLine (int index) {
    cout << '\n';
    enum {
        kMaxBufferSize = (sizeof (int) == 2)
                       ? 7
                       : (sizeof (int) == 4)
                       ? 11
                       : 128
    };
    char buffer[kMaxBufferSize];
    TextWrite (buffer, buffer + kMaxBufferSize - 1, index);
    int length = StrandLength (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1))
        printf ("%s_", buffer);
    for (int j = 0; j < lettersLeft; ++j)
        cout << buffer[j];
    cout << '\n';
}

void PrintPause (const char* string) {
#if DEBUG
    cout << "\n" << string << "\n";
    system ("PAUSE");
#endif
}

void PrintNL () {
    cout << '\n';
}

void PrintBar (const char* input) {
    cout << "\n| " << input << '\n';
}

void PrintBreak (const char* header, char c, int num_lines,
                 int console_width) {
    for (int i = 0; i < num_lines; ++i)
        cout << '\n';
    cout << header;
    int length = StrandLength (header);
    for (int i = 0; i < console_width - length; ++i)
        cout << c;
    cout << '\n';
}

void PrintCentered (const char* string, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return;
    }
    // We need to leave at least one space to the left and right of
    int length = StrandLength (string);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                cout << '\n';
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i)
        cout << '\n';
    printf (string);
    for (offset = width - length - offset; offset <= width; ++offset)
        cout << '\n';
}

void PrintCentered (const char* input, int width, bool is_last,
                    char column_delimeter)
{
    if (width < 1)
        return;
    cout << column_delimeter;
    int length = StrandLength (input),
        num_spaces;

    if (width < length) {
        char format[32];
        format[0] = '%';
        TextWrite (&format[1], format + 32, (is_last ? width - 2 : width - 1));
        cout << column_delimeter;
        printf (format, input);
        if (is_last)
            cout << column_delimeter << '\n';
    }
    num_spaces = width - length - 1 - (is_last ? 1 : 0);
    for (int i = 0; i < num_spaces / 2; ++i)
        cout << ' ';
    cout << input;
    for (int i = 0; i < num_spaces - (num_spaces / 2); ++i)
        cout << ' ';
    if (is_last)
        cout << column_delimeter << '\n';
}

void PrintColumnBreak (int num_columns, char column_delimeter, char break_char,
                       int width) {
    int column_width = width / num_columns;
    for (int i = 0; i < num_columns - 1; ++i) {
        cout << column_delimeter;
        for (int j = 0; j < column_width - 1; ++j)
            cout << break_char;
    }
    cout << column_delimeter;
    for (int j = 0; j < column_width - 2; ++j)
        cout << break_char;
    cout << column_delimeter << '\n';
}

void PrintPageCentered (const char* input, int num_columns) {
    char c;   //< The current char.
    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (input == nullptr)
        return;
    do {
        const char* end_line = StrandLineEnd (input, num_columns);
        size_t row_length = end_line - input,
            num_left_spaces = (num_columns / 2) - (row_length / 2),
            i;
        // Print left whitespace.
        for (i = num_left_spaces; i != 0; --i)
            cout << ' ';
        // Print input.
        for (i = row_length; i != 0; --i)
            cout << *input++;
        for (i = num_columns - num_left_spaces - row_length; i != 0; --i)
            cout << ' ';
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
        while (isspace (c = *input++));
    } while (c);
}

void PrintPageRight (const char* input, int num_columns) {
    char c;  //< The current char.

    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (input == nullptr)
        return;
    do {
        const char* end_line = StrandLineEnd (input, num_columns);
        size_t row_length = end_line - input,
            num_left_spaces = num_columns - row_length,
            i;
        // Print left whitespace.
        for (i = num_left_spaces; i != 0; --i)
            cout << ' ';
        // Print input.
        for (i = row_length; i != 0; --i)
            cout << *input++;
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
        while (isspace (c = *input++));
    } while (c);
}

void PrintPage (const char* input, int indentation,
                       char bullet, int index, int tab_size,
                       int num_columns) {
    num_columns -= 4;
    cout << "\n| ";
    int cursor; //< The column number of the cursor.
    char c = *input++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || input == nullptr) { //< It's an empty input.
        for (int i = num_columns; i > 0; --i)
            cout << ' ';
        cout << "\n|\n";
        return;
    }

    // Make the input for the bullet.
    if (isdigit (bullet)) { // Then we have to print a number bullet.
        TextWrite (buffer, buffer + 15, index);
        //< + 2 for "\n| " - 2 for the bullet offset.
        //char format[16];
        //format[0] = '%';
        //sprintf_s (&format[1], 16, "%%%us", indentation * tab_size);
        //printf (format, buffer);
    } else if (isalpha (bullet)) { // Then it's a lower case bullet.
        cursor = 0;
        bullet += index % 26;  //<
        for (; index > 0; index -= 26)
            buffer[cursor++] = bullet;
        buffer[cursor] = '\0';
    } else {
        buffer[0] = bullet;
        buffer[1] = 0;
    }
    // Prints the char.
    int num_spaces = tab_size * indentation;
    while (c) {
        for (cursor = num_spaces; cursor > 0; --cursor)
            cout << ' ';
        cursor = num_spaces;
        while (c && (cursor++ < num_columns)) {
            cout << c;
            c = *input++;
        }
        cout << " |\n";
    }
    cout << '\n';
}

void PrintMemory (const void* address, const void* end) {
    printf ("\n|%i", 0);
    // Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    cout << '\n' << '|';
    for (int i = 0; i < 65; ++i)
        cout << '_';

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < end) {
        cout << '\n' << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            putchar (temp);
            ++chars;
        }
        printf ("| 0x%p", chars - 64);
    }
    cout << '\n' << '|';
    for (int i = 0; i < 64; ++i)
        cout << '_';
    printf ("| 0x%p\n", chars + Diff (address, end));
}
}       //< namespace _
#endif  //< SCRIPT_USING_PRINT
