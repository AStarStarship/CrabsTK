/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_text.cc
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

#include "text.h"
#include "memory.h"

using namespace std;

namespace _ {

const char* EmptyString () {
    return "";
}

void StringCopy (char* destination, const char* source) {
    if (destination == nullptr)
        return;
    if (source == nullptr)
        return;
    char c = *source;
    ++source;
    while (c) {
        *destination = c;
        ++destination;
        c = *source;
        ++source;
    }
    *destination = 0;
}

char* StringConcat (const char* string, int integer) {
    enum {
        kBufferSize = 12
    };
    if (string == nullptr) {
        return nullptr;
    }
    int  length = StringLength (string);
    char* result = new char[length + kBufferSize];
    StringCopy (result, string);
    TextWrite (result + length, result + length + kBufferSize - 1, integer);
    return result;
}

const char* StringEquals (const char* input, const char* query) {
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;
    //cout << "Comparing strings equal \"" << input << "\" to \"" << query
    //    << "\"\n";

    char i = *input,
        q = *query;
    while (i) {
        //cout << i;
        if (i != q) { // Not a hit.
            //cout << "; but it's not a hit\n";
            return nullptr;
        }
        if (q == 0) { // Hit!
            //cout << "; found hit at ";
            //PrintPointerNL (input);
            return input;
        }
        i = *(++input);
        q = *(++query);
    }
    if (q != 0) {
        //cout << "; Not a hit: no nil-term char found\n";
        return nullptr;
    }
    //cout << "; found hit at ";
    //PrintPointerNL (input);
    return input; //< Find hit!
}

const char* StringEquals (const char* input, const char* query,
                          char delimiter) {
    cout << "Comparing strings \"" << input << "\" to \"" << query
        << "\"\n";
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    //input = SkipSpacesString (input); //< I think this is token compare
    //query = SkipSpacesString (query);

    char i = *input,
        q = *query;
    while (i != delimiter) {
        if (i != q) { // Not a hit.
            cout << "\n| not a hit";
            return nullptr;
        }
        cout << i;
        i = *(++input);
        q = *(++query);
    }
    if (q != delimiter) {
        cout << "\n| reached nil-term char but no q:\'" << q
            << "\' is not the delimiter.";
        return nullptr;
    }
    cout << "; found hit at ";
    PrintPointerNL (input);
    return input; //< Find hit!
}

int StringLength (const char* string, char delimiter) {
    if (string == nullptr)
        return -1;
    int count = 0;
    //cout << "Checking string length for " << string << '\n';
    char c = *string;
    //cout << c;
    while (c) {
        ++count;
        c = *(++string);
        //cout << c;
    }
    //cout << '\n';
    return count;
}

int StringWrite (char* buffer_start, char* buffer_stop, int integer) {
    return 0;
}

hash16_t Hash16 (const char* string, hash16_t hash) {
    byte c = *string;
    while (c) {
        hash = Hash16 (c, hash);
        ++string;
        c = *string;
    }
    return hash;
}

hash16_t Hash32 (const char* string, hash32_t hash) {
    byte c = *string;
    while (c) {
        hash = Hash32 (c, hash);
        ++string;
        c = *string;
    }
    return hash;
}

hash64_t Hash64 (const char* string, hash64_t hash) {
    byte c = *string;
    while (c) {
        hash = Hash64 (c, hash);
        ++string;
        c = *string;
    }
    return hash;
}

#if USE_MORE_ROM

void PrintLine (char token, int column_width) {
    cout << '\n';
    for (int i = 0; i < column_width; ++i)
        cout << token;
}

void PrintLine (const char* start_string, char token, int length) {
    cout << '\n' << start_string;
    for (int i = 0; i < 79; ++i)
        cout << token;
}

void PrintDebug (const char* message,
                 const char* end_string,
                 const char* begin_string) {
#if DEBUG
    cout << begin_string << message << end_string;
#endif
}

void PrintLineBreak (const char* message, int top_bottom_margin,
                            char c, int num_columns) {
    PrintLines (top_bottom_margin);
    cout << message;
    PrintLine (c, num_columns);
}

void PrintLines (int num_rows) {
    cout << '\r';
    for (int i = 0; i < num_rows - 1; ++i)
        cout << '\n';
}

void PrintStringLine (const char* string) {
    PrintLine ();
    int length = StringLength (string),
        i;

    for (i = 0; i < 80; i += (length + 1))
        cout << string << '_';
    for (int j = 0; j < 80 % (length + 1); ++j)
        cout << string[j];
    PrintLine ();
}

void PrintDebugHex (const char* message, char value) {
#if DEBUG
    printf ("\n| %s '%c':0x%x", message, value, value);
#endif
}

char* TextWrite (char* buffer, char* buffer_end, int value) {
    // Stolen from https://goo.gl/waaF1G
    // @todo Convert to pointer arithmetic.

    if (buffer == nullptr) {
        std::cout << "\n| null buffer!";
        return 0;
    }

    if (buffer_end == nullptr) {
        std::cout << "\n| null buffer_end!";
        return 0;
    }
    if (buffer_end <= buffer) {
        // We have to have room for at least a zero.
        std::cout << "\n| buffer overflow!!";
        return 0;
    }

    int  n = (sizeof (int) == 2) ? -32767 : -2147483647,
         i = 0;
    bool is_negative = n < 0;
    int digit = is_negative ? -n : n;
    char* buffer_t,
        * buffer_i_minus_t_minus_1;

    while(digit != 0) {
        buffer[i++] = digit % 10 + '0';
        digit = digit / 10;
    }

    if(is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    for(int t = 0; t < i / 2; t++) {
        buffer_i_minus_t_minus_1 = &buffer[i - t - 1],
        buffer_t = &buffer[t];

        *buffer_t ^= *buffer_i_minus_t_minus_1;
        *buffer_i_minus_t_minus_1 ^= *buffer_t;
        *buffer_t ^= buffer[i-t-1];
        //buffer[t    ] ^= *buffer[i-t-1];
        //buffer[i-t-1] ^= buffer[t];
        //buffer[t    ] ^= buffer[i-t-1];
    }

    if(n == 0) {
        *buffer = '0';
        *(buffer + 1) = '\0';
    }
    return buffer + i;
}

char* TextWrite (char* buffer, char* buffer_end, uint value) {
    // Stolen from https://goo.gl/waaF1G

    if (buffer == nullptr) {
        std::cout << "\n| buffer was null!";
        return 0;
    }

    if (buffer_end == nullptr) {
        std::cout << "\n| buffer_end was null!";
        return 0;
    }
    if (buffer_end <= buffer) {
        // We have to have room for at least a zero.
        std::cout << "\n| buffer overflow!!";
        return 0;
    }

    int  n = (sizeof (int) == 2) ? 32767 : 2147483647,
         i = 0;
    int digit = n;
    char* buffer_t,
        * buffer_i_minus_t_minus_1;

    while(digit != 0) {
        buffer[i++] = digit % 10 + '0';
        digit = digit / 10;
    }

    buffer[i] = '\0';

    for(int t = 0; t < i / 2; t++) {
        buffer_i_minus_t_minus_1 = &buffer[i - t - 1],
        buffer_t = &buffer[t];

        *buffer_t ^= *buffer_i_minus_t_minus_1;
        *buffer_i_minus_t_minus_1 ^= *buffer_t;
        *buffer_t ^= buffer[i-t-1];
        //buffer[t    ] ^= *buffer[i-t-1];
        //buffer[i-t-1] ^= buffer[t];
        //buffer[t    ] ^= buffer[i-t-1];
    }

    if(n == 0) {
        *buffer = '0';
        *(buffer + 1) = '\0';
    }
    return buffer + i;
}

const char* TextRead (const char* buffer, int& result) {
    // Algorithm:
    // 1. Check for sign char and store as variable named sign.
    // 2. Count length of int string and handle error in case of overflow.
    // 3. Get pointer to last char.
    // 4. Set variable named value to the first char and multiplier to 1.
    // 4. Word backwards in a for loop from end of int string to calculate each
    //    decimal place by multiplying (c - '0') times a multiplier times 10
    //    for each loop.
    // 5. Return sign times the resulting value above.
    enum {
        kMaxLetters = sizeof (int) == 32 ? 10 : 6, //< int can be 16 or 32-bit.
    };
    if (buffer == nullptr) {
        std::cout << "\n| Buffer was null!";
        return 0;
    }
    char c = *buffer++;
    int length,
        sign = 1,
        value,
        multiplier;
    if (c == '-') {
        sign = -1;
        c = *buffer++;
    }
    if (c < '0') {    // First char must be a number!.
        return nullptr;
    }
    if (c > '9') {    // First char must be a number!.
        return nullptr;
    }
    for (length = 1; length < kMaxLetters; ++length) {
        if (c < '0')
            break;
        if (c > '9')
            break;
        c = *buffer++;
    }
    if (length > kMaxLetters) { // int overflow!
        return 0;
    }
    value = c - '0';
    multiplier = 1;
    for (int i = 0; i < length; ++i) {
        multiplier *= 10;
        c = *(buffer - i);
        value += ((c - '0') * multiplier);
    }
    result = value * sign;
    return buffer + 1;
}

const char* TextRead (const char* buffer, float& result) {
    return nullptr;
}

const char* NewLineString () {
    return "\n";
}

const char* ErrorHeader () {
    return "\n| Error: ";
}

const char* VerticalBar () {
    return "\n| ";
}

void PrintLine (const char* string) {
    cout << '\n' << string;
}

void PrintError (const char* message, const char* end_string) {
#if DEBUG
    cout << ErrorHeader () << message << end_string;
#endif
}

void PrintDebugPointer (const char* message, const void* address) {
#if DEBUG
    printf ("\n| Error at address 0x%p: %s", address, message);
#endif
}

char* DuplicateString (const char* string, char delimiter) {
    int length = StringLength (string);
    if (length < 0) {
        cout << "Error duplicating char.";
        return nullptr;
    }
    char* buffer = new char[length + 1];
    // (buffer, length + 1, string, length + 1);
    MemoryCopy (buffer, length + 1, string);
    return buffer;
}

void DestroyDuplicate (const byte* string) {
    if (string == nullptr) return;
}

byte NibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    if (b > 9)
        return b + ('a' - 10);
    return b + '0';
}

byte NibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b > 9) return b + ('A' - 10);
    return b + '0';
}

