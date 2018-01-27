/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_token.cc
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

#include "token.h"
#include "memory.h"
#include "clock.h"
#include "bsq.h"

namespace _ {

#if USING_SCRIPT_TEXT
const char* TokenEnd (const char* token) {
    char c = *token;
    while (c) {
        c = *(++token);
    }
    return token;
}

const char* TokenEnd (const char* token, char delimiter) {
    char c = *token;
    while (c) {
        if (c == delimiter) {
            return token;
        }
        c = *(++token);
    }
    return token;
}

int TokenLength (const char* token) {
    if (!token)
        return -1;
    int count = 0;
    //Write () << "Checking string length for " << string << '\n';
    char c = *token;
    //Write () << c;
    while (c != 0) {
        ++count;
        c = *(++token);
        //Write () << c;
    }
    //Write () << '\n';
    return count;
}

int TokenLength (const char* token, char delimiter) {
    if (!token) {
        return -1;
    }
    int count = 0;
    //Write () << "Checking string length for " << string << '\n';
    char c = *token;
    //Write () << c;
    while (!c) {
        if (c == delimiter) {
            return count;
        }
        ++count;
        c = *(++token);
        //Write () << c;
    }
    //Write () << '\n';
    return -1;
}

char* TokenClone (const char* token) {
    if (!token) {
        token = "";
    }
    int length = TokenLength (token);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    TokenWrite (clone, clone + length, token);
    return clone;
}

char* TokenClone (const char* token, char delimiter) {
    if (!token) {
        token = "";
    }
    int length = TokenLength (token);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    TokenWrite (clone, clone + length, token, delimiter);
    return clone;
}

const char* TokenSkipNumbers (const char* token) {
    if (!token) {
        return nullptr;
    }
    char c = *token;
    if (c == '-') {  // It might be negative.
        c = *(++token);
        if (!isdigit (c)) {  // it's not negative.
            return token - 1;
        }
        c = *(++token);
    }
    while (isdigit (c)) {
        c = *(++token);
    }
    return token;
}

const char* TokenEquals (const char* token_a, const char* token_b) {
    if (!token_a) {
        return nullptr;
    }
    if (!token_b) {
        return nullptr;
    }
    //Write () << "\nComparing \"" << target << "\" to \"" << query << "\"";

    char a = *token_a,
         b = *token_b;
    while (a) {
        Write () << a;
        if (a != b) { // Not a hit.
                      //Write () << "\nBut it's not a hit\n";
            return nullptr;
        }
        if (b == 0) { // Hit!
                      //Write () << "\nFound hit at ";
                      //PrintHexNL (target);
            return token_a;
        }
        a = *(++token_a);
        b = *(++token_b);
    }
    if (b != 0) {
        Write () << "\nNot a hit: no nil-term char found";
        return nullptr;
    }
    Write () << "\nFound hit at ";
    //PrintHexNL (target);
    return token_a; //< Find hit!
}

const char* TokenEquals (const char* begin, const char* end, const char* string_b) {
    if (!begin) {
        return nullptr;
    }
    if (begin > end) {
        return nullptr;
    }
    if (!string_b) {
        return nullptr;
    }
    //Write () << "\nComparing \"" << target << "\" to \"" << query << "\"";

    char a = *begin,
         b = *string_b;
    while (a) {
        Write () << a;
        if (a != b) { // Not a hit.
            //Write () << "\nBut it's not a hit\n";
            return nullptr;
        }
        if (b == 0) { // Hit!
            //Write () << "\nFound hit at ";
            //PrintHexNL (target);
            return begin;
        }
        if (begin > end) {
            return nullptr;
        }
        a = *(++begin);
        b = *(++string_b);
    }
    if (b != 0) {
        Write () << "\nNot a hit: no nil-term char found";
        return nullptr;
    }
    Write () << "\nFound hit at ";
    //PrintHexNL (target);
    return begin; //< Find hit!
}

const char* TokenEquals (const char* token_a, const char* token_b,
                         char delimiter) {
    //Write () << "Comparing \"" << target << "\" to \"" << query
    //    << "\"\n";
    if (!token_a) {
        return nullptr;
    }
    if (!token_b) {
        return nullptr;
    }

    //target = TokenSkipSpaces (target); //< I think this is token compare
    //query = TokenSkipSpaces (query);

    char i = *token_a,
         q = *token_b;
    while (i != delimiter) {
        if (i != q) { // Not a hit.
            //Write () << "\nnot a hit";
            return nullptr;
        }
        //Write () << i;
        i = *(++token_a);
        q = *(++token_b);
    }
    if (q != delimiter) {
        //Write () << "\nreached nil-term char but no q:\'" << q
        //    << "\' is not the delimiter.";
        return nullptr;
    }
    //Write () << "\nFound hit at ";
    //PrintHexNL (target);
    return token_a; //< Find hit!
}

const char* TokenEquals (const char* begin, const char* end, const char* query,
                          char delimiter) {
    //Write () << "Comparing \"" << target << "\" to \"" << query
    //    << "\"\n";
    if (!begin) {
        return nullptr;
    }
    if (begin > end) {
        return nullptr;
    }
    if (!query) {
        return nullptr;
    }

    //target = TokenSkipSpaces (target); //< I think this is token compare
    //query  = TokenSkipSpaces (query);

    char i = *begin,
        q = *query;
    while (i != delimiter) {
        if (i != q) { // Not a hit.
                      //Write () << "\nnot a hit";
            return nullptr;
        }
        if (begin > end) {
            return nullptr;
        }
        //Write () << i;
        i = *(++begin);
        q = *(++query);
    }
    if (q != delimiter) {
        //Write () << "\nreached nil-term char but no q:\'" << q
        //    << "\' is not the delimiter.";
        return nullptr;
    }
    //Write () << "\nFound hit at ";
    //PrintHexNL (target);
    return begin; //< Find hit!
}

const char* TokenLineEnd (const char* target, int num_columns) {
    char c;
    // Scroll to the end of the line.
    c = *target++;
    while (c) {
        if (num_columns-- < 0) { // We've reached the end.
            // Scroll left till we hit whitespace (if any).
            while (!IsSpace (c)) {
                c = *(--target);
            }
            // Then scroll past the whitespace.
            while (IsSpace (c)) {
                c = *(--target);
            }
            return target;
            c = *target++;
        }
        c = *target++;
    }
    return target - 1;
}

const char* TokenSkipZeros (const char* target) {
    if (target == nullptr)
        return nullptr;
    char c = *target,
        d;
    if (c != '0') {
        return target;
    }
    d = *target;
    // We know the first char is a '0' so just loop till c and d aren't '0'.
    while (c == d) {
        c = d;
        d = *(++target);
    }
    if (!isdigit (d))       //< Special case for "...00" char.
        return target - 1;
    return target;
}

const char* TokenSkipSpaces (const char* token) {
    if (!token) {
        return nullptr;
    }
    char c = *token;
    while (IsSpace (c)) {
        //Write () << '.';
        if (!c) { //< This isn't an error as far as I can see.
            return token;
        }
        ++token;
        c = *token;
    }
    return token;
}

const char* TokenFind (const char* target, const char* query, char delimiter) {
    if (target == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    char string = *target, //< Current target char.
         t      = *query, //< Current query char.
         c      = t;      //< The first char of the query we're searching for.
    if (c == delimiter)   //< We're not allowing empty queries.
        return nullptr;
    const char* start_of_query,
              * cursor = target;
    query = TokenSkipSpaces (query);

    // Scroll through each char and match it to the query char.
    while (string) {
        if (string == c) { // The first char matches:
            // Setup to compare the strings;
            start_of_query = target;
            cursor = query;
            t = c;
            // check the rest of the char:
            while (string == t) {
                string = *(++target);
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
        string = *(++target);
        t = *(++cursor);

    }
    // If we haven't found it by now it's not in the target.
    return nullptr;
}

const char* TokenEmpty () {
    return "";
}

const char* TokenCR () {
    return "\n";
}

const char* TokenErrorHeader () {
    return "\nError: ";
}

const char* TokenNewLine () {
    return "\n";
}

const char* TokenSkipSpaces (const char* token, const char* token_end) {
    if (!token) {
        return nullptr;
    }
    if (token > token_end) {
        return nullptr;
    }
    //Write () << "\nSkipping spaces: ";
    char c = *token;
    while (IsSpace (c)) {
        //Write () << '.';
        if (!c) {
            return nullptr;
        }
        if (++token > token_end) {
            return nullptr;
        }
        c = *token;
    }
    return token;
}

char* TokenWrite (char* target, char* target_end, const char* token) {
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }
    char* cursor = target;
    char c = *token;
    ++token;
    while (c) {
        if (cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return nullptr;
        }
        *cursor = c;
        ++cursor;
        c = *token;
        ++token;
    }
    *cursor = 0;
    return cursor;
}

char* TokenWrite (char* target, char* target_end, const char* token,
                   char delimiter) {
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }
    char* cursor = target;
    char s = *token;
    while (s != delimiter) {
        if (!s) {
            *target = 0; //< Replace the nil-term char.
            return nullptr;
        }
        *cursor = s;
        if (++cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return nullptr;
        }
        ++token;
        s = *token;
    }
    *cursor = s;
    return cursor;
}

char* TokenWrite (char* target, char* target_end, const char* token,
                   const char* token_end) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }
    if (token > token_end) {
        return nullptr;
    }
    char* cursor = target;
    char t = *token;
    while (t) {
        *cursor = t;
        if (++cursor > target_end) {
            *target = 0;
            return nullptr;
        }
        if (++token > token_end) {
            *target = 0;
            return nullptr;
        }
        t = *token;
    }
    *cursor = t;
    return cursor;
}

