
/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/strand.h
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

#ifndef HEADER_FOR_SCRIPT_STRAND
#define HEADER_FOR_SCRIPT_STRAND

#include "config.h"

namespace _ {

/** String utilities.
    @ingroup Token

    Definition:

    Tokens are defined as having a beginning of a buffer, and end of a buffer,
    and a nil-term char. A theoretical string, in contrast, does not have a
    buffer. ASCII and Unicode strands define a whitespace as any non-printable 
    ASCII char including the nil-term char to help speed up string tokening.
    const char* in ROM are treated as ideal strings via the compiler's nil-term
    char guarantee, which isn't 100% but enough to fudge it.

    @todo Add Find and other functions to Token. */
inline bool IsSpace (char character) {
    //if (character == '.') {
    //    return true;
    //}
    return character <= ' ';
}

/** Reads a time or time delta from a a char starting with an '@' sign..
    @param input  The char to parse.
    @param hour   The location to write the number of hours to.
    @param minute The location to write the number of minutes to.
    @param Second The location to write the number of seconds to.
    @return The offset The end of where the parser exited successfully at.

    @code
    @4        (This is 4AM)
    @4PM      (No space required)
    @4:20P    (Or M)
    @4:20 PM
    @16:20
    @4:20 am
    @4:20a.m.
    @4:20:00
    @4:20:00AM
    @16:20:00
    @endcode

KABUKI const char* TokenReadTime (const char* input, int* hour,
                                         int* minute, int* second);*/

/** Converts a keyboard input to char and deletes the char. 

KABUKI const char* TokenReadTime (const char* input,
                                              tm* std_tm);*/

/** Converts a keyboard input to a time_t.
KABUKI const char* TokenReadTime (const char* input, time_t& result); */

/** Converts a keyboard input to a time_t.
KABUKI char* TokenReadTime (char* input, time_t& result); */

/** Converts the given string to a 8-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, int8_t& result);

/** Converts the given string to a 8-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, uint8_t& result);

/** Converts the given string to a 16-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, int16_t& result);

/** Converts the given string to a 16-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, uint16_t& result);

/** Converts the given string to a 32-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, int32_t& result);

/** Converts the given string to a 32-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, uint32_t& result);

/** Converts the given string to a 64-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, int64_t& result);

/** Converts the given string to a 64-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, uint64_t& result);

/** Converts the given string to a 32-bit floating-point number.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, float& result);

/** Converts the given string to a 64-bit floating-point number.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* TokenRead (const char* begin, double& result);

/** Reads a whitespace-delineated token from the given text buffer.
    @param begin     Beginning address of the input buffer.
    @param end       The end address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    strands. */
KABUKI const char* TokenRead (const char* begin, const char* end, 
                               char* token, char* token_end);

/** Reads a explicitly-delineated token from the given text buffer.
    @param begin     Beginning address of the input buffer.
    @param end       The end address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI const char* TokenRead (const char* text, const char* end, 
                               char* token, char* token_end, char delimiter);

/** Reads a int8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               int8_t& result);

/** Reads a uint8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               uint8_t& result);

/** Reads a int16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                             int16_t& result);

/** Reads a uint16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               uint16_t& result);

/** Reads a int32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               int32_t& result);

/** Reads a uint32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               uint32_t& result);

/** Reads a int64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end, 
                               int64_t& result);

/** Reads a uint64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               uint64_t& result);

/** Reads a float from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                               float& result);

/** Reads a double from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* TokenRead (const char* begin, const char* end,
                              double& result);

/** Converts the given value to a char. */
template<typename T>
inline char Char (T value) {
    if (value < 32) return ' ';
    return value;
}

/** Writes the given std_tm to the char buffer.
KABUKI char* TokenWriteTime (char* begin, char* end, tm* std_tm); */

/** Writes the given time to the char buffer.
KABUKI char* TokenWriteTime (char* begin, char* end, time_t t); */

/** Scrolls over to the end of the strand string.
    @return  Returns nil upon failure or a pointer to the nil-term char
             upon success.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI const char* TokenEnd (const char* begin);

/** Scrolls over to the next double quote mark.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI const char* TokenEnd (const char* begin, char delimiter);

/** Gets the length of the given char.
    @return  Returns -1 if the strand char is nil.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI int TokenLength (const char* begin);

/** Gets the length of the given char.
    @return  Returns -1 if the strand char is nil.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI int TokenLength (const char* begin, char delimiter);

/** Clones the given string.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
KABUKI char* TokenClone (const char* begin);

/** Clones the given string with given NON-ZERO delimiter.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
//KABUKI char* TokenClone (const char* begin, char delimiter);

/** Returns a pointer to the char at the end of the line. */
KABUKI const char* TokenLineEnd (const char* begin, int num_columns);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the strand char. */
KABUKI const char* TokenSkipNumbers (const char* begin);

/** Skips the leading zeros of a number if there are any. */
KABUKI const char* TokenSkipZeros (const char* begin);

/** Skips all the spaces at the start of the char. */
KABUKI const char* TokenSkipSpaces (const char* begin);

/** Compares the source and query char as nil-terminated strings. */
KABUKI const char* TokenEquals (const char* begin, const char* query);

/** Compares the source and query char as nil-terminated strings. */
KABUKI const char* TokenEquals (const char* begin, const char* end,
                                 const char* query);

/** Compares the source and query char using the delimiter to terminate the 
    query. */
KABUKI const char* TokenEquals (const char* begin, const char* query,
                                 char delimiter);

/** Compares the source and query char using the delimiter to terminate the 
    query. */
KABUKI const char* TokenEquals (const char* begin, const char* end, 
                                 const char* query, char delimiter);

/** Parses the given char for the given token, setting the following
    whitespace to nil.
    @param  char The char to search.
    @param  token  The token to search for.
    @return Returns nil if the search failed and a pointer to the first hit
            upon success. */
KABUKI const char* TokenRead (const char* begin, char* text,
                               char*end, char delimiter);

/** Searches the given char for the given char.
    @param  strand      The char to search.
    @param  query      The char to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns nil if the parsing failed and a pointer to the first char
            after the end of the token upon success. */
KABUKI const char* TokenFind (const char* begin, const char* query, 
                               char delimiter = 0);

/** Searches the given char for the given char, setting the following
    whitespace to nil.
    @param  strand      The char to search.
    @param  query      The token to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns nil if the search failed and a pointer to the first hit
            upon success. */
KABUKI char* TokenFind (char* begin, const char* query, char delimiter = 0);

/** Group of functions for working with strings with explicitly declared end
    of buffer.
    Currently the read-write functions use sprintf. This is not the optimal 
    behavior and the functions needs to bypass the formating string.
    @ingroup Text
*/

enum {
    kMaxDigitsFloat  = FLT_MAX_10_EXP + 2,
    kMaxDigitsDouble = DBL_MAX_10_EXP + 2,
};

/** Empty string. */
KABUKI const char* TokenEmpty ();

/** New line string. */
KABUKI const char* TokenCR ();

/** Error header string. */
KABUKI const char* TokenErrorHeader ();

/** New line and vertical bar "\n// " string. */
KABUKI const char* TokenNewLine ();

/** Skips all the spaces at the start of the char.
    @param  begin Beginning address of the read buffer.
    @param  end   The end address of the input buffer.
    @return A pointer to the end of the token read or if no token read. */
KABUKI const char* TokenSkipSpaces (const char* begin, const char* end);

/** Writes the given strand to the text buffer.
    @param begin Beginning address of the write buffer.
    @param end   The end address of the buffer.
    @param value The potentially unsafe string to write. */
KABUKI char* TokenWrite (char* begin, char* end, const char* strand);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI char* TokenWrite (char* begin, char* end, const char* strand,
                          char delimiter);

/** Copies a char from the source to the text.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
                     strands. */
KABUKI char* TokenWrite (char* begin, char* end, const char* text,
                          const char* strand_end, char delimiter);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, int8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, uint8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, int16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, uint16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, int32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, uint32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, int64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, uint64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, float value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* TokenWrite (char* begin, char* end, double value);

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* TokenLineEnd (const char* begin, const char* end,
                                  int num_columns);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the strand char. */
KABUKI const char* TokenNextNonNumber (const char* begin, const char* end);

/** Parses the given char for the given token, setting the following
    whitespace to nil.
    @param  char The char to search.
    @param  token  The token to search for.
    @return Returns nil if the search failed and a pointer to the first hit
            upon success. */
KABUKI const char* TokenRead (const char* begin, char* text,
                              char*end, char delimiter);

/** Reads a whitespace-delineated token from the given text buffer.
    @param begin     Beginning address of the input buffer.
    @param end       The end address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    strands. */
KABUKI const char* TokenRead (const char* begin, const char* end, 
                              char* token, char* token_end);

/** Reads a explicitly-delineated token from the given text buffer.
    @param begin     Beginning address of the input buffer.
    @param end       The end address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI const char* TokenRead (const char* text, const char* end, 
                              char* token, char* token_end, char delimiter);

/** A group of one or more UTF-8 strings in a ring buffer Slot.
    
    # String vs Token and Buffer Terminology

    By definition, a string will always have a nil-term char. A strand is 
    guaranteed to have at least one nil-term char. After the first nil-term
    char there may be more zeros from non-overwritten memory.

    # Op

    Token is intended to work similar to the std::cout mixed with a string
    using overloaded operator<<. A Token takes a pointer to the beginning 
    of the strand buffer and the end of the buffer. A cursor is used to 
    both read and write the Token. When writing the cursor will always 
    be a nil-term char (given no external modifying sources.).

    # Read Ops

    To read from a Token reset the cursor to where you would like to read 
    from and read with either the Token::Read functions or the overloaded
    operator>>.

    @code
    enum { kSize = 256 };
    char buffer[kSize];
    Token static_example (kSize, buffer),
           dynamic_example (kSize);
    @endcode

    @see _::Text
*/
class Token {
    public:

