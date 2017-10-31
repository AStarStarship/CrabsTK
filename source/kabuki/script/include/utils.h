/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/utils.h
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_UTILS_H
#define SCRIPT_UTILS_H

#include "module_config.h"

namespace _ {

KABUKI const char* NewLineString ();
KABUKI const char* ErrorHeader ();
KABUKI const char* VerticalBar ();

/* Returns the inverse of the given value.
   For code metadata purposes. */
template<typename T>
T MaxValue () {
    return ~(T)0;
}


/** Prints the given char to the std::out. */
KABUKI void PrintLine (const char* string);

KABUKI void PrintDebug (const char* message,
                        const char* end_string   = NewLineString (),
                        const char* begin_string = VerticalBar   ());

KABUKI void PrintError (const char* message, 
                        const char* end_string = NewLineString ());

KABUKI void PrintDebugPointer (const char* message, const void* address);

KABUKI void PrintDebugHex (const char* message, char value);

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

/** Gets the length of the given char.
    @return Returns -1 if the input char is null.
*/
KABUKI int StringLength (const char* string, char delimiter = 0);

/** Creates a duplicate copy of the given char using dynamic memory. */
KABUKI const char* DuplicateString (const char* string, char delimiter = 0);

/** Destroys the duplicate char created by const byte* duplicate
(const byte*). */
KABUKI void DestroyDuplicate (const byte* string);

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
KABUKI void Delete (void* buffer);

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


/** Creates/Gets one of n static buffers of the specified size. */
template<typename T, uint_t kBufferNumber, uint_t kBufferSize>
KABUKI T* Buffer () {
    static T buffer[kBufferSize + 1];
    return buffer;
}

/** reinterpret_cast(string) a the given base and offset to an object pointer. */
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
    //                                                        Mask
    return ((~reinterpret_cast<uintptr_t> (ptr)) + 1) & (sizeof (T) - 1);
}

KABUKI byte* WordAlign (byte* ptr);

/** Converts a single byte a one-byte hex representation. */
KABUKI byte NibbleToLowerCaseHex (byte b);