char* TokenWrite (char* target, char* target_end, const char* token,
                   const char* token_end, char delimiter) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }
    if (token > token_end) {
        return nullptr;
    }
    char* cursor = target;
    char t = *token;
    while (t != delimiter) {
        if (!t) {
            //Write () << "\nThere was a !t error in TokenWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return nullptr;
        }
        if (++token > token_end) {
            *target = 0;
            return nullptr;
        }
        t = *token;
    }
    *target = t;
    return target;
}

char* TokenWrite (char* target, char* target_end, int8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, uint8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, int16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, uint16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, int32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, uint32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, int64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%lli", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, uint64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t result = sprintf_s (target, target_end - target, "%llu", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    Write () << "\n!!! " << target << " result:" << result;
    return target + result;
}

char* TokenWrite (char* target, char* target_end, float value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

char* TokenWrite (char* target, char* target_end, double value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

const char* TokenLineEnd (const char* token, const char* token_end,
                          int num_columns) {
    if (!token) {
        return nullptr;
    }
    if (token_end > token) {
        return nullptr;
    }
    char c;
    // Scroll to the end of the line.
    c = *token++;
    while (c) {
        if (token_end > token) {
            return nullptr;
        }
        if (num_columns-- < 0) { // We've reached the end.

            // Scroll left till we hit whitespace (if any).
            while (!IsSpace (c)) {
                c = *(--token);
            }
            // Then scroll past the whitespace.
            while (IsSpace (c)) {
                c = *(--token);
            }
            return token;
            c = *token++;
        }
        c = *token++;
    }
    return token - 1;
}

const char*TokenNextNonNumber (const char* token, const char* token_end) {
    if (token > token_end) {
        return nullptr;
    }
    char c = *token;
    if (c == '-') {  // It might be negative.
        if (token > token_end) {
            return nullptr;
        }
        c = *(++token);
        if (!isdigit (c)) {  // it's not negative.
            return token - 1;
        }
        c = *(++token);
    }
    while (isdigit (c)) {
        if (token > token_end) {
            return nullptr;
        }
        c = *(++token);
    }
    return token;
}

bool TokenQualifies (const char* token) {
    if (token == nullptr) {
        return false;
    }
    char c = *token;
    while (c) {
        if (!IsSpace (c)) {
            return true;
        }
        c = *(++token);
    }
    return false;
}

bool TokenQualifies (const char* text, const char* token_end) {
    if (!text) {
        return false;
    }
    if (text > token_end) {
        return false;
    }
    char c = *text;
    while (c) {
        if (!IsSpace (c)) {
            // The token must end at or before the target_end.
            do {
                if (++text > token_end) {
                    return false;
                }
                c = *text;
                if (!IsSpace (c)) {
                    return true;
                }
            } while (c);
            return true;
        }
        if (++text > token_end) {
            return false;
        }
        c = *text;
    }
    return false;
}

const char* TokenEquals (const char* token, const char* token) {
    token = TokenSkipSpaces (token);
    if (!token) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }

    char a = *token,
         b = *token;
    int  result;

    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << "\nComparing \"" << token << "\" to \"" << token << "\"";
    #endif

    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << "\nb - a = " << b << " - " << a << " = " << result;
        #endif
        if (result) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is not a hit.";
            #endif
            return nullptr;
        }
        if (!a) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is a partial match but !a.";
            #endif
            return nullptr;
        }
        ++token;
        ++token;
        a = *token;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << " is only a partial match but found " << (a ? "a" : "space");
        #endif
        return nullptr;
    }
    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << " is a match!";
    #endif
    return token;
}

