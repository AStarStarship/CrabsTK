/** kabuki::script
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

/** A portion of a string without a nil-term char.
    Without explicitly specifying the end of a string's buffer it is not 
    possible to determinately prove there is a nil-term char.
    @ingroup Strand
*/

/** Scrolls over to the end of the strand string.
    @return  Returns nil upon failure or a pointer to the nil-term char
             upon success.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI const char* StrandEnd (const char* target);

/** Scrolls over to the next double quote mark.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI const char* StrandEnd (const char* target, char delimiter);

/** Gets the length of the given char.
    @return  Returns -1 if the strand char is null.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI int StrandLength (const char* target);

/** Gets the length of the given char.
    @return  Returns -1 if the strand char is null.
    @warning This function is only safe to use on ROM strings with a nil-term
             char. */
KABUKI int StrandLength (const char* target, char delimiter);

/** Clones the given string.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
KABUKI char* StrandClone (const char* target);

/** Clones the given string with given NON-ZERO delimiter.
    @param  A nil-terminated string in ROM.
    @return Returns a new copy you must delete. */
KABUKI char* StrandClone (const char* target, char delimiter);

/** Converts the given string to a 8-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, int8_t& result);

/** Converts the given string to a 8-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, uint8_t& result);

/** Converts the given string to a 16-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, int16_t& result);

/** Converts the given string to a 16-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, uint16_t& result);

/** Converts the given string to a 32-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, int32_t& result);

/** Converts the given string to a 32-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, uint32_t& result);

/** Converts the given string to a 64-bit signed integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, int64_t& result);

/** Converts the given string to a 64-bit unsigned integer.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, uint64_t& result);

/** Converts the given string to a 32-bit floating-point number.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, float& result);

/** Converts the given string to a 64-bit floating-point number.
    @param  strand A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* StrandRead (const char* target, double& result);

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* StrandLineEnd (const char* target,
                                      int num_columns);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the strand char. */
KABUKI const char* StrandSkipNumbers (const char* target);

/** Skips the leading zeros of a number if there are any. */
KABUKI const char* StrandSkipZeros (const char* target);

/** Skips all the spaces at the start of the char. */
KABUKI const char* StrandSkipSpaces (const char* target);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* TokenEquals (const char* target, const char* token);

/** Compares the source and query char as nil-terminated strings. */
KABUKI const char* StrandEquals (const char* target, const char* query);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* StrandEquals (const char* target, const char* query,
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
KABUKI const char* StrandRead (const char* target, char* text,
                                char*text_end, char delimiter);


/** Parses the token by scrolling to the end of it and setting the end to null.
    @param  strand The char to search.
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
KABUKI char* StrandParseToken (char* target);

/** Searches the given char for the given char.
    @param  strand      The char to search.
    @param  query      The char to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the parsing failed and a pointer to the first char
            after the end of the token upon success. */
KABUKI const char* StrandFind (const char* target, const char* query, char delimiter = 0);

/** Searches the given char for the given char, setting the following
    whitespace to null.
    @param  strand      The char to search.
    @param  query      The token to search for.
    @param  delimiter The delimiter for the token, Example: '\"'
    @return Returns null if the search failed and a pointer to the first hit
            upon success. */
KABUKI char* StrandFind (char* target, const char* query, char delimiter = 0);

}       //< namespace _
#endif  //< HEADER_FOR_SCRIPT_SCRIBBLE
