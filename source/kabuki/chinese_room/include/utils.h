/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/utils.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_UTILS_H
#define CHINESE_ROOM_UTILS_H

#include "config.h"

namespace _ {

static const char ErrorHeader[] = "| Error: \0",
                  VerticalBar[] = "| \0";

KABUKI void PrintLine (const char* s) {
    std::cout << '\n' << s << '\n';
}

KABUKI void PrintDebug (const char* message, 
                        const char* end_string   = NewLineString,
                        const char* begin_string = VerticalBar) {
#if DEBUG
    std::cout << begin_string << message << end_string;
#endif
}

KABUKI void PrintError (const char* message,
                             const char* end_string = NewLineString) {
#if DEBUG
    std::cout << ErrorHeader << message << end_string;
#endif
}

KABUKI void PrintDebugPointer (const char* message, const void* address) {
#if DEBUG
    printf ("| Error at address 0x%p: %s\n", address, message);
#endif
}

KABUKI void PrintDebugHex (const char* message, char value) {
#if DEBUG
    printf ("| %s '%c':0x%x\n", message, value, value);
#endif
}

template<typename T>
KABUKI void PrintDebugSignedHex (const char* message, T value) {
#if DEBUG
    printf ("| %s:'%i':0x%x\n", message, value, value);
#endif
}

template<typename T>
KABUKI void PrintDebugUnsignedHex (const char* message, T value) {
#if DEBUG
    printf ("%s:'%u':0x%x\n", message, value, value);
#endif
}

template<typename T>
KABUKI void PrintDebugError (const char* message, T expected, T found) {
#if DEBUG
    std::out << ErrorHeader << "expecting " << expected << " and found " 
             << found << " attempting: " << message;
#endif
}

/** Gets the length of the given string.
    @return Returns -1 if the input string is null.
*/
KABUKI int StringLength (const char* s, char delimiter = 0) {
    if (s == nullptr)
        return -1;
    int count = 0;

    char c; c = *s;
    while (c)
        c = *(++s);
    return count;
}

/** Creates a duplicate copy of the given string using dynamic memory. */
KABUKI const char* DuplicateString (const char* s, char delimiter = 0) {
    int length = StringLength (s);
    if (length < 0)
        s = "Error duplicating string.";
    char* buffer = new char[length + 1];
    memcpy_s (buffer, length + 1, s, length + 1);
    return buffer;
}

/** Destroys the duplicate string created by const byte* duplicate
(const byte*). */
KABUKI void DestroyDuplicate (const byte* s) {
    if (s == nullptr) return;
}

/*< Gets a random number. */
template<typename T>
KABUKI T GetRandom () {
    srand (time (nullptr));
    T random_number = static_cast<T>(rand ());
    return random_number;
}

/*< Creates a buffer from dynamic memory. */
template<typename T, typename U>
KABUKI T* New (U const size, U min_size) {
    if (size < min_size) return nullptr;
    byte* buffer;
    try {
        buffer = new byte[size];
#if DEBUG
        memset (buffer, '\0', size);
#endif
        return reinterpret_cast<T*>(buffer);
    } catch (...) {
        return nullptr;
    }
}
/** Deletes the given dynamic memory. */
KABUKI void Delete (void* buffer) {
    if (buffer == nullptr) return;
    delete[] buffer;
}

/** Creates/Gets a static buffer of the specified size. */
template<uint_t kBufferSize>
KABUKI byte* Buffer () {
    static byte buffer[kBufferSize + 1];
    return buffer;
}

/** Creates/Gets one of n static buffers of the specified size. */
template<uint_t kBufferNumber, uint_t kBufferSize>
KABUKI byte* Buffer () {
    static byte buffer[kBufferSize + 1];
    return buffer;
}

/** reinterpret_cast(s) a the given base and offset to an object pointer. */
template<typename T>
KABUKI T* PointTo (void* base, uint_t offset) {
    return reinterpret_cast<T*>(reinterpret_cast<byte*>(base) + offset);
}

/** Returns the number to add to word-align the given pointer to a uint_t-bit
    boundary.
    @param ptr The address to align.
    @return The offset to add to the ptr to word align it.
    */
template<typename T>
KABUKI uintptr_t WordAlignOffset (void* ptr) {
    // Algorithm works by inverting the bits, mask of the LSbs and adding 1.
    // This allows the algorithm to word align without any if statements.
    // The algorithm works the same for all memory widths as proven by the
    // truth table bellow.
    // ~000 = 111 => 000 + 111 + 1 = 0x1000
    // ~001 = 110 => 001 + 110 + 1 = 0x1000
    // ~010 = 101 => 010 + 101 + 1 = 0x1000
    // ~011 = 100 => 011 + 100 + 1 = 0x1000
    // ~100 = 011 => 100 + 011 + 1 = 0x1000
    // ~101 = 010 => 101 + 010 + 1 = 0x1000
    // ~110 = 001 => 110 + 001 + 1 = 0x1000
    // ~111 = 000 => 111 + 000 + 1 = 0x1000

    static const uintptr_t mask = sizeof (T) - 1;
    return ((~reinterpret_cast<uintptr_t> (ptr)) + 1) & mask;
}

/** Converts a single byte a one-byte hex representation. */
KABUKI byte NibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    if (b > 9)
        return b + ('a' - 10);
    return b + '0';
}