uint16_t ToLowerCaseHex (byte b) {
    uint16_t value = NibbleToLowerCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToLowerCaseHex (b >> 4);
    return value;
}

uint16_t ToUpperCaseHex (byte b) {
    uint16_t value = NibbleToUpperCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToUpperCaseHex (b >> 4);
    return value;
}

int ToByte (byte c) {
    //printf ("toByte (byte c): %c ", c);
    if (c < '0') return -1;
    if (c >= 'a') {
        if (c > 'f') return -1;
        //printf ("output!: %i \n", c - ('a' - 10));
        return c - ('a' - 10);
    }
    if (c >= 'A') {
        if (c > 'F') return -1;
        return c - ('A' - 10);
    }
    if (c > '9') return -1;
    //printf ("output: %i \n", c - ('a' - 10));
    return c - '0';
}

int ToByte (uint16_t h) {
    // Note: This works but the upper and lower are switched... pay no
    // attention to the man behind the curtain #HandWave
    //printf ("toByte (uint16_t c): %c%c\n", (byte) (h << 8), (byte)h);
    int lowerValue = ToByte (static_cast<byte> (h >> 8));
    //printf (" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = ToByte (static_cast<byte> (h));
    if (upperValue < 0) return -1;
    //printf (" upperValue: %i \nlowerValue | (upperValue << 4): %i\n",
    //    upperValue, lowerValue | (upperValue << 4));

    //PrintLine ();
    return lowerValue | (upperValue << 4);
}