    enum {
        // Min buffer size in bytes.
        kSizeMin        = 1,
        // Init buffer size in bytes.
        kInitBufferSize = SCRIPT_STRAND_INIT_BUFFER_SIZE,
    };

    /** Constructs a Text from the given strand.
        @param begin Pointer to the beginning of the buffer or nil to create 
                     new buffer on the heap. 
        @param size  The NON-ZERO size of the buffer.
        @warning If size is 0 it will be set to the kMinBufferSize. */
    Token (size_t size = kInitBufferSize, char* begin = nullptr);

    /** Constructor copies the other Text buffer up the cursor. */
    Token (const Token& other);

    /** Virtual deletes the cursor_ if the is_dynamic_ flag is set. */
    ~Token ();

    /** Deep copies the state of the other object.
        @return A reference to *this. */
    Token& SetEquals (const Token& other);

    /** Flags if this Token uses dynamic memory.
        @return Returns true if using dynamic memory and false otherwise. */
    bool IsDynamic ();

    /** Sets the buffer size to the new_size without copying.
        @param  new_size The new size of the buffer. 
        @reutrn Nil upon success or an error string upon failure. */
    const char* SetSize (size_t new_size);

    /** Resizes the buffer to the new_size and copies the string.
        @param  new_size The new size of the buffer. 
        @reutrn Nil upon success or an error string upon failure. */
    const char* Resize (size_t new_size);