/** Converts a single byte a one-byte hex representation. */
KABUKI byte NibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b > 9) return b + ('A' - 10);
    return b + '0';
}

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t ToLowerCaseHex (byte b) {
    uint16_t value = NibbleToLowerCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToLowerCaseHex (b >> 4);
    return value;
}

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t ToUpperCaseHex (byte b) {
    uint16_t value = NibbleToUpperCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToUpperCaseHex (b >> 4);
    return value;
}

/** Converts a single hex byte a byte.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int ToByte (byte c) {
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

/** Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int ToByte (uint16_t h) {
    // Note: This works but the upper and lower are switched... pay no 
    // attention to the man behind the curtain #HandWave
    //printf ("toByte (uint16_t c): %c%c\n", (byte) (h << 8), (byte)h);
    int lowerValue = ToByte (static_cast<byte> (h >> 8));
    //printf (" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = ToByte (static_cast<byte> (h));
    if (upperValue < 0) return -1;
    //printf (" upperValue: %i \nlowerValue | (upperValue << 4): %i\n",
    //	upperValue, lowerValue | (upperValue << 4));

    //PrintLine ();
    return lowerValue | (upperValue << 4);
}

/** Calculates the difference between the begin and end address. */
KABUKI uintptr_t Diff (void* begin, void* end) {
    return static_cast<uintptr_t>(reinterpret_cast<byte*> (end) -
                                  reinterpret_cast<byte*> (begin));
}

/** Calculates the difference between the begin and end address. */
KABUKI uintptr_t Diff (const void* begin, const void* end) {
    byte* start = static_cast<byte*> (const_cast<void*> (begin)),
        *stop = static_cast<byte*> (const_cast<void*> (end));
    return static_cast<uintptr_t> (stop - start);
}

/** Gets a randomly generated 32-bit hash. */
template<typename T>
KABUKI uint_t RandomHash () {
    srand (time (0));
    return static_cast<T> (rand ());
}

/** Hashes a single byte. */
KABUKI hash16_t Hash16 (char c, hash16_t hash) {
    //PrintHex (c);
    hash16_t cprime = c * 65521;
    return cprime + hash;
}