const char* TokenEquals (const char* text, const char* token_end,
                         const char* token) {
    text = TokenSkipSpaces (text, token_end);
    if (!text) {
        return nullptr;
    }
    if (!token) { // We're assuming token is in ROM and has no whitespace.
        return nullptr;
    }
    char a = *text,
         b = *token;
    int  result;

    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << "\nComparing \"" << text << "\" to \"" << token << "\"";
    #endif

    a = *text;
    b = *token;
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        //Write () << "\nb - a = " << b << " - " << a << " = " << result;
        if (result) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " has unmatched chars b - a = " << b << " - " << a << " = "
                 << result;
            #endif
            return nullptr;
        }
        if (!a) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is a partial match but !a.";
            #endif
            return nullptr;
        }
        if (++text > token_end) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " but text buffer overflowed!";
            #endif
            return nullptr;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << " is only a partial match but found " << (a ? "a" : "space");
        #endif
        return nullptr;
    }
    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << " is a match!";
    #endif
    return text;
}

int TokenCompare (const char* token, const char* token) {
    int a,
        b,
        result;
    if (!token) {
        if (!token) {
            return 0;
        }
        return *token;
    }
    if (!token) {
        return 0 - *token;
    }

    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << "\nComparing \"" << text << "\" to \"" << token << "\"";
    #endif
    a = *token;
    b = *token;
    if (!a) {
        if (!a) {
            return 0;
        }
        return b;
    }
    if (!b) { // I like !t code rather than !c code. :-)
        if (!a) {
            return 0;
        }
        return 0 - a;
    }
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << "\nb - a = " << b << " - " << a << " = " << result;
        #endif
        if (result) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is not a hit.";
            #endif
            return result;
        }
        if (!a) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is a partial match but !a.";
            #endif
            return result;
        }
        ++token;
        ++token;
        a = *token;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << " is only a partial match but found " << (a?"a":"space");
        #endif
        return b - a;
    }
    return 0;
}

int TokenCompare (const char* text, const char* token_end,
                  const char* token) {
    char a = *text,
        b = *token;
    int result;

    if (!text) {
        if (!token) {
            return 0;
        }
        a = 0;
        b = *token;
        return b - a;
    }
    if (!token) {
        a = *text;
        b = 0;
        return b - a;
    }
    if (text > token_end) {
        return *token;
    }
    // Algorithm combines loops for better performance.

    a = *text;
    b = *token;
    if (!a) {
        if (!b) {
            return 0;
        }
        return b;
    }
    if (!b) {
        if (!a) {
            return 0;
        }
        return 0 - a;
    }
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << "\nb - a = " << b << " - " << a << " = " << result;
        #endif
        if (result) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is not a hit.";
            #endif
            return result;
        }
        if (!a) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " is a partial match but !a.";
            #endif
            return result;
        }
        if (++text > token_end) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " but buffer overflowed!";
            #endif
            return result;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << " is only a partial match but found " << (a?"a":"space");
        #endif
        return b - a;
    }
    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << " is a match!";
    #endif
    return 0;
}

const char* TokenEnd (const char* target) {
    if (!target) {
        return nullptr;
    }
    target = TokenSkipSpaces (target);

    char c = *target;
    while (c) {
        if (IsSpace (c))
            return target + 1;
        c = *(++target);
    }
    // We're skipping spaces because we're expecting good data so this is an
    // error.
    return nullptr;
}