    /** Clears the strand buffer without overwriting it. */
    Token& Clear ();

    /** Gets the char count of the buffer in bytes. */
    int Length () const;

    /** Gets the char count of the buffer in bytes. */
    int Space () const;
    
    /** Gets a pointer to the beginning of the buffer. */
    char* GetBegin ();

    /** Gets a pointer to the current write location in the buffer_. */
    char* GetCursor ();

    /** Gets a pointer to the end of the buffer. */
    char* GetEnd ();

    /** Sets the cursor to the new value. 
        @return Returns false if the new_cursor is out of bounds. */
    bool SetCursor (char* new_cursor);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Print (char value);
    
    /** Writes the given string to the buffer_.
        @param  string The string to write. 
        @return Reference to *this. */
    Token& Write (const char* string);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (int8_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (uint8_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (int16_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (uint16_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (int32_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (uint32_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (int64_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Token& Write (uint64_t value);

    /** Writes the given value to the buffer_.
        @param  Token to write.
        @return Reference to *this. */
    Token& Write (float value);

    /** Writes the given value to the buffer_.
        @param  Token to write.
        @return Reference to *this. */
    Token& Write (double value);

    /** Writes the given strand to the buffer_.
        @param  Token to write.
        @return Reference to *this. */
    Token& Write (Token& strand);

    /** Prints the given token a specified num_times. */
    Token& Line (char token = '-', const char* left_column_strand = "\n//",
                  int num_columns = 80);

    /** Prints an 80-char line of the char repeating with an underscore
        i.e. s_s_... */
    Token& StringLine (const char* strand, int num_columns = 80);

    /** Prints a bunch of new lines followed by the given message and a line of
    the specified token. */
    Token& LineBreak (const char* message, int top_bottom_margin,
                       char c = '-', int num_columns = 80);

    /** Prints a vertical tab with the given number of rows. */
    Token& Lines (int numRows = 10);

    /** Prints the given byte in Hex.
        This function prints the hex in big endian. */
    Token& Hex (byte c);

    /** Print's out the given pointer to the strand. */
    Token& Hex (const void* pointer);

    Token& Error (const char* message, const char* end_string = TokenCR ());

    /** Prints the params.
        @params A B-Sequence header.
        @return A reference to *this. */
    Token& Bsq (const uint_t* params);

    /** Prints the given value in hex format. */
    template<typename Type>
    Token& Hex (Type value) {
        char buffer[sizeof (Type) * 2 + 1];
        sprintf_s (buffer, "%x", &value);
    #if SCRIPT_DEBUG == SCRIPT_STRAND
        Write ("0x");
        int length = TokenLength (buffer);
        for (int i = 0; i < length; ++i)
            Write ('0');
        Write (buffer);
    #endif
        return *this;
    }

    /** Prints an 80-char line of the number repeating with an underscore
    i.e. 1_1_... */
    Token& NumberLine (int index);

    /** Prints the array starting at the base_ptr with the given numElements 
        with a header and given format. 
    template<typename Type>
    Text& Array (const char* header,
                 const char* format,
                 Type* base_ptr,
                 Type numElements) {
        PrintLine ();
        std::cout << header << ": numElements: " << numElements << '\n';
        for (Type i = 0; i < numElements; ++i) {
            printf (format, i);
            std::cout << ": ";
            Write (base_ptr[i]);
            std::cout << '\n';
        }
        std::cout << '\n';
        PrintLine ();
    }*/

    /** Prints a new line. */
    Token& NewLine ();

    /** Prints the given char with a '|' at the beginning followed by a new 
        line */
    Token& Heading (const char* input);

    /** Prints a line break. */
    Token& Break (const char* header = "\n_", char c = '_', int num_lines = 0,
                   int console_width = 80);

    /** Prints a line break with the given number of columns. */
    Token& ColumnBreak (int num_columns, char column_delimeter = '|',
                         char break_char = '-', int width = 80);

    /** Prints the given char centered with a horizontal page bar to the left
        and right of the row.
        @param input The char to print.
        @param num_columns */
    Token& Centered (const char* input, int num_columns);

    /** Prints the given strand centered about the given width in console char
        rows.
        If char is too small to fit in the width, function will print as much of
        the char as it has room for with a "..." If the given width is less than
        5 then only dots will be printed. */

    /** Prints the given char centered. */
    Token& Centered (const char* input, int width, bool is_last,
                      char column_delimeter = '|');

    /** Prints the given strand justified right to this strand.
        @param input The strand to print.
        @param num_columns The number of columns per row. */
    Token& Right (const char* strand, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (int8_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (uint8_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (int16_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (uint16_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (int32_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (uint32_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (int64_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (uint64_t value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (float value, int num_columns = 80);

    /** Prints the given value justified right to this strand.
        @param value The char to print.
        @param num_columns The number of columns per row. */
    Token& Right (double value, int num_columns = 80);

    /** Prints the given input indented and bulleted with and '|' at the 
        beginning and end of the line representing a page.

        @code
        print ("List Item 1", 0
        @endcode

        @param input The input to print.
        @param indent The number of tabs indent level.
        @param bullet The type of bullet to print.
        @param index  The index of the bullet in the list.
        @param tab_size The number of spaces per tab.
        @param num_columns The number of columns per line. */
    Token& Page (const char* input = "", int indentation = 0,
                  char bullet = '*', int index = 0, int tab_size = 4,
                  int num_columns = 80);

    /** Prints out the contents of the address to the debug stream. */
    Token& Memory (const void* address, const void* end);

    /** Prints out the contents of the address to the debug stream. */
    Token& Memory (const void* address, int size);

    /** Prints out the contents of the address to the debug stream. */
    //inline Text& Memory (const void* address, int num_bytes) {
    //    return Memory (address,
    //                   reinterpret_cast<const char*> (address) + num_bytes);
    //}

    /** Prints out the contents of the address to the debug stream. */
    //inline Text& Memory (const void* address, uint num_bytes) {
    //    return Memory (address,
    //                   reinterpret_cast<const char*> (address) + num_bytes);
    //}


    /** Prints the first whitespace or nil terminated token from the string.
        @param strand A nil-terminated string in ROM.
        @return A pointer to the end of the token or nil upon failure. */
    Token& Token (const char* strand);

    /** Prints the first whitespace or nil terminated token from the strand.
        @param strand     A nil-terminated string in RAM or ROM.
        @param strand_end End of the strand buffer.
        @return A pointer to the end of the token or nil upon failure. */
    Token& Token (const char* strand, const char* strand_end);

    /** Prints the time to the console. */
    Token& TimeStruct (tm* std_tm);

    /** Prints the given time to the console. */
    Token& TimeStruct (tm* std_tm, int32_t microseconds);

    /** Prints the given time to the console. */
    Token& Time (time_t t);

    /** Prints the given Script Timestamp to the console. */
    Token& Timestamp (time_us_t timestamp);

    /** Prints the given time to the console. */
    Token& DateTime (time_t t);

    /** Overloaded operator= clones the other Text. */
    Token& operator= (const Token& other);

    private:

    Slot slot;        //< The Token's slot.
    bool is_dynamic_; //< Flag for if this uses dynamic memory.

};  //< struct Text

}   //< namespace _

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    strand. */
inline _::Token& operator<< (_::Token& strand_a, _::Token& strand_b) {
    return strand_a.Write (strand_b);
}

/** Op << writes a nil-terminated UTF-8 or ASCII string to the
    strand. */
inline _::Token& operator<< (_::Token& strand, const char* string) {
    return strand.Write (string);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, int8_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, uint8_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, int16_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, uint16_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, int32_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, uint32_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, int64_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, uint64_t value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, float value) {
    return strand.Write (value);
}

/** Op << writes the given value to the strand. */
inline _::Token& operator<< (_::Token& strand, double value) {
    return strand.Write (value);
}

#endif  //< HEADER_FOR_SCRIPT_STRAND