/** Hashes the given string using the primeHash function. */
KABUKI hash16_t Hash16 (const char* s, hash16_t hash = 65521) {
    byte c = *s;
    while (c) {
        hash = Hash16 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

/** Hashes a single byte. */
KABUKI hash32_t Hash32 (char c, hash32_t hash) {
    hash32_t cprime = c * 4294967291;
    return cprime + hash;
}

/** Hashes the given string using the primeHash function. */
KABUKI hash16_t Hash32 (const char* s, hash32_t hash = 4294967291) {
    byte c = *s;
    while (c) {
        hash = Hash32 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

/** Hashes a single byte. */
KABUKI hash64_t Hash64 (char c, hash64_t hash) {
    hash64_t cprime = c * 18446744073709551557;
    return cprime + hash;
}

/** Hashes the given string using the primeHash function. */
KABUKI hash64_t Hash64 (const char* s, hash64_t hash = 18446744073709551557) {
    byte c = *s;
    while (c) {
        hash = Hash64 (c, hash);
        ++s;
        c = *s;
    }
    return hash;
}

template<typename T>
KABUKI bool IsNaN (T value) {
    T nan = sizeof (T) == 8 ? (T) 0xFF :
            sizeof (T) == 4 ? (T) 0xFFFF :
            sizeof (T) == 2 ? (T) 0xFFFFFFFF :
            sizeof (T) == 1 ? (T) 0xFFFFFFFFFFFFFFFF : 0;
    return value == nan;
}

KABUKI timestamp_t TimestampNow () {
    using namespace std::chrono;
    time_point<system_clock> now;
    auto duration = now.time_since_epoch ();
    auto millis = duration_cast<milliseconds>(duration).count ();
    return millis;
}

/** Prints the given byte in Hex.
This function prints the hex in big endian. */
KABUKI void PrintHex (byte c) {
    uint16_t chars = ToUpperCaseHex (c);
    putchar ((char)chars);
    putchar ((char)(chars >> 8));
    putchar (' ');
}

/** Prints the given string center in the given width.
If string is too small to fit in the width, function will print as much of
the string as it has room for with a "..." If the given width is less than
5, than
*/
KABUKI void PrintCentered (const char* s, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return;
    }
    // We need to leave at least one space to the left and right of 
    int length = StringLength (s);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
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
KABUKI void PrintChar (char c) {
    if ((c > 0 && c < ' ') || c == 127) {
        std::cout << 1;
        return;
    }
    std::cout << c;
}

/** Prints a line with the given token and number of columns to the debug
stream. */
KABUKI void PrintLine (char token = '-', int column_width = 80) {
    std::cout << '\n';
    for (int i = 0; i < column_width; ++i)
        std::cout << token;
    std::cout << '\n';
}

KABUKI void PrintLine (const char* start_string, char token, int length = 79) {
    std::cout << '\n' << start_string;
    for (int i = 0; i < 79; ++i)
        std::cout << token;
    std::cout << '\n';
}

/** Prints a vertical tab with the given number of rows. */
KABUKI void PrintLines (int numRows = 10) {
    std::cout << '\r';
    for (int i = 0; i < numRows - 1; ++i)
        std::cout << '\n';
}

/** Prints out the contents of the address to the debug stream. */
KABUKI void PrintMemory (const void* address, const void* end) {
    printf ("|%i", 0);
    // Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    std::cout << '\n';
    std::cout << '|';
    for (int i = 0; i < 65; ++i)
        std::cout << '_';
    std::cout << '\n';

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < end) {
        std::cout << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            PrintChar (temp);
            ++chars;
        }
        printf ("| 0x%x\n", reinterpret_cast<uintptr_t> (chars) - 64);
    }
    std::cout << '|';
    for (int i = 0; i < 64; ++i)
        std::cout << '_';
    printf ("| 0x%x\n", reinterpret_cast<uintptr_t> (chars) + Diff (address, end));
}

KABUKI void PrintMemory (const void* address, size_t size) {
    const char* end = reinterpret_cast<const char*>(address) + size;
    PrintMemory (address, end);
}

/** Prints the given value in hex format. */
template<typename Type>
KABUKI void PrintHex (Type value) {
    char buffer[sizeof (Type) * 2 + 1];
    sprintf_s (buffer, "%x", &value);
    std::cout << "0x";

    int length = strlen (buffer);
    for (int i = 0; i < length; ++i)
        std::cout << '0';
    std::cout << buffer;
}

/** Creates the format string for a key-value pair where the key is
right-aligned to the column_width. */
KABUKI char CreateKeyValueFormatString (char* s, char column_width, char type) {
    char hundreds = (column_width / 100),
        decimal = (column_width % 10),
        tens = (column_width - hundreds - decimal) / 10;
    s[0] = '%';
    if (column_width <  10) {
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
    } else if (column_width < 100) {
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
template<char kColumnWidth>
KABUKI void PrintHex (const char* header, void* value) {
    KABUKI char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'p');
    printf (formatString, header, value);
}

/** */
template<char kColumnWidth, typename Type>
KABUKI void PrintSignedHex (const char* header, Type value) {
    KABUKI char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'i');
    printf ("formatString: %s\n", formatString);
    printf (formatString, header, value);
}

/** */
template<char kColumnWidth, typename Type>
KABUKI void PrintUnsignedHex (const char* header, Type value) {
    KABUKI char formatString[12],
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'u');
    printf (formatString, header, value);
}

/** Prints an 80-char line of the number repeating with an underscore i.e. 1_1_... */
KABUKI void PrintNumberLine (int index) {
    std::cout << '\n';
    enum { MaxBufferSize = (sizeof (int) == 2) ? 7 : (sizeof (int) == 4) ? 11 : 128 };
    char buffer[MaxBufferSize];
    sprintf_s (buffer, MaxBufferSize, "%u", index);
    int length = StringLength (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1))
        printf ("%s_", buffer);
    for (int j = 0; j < lettersLeft; ++j)
        std::cout << buffer[j];
    std::cout << '\n';
}

