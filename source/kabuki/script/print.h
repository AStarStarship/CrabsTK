/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/text.h
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_SCRIPT_PRINT
#define HEADER_FOR_SCRIPT_PRINT

#include "text.h"
#if SCRIPT_USING_PRINT

namespace _ {

template<typename T>
inline char Char (T value) {
    if (value < 32) return ' ';
    return value;
}

/** Prints the given token a specified num_times. */
KABUKI void PrintLine (char token = '-', const char* left_column_text = "\n|",
                       int length = 79);

/** Prints a bunch of new lines followed by the given message and a line of
the specified token. */
KABUKI void PrintLineBreak (const char* message,
                            int top_bottom_margin,
                            char c = '-',
                            int num_columns = 80);

/** Prints a vertical tab with the given number of rows. */
KABUKI void PrintLines (int numRows = 10);

/** Print's out the given pointer to the console. */
inline void PrintPointerNL (const void* pointer) {
    // @todo Replace with PrintHex.
    printf ("0x%p\n", pointer);
}

/** Prints an 80-char line of the char repeating with an underscore
i.e. s_s_... */
KABUKI void PrintLine (const char* string);

/** Prints the given char to the stdout. */
KABUKI void PrintLine (const char* string);

KABUKI void PrintError (const char* message,
                        const char* end_string = TextCR ());

/** Converts a single byte a one-byte hex representation. */
KABUKI byte MemoryNibbleToLowerCaseHex (byte b);

/** Converts a single byte a one-byte hex representation. */
KABUKI byte MemoryNibbleToUpperCaseHex (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t MemoryByteToLowerCaseHex (byte b);

/** Converts a single byte a two-byte hex representation. */
KABUKI uint16_t MemoryByteToUpperCaseHex (byte b);

/** Converts a single hex byte a byte.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int MemoryHexToByte (byte c);

/** Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex byte.
*/
KABUKI int MemoryHexToByte (uint16_t h);

/** Gets a randomly generated 32-bit hash. */
template<typename T>
KABUKI uint_t RandomHash () {
    srand (time (0));
    return static_cast<T> (rand ());
}

/** Prints the given byte in Hex.
    This function prints the hex in big endian. */
KABUKI void PrintHex (byte c);

/** Print's out the given pointer to the console. */
inline void PrintPointer (const void* pointer) {
    // @todo Replace with PrintHex.
    printf ("0x%p", pointer);
}

/** Prints the given strand centered about the given width in console char rows.
    If char is too small to fit in the width, function will print as much of
    the char as it has room for with a "..." If the given width is less than
    5, then only dots will be printed. */
KABUKI void PrintCentered (const char* strand, int width);

/** Prints the given strand centered about the given width in console char rows.
    If char is too small to fit in the width, function will print as much of
    the char as it has room for with a "..." If the given width is less than
    5 then only dots will be printed. */

/** Prints the given char centered. */
KABUKI void PrintCentered (const char* input, int width, bool is_last,
                           char column_delimeter = '|');

/** Prints the given char to the stdout if it is printable, and prints ADR. */
KABUKI void PrintChar (char c);

/** Prints the given value in hex format. */
template<typename Type>
KABUKI void PrintHex (Type value) {
    char buffer[sizeof (Type) * 2 + 1];
    sprintf_s (buffer, "%x", &value);
    std::cout << "0x";

    int length = StrandLength (buffer);
    for (int i = 0; i < length; ++i)
        std::cout << '0';
    std::cout << buffer;
}

/** Creates the format char for a key-value pair where the key is
right-aligned to the column_width. */
KABUKI char PrintCreateKeyValueFormatText (char* string, char column_width,
                                        char type);

/** */
template<char kColumnWidth>
KABUKI void PrintHex (const char* header, void* value) {
    char formatText[12],
         nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                    'p');
    printf (formatText, header, value);
}

/** */
template<char kColumnWidth, typename Type>
KABUKI void PrintSignedHex (const char* header, Type value) {
    char formatText[12],
         nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                    'i');
    printf ("formatText: %s\n", formatText);
    printf (formatText, header, value);
}

/** */
template<char kColumnWidth, typename Type>
KABUKI void PrintUnsignedHex (const char* header, Type value) {
    char formatText[12],
         nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                    'u');
    printf (formatText, header, value);
}

/** Prints an 80-char line of the number repeating with an underscore
    i.e. 1_1_... */
KABUKI void PrintNumberLine (int index);

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
KABUKI void PrintArray (const char* header,
                        const char* format,
                        Type* base_ptr,
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

/** Prints the given char with a '|' at the beginning followed by a new line */
KABUKI void PrintBar (const char* input);

/** Prints a line break. */
KABUKI void PrintBreak (const char* header = "\n_",
                        char c = '_',
                        int num_lines = 0,
                        int console_width = 80);

/** Prints a line break with the given number of columns. */
KABUKI void PrintColumnBreak (int num_columns,
                              char column_delimeter = '|',
                              char break_char = '-',
                              int width = 80);

/** Prints the given char centered with a horizontal page bar to the left
    and right of the row.
    @param input The char to print.
    @param num_columns
*/
KABUKI void PrintPageCentered (const char* input,
                               int num_columns);

/** Prints the given char justified right.
    @param input The char to print.
    @param num_columns The number of columns per row. */
KABUKI void PrintPageRight (const char* input,
                            int num_columns);

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

/** Prints out the contents of the address to the debug stream. */
KABUKI void PrintMemory (const void* address, const void* end);

/** Prints out the contents of the address to the debug stream. */
inline void PrintMemory (const void* address, int num_bytes) {
    return PrintMemory (address,
                        reinterpret_cast<const byte*> (address) + num_bytes);
}
/** Prints out the contents of the address to the debug stream. */
inline void PrintMemory (const void* address, uint num_bytes) {
    return PrintMemory (address,
                        reinterpret_cast<const byte*> (address) + num_bytes);
}

}       //< namespace _
#endif  //< SCRIPT_USING_PRINT
#endif  //< HEADER_FOR_SCRIPT_PRINT