/** Converts a single byte a one-byte hex representation. */
KABUKI byte NibbleToUpperCaseHex (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t ToLowerCaseHex (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t ToUpperCaseHex (byte b);

/** Converts a single hex byte a byte.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int ToByte (byte c);

/** Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int ToByte (uint16_t h);

/** Calculates the difference between the begin and end address. */
KABUKI uintptr_t Diff (void* begin, void* end);

/** Calculates the difference between the begin and end address. */
KABUKI uintptr_t Diff (const void* begin, const void* end);

/** Gets a randomly generated 32-bit hash. */
template<typename T>
KABUKI uint_t RandomHash () {
    srand (time (0));
    return static_cast<T> (rand ());
}

/** Hashes a single byte. */
KABUKI hash16_t Hash16 (char c, hash16_t hash);

/** Hashes the given char using the primeHash function. */
KABUKI hash16_t Hash16 (const char* string, hash16_t hash = 65521);

/**  */
hash16_t Hash16UI2 (uint16_t value, hash16_t hash);


hash16_t Hash16UI4 (uint32_t value, hash16_t hash);


hash16_t Hash16UI8 (uint64_t value, hash16_t hash);

/** Hashes a single byte. */
KABUKI hash32_t Hash32 (char c, hash32_t hash);

/** Hashes the given char using the primeHash function. */
KABUKI hash16_t Hash32 (const char* string, hash32_t hash = 4294967291);

/** Hashes a single byte. */
KABUKI hash64_t Hash64 (char c, hash64_t hash);

/** Hashes the given char using the primeHash function. */
KABUKI hash64_t Hash64 (const char* string, hash64_t hash = 18446744073709551557);

template<typename T>
KABUKI bool IsNaN (T value) {
    T nan = sizeof (T) == 8 ? (T) 0xFF :
            sizeof (T) == 4 ? (T) 0xFFFF :
            sizeof (T) == 2 ? (T) 0xFFFFFFFF :
            sizeof (T) == 1 ? (T) 0xFFFFFFFFFFFFFFFF : 0;
    return value == nan;
}

/** Prints the given byte in Hex.
    This function prints the hex in big endian. */
KABUKI void PrintHex (byte c);

/** Prints the given char center in the given width.
    If char is too small to fit in the width, function will print as much of
    the char as it has room for with a "..." If the given width is less than
    5, than */
KABUKI void PrintCentered (const char* string, int width);

/** Prints the given char to the stdout if it is printable, and prints ADR. */
KABUKI void PrintChar (char c);

/** Prints a line with the given token and number of columns to the debug
stream. */
KABUKI void PrintLine (char token = '-', int column_width = 80);

KABUKI void PrintLine (const char* start_string, char token, int length = 79);

/** Prints a vertical tab with the given number of rows. */
KABUKI void PrintLines (int numRows = 10);

/** Prints out the contents of the address to the debug stream. */
KABUKI void PrintMemory (const void* address, const void* end);

KABUKI void PrintMemory (const void* address, size_t size);

/** Prints the given value in hex format. */
template<typename Type>
KABUKI void PrintHex (Type value) {
    char buffer[sizeof (Type) * 2 + 1];
    sprintf_s (buffer, "%x", &value);
    std::cout << "0x";

    int length = StringLength (buffer);
    for (int i = 0; i < length; ++i)
        std::cout << '0';
    std::cout << buffer;
}

/** Creates the format char for a key-value pair where the key is
right-aligned to the column_width. */
KABUKI char CreateKeyValueFormatString (char* string, char column_width, char type);

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
        nullTermChar = CreateKeyValueFormatString (formatString, kColumnWidth, 'user');
    printf (formatString, header, value);
}

/** Prints an 80-char line of the number repeating with an underscore i.e. 1_1_... */
KABUKI void PrintNumberLine (int index);

/** Prints an 80-char line of the char repeating with an underscore
i.e. s_s_... */
KABUKI void PrintStringLine (const char* string);

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
KABUKI void PrintPause (const char* string);

/** Prints a new line. */
KABUKI void PrintNL ();

/** Prints a bunch of new lines followed by the given message and a line of
the specified token. */
KABUKI void PrintLineBreak (const char* message, int top_bottom_margin,
                            char c = '-', int num_columns = 80);

/** Copies a char from the source to the destination. */
KABUKI void StringCopy (char* destination, const char* source,
    char delimeter = 0);

/** Clones the given char. */
KABUKI char* StringClone (const char* input, char delimeter = 0);

/** Prints the given char with a '|' at the beginning followed by a new line */
KABUKI void PrintBar (const char* input);

/** Prints a line break. */
KABUKI void PrintBreak (const char* header = "\n_", char c = '_', int num_lines = 0,
                        int console_width = 80);

/** Prints the given char centered. */
KABUKI void PrintCentered (const char* input, int width, bool is_last = false,
                           char column_delimeter = '|');

/** Prints a line break with the given number of columns. */
KABUKI void PrintColumnBreak (int num_columns, char column_delimeter = '|', char break_char = '-',
                              int width = 80);

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* FindEndOfRow (const char* input, int num_columns);

/** Prints the given char centered with a horizontal page bar to the left 
    and right of the row.
    @param input The char to print.
    @param num_columns 
*/
KABUKI void PrintPageCentered (const char* input, int num_columns);

/** Prints the given char justified right.
    @param input The char to print.
    @param num_columns The number of columns per row. */
KABUKI void PrintPageRight (const char* input, int num_columns);

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
    @param num_columns The number of columns per line. */
KABUKI void PrintPage (const char* input = "", int indentation = 0,
                       char bullet = '*', int index = 0, int tab_size = 4,
                       int num_columns = 80);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the input char. */
KABUKI const char* NextNonNumberString (const char* input);

/** Returns the pointer to the next char in the char that is not an ASCII 
    number.
    @return A pointer to the next non-number in the input char. */
KABUKI char* NextNonNumber (char* input);

/** Skips the leading zeros of a number if there are any. */
KABUKI const char* SkipLeadingZerosString (const char* input);

/** Skips the leading zeros of a number if there are any. */
KABUKI char* SkipLeadingZeros (char* input);

/** Skips all the spaces at the start of the char. */
KABUKI const char* SkipSpacesString (const char* input);

/** Skips all the spaces at the start of the char. */
KABUKI char* SkipSpaces (char* input);

/** Gets the end of the current token from the char.
    @return Returns a pointer to the end of the token. */
KABUKI const char* EndOfTokenString (const char* input);

/** Gets the end of the current token from the char.
    @return Returns a pointer to the end of the token. */
KABUKI char* EndOfToken (char* input);

/** Scrolls over to the next double quote mark. */
KABUKI const char* EndOfString (const char* input, char delimiter = '\"');

/** Scrolls over to the next double quote mark. */
KABUKI char* EndOf (char* input, char delimiter = '\"');

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* CompareTokenString (const char* input, const char* token);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI char* CompareToken (const char* input, const char* query);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* StringCompare (const char* input, const char* query,
    char delimiter = 0);

/** Compares the source and query char using the delimiter to terminate the
    query. */
KABUKI char* Compare (char* source, const char* query, char delimiter = 0);

/** Parses the given char for the given token, setting the following 
    whitespace to null.
    @param  char The char to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
KABUKI const char* ParseString (const char* input, char* destination,
    int buffer_size, char delimiter = 0);

/** Parses the given char for the given token, setting the following 
    whitespace to null.
    @param  char The char to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
KABUKI char* Parse (char* input, char* destination, int buffer_size,
    char delimiter = 0);


/** Parses the token by scrolling to the end of it and setting the end to null.
    @param  input The char to search.
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
KABUKI char* ParseToken (char* input);

/** Searches the given char for the given char.
    @param  input      The char to search.
    @param  query      The char to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the parsing failed and a pointer to the first char
            after the end of the token upon success. */
KABUKI const char* FindString (const char* input, const char* query, char delimiter = 0);

/** Searches the given char for the given char, setting the following 
    whitespace to null.
    @param  input      The char to search.
    @param  query      The token to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the search failed and a pointer to the first hit
            upon success.
*/
KABUKI char* Find (char* input, const char* query, char delimiter = 0);

/** Parses the char for a int.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the char upon success. */
KABUKI const char* ParseIntString (const char* input, int* value);

/** Parses the char for a int.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the char upon success. */
KABUKI char* ParseInt (char* input, int* value);

enum {
    kMaxDigitsFloat = DBL_MAX_10_EXP + 2
};

/** Parses the char for a float.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the char upon success. */

KABUKI const char* ParseFloatString (const char* input, float* value);

/** Parses the char for a float.
    @return Returns null there was not a number and a pointer to after the 
            number ends in the char upon success. */
KABUKI char* ParseFloat (char* input, float* value);

/** Checks if the given char is a token. 
    @param input The char to check.
    @returns Returns true if the given char is a token. */
KABUKI bool IsToken (const char* input);

/** Compute the next highest power of 2. */
KABUKI int RoundToPowerOf2 (int value);

}       //< namespace _

#endif  //< SCRIPT_UTILS_H
