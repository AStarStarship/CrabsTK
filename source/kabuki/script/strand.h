/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/strand.h
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

#ifndef HEADER_FOR_SCRIPT_SCRIBBLE
#define HEADER_FOR_SCRIPT_SCRIBBLE

#include "config.h"

namespace _ {

/** String utilities.
    @ingroup Strand

    A string is specifically a nil-terminated sequence of bytes. A strand
    may or not be a string, and often is the case is a strand is a token
    or a buffer overflow situation.
*/

/** Writes the given std_tm to the char buffer. */
KABUKI char* StrandWriteTime (char* begin, char* end, tm* std_tm);

/** Writes the given time to the char buffer. */
KABUKI char* TextWriteTime (char* begin, char* end, time_t t);

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
*/
KABUKI const char* StrandReadTime (const char* input, int* hour,
                                         int* minute, int* second);

/** Converts a keyboard input to char and deletes the char. 
*/
KABUKI const char* StrandReadTime (const char* input,
                                              tm* std_tm);

/** Converts a keyboard input to a time_t. */
KABUKI const char* StrandReadTime (const char* input, time_t& result);

/** Converts a keyboard input to a time_t. */
KABUKI char* StrandReadTime (char* input, time_t& result);

/** Converts the given value to a char. */
template<typename T>
inline char Char (T value) {
    if (value < 32) return ' ';
    return value;
}

/** Scrolls over to the end of the strand string.
    @return  Returns nil upon failure or a pointer to the nil-term char
             upon success.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI const char* StrandEnd (const char* begin);

/** Scrolls over to the next double quote mark.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI const char* StrandEnd (const char* begin, char delimiter);

/** Gets the length of the given char.
    @return  Returns -1 if the strand char is null.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI int StrandLength (const char* begin);

/** Gets the length of the given char.
    @return  Returns -1 if the strand char is null.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI int StrandLength (const char* begin, char delimiter);

/** Clones the given string.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
KABUKI char* StrandClone (const char* begin);

/** Clones the given string with given NON-ZERO delimiter.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
//KABUKI char* StrandClone (const char* begin, char delimiter);

/** Deletes the given string and clones the strand.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
KABUKI char* StrandReplace (const char* begin, char* string);

/** Clones the given string with given NON-ZERO delimiter.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
//KABUKI char* StrandReplace (const char* begin, char delimiter);

/** Converts the given string to a 8-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, int8_t& result);

/** Converts the given string to a 8-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, uint8_t& result);

/** Converts the given string to a 16-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, int16_t& result);

/** Converts the given string to a 16-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, uint16_t& result);

/** Converts the given string to a 32-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, int32_t& result);

/** Converts the given string to a 32-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, uint32_t& result);

/** Converts the given string to a 64-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, int64_t& result);

/** Converts the given string to a 64-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, uint64_t& result);

/** Converts the given string to a 32-bit floating-point number.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, float& result);

/** Converts the given string to a 64-bit floating-point number.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* begin, double& result);

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* StrandLineEnd (const char* begin,
                                      int num_columns);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the strand char. */
KABUKI const char* StrandSkipNumbers (const char* begin);

/** Skips the leading zeros of a number if there are any. */
KABUKI const char* StrandSkipZeros (const char* begin);

/** Skips all the spaces at the start of the char. */
KABUKI const char* StrandSkipSpaces (const char* begin);

/** Compares the source and query char as nil-terminated strings. */
KABUKI const char* StrandEquals (const char* begin, const char* query);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* StrandEquals (const char* begin, const char* query,
                                 char delimiter);

/** Compares the source and query char using the delimiter to terminate the
    query. */
//KABUKI char* StrandEquals (char* source, const char* query, char delimiter = 0);

/** Parses the given char for the given token, setting the following
    whitespace to null.
    @param  char The char to search.
    @param  token  The token to search for.
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
KABUKI const char* StrandRead (const char* begin, char* text,
                                char*end, char delimiter);

/** Searches the given char for the given char.
    @param  strand      The char to search.
    @param  query      The char to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the parsing failed and a pointer to the first char
            after the end of the token upon success. */
KABUKI const char* StrandFind (const char* begin, const char* query, char delimiter = 0);

/** Searches the given char for the given char, setting the following
    whitespace to null.
    @param  strand      The char to search.
    @param  query      The token to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
KABUKI char* StrandFind (char* begin, const char* query, char delimiter = 0);

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
KABUKI const char* TextEmpty ();

/** New line string. */
KABUKI const char* TextCR ();

/** Error header string. */
KABUKI const char* TextErrorHeader ();

/** New line and vertical bar "\n| " string. */
KABUKI const char* TextNewLine ();

/** Skips all the spaces at the start of the char.
    @param  begin Beginning address of the read buffer.
    @param  end   End address of the input buffer.
    @return A pointer to the end of the token read or if no token read. */
KABUKI const char* TextSkipSpaces (const char* begin, const char* end);

/** Writes the given strand to the text buffer.
    @param begin Beginning address of the write buffer.
    @param end   End address of the buffer.
    @param value The potentially unsafe string to write. */
KABUKI char* StrandWrite (char* begin, char* end, const char* strand);

/** Copies a char from the source to the text.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI char* StrandWrite (char* begin, char* end, const char* strand,
                        char delimiter);

/** Copies a char from the source to the text.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI char* StrandWrite (char* begin, char* end, const char* text,
                          const char* strand_end, char delimiter);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, int8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, uint8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, int16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, uint16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, int32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, uint32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, int64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, uint64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, float value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   End address of the buffer.
    @param value The value to write. */
KABUKI char* StrandWrite (char* begin, char* end, double value);

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* StrandLineEnd (const char* begin, const char* end,
                                  int num_columns);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the strand char. */
KABUKI const char* StrandNextNonNumber (const char* begin, const char* end);

/** Reads a whitespace-delineated token from the given text buffer.
    @param begin     Beginning address of the input buffer.
    @param end       End address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    strands. */
KABUKI const char* TextRead (const char* begin, const char* end, 
                             char* token, char* token_end);

/** Reads a explicitly-delineated token from the given text buffer.
    @param begin     Beginning address of the input buffer.
    @param end       End address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI const char* TextRead (const char* text, const char* end, 
                             char* token, char* token_end, char delimiter);

/** Reads a int8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             int8_t& result);

/** Reads a uint8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             uint8_t& result);

/** Reads a int16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             int16_t& result);

/** Reads a uint16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             uint16_t& result);

/** Reads a int32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             int32_t& result);

/** Reads a uint32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             uint32_t& result);

/** Reads a int64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end, 
                             int64_t& result);

/** Reads a uint64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             uint64_t& result);

/** Reads a float from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             float& result);

/** Reads a double from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    End address of the buffer.
    @param result The number read. */
KABUKI const char* TextRead (const char* begin, const char* end,
                             double& result);

}       //< namespace _
#endif  //< HEADER_FOR_SCRIPT_SCRIBBLE