const char* TokenEnd (const char* text, const char* token_end) {
    if (!text) {
        return nullptr;
    }
    if (text > token_end) {
        return nullptr;
    }
    text = TokenSkipSpaces (text, token_end);

    char c = *text;
    while (c) {
        if (IsSpace (c)) {
            return text;
        }
        if (++text > token_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
}

Token& Token::Line (char token, const char* header, int length) {
    //Write () << header << "Writing Line with length " << length;
    char* cursor = cursor_,
        *stop = cursor,
        *end = GetEnd ();
    stop = TokenWrite (cursor, end, header);
    if (!stop) {
        //Write () << "\nError writing header!";
        return *this;
    }
    //Write () << "... wrote " << TokenLength (cursor) << " chars.";
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    //Write () << "\nnew_stop_length:" << stop - cursor;

    while (cursor < stop) {
        *cursor++ = token;
    }
    *cursor = 0;
    //Write () << "\nWrote " << TokenLength (cursor_) << " chars: " 
    //          << cursor_ << '\n';
    cursor_ = cursor;
    return *this;
}

Token& Token::StringLine (const char* string, int num_columns) {
    //Line ();
    char* cursor = cursor_,
        * stop   = cursor + num_columns + 1, //< +1 for nil-term char.
        * end    = GetEnd ();
    const char* read = string;
    if (num_columns < 1) {
        return *this;
    }
    if (!string) {
        return *this;
    }
    if (cursor == end) {
        return *this;
    }
    if (stop > end) { // Chop of some of the columns.
        stop = end;
    }

    while (cursor < stop) {
        char c = *read++;
        if (!c) {
            *cursor++ = '_';
            read = string;
        }
    }
    *cursor_ = 0;
    cursor_ = cursor;
    return *this;
}

Token& Token::LineBreak (const char* message, int top_bottom_margin,
                           char c, int num_columns) {
    Lines (top_bottom_margin);
    *this << "\n" << message;
    return Line (c, "\n", num_columns);
}

Token& Token::Lines (int num_rows) {
    char* cursor = cursor_,
        *end = GetEnd (),
        *stop = cursor + num_rows + 1;
    while (cursor < stop) {
        *cursor++ = '\n';
    }
    *cursor = 0;
    cursor_ = cursor + 1;
    return *this;
}

Token& Token::Hex (const void* pointer) {
    // @todo Replace with PrintHex.
    int bytes_written = sprintf_s (cursor_, end_ - cursor_,
                                   "0x%p", pointer);
    cursor_ += bytes_written;
    return *this;
}

Token& Token::Hex (byte c) {
    uint16_t chars = MemoryByteToUpperCaseHex (c);
    return *this << (char)chars << ((char)(chars >> 8)) << ' ';
}

Token& Token::Memory (const void* address, const void* stop) {
    *this << "\n" << 0;
    //  columns
    for (int i = 8; i <= 66; i += 8) {
        Right (i, 8);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 65; ++i) {
        *this << '_';
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < stop) {
        Write ('\n') << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= stop)
                temp = 'x';
            putchar (temp);
            ++chars;
        }
        *this << "| " << Hex (chars + MemoryVector (address, stop));// (chars - 64);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 64; ++i) {
        Write ('_');
    }
    return *this << "| " << Hex (chars + MemoryVector (address, stop));
}

Token& Token::Memory (const void* address, int size) {
    return Memory (address, reinterpret_cast<const char*> (address) + size);
}

Token& Token::Error (const char* message, const char* end_string) {
    return Write ("\nError: ").Write (message).Write (end_string);
}

Token& Token::Bsq (const uint_t* params) {
    return BsqPrint (params, *this);
}

Token& Token::Print (char c) {
    if (Space () < 1) {
        return *this;
    }
    if (c < ' ') {
        return *this;
    }
    if (c == 127) {
        return *this;
    }
    return *this << c;
}

Token& Token::NumberLine (int index) {
    Write ('\n');
    enum {
        kMaxBufferSize = (sizeof (int) == 2)
        ? 7
        : (sizeof (int) == 4)
        ? 11
        : 128
    };
    char buffer[kMaxBufferSize];
    TokenWrite (buffer, buffer + kMaxBufferSize - 1, index);
    int length = TokenLength (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1)) {
        printf ("%s_", buffer);
    }
    for (int j = 0; j < lettersLeft; ++j) {
        *this << buffer[j];
    }
    return Write ('\n');
}

Token& Token::NewLine () {
    return *this << "\n";
}

Token& Token::Heading (const char* input) {
    return *this << "\n" << input << '\n';
}

Token& Token::Break (const char* header, char c, int num_lines,
                       int console_width) {
    for (int i = 0; i < num_lines; ++i) {
        Write ('\n');
    }
    *this << header;
    int length = TokenLength (header);
    for (int i = 0; i < console_width - length; ++i) {
        *this << c;
    }
    return Write ('\n');
}

Token& Token::Centered (const char* string, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return *this;
    }
    // We need to leave at least one space to the left and right of
    int length = TokenLength (string);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                Write ('\n');
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i) {
        Write ('\n');
    }
    printf (string);
    for (offset = width - length - offset; offset <= width; ++offset) {
        Write ('\n');
    }
    return *this;
}
/*
Text& Text::Centered (const char* input, int num_columns) {
    char c;   //< The current char.
    if (num_columns <= 4) { // We need at least 4 columns for this to work.
        return *this;
    }
    if (!input) {
        return *this;
    }
    do {
        const char* end_line = TokenLineEnd (input, num_columns);
        size_t row_length = end_line - input,
        num_left_spaces = (num_columns / 2) - (row_length / 2),
        i;
        //  left whitespace.
        for (i = num_left_spaces; i != 0; --i) {
            Char (' ');
        }
        //  input.
        for (i = row_length; i != 0; --i) {
            *this << *input++;
        }
        for (i = num_columns - num_left_spaces - row_length; i != 0; --i) {
            Char (' ');
        }
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
    while (IsSpace (c = *input++));
    } while (c);
    return *this;
}*/

Token& Token::Centered (const char* input, int width, bool is_last,
                          char column_delimeter) {
    if (width < 1) {
        return *this;
    }
    *this << column_delimeter;
    int length = TokenLength (input),
        num_spaces;

    if (width < length) {
        char format[32];
        format[0] = '%';
        TokenWrite (&format[1], format + 32, (is_last ? width - 2 : width - 1));
        *this << column_delimeter;
        sprintf_s (GetBegin (), Space (), format, input);
        if (is_last) {
            *this << column_delimeter << '\n';
        }
    }
    num_spaces = width - length - 1 - (is_last ? 1 : 0);
    for (int i = 0; i < num_spaces / 2; ++i) {
        Write (' ');
    }
    *this << input;
    for (int i = 0; i < num_spaces - (num_spaces / 2); ++i) {
        Write (' ');
    }
    if (is_last) {
        *this << column_delimeter << '\n';
    }
    return *this;
}