/** Prints an 80-char line of the string repeating with an underscore
i.e. s_s_... */
KABUKI void PrintStringLine (const char* s) {
    PrintLine ();
    int length = StringLength (s),
        i;

    for (i = 0; i < 80; i += (length + 1))
        std::cout << s << '_';
    for (int j = 0; j < 80 % (length + 1); ++j)
        std::cout << s[j];
    PrintLine ();
}

/** Prints the given value to the console and prompts the user to press any key
to continue. */
template<typename Type, const char* format>
KABUKI Type PrintReturn (Type value) {
    printf (format, value);
    return value;
}

/** Prints the array starting at the base_ptr with the given numElements with a
header and given format. */
template<typename Type>
KABUKI void PrintArray (const char* header, const char* format, Type* base_ptr,
                        Type numElements) {
    PrintLine ();
    std::cout << header << ": numElements: " << numElements << '\n';
    for (Type i = 0; i < numElements; ++i) {
        printf (format, i);
        std::cout << ": ";
        printf (format, base_ptr[i]);
        std::cout << '\n';
    }
    std::cout << '\n';
    PrintLine ();
}

/** Prints an error message and pauses the system. */
KABUKI void PrintPause (const char* s) {
#if DEBUG
    std::cout << "\n" << s << "\n";
    system ("PAUSE");
#endif
}

/** Prints a new line. */
KABUKI void PrintNL () {
    std::cout << '\n';
}

/** Prints a bunch of new lines followed by the given message and a line of
the specified token. */
KABUKI void PrintLineBreak (const char* message, int top_bottom_margin,
                            char c = '-', int num_columns = 80) {
    PrintLines (top_bottom_margin);
    std::cout << message;
    PrintLine (c, num_columns);
}

/** Copies a string from the source to the destination. */
KABUKI void CopyString (char* destination, const char* source,
                        char delimeter = 0) {
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
    *destination = '\0';
}

/** Clones the given string. */
KABUKI char* CloneString (const char* input, char delimeter = 0) {
    if (input == nullptr)
        input = "";
    size_t length = StringLength (input);
    char* clone = new char[length + 1];
    CopyString (clone, input);
    return clone;
}

/** Prints the given string with a '|' at the beginning followed by a new line */
KABUKI void PrintBar (const char* input) {
    std::cout << "\n| " << input << '\n';
}

/** Prints a line break. */
KABUKI void PrintBreak (const char* header = "\n_", char c = '_', int num_lines = 0, 
                 int console_width = 80) {
    for (int i = 0; i < num_lines; ++i)
        std::cout << '\n';
    std::cout << header;
    int length = StringLength (header);
    for (int i = 0; i < console_width - length; ++i)
        std::cout << c;
    std::cout << '\n';
}

