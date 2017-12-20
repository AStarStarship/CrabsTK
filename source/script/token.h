/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/token.h
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

#ifndef HEADER_FOR_SCRIPT_TOKEN
#define HEADER_FOR_SCRIPT_TOKEN

#include "operation.h"

#if SCRIPT_USING_TOKEN

namespace _ {

/** Group of functions for working with strings with explicitly declared end
    of buffer.
    Currently the read-write functions use sprintf. This is not the optimal 
    behavior and the functions needs to bypass the formating string.
    @ingroup Text
*/

/** Checks if the given char is a token.
    @param strand The char to check.
    @returns Returns true if the given char is a token. */
KABUKI bool TokenQualifies (const char* strand);

/** Checks if the given char is a token.
    @param strand The char to check.
    @returns Returns true if the given char is a valid token. */
KABUKI bool TokenQualifies (const char* text, const char* strand_end);

/** Reads the next token .
    @param  text     A nil-terminated string that may start with the token.
    @param  strand_end End of the text buffer.
    @param  token    A nil-terminated string that DOES NOT HAVE whitespace.
    @return A pointer to the end of the token or nil upon failure. */
KABUKI const char* TokenRead (const char* text, const char* strand_end,
                              char* target, char* target_end);

/** Compares the source and query char using the delimiter to terminate the
    query.
    @param  strand A nil-terminated string that may start with the token.
    @param  token  A nil-terminated string that DOES NOT HAVE whitespace.
    @return 0 if the tokens match, <0 if text < token, else, else >0. */
KABUKI int TokenCompare (const char* strand, const char* token);

/** Compares the source and query char using the delimiter to terminate the
    query.
    @param  text     A nil-terminated string that may start with the token.
    @param  strand_end End of the text buffer.
    @param  token    A nil-terminated string that DOES NOT HAVE whitespace.
    @return 0 if the tokens match, <0 if text < token, else, else >0. */
KABUKI int TokenCompare (const char* text, const char* strand_end,
                         const char* token);

/** Compares the source and query char using the delimiter to terminate the
    query.
    @param  strand A nil-terminated string that may start with the token.
    @param  token  A nil-terminated string that DOES NOT HAVE whitespace.
    @return A pointer to the end of the token or nil upon failure. */
KABUKI const char* TokenEquals (const char* strand, const char* token);

/** Compares the source and query char using the delimiter to terminate the
    query.
    @param  text     A nil-terminated string that may start with the token.
    @param  strand_end End of the text buffer.
    @param  token    A nil-terminated string that DOES NOT HAVE whitespace.
    @return A pointer to the end of the token or nil upon failure. */
KABUKI const char* TokenEquals (const char* text, const char* strand_end,
                                const char* token);

/** Gets the end of the current whitespace-delineated token.
    @param  strand A nil-terminated string.
    @return Returns a pointer to the end of the token. */
KABUKI const char* TokenEnd (const char* strand);

/** Gets the end of the current whitespace-delineated token.
    @param  text A nil-terminated string.
    @param  strand_end End of the text buffer.
    @return A pointer to the end of the token or nil upon failure. */
KABUKI const char* TokenEnd (const char* text, const char* strand_end);

}   //< namespace _

#endif  //< SCRIPT_USING_TOKEN
#endif  //< HEADER_FOR_SCRIPT_TOKEN