Token& Token::ColumnBreak (int num_columns, char column_delimeter,
                             char break_char, int width) {
    int column_width = width / num_columns;
    for (int i = 0; i < num_columns - 1; ++i) {
        *this << column_delimeter;
        for (int j = 0; j < column_width - 1; ++j) {
            *this << break_char;
        }
    }
    *this << column_delimeter;
    for (int j = 0; j < column_width - 2; ++j) {
        *this << break_char;
    }
    return *this << column_delimeter << '\n';
}

Token& Token::Right (const char* token, int num_columns) {
    char c;  //< The current char.

    if (num_columns <= 4) {// We need at least 4 columns for this to work.
        return *this;
    }
    if (!token) {
        return *this;
    }
    do {
        const char* end_line = TokenLineEnd (token, num_columns);
        size_t row_length = end_line - token,
            num_left_spaces = num_columns - row_length,
            i;
        //  left whitespace.
        for (i = num_left_spaces; i != 0; --i) {
            Write (' ');
        }
        //  input.
        for (i = row_length; i != 0; --i) {
            *this << *token++;
        }
        // Increment the input pointer and scroll past the whitespace
        token += row_length;
        while (IsSpace (c = *token++));
    } while (c);
    return *this;
}

Token& Token::Right (int8_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint8_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (int16_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint16_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (int32_t value, int num_columns) {
    char buffer[16];
    sprintf_s (buffer, 16, "%i", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint32_t value, int num_columns) {
    char buffer[16];
    sprintf_s (buffer, 16, "%u", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (int64_t value, int num_columns) {
    char buffer[24];
    sprintf_s (buffer, 24, "%lld", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint64_t value, int num_columns) {
    char buffer[24];
    sprintf_s (buffer, 24, "%llu", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (float value, int num_columns) {
    char buffer[kMaxDigitsFloat];
    sprintf_s (buffer, kMaxDigitsFloat, "%f", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (double value, int num_columns) {
    char buffer[kMaxDigitsDouble];
    sprintf_s (buffer, kMaxDigitsDouble, "%f", value);
    return Right (buffer, num_columns);
}

Token& Token::Token (const char* token) {
    if (!token) {
        return *this;
    }
    char c = *token;
    while (c && !IsSpace (c)) {
        *this << c;
    }
    return *this;
}

Token& Token::Token (const char* token, const char* token_end) {
    if (!token) {
        return *this;
    }
    if (token > token_end) {
        return *this;
    }
    char c = *token;
    while (c && !IsSpace (c)) {
        *this << c;
        if (++token > token_end) {
            return *this;
        }
    }
    return *this;
}

Token& Token::TimeStruct (std::tm* std_tm) {
    if (!std_tm) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1
        << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec;
}

Token& Token::TimeStruct (std::tm* std_tm, int32_t microseconds) {
    if (!std_tm) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1
        << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec << ":"
        << microseconds;
}

Token& Token::Time (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << TimeStruct (&std_tm);
}

Token& Token::Timestamp (time_us_t timestamp) {
    time_t t = ClockGetSeconds (timestamp);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    int32_t microseconds = ClockGetMicroseconds (timestamp);
    return TimeStruct (&std_tm, microseconds);
}

Token& Token::TimeDate (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << std_tm.tm_hour << ":" << std_tm.tm_min << ":"
        << std_tm.tm_sec;
}

/*
Token& Token::Page (const char* input, int indentation,
                      char bullet, int index, int tab_size,
                      int num_columns) {
    num_columns -= 4;
    *this << "\n";
    int cursor; //< The column number of the cursor.
    char c = *input++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || input == nullptr) { //< It's an empty input.
        for (int i = num_columns; i > 0; --i) {
            Write (' ');
        }
        *this << "\n\n";
        return *this;
    }

    // Make the input for the bullet.
    if (isdigit (bullet)) { // Then we have to print a number bullet.
        TokenWrite (buffer, buffer + 15, index);
        //< + 2 for "\n" - 2 for the bullet offset.
        //char format[16];
        //format[0] = '%';
        //sprintf_s (&format[1], 16, "%%%us", indentation * tab_size);
        //printf (format, buffer);
    }
    else if (isalpha (bullet)) { // Then it's a lower case bullet.
        cursor = 0;
        bullet += index % 26;  //<
        for (; index > 0; index -= 26)
            buffer[cursor++] = bullet;
        buffer[cursor] = '\0';
    }
    else {
        buffer[0] = bullet;
        buffer[1] = 0;
    }
    // s the char.
    int num_spaces = tab_size * indentation;
    while (c) {
        for (cursor = num_spaces; cursor > 0; --cursor)
            Write (' ');
        cursor = num_spaces;
        while (c && (cursor++ < num_columns)) {
            *this << c;
            c = *input++;
        }
        *this << " |\n";
    }
    return  Write ('\n');
}*/

const char* TokenRead (const char* token, const char* token_end,
                       char* token, char* token_end) {
    return TokenWrite (token, token_end, token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                      char* token, char* token_end, char delimiter) {
    return TokenWrite (token, token_end, token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (token, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (token, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (token, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (token, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (token, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (token, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    __int64 extra_copy;
    if (!sscanf_s (token, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    unsigned __int64 extra_copy;
    if (!sscanf_s (token, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (token, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, const char* token_end,
                       double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (token, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenNextNonNumber (token, token_end);
}

const char* TokenRead (const char* token, int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (token, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (token, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (token, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (token, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (token, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (token, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int64_t extra_copy;
    if (!sscanf_s (token, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint64_t extra_copy;
    if (!sscanf_s (token, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* token, float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (token, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenSkipNumbers (token);
}

const char* TokenRead (const char* string, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (string, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TokenSkipNumbers (string);
}

const char* TokenRead (const char* text, const char* token_end,
                       char* target, char* target_end) {
    text = TokenSkipSpaces (text, token_end);
    if (!text) {
        return nullptr;
    }
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }

    char c = *text;
    while (c && !IsSpace (c)) {
        #if DEBUG_KABUKI_DATA_TOKEN
        Write () << c;
        #endif
        if (++text > token_end) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " but text buffer overflowed!";
            #endif
            return nullptr;
        }
        if (++target > target_end) {
            #if DEBUG_KABUKI_DATA_TOKEN
            Write () << " but target buffer overflowed!";
            #endif
            return nullptr;
        }
        *target = c;
        c = *text;
    }
    #if DEBUG_KABUKI_DATA_TOKEN
    Write () << '\"';
    #endif
    *target = 0;
    return text;

}

const char* TokenRead (const char* target, char* token,
                         char* token_end, char delimiter) {
    //Write () << "\nparse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n";
    if (!target) {
        Write () << "\n!target";
        return nullptr;
    }
    if (!token) {
        Write () << "\n!token";
        return nullptr;
    }
    if (token > token_end) {
        Write () << "\ndestination > token_end";
        return nullptr;
    }
    //Write () << delimiter;
    char c = *target; //< Current char from target.
    while (c) {
        if (c == delimiter) {
            //Write () << delimiter << " parsed.\n";
            *token = 0;
            if (delimiter == 0) {
                return target;
            }
            return target + 1;
        }
        //Write () << c;
        if (target > token_end) {
            Write () << "\nBuffer overflow!";
            return nullptr;
        }
        *token++ = c;
        c = *(++target);
    }
    //Write () << delimiter << " done.\n";
    *token = 0;
    if (delimiter == 0) {
        return target;
    }
    return target + 1;
}

Token::Token (size_t size, char* buffer) :
    is_dynamic_ (!buffer){
    if (size < kSizeMin) {
        size = kSizeMin;
    }
    if (!buffer) {
        buffer = new char[size];
    }
    *buffer = 0;
    begin_  = buffer;
    cursor_ = buffer;
    end_    = buffer + size;
    #if SCRIPT_DEBUG == SCRIPT_STRAND
    MemoryClear (buffer, kSize);
    #endif  //< SCRIPT_DEBUG == SCRIPT_STRAND
}

Token::Token (const Token& other) :
    is_dynamic_ (other.is_dynamic_) {
    if (is_dynamic_) { // Clone the dynamic memory.
        size_t size = other.end_ - other.begin_;
        char* buffer = new char[size];
        begin_ = buffer;
        if (!TokenWrite (buffer, buffer + size, other.begin_)) {
            *begin_ = 0;
        }
        cursor_ = buffer;
        end_    = buffer + size;
    } else { // Just shallow copy.
        begin_      = other.begin_;
        cursor_     = other.cursor_;
        end_        = other.end_;
        is_dynamic_ = other.is_dynamic_;
    }
    // Nothing to do here! ({:->)-+=<
}

Token::~Token () {
    if (!is_dynamic_) {
        return;
    }
    delete cursor_;
}

bool Token::IsDynamic () {
    return is_dynamic_;
}

const char* Token::SetSize (size_t new_size) {
    if (!is_dynamic_) {
        return "Token is not dynamic";
    }
    if (new_size < kSizeMin) {
        return "new_size too small";
    }
    delete begin_;
    begin_ = new char[new_size];
    return nullptr;
}

const char* Token::Resize (size_t new_size) {
    if (!is_dynamic_) {
        return "Can't resize this Token";
    }
    if (!new_size) {
        return "new_size can't be nil";
    }
    char* new_buffer = TokenClone (begin_);
    if (!new_buffer) {
        return "Error cloning token";
    }
    delete begin_;
    begin_ = new_buffer;
    return nullptr;
}

Token& Token::Clear () {
    *begin_ = 0;
    cursor_ = begin_;
    return *this;
}

int Token::Length () const {
    return (int)(cursor_ - begin_);
}

int Token::Space () const {
    return (int)(end_ - cursor_);
}

Token& Token::SetEquals (const Token& other) {
    if (this == &other) {
        return *this;
    }
    int other_count = other.Length ();
    MemoryCopy (begin_, begin_ + other_count, other.begin_,
                other.begin_ + other_count);
    cursor_ = begin_ + other_count;
    return *this;
}

char* Token::GetBegin () {
    return begin_;
}

bool Token::SetCursor (char* new_cursor) {
    if (new_cursor < begin_) {
        return false;
    }
    if (new_cursor > end_) {
        // Can't be nil-term char.
        return false;
    }
    cursor_ = new_cursor;
    return true;
}

char* Token::GetCursor () {
    return cursor_;
}

char* Token::GetEnd () {
    return end_;
}

Token& Token::operator= (const Token& other) {
    return SetEquals (other);
}

Token& Token::Write (const char* token) {
    char* cursor = TokenWrite (cursor_, GetEnd (), token);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (int8_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite int8_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (uint8_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite uint8_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (int16_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite int16_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (uint16_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite uint16_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (int32_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite int32_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (uint32_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite uint32_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (int64_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite int64_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (uint64_t value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //Write () << "\nWrite uint64_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (float value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    //Write () << "\nWrite float:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (double value) {
    char* cursor = TokenWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    //Write () << "\nWrite double:" << begin_;
    cursor_ = cursor;
    return *this;
}

Token& Token::Write (Token& token) {
    return Write (token.GetBegin ());
}

Token& Token::Line (char token, const char* header, int length) {
    //Write () << header << "Writing Line with length " << length;
    char* cursor = cursor_,
        *stop = cursor,
        *end = GetEnd ();
    stop = TokenWrite (cursor, end, header);
    if (!stop) {
        //Write () << "\nError writing header!";
        return *this;
    }
    //Write () << "... wrote " << TokenLength (cursor) << " chars.";
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    //Write () << "\nnew_stop_length:" << stop - cursor;

    while (cursor < stop) {
        *cursor++ = token;
    }
    *cursor = 0;
    //Write () << "\nWrote " << TokenLength (cursor_) << " chars: " 
    //          << cursor_ << '\n';
    cursor_ = cursor;
    return *this;
}

Token& Token::StringLine (const char* string, int num_columns) {
    //Line ();
    char* cursor = cursor_,
        * stop   = cursor + num_columns + 1, //< +1 for nil-term char.
        * end    = GetEnd ();
    const char* read = string;
    if (num_columns < 1) {
        return *this;
    }
    if (!string) {
        return *this;
    }
    if (cursor == end) {
        return *this;
    }
    if (stop > end) { // Chop of some of the columns.
        stop = end;
    }

    while (cursor < stop) {
        char c = *read++;
        if (!c) {
            *cursor++ = '_';
            read = string;
        }
    }
    *cursor_ = 0;
    cursor_ = cursor;
    return *this;
}

Token& Token::LineBreak (const char* message, int top_bottom_margin,
                           char c, int num_columns) {
    Lines (top_bottom_margin);
    *this << "\n" << message;
    return Line (c, "\n", num_columns);
}

Token& Token::Lines (int num_rows) {
    char* cursor = cursor_,
        *end = GetEnd (),
        *stop = cursor + num_rows + 1;
    while (cursor < stop) {
        *cursor++ = '\n';
    }
    *cursor = 0;
    cursor_ = cursor + 1;
    return *this;
}

Token& Token::Hex (const void* pointer) {
    // @todo Replace with PrintHex.
    int bytes_written = sprintf_s (cursor_, end_ - cursor_,
                                   "0x%p", pointer);
    cursor_ += bytes_written;
    return *this;
}

Token& Token::Hex (byte c) {
    uint16_t chars = MemoryByteToUpperCaseHex (c);
    return *this << (char)chars << ((char)(chars >> 8)) << ' ';
}

Token& Token::Memory (const void* address, const void* stop) {
    *this << "\n" << 0;
    //  columns
    for (int i = 8; i <= 66; i += 8) {
        Right (i, 8);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 65; ++i) {
        *this << '_';
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < stop) {
        Write ('\n') << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= stop)
                temp = 'x';
            putchar (temp);
            ++chars;
        }
        *this << "| " << Hex (chars + MemoryVector (address, stop));// (chars - 64);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 64; ++i) {
        Write ('_');
    }
    return *this << "| " << Hex (chars + MemoryVector (address, stop));
}

Token& Token::Memory (const void* address, int size) {
    return Memory (address, reinterpret_cast<const char*> (address) + size);
}

Token& Token::Error (const char* message, const char* end_string) {
    return Write ("\nError: ").Write (message).Write (end_string);
}

Token& Token::Bsq (const uint_t* params) {
    return BsqPrint (params, *this);
}

Token& Token::Print (char c) {
    if (Space () < 1) {
        return *this;
    }
    if (c < ' ') {
        return *this;
    }
    if (c == 127) {
        return *this;
    }
    return *this << c;
}

Token& Token::NumberLine (int index) {
    Write ('\n');
    enum {
        kMaxBufferSize = (sizeof (int) == 2)
        ? 7
        : (sizeof (int) == 4)
        ? 11
        : 128
    };
    char buffer[kMaxBufferSize];
    TokenWrite (buffer, buffer + kMaxBufferSize - 1, index);
    int length = TokenLength (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1)) {
        printf ("%s_", buffer);
    }
    for (int j = 0; j < lettersLeft; ++j) {
        *this << buffer[j];
    }
    return Write ('\n');
}

Token& Token::NewLine () {
    return *this << "\n";
}

Token& Token::Heading (const char* input) {
    return *this << "\n" << input << '\n';
}

Token& Token::Break (const char* header, char c, int num_lines,
                       int console_width) {
    for (int i = 0; i < num_lines; ++i) {
        Write ('\n');
    }
    *this << header;
    int length = TokenLength (header);
    for (int i = 0; i < console_width - length; ++i) {
        *this << c;
    }
    return Write ('\n');
}

Token& Token::Centered (const char* string, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return *this;
    }
    // We need to leave at least one space to the left and right of
    int length = TokenLength (string);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                Write ('\n');
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i) {
        Write ('\n');
    }
    printf (string);
    for (offset = width - length - offset; offset <= width; ++offset) {
        Write ('\n');
    }
    return *this;
}
/*
Text& Text::Centered (const char* input, int num_columns) {
    char c;   //< The current char.
    if (num_columns <= 4) { // We need at least 4 columns for this to work.
        return *this;
    }
    if (!input) {
        return *this;
    }
    do {
        const char* end_line = TokenLineEnd (input, num_columns);
        size_t row_length = end_line - input,
        num_left_spaces = (num_columns / 2) - (row_length / 2),
        i;
        //  left whitespace.
        for (i = num_left_spaces; i != 0; --i) {
            Char (' ');
        }
        //  input.
        for (i = row_length; i != 0; --i) {
            *this << *input++;
        }
        for (i = num_columns - num_left_spaces - row_length; i != 0; --i) {
            Char (' ');
        }
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
    while (IsSpace (c = *input++));
    } while (c);
    return *this;
}*/

Token& Token::Centered (const char* input, int width, bool is_last,
                          char column_delimeter) {
    if (width < 1) {
        return *this;
    }
    *this << column_delimeter;
    int length = TokenLength (input),
        num_spaces;

    if (width < length) {
        char format[32];
        format[0] = '%';
        TokenWrite (&format[1], format + 32, (is_last ? width - 2 : width - 1));
        *this << column_delimeter;
        sprintf_s (GetBegin (), Space (), format, input);
        if (is_last) {
            *this << column_delimeter << '\n';
        }
    }
    num_spaces = width - length - 1 - (is_last ? 1 : 0);
    for (int i = 0; i < num_spaces / 2; ++i) {
        Write (' ');
    }
    *this << input;
    for (int i = 0; i < num_spaces - (num_spaces / 2); ++i) {
        Write (' ');
    }
    if (is_last) {
        *this << column_delimeter << '\n';
    }
    return *this;
}

Token& Token::ColumnBreak (int num_columns, char column_delimeter,
                             char break_char, int width) {
    int column_width = width / num_columns;
    for (int i = 0; i < num_columns - 1; ++i) {
        *this << column_delimeter;
        for (int j = 0; j < column_width - 1; ++j) {
            *this << break_char;
        }
    }
    *this << column_delimeter;
    for (int j = 0; j < column_width - 2; ++j) {
        *this << break_char;
    }
    return *this << column_delimeter << '\n';
}

Token& Token::Right (const char* token, int num_columns) {
    char c;  //< The current char.

    if (num_columns <= 4) {// We need at least 4 columns for this to work.
        return *this;
    }
    if (!token) {
        return *this;
    }
    do {
        const char* end_line = TokenLineEnd (token, num_columns);
        size_t row_length = end_line - token,
            num_left_spaces = num_columns - row_length,
            i;
        //  left whitespace.
        for (i = num_left_spaces; i != 0; --i) {
            Write (' ');
        }
        //  input.
        for (i = row_length; i != 0; --i) {
            *this << *token++;
        }
        // Increment the input pointer and scroll past the whitespace
        token += row_length;
        while (IsSpace (c = *token++));
    } while (c);
    return *this;
}

Token& Token::Right (int8_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint8_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (int16_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint16_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (int32_t value, int num_columns) {
    char buffer[16];
    sprintf_s (buffer, 16, "%i", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint32_t value, int num_columns) {
    char buffer[16];
    sprintf_s (buffer, 16, "%u", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (int64_t value, int num_columns) {
    char buffer[24];
    sprintf_s (buffer, 24, "%lld", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (uint64_t value, int num_columns) {
    char buffer[24];
    sprintf_s (buffer, 24, "%llu", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (float value, int num_columns) {
    char buffer[kMaxDigitsFloat];
    sprintf_s (buffer, kMaxDigitsFloat, "%f", value);
    return Right (buffer, num_columns);
}

Token& Token::Right (double value, int num_columns) {
    char buffer[kMaxDigitsDouble];
    sprintf_s (buffer, kMaxDigitsDouble, "%f", value);
    return Right (buffer, num_columns);
}

Token& Token::Token (const char* token) {
    if (!token) {
        return *this;
    }
    char c = *token;
    while (c && !IsSpace (c)) {
        *this << c;
    }
    return *this;
}

Token& Token::Token (const char* token, const char* token_end) {
    if (!token) {
        return *this;
    }
    if (token > token_end) {
        return *this;
    }
    char c = *token;
    while (c && !IsSpace (c)) {
        *this << c;
        if (++token > token_end) {
            return *this;
        }
    }
    return *this;
}

Token& Token::TimeStruct (std::tm* std_tm) {
    if (!std_tm) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1
        << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec;
}

Token& Token::TimeStruct (std::tm* std_tm, int32_t microseconds) {
    if (!std_tm) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1
        << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec << ":"
        << microseconds;
}

Token& Token::Time (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << TimeStruct (&std_tm);
}

Token& Token::Timestamp (time_us_t timestamp) {
    time_t t = ClockGetSeconds (timestamp);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    int32_t microseconds = ClockGetMicroseconds (timestamp);
    return TimeStruct (&std_tm, microseconds);
}

Token& Token::TimeDate (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << std_tm.tm_hour << ":" << std_tm.tm_min << ":"
        << std_tm.tm_sec;
}

/*
Token& Token::Page (const char* input, int indentation,
                      char bullet, int index, int tab_size,
                      int num_columns) {
    num_columns -= 4;
    *this << "\n";
    int cursor; //< The column number of the cursor.
    char c = *input++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || input == nullptr) { //< It's an empty input.
        for (int i = num_columns; i > 0; --i) {
            Write (' ');
        }
        *this << "\n\n";
        return *this;
    }

    // Make the input for the bullet.
    if (isdigit (bullet)) { // Then we have to print a number bullet.
        TokenWrite (buffer, buffer + 15, index);
        //< + 2 for "\n" - 2 for the bullet offset.
        //char format[16];
        //format[0] = '%';
        //sprintf_s (&format[1], 16, "%%%us", indentation * tab_size);
        //printf (format, buffer);
    }
    else if (isalpha (bullet)) { // Then it's a lower case bullet.
        cursor = 0;
        bullet += index % 26;  //<
        for (; index > 0; index -= 26)
            buffer[cursor++] = bullet;
        buffer[cursor] = '\0';
    }
    else {
        buffer[0] = bullet;
        buffer[1] = 0;
    }
    // s the char.
    int num_spaces = tab_size * indentation;
    while (c) {
        for (cursor = num_spaces; cursor > 0; --cursor)
            Write (' ');
        cursor = num_spaces;
        while (c && (cursor++ < num_columns)) {
            *this << c;
            c = *input++;
        }
        *this << " |\n";
    }
    return  Write ('\n');
}*/

#endif  //< USING_SCRIPT_TEXT
}       //< namespace _