hash16_t Hash16UI2 (uint16_t value, hash16_t hash) {
    hash = ((value & 0xff) * kLargest16BitPrime) + hash;
    hash = ((value >> 8  ) * kLargest16BitPrime) + hash;
    return hash;
}

hash16_t Hash16UI4 (uint32_t value, hash16_t hash) {
    hash = ((value & 0xff        ) * kLargest16BitPrime) + hash;
    hash = (((value >> 8 ) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 16) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 24) & 0xff) * kLargest16BitPrime) + hash;
    return hash;
}

hash16_t Hash16UI8 (uint64_t value, hash16_t hash) {
    hash = ((value & 0xff        ) * kLargest16BitPrime) + hash;
    hash = (((value >> 8 ) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 16) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 24) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 32) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 40) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 48) & 0xff) * kLargest16BitPrime) + hash;
    hash = (((value >> 56) & 0xff) * kLargest16BitPrime) + hash;
    return hash;
}

void PrintHex (byte c) {
    uint16_t chars = ToUpperCaseHex (c);
    putchar ((char)chars);
    putchar ((char)(chars >> 8));
    putchar (' ');
}

void PrintCentered (const char* string, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return;
    }
    // We need to leave at least one space to the left and right of
    int length = StringLength (string);
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

void PrintChar (char c) {
    if ((c > 0 && c < ' ') || c == 127) {
        cout << 1;
        return;
    }
    cout << c;
}