/** Prints the given string centered. */
KABUKI void PrintCentered (const char* input, int width, bool is_last = false,
                    char column_delimeter = '|')
{
    if (width < 1)
        return;
    std::cout << column_delimeter;
    int length = strlen (input),
        num_spaces;

    if (width < length) {
        char format[32];
        sprintf_s (format, 32, "%%%i", is_last ? width - 2 : width - 1);
        std::cout << column_delimeter;
        printf (format, input);
        if (is_last)
            std::cout << column_delimeter << '\n';
    }
    num_spaces = width - length - 1 - (is_last ? 1 : 0);
    for (int i = 0; i < num_spaces / 2; ++i)
        std::cout << ' ';
    std::cout << input;
    for (int i = 0; i < num_spaces - (num_spaces / 2); ++i)
        std::cout << ' ';
    if (is_last)
        std::cout << column_delimeter << '\n';
}

/** Prints a line break with the given number of columns. */
KABUKI void PrintColumnBreak (int num_columns, char column_delimeter = '|', char break_char = '-', 
                       int width = 80) {
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

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* FindEndOfRow (const char* input, int num_columns) {
    char c;
    // Scroll to the end of the line.
    while (c = *input++) {
        if (num_columns-- < 0) // We've reached the end.
        {
            // Scroll left till we hit whitespace (if any).
            while (!isspace (c))
                c = *(--input);
            // Then scroll past the whitespace.
            while (isspace (c));
            c = *(--input);
            return input;
        }
    }
    return --input;
}

/** Prints the given string centered with a horizontal page bar to the left 
    and right of the row.
    @param input The string to print.
    @param num_columns 
*/
KABUKI void PrintPageCentered (const char* input, int num_columns) {
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
            std::cout << ' ';
        // Print input.
        for (i = row_length; i != 0; --i)
            std::cout << *input++;
        for (i = num_columns - num_left_spaces - row_length; i != 0; --i)
            std::cout << ' ';
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
        while (isspace (c = *input++));
    } while (c);
}

KABUKI void PrintPageRight (const char* input, int num_columns) {
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
            std::cout << ' ';
        // Print input.
        for (i = row_length; i != 0; --i)
            std::cout << *input++;
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
        while (isspace (c = *input++));
    } while (c);
}

/** Prints the given input indented and bulleted with and '|' at the beginning 
    and end of the line representing a page.

    @code
    print ("List Item 1", 0
    @endcode

    @param input The input to print.
    @param indent The number of tabs indent level.
    @param bullet The type of bullet to print.
    @param index  The index of the bullet in the list.
    @param tab_size The number of spaces per tab.
    @param num_columns The number of columns per line.
 */
KABUKI void PrintPage (const char* input = "", int indentation = 0,
                       char bullet = '*', int index = 0, int tab_size = 4,
                       int num_columns = 80) {
    num_columns -= 4;
    std::cout << "| ";
    int cursor; //< The column number of the cursor.
    char c = *input++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || input == nullptr)  //< It's an empty input.
    {
        for (int i = num_columns; i > 0; --i)
            std::cout << ' ';
        std::cout << "|\n";
        return;
    }

    // Make the input for the bullet.
    if (isdigit (bullet)) // Then we have to print a number bullet.
    {
        sprintf_s (&buffer[0], 15, "%i", index);
        //< + 2 for "| " - 2 for the bullet offset.
        char format[16];
        sprintf_s (&format[1], 16, "%%%us", indentation * tab_size);
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
            c = *input++;
        }
        std::cout << " |\n";
    }
    std::cout << '\n';
}


/** Returns the pointer to the next char in the string that is not an ASCII
    number.
    @return A pointer to the next non-number in the input string. */