char CreateKeyValueFormatString (char* string, char column_width, char type) {
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
    int length = StringLength (buffer),
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

void StringCopy (char* destination, const char* source, char delimeter) {
    if (destination == nullptr)
        return;
    if (source == nullptr)
        return;
    char c = *source;
    ++source;
    while (c != delimeter) {
        *destination = c;
        ++destination;
        c = *source;
        ++source;
    }
    *destination = 0;
}

void StringCopy (char* destination, char* buffer_end, const char* source) {
    if (destination == nullptr)
        return;
    if (buffer_end == nullptr)
        return;
    if (destination > buffer_end)
        return;
    if (source == nullptr)
        return;
    char c = *source;
    ++source;
    while (c) {
        *destination = c;
        if (++destination > buffer_end) {
            //cout << "\n| Buffer overflow!";
            return;
        }
        c = *source;
        ++source;
    }
    *destination = 0;
}

void StringCopy (char* destination, char* buffer_end, const char* source,
                 char delimeter) {
    if (destination == nullptr)
        return;
    if (buffer_end == nullptr)
        return;
    if (destination > buffer_end)
        return;
    if (source == nullptr)
        return;
    char c = *source;
    ++source;
    while (c != delimeter) {
        *destination = c;
        if (++destination > buffer_end) {
            //cout << "\n| Buffer overflow!";
            return;
        }
        c = *source;
        ++source;
    }
    *destination = 0;
}

char* StringClone (const char* input, char delimeter) {
    if (input == nullptr)
        input = "";
    int length = StringLength (input);
    char* clone = new char[length + 1];
    StringCopy (clone, input);
    return clone;
}

void PrintBar (const char* input) {
    cout << "\n| " << input << '\n';
}

void PrintBreak (const char* header, char c, int num_lines,
                 int console_width) {
    for (int i = 0; i < num_lines; ++i)
        cout << '\n';
    cout << header;
    int length = StringLength (header);
    for (int i = 0; i < console_width - length; ++i)
        cout << c;
    cout << '\n';
}

void PrintCentered (const char* input, int width, bool is_last,
                    char column_delimeter)
{
    if (width < 1)
        return;
    cout << column_delimeter;
    int length = StringLength (input),
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

const char* FindEndOfRow (const char* input, int num_columns) {
    char c;
    // Scroll to the end of the line.
    c = *input++;
    while (c) {
        if (num_columns-- < 0) { // We've reached the end.
            // Scroll left till we hit whitespace (if any).
            while (!isspace (c))
                c = *(--input);
            // Then scroll past the whitespace.
            while (isspace (c))
                c = *(--input);
            return input;
            c = *input++;
        }
        c = *input++;
    }
    return --input;
}

void PrintPageCentered (const char* input, int num_columns) {
    char c;   //< The current char.
    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (input == nullptr)
        return;
    do {
        const char* end_row = FindEndOfRow (input, num_columns);
        size_t row_length = end_row - input,
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
        const char* end_row = FindEndOfRow (input, num_columns);
        size_t row_length = end_row - input,
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

const char* NextNonNumberString (const char* input) {
    if (input == nullptr)
        return nullptr;
    char c = *input;
    if (c == '-') {  // It might be negative.
        c = *(++input);
        if (!isdigit (c)) {  // it's not negative.
            return input - 1;
        }
        c = *(++input);
    }
    while (isdigit (c)) {
        c = *(++input);
    }
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
    if (c != '0') {
        return input;
    }
    d = *input;
    // We know the first char is a '0' so just loop till c and d aren't '0'.
    while (c == d) {
        c = d;
        d = *(++input);
    }
    if (!isdigit (d))       //< Special case for "...00" char.
        return input - 1;
    return input;
}

char* SkipLeadingZeros (char* input) {
    return (char*)SkipLeadingZerosString (input);
}

const char* SkipSpacesString (const char* input) {
    if (input == nullptr)
        return nullptr;
#if SCRIPT_DEBUG
    cout << "\nSkipping spaces: ";
#endif    //< SCRIPT_DEBUG
    char c = *input;
    while (c) {
#if SCRIPT_DEBUG
        cout << '.';
#endif    //< SCRIPT_DEBUG
        if (!isspace (c))
            return input;
        ++input;
        c = *input;
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
        if (c == delimiter) {
            return input;
        }
        c = *(++input);
    }
    return input;
}

char* EndOf (char* input, char delimiter) {
    return (char*)EndOfString (input, delimiter);
}

const char* CompareTokenString (const char* input, const char* token) {
    if (input == nullptr) {
        return nullptr;
    }
    if (token == nullptr) {
        return nullptr;
    }

    input = SkipSpacesString (input);
    token = SkipSpacesString (token);

    char a = *input,
        b = *token;
    if (b == 0) { // Nothing to do.
        return nullptr;
    }
    while (a && !isspace (a)) {
        //cout << "> a: " << a << " b: " << b << '\n';
        if (a != b) {
            return nullptr;
        }
        if (!b) {
            return nullptr;
        }
        a = *(++input);
        b = *(++token);
    }
    //cout << "> Find hit!\n";
    return input;
}

char* CompareToken (const char* input, const char* query) {
    return (char*)CompareTokenString (input, query);
}

char* StringCompare (char* source, const char* query, char delimiter) {
    return (char*)StringEquals (source, query, delimiter);
}

const char* ParseString (const char* input, char* destination,
                         char* buffer_end, char delimiter) {
    //cout << "> parse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n| ";
    if (input == nullptr) {
        cout << "\n| input == nullptr";
        return nullptr;
    }
    if (destination == nullptr) {
        cout << "\n| destination == nullptr";
        return nullptr;
    }
    if (destination > buffer_end) {
        cout << "\n| destination > buffer_end";
        return nullptr;
    }
    //cout << delimiter;
    char c = *input; //< Current char from input.
    while (c) {
        if (c == delimiter) {
            //cout << delimiter << " parsed.\n";
            *destination = 0;
            if (delimiter == 0) {
                return input;
            }
            return input + 1;
        }
        //cout << c;
        if (input > buffer_end) {
            cout << "\n| Buffer overflow!";
            return nullptr;
        }
        *destination++ = c;
        c = *(++input);
    }
    //cout << delimiter << " done.\n";
    *destination = 0;
    if (delimiter == 0) {
        return input;
    }
    return input + 1;
}

char* Parse (char* input, char* destination, char* buffer_end, char delimiter) {
    return (char*)ParseString (input, destination, buffer_end, delimiter);
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

const char* FindString (const char* input, const char* query, char delimiter) {
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    char string = *input, //< Current input char.
         t      = *query, //< Current query char.
         c      = t;      //< The first char of the query we're searching for.
    if (c == delimiter)   //< We're not allowing empty queries.
        return nullptr;
    const char* start_of_query,
              * cursor = input;
    query = SkipSpacesString (query);

    // Scroll through each char and match it to the query char.
    while (string) {
        if (string == c) { // The first char matches:
            // Setup to compare the strings;
            start_of_query = input;
            cursor = query;
            t = c;
            // check the rest of the char:
            while (string == t) {
                string = *(++input);
                t = *(++cursor);
                if (t == delimiter) {      // Once we've reached the delimiter
                    return start_of_query;  // it's a match!
                }
                if (!string) { // We've reached the end of char without a hit.
                    return nullptr;
                }
            }
        }
        // The char did not match so repeat the process for each char.
        string = *(++input);
        t = *(++cursor);

    }
    // If we haven't found it by now it's not in the input.
    return nullptr;
}

char* Find(char* input, const char* query, char delimiter) {
    return (char*)FindString (input, query, delimiter);
}

const char* ParseIntString (const char* input, int* value) {
    return nullptr;
}

char* ParseInt (char* input, int* value) {
    return nullptr;
}

enum {
    kMaxDigitsFloat = DBL_MAX_10_EXP + 2
};

const char* ParseFloatString (const char* input, float* value) {
    return nullptr;
}

char* ParseFloat (char* input, float* value) {
    return nullptr;
}

bool IsToken (const char* input) {
    if (input == nullptr) {
        return false;
    }
    char c = *input;
    if (!isspace (c)) {
        return false;
    }
    if (isdigit (c)) {
        return false;
    }
    while (c) {
        if (isspace (c)) {
            return false;
        }
        c = *(++input);
    }
    return true;
}

int RoundToPowerOf2 (int value) {
    if (value < 0) {
        return 4;
    }
    // @cite https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    unsigned int v = (unsigned int)value;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return (int)value;
}

char KeyboardChar (const char* header) {
    std::cout << header;
    char c = getchar ();
    std::cout << '\n';
    return c;
}

int KeyboardInt (const char* header) {
    int number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

float KeyboardFloat (const char* header) {
    float number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

char* KeyboardString (const char* header, int buffer_size) {
    if (buffer_size < 0) {
        return nullptr;
    }
    char* buffer = new char[buffer_size];
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');

    return buffer;
}

void KeyboardString (const char* header, char* buffer, int buffer_size) {
    if (buffer == nullptr) {
        return;
    }
    if (buffer_size < 0) {
        return;
    }
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');
}
#endif  //< MEMORY_PROFILE > USE_MORE_ROM
}       //< namespace _