KABUKI const char* NextNonNumberString (const char* input) {
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

/** Returns the pointer to the next char in the string that is not an ASCII 
    number.
    @return A pointer to the next non-number in the input string. */
KABUKI char* NextNonNumber (char* input) {
    return (char*)NextNonNumberString (input);
}

/** Skips the leading zeros of a number if there are any. */
KABUKI const char* SkipLeadingZerosString (const char* input) {
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

/** Skips the leading zeros of a number if there are any. */
KABUKI char* SkipLeadingZeros (char* input) {
    return (char*)SkipLeadingZerosString (input);
}

/** Skips all the spaces at the start of the string. */
KABUKI const char* SkipSpacesString (const char* input) {
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

/** Skips all the spaces at the start of the string. */
KABUKI char* SkipSpaces (char* input) {
    return (char*)SkipSpacesString (input);
}

/** Gets the end of the current token from the string.
    @return Returns a pointer to the end of the token. */
KABUKI const char* EndOfTokenString (const char* input) {
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

/** Gets the end of the current token from the string.
    @return Returns a pointer to the end of the token. */
KABUKI char* EndOfToken (char* input) {
    return (char*)EndOfTokenString (input);
}

/** Scrolls over to the next double quote mark. */
KABUKI const char* EndOfString (const char* input, char delimiter = '\"') {
    char c = *input;
    while (c) {
        if (c == delimiter)
            return input;
        c = *(++input);
    }
    return input;
}

/** Scrolls over to the next double quote mark. */
KABUKI char* EndOf (char* input, char delimiter = '\"') {
    return (char*)EndOfString (input, delimiter);
}

/** Compares the source and query string using the delimiter to terminate the query. */
KABUKI const char* CompareTokenString (const char* input, const char* token) {
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

/** Compares the source and query string using the delimiter to terminate the query. */
KABUKI char* CompareToken (const char* input, const char* query) {
    return (char*)CompareTokenString (input, query);
}

/** Compares the source and query string using the delimiter to terminate the query. */
KABUKI const char* CompareString (const char* input, const char* query,
                                  char delimiter = 0) {
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

/** Compares the source and query string using the delimiter to terminate the
    query. */
KABUKI char* Compare (char* source, const char* query, char delimiter = 0) {
    return (char*)CompareString (source, query, delimiter);
}

/** Parses the given string for the given token, setting the following 
    whitespace to null.
    @param  string The string to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
KABUKI const char* ParseString (const char* input, char* destination,
                                int buffer_size, char delimiter = 0) {
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

/** Parses the given string for the given token, setting the following 
    whitespace to null.
    @param  string The string to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
KABUKI char* Parse (char* input, char* destination, int buffer_size,
                    char delimiter = 0) {
    return (char*)ParseString (input, destination, buffer_size, delimiter);
}


/** Parses the token by scrolling to the end of it and setting the end to null.
    @param  input The string to search.
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
KABUKI char* ParseToken (char* input) {
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

/** Searches the given string for the given string.
    @param  input      The string to search.
    @param  query      The string to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the parsing failed and a pointer to the first char
            after the end of the token upon success. */
KABUKI const char* FindString (const char* input, const char* query, char delimiter = 0) {
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

/** Searches the given string for the given string, setting the following 
    whitespace to null.
    @param  input      The string to search.
    @param  query      The token to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
KABUKI char* Find(char* input, const char* query, char delimiter = 0) {
    return (char*)FindString (input, query, delimiter);
}

/** Parses the string for a int.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */
KABUKI const char* ParseIntString (const char* input, int* value) {
    if (input == nullptr)
        return nullptr;
    if (value == nullptr)
        return nullptr;
    if (!sscanf_s (input, "%i", value))
        return nullptr;
    return EndOfTokenString (input);
}

/** Parses the string for a int.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */
KABUKI char* ParseInt (char* input, int* value) {
    return (char*)ParseIntString (input, value);
}

enum {
    kMaxDigitsFloat = DBL_MAX_10_EXP + 2
};

/** Parses the string for a float.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */

KABUKI const char* ParseFloatString (const char* input, float* value) {
    if (input == nullptr)
        return nullptr;
    if (value == nullptr)
        return nullptr;
    if (!sscanf_s (input, "%f", value))
        return nullptr;
    return EndOfTokenString (input);
}

/** Parses the string for a float.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the string upon success. */
KABUKI char* ParseFloat (char* input, float* value) {
    return (char*)ParseFloatString (input, value);
}

/** Checks if the given string is a token. 
    @param input The string to check.
    @returns Returns true if the given string is a token. */
KABUKI bool IsToken (const char* input) {
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

}       //< namespace _

#endif  //< CHINESE_ROOM_UTILS_H
