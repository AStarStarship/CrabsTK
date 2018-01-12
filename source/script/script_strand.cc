/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_strand.cc
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

#include "strand.h"
#include "memory.h"
#include "clock.h"
#include "bsq.h"

namespace _ {

#if USING_SCRIPT_TEXT
const char* StrandEnd (const char* strand) {
    char c = *strand;
    while (c) {
        c = *(++strand);
    }
    return strand;
}

const char* StrandEnd (const char* strand, char delimiter) {
    char c = *strand;
    while (c) {
        if (c == delimiter) {
            return strand;
        }
        c = *(++strand);
    }
    return strand;
}

int StrandLength (const char* strand) {
    if (strand == nullptr)
        return -1;
    int count = 0;
    //std::cout << "Checking string length for " << string << '\n';
    char c = *strand;
    //std::cout << c;
    while (c != 0) {
        ++count;
        c = *(++strand);
        //std::cout << c;
    }
    //std::cout << '\n';
    return count;
}

int StrandLength (const char* strand, char delimiter) {
    if (strand == nullptr)
        return -1;
    int count = 0;
    //std::cout << "Checking string length for " << string << '\n';
    char c = *strand;
    //std::cout << c;
    while (!c) {
        if (c == delimiter) {
            return count;
        }
        ++count;
        c = *(++strand);
        //std::cout << c;
    }
    //std::cout << '\n';
    return -1;
}

char* StrandClone (const char* strand) {
    if (strand == nullptr)
        strand = "";
    int length = StrandLength (strand);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    StrandWrite (clone, clone + length, strand);
    return clone;
}

char* StrandClone (const char* strand, char delimiter) {
    if (strand == nullptr)
        strand = "";
    int length = StrandLength (strand);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    StrandWrite (clone, clone + length, strand, delimiter);
    return clone;
}

char* StrandReplace (const char* strand, char* string) {
    if (!strand) {
        return nullptr;
    }
    if (!string) {
        return nullptr;
    }
    delete string;
    int length = StrandLength (strand);
    char* clone = new char[length + 1];
    StrandWrite (clone, clone + length, strand);
    return clone;
}

char* StrandReplace (const char* strand, char delimiter) {
    if (strand == nullptr)
        strand = "";
    int length = StrandLength (strand);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    StrandWrite (clone, clone + length, strand, delimiter);
    return clone;
}

const char* StrandSkipNumbers (const char* strand) {
    if (strand == nullptr)
        return nullptr;
    char c = *strand;
    if (c == '-') {  // It might be negative.
        c = *(++strand);
        if (!isdigit (c)) {  // it's not negative.
            return strand - 1;
        }
        c = *(++strand);
    }
    while (isdigit (c)) {
        c = *(++strand);
    }
    return strand;
}

const char* StrandRead (const char* strand, int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (strand, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (strand, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (strand, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (strand, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (strand, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (strand, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int64_t extra_copy;
    if (!sscanf_s (strand, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint64_t extra_copy;
    if (!sscanf_s (strand, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* strand, float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (strand, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandRead (const char* string, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (string, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (string);
}

const char* StrandEquals (const char* strand_a, const char* strand_b) {
    if (!strand_a) {
        return nullptr;
    }
    if (!strand_b) {
        return nullptr;
    }
    //std::cout << "\n| Comparing \"" << target << "\" to \"" << query << "\"";

    char a = *strand_a,
         b = *strand_b;
    while (a) {
        std::cout << a;
        if (a != b) { // Not a hit.
                      //std::cout << "\n| But it's not a hit\n";
            return nullptr;
        }
        if (b == 0) { // Hit!
                      //std::cout << "\n| Found hit at ";
                      //PrintPointerNL (target);
            return strand_a;
        }
        a = *(++strand_a);
        b = *(++strand_b);
    }
    if (b != 0) {
        std::cout << "\n| Not a hit: no nil-term char found";
        return nullptr;
    }
    std::cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return strand_a; //< Find hit!
}

const char* StrandEquals (const char* begin, const char* end, const char* string_b) {
    if (!begin) {
        return nullptr;
    }
    if (begin > end) {
        return nullptr;
    }
    if (!string_b) {
        return nullptr;
    }
    //std::cout << "\n| Comparing \"" << target << "\" to \"" << query << "\"";

    char a = *begin,
         b = *string_b;
    while (a) {
        std::cout << a;
        if (a != b) { // Not a hit.
            //std::cout << "\n| But it's not a hit\n";
            return nullptr;
        }
        if (b == 0) { // Hit!
            //std::cout << "\n| Found hit at ";
            //PrintPointerNL (target);
            return begin;
        }
        if (begin > end) {
            return nullptr;
        }
        a = *(++begin);
        b = *(++string_b);
    }
    if (b != 0) {
        std::cout << "\n| Not a hit: no nil-term char found";
        return nullptr;
    }
    std::cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return begin; //< Find hit!
}

const char* StrandEquals (const char* strand_a, const char* strand_b,
                            char delimiter) {
    //std::cout << "Comparing \"" << target << "\" to \"" << query
    //    << "\"\n";
    if (!strand_a) {
        return nullptr;
    }
    if (!strand_b) {
        return nullptr;
    }

    //target = StrandSkipSpaces (target); //< I think this is token compare
    //query = StrandSkipSpaces (query);

    char i = *strand_a,
         q = *strand_b;
    while (i != delimiter) {
        if (i != q) { // Not a hit.
            //std::cout << "\n| not a hit";
            return nullptr;
        }
        //std::cout << i;
        i = *(++strand_a);
        q = *(++strand_b);
    }
    if (q != delimiter) {
        //std::cout << "\n| reached nil-term char but no q:\'" << q
        //    << "\' is not the delimiter.";
        return nullptr;
    }
    //std::cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return strand_a; //< Find hit!
}

const char* StrandEquals (const char* begin, const char* end, const char* query,
                          char delimiter) {
    //std::cout << "Comparing \"" << target << "\" to \"" << query
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

    //target = StrandSkipSpaces (target); //< I think this is token compare
    //query = StrandSkipSpaces (query);

    char i = *begin,
        q = *query;
    while (i != delimiter) {
        if (i != q) { // Not a hit.
                      //std::cout << "\n| not a hit";
            return nullptr;
        }
        if (begin > end) {
            return nullptr;
        }
        //std::cout << i;
        i = *(++begin);
        q = *(++query);
    }
    if (q != delimiter) {
        //std::cout << "\n| reached nil-term char but no q:\'" << q
        //    << "\' is not the delimiter.";
        return nullptr;
    }
    //std::cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return begin; //< Find hit!
}

/*
char* StrandWrite (char* strand, int value) {
    // Stolen from https://goo.gl/waaF1G
    // @todo Convert to pointer arithmetic.

    if (strand == nullptr) {
        std::cout << "\n| nil buffer!";
        return 0;
    }

    if (strand_end == nullptr) {
        std::cout << "\n| nil strand_end!";
        return 0;
    }
    if (strand_end <= strand) {
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
        strand[i++] = digit % 10 + '0';
        digit = digit / 10;
    }

    if(is_negative) {
        strand[i++] = '-';
    }

    strand[i] = '\0';

    for(int t = 0; t < i / 2; t++) {
        buffer_i_minus_t_minus_1 = &strand[i - t - 1],
        buffer_t = &strand[t];

        *buffer_t ^= *buffer_i_minus_t_minus_1;
        *buffer_i_minus_t_minus_1 ^= *buffer_t;
        *buffer_t ^= strand[i-t-1];
        //buffer[t    ] ^= *buffer[i-t-1];
        //buffer[i-t-1] ^= buffer[t];
        //buffer[t    ] ^= buffer[i-t-1];
    }

    if(n == 0) {
        *strand = '0';
        *(strand + 1) = '\0';
    }
    return strand + i;
}

const char* StrandRead (const char* strand, int& result) {
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
    if (strand == nullptr) {
        std::cout << "\n| Buffer was nil!";
        return 0;
    }
    char c = *strand++;
    int length,
        sign = 1,
        value,
        multiplier;
    if (c == '-') {
        sign = -1;
        c = *strand++;
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
        c = *strand++;
    }
    if (length > kMaxLetters) { // int overflow!
        return 0;
    }
    value = c - '0';
    multiplier = 1;
    for (int i = 0; i < length; ++i) {
        multiplier *= 10;
        c = *(strand - i);
        value += ((c - '0') * multiplier);
    }
    result = value * sign;
    return strand + 1;
} */

const char* StrandLineEnd (const char* target, int num_columns) {
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

const char* StrandSkipZeros (const char* target) {
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

const char* StrandSkipSpaces (const char* strand) {
    if (!strand) {
        return nullptr;
    }
    char c = *strand;
    while (IsSpace (c)) {
        //std::cout << '.';
        if (!c) { //< This isn't an error as far as I can see.
            return strand;
        }
        ++strand;
        c = *strand;
    }
    return strand;
}

const char* StrandRead (const char* target, char* strand,
                         char* strand_end, char delimiter) {
    //std::cout << "\n| parse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n| ";
    if (!target) {
        std::cout << "\n| !target";
        return nullptr;
    }
    if (!strand) {
        std::cout << "\n| !strand";
        return nullptr;
    }
    if (strand > strand_end) {
        std::cout << "\n| destination > strand_end";
        return nullptr;
    }
    //std::cout << delimiter;
    char c = *target; //< Current char from target.
    while (c) {
        if (c == delimiter) {
            //std::cout << delimiter << " parsed.\n";
            *strand = 0;
            if (delimiter == 0) {
                return target;
            }
            return target + 1;
        }
        //std::cout << c;
        if (target > strand_end) {
            std::cout << "\n| Buffer overflow!";
            return nullptr;
        }
        *strand++ = c;
        c = *(++target);
    }
    //std::cout << delimiter << " done.\n";
    *strand = 0;
    if (delimiter == 0) {
        return target;
    }
    return target + 1;
}

const char* StrandFind (const char* target, const char* query, char delimiter) {
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
    query = StrandSkipSpaces (query);

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

const char* StrandEmpty () {
    return "";
}

const char* StrandCR () {
    return "\n";
}

const char* StrandErrorHeader () {
    return "\n| Error: ";
}

const char* StrandNewLine () {
    return "\n| ";
}

const char* StrandSkipSpaces (const char* strand, const char* strand_end) {
    if (!strand) {
        return nullptr;
    }
    if (strand > strand_end) {
        return nullptr;
    }
    //std::cout << "\n| Skipping spaces: ";
    char c = *strand;
    while (IsSpace (c)) {
        //std::cout << '.';
        if (!c) {
            return nullptr;
        }
        if (++strand > strand_end) {
            return nullptr;
        }
        c = *strand;
    }
    return strand;
}

char* StrandWrite (char* target, char* target_end, const char* strand) {
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!strand) {
        return nullptr;
    }
    char* cursor = target;
    char c = *strand;
    ++strand;
    while (c) {
        if (cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return nullptr;
        }
        *cursor = c;
        ++cursor;
        c = *strand;
        ++strand;
    }
    *cursor = 0;
    return cursor;
}

char* StrandWrite (char* target, char* target_end, const char* strand,
                 char delimiter) {
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!strand) {
        return nullptr;
    }
    char* cursor = target;
    char s = *strand;
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
        ++strand;
        s = *strand;
    }
    *cursor = s;
    return cursor;
}

char* StrandWrite (char* target, char* target_end, const char* strand,
                 const char* strand_end) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!strand) {
        return nullptr;
    }
    if (strand > strand_end) {
        return nullptr;
    }
    char* cursor = target;
    char t = *strand;
    while (t) {
        *cursor = t;
        if (++cursor > target_end) {
            *target = 0;
            return nullptr;
        }
        if (++strand > strand_end) {
            *target = 0;
            return nullptr;
        }
        t = *strand;
    }
    *cursor = t;
    return cursor;
}

char* StrandWrite (char* target, char* target_end, const char* strand,
                 const char* strand_end, char delimiter) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!strand) {
        return nullptr;
    }
    if (strand > strand_end) {
        return nullptr;
    }
    char* cursor = target;
    char t = *strand;
    while (t != delimiter) {
        if (!t) {
            //std::cout << "\n| There was a !t error in StrandWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return nullptr;
        }
        if (++strand > strand_end) {
            *target = 0;
            return nullptr;
        }
        t = *strand;
    }
    *target = t;
    return target;
}

char* StrandWrite (char* target, char* target_end, int8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, uint8_t value) {
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

char* StrandWrite (char* target, char* target_end, int16_t value) {
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

char* StrandWrite (char* target, char* target_end, uint16_t value) {
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

char* StrandWrite (char* target, char* target_end, int32_t value) {
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

char* StrandWrite (char* target, char* target_end, uint32_t value) {
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

char* StrandWrite (char* target, char* target_end, int64_t value) {
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

char* StrandWrite (char* target, char* target_end, uint64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t result = sprintf_s (target, target_end - target, "%llu", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    std::cout << "\n| !!! " << target << " result:" << result;
    return target + result;
}

char* StrandWrite (char* target, char* target_end, float value) {
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

char* StrandWrite (char* target, char* target_end, double value) {
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

/*
char* StrandWrite (char* target, char* target_end, int32_t value) {

    // Stolen from https://goo.gl/waaF1G
    // @todo Convert to pointer arithmetic.

    if (target == nullptr) {
        std::cout << "\n| nil buffer!";
        return 0;
    }

    if (target_end == nullptr) {
        std::cout << "\n| nil buffer_strand_end!";
        return 0;
    }
    if (target_end <= target) {
        // We have to have room for at least a zero.
        std::cout << "\n| buffer overflow!!";
        return 0;
    }

    int  n = (sizeof (int) == 2)?-32767:-2147483647,
    i = 0;
    bool is_negative = n < 0;
    int digit = is_negative?-n:n;
    char* buffer_t,
    *buffer_i_minus_t_minus_1;

    while (digit != 0) {
        target[i++] = digit % 10 + '0';
        digit = digit / 10;
    }

    if (is_negative) {
        target[i++] = '-';
    }

    target[i] = '\0';

    for (int t = 0; t < i / 2; t++) {
        buffer_i_minus_t_minus_1 = &target[i - t - 1],
        buffer_t = &target[t];

        *buffer_t ^= *buffer_i_minus_t_minus_1;
        *buffer_i_minus_t_minus_1 ^= *buffer_t;
        *buffer_t ^= target[i - t - 1];
        //buffer[t    ] ^= *buffer[i-t-1];
        //buffer[i-t-1] ^= buffer[t];
        //buffer[t    ] ^= buffer[i-t-1];
    }

    if (n == 0) {
        *target = '0';
        *(target + 1) = '\0';
    }
return target + i;
}

char* StrandWrite (char* target, char* target_end, uint value) {
    // Stolen from https://goo.gl/waaF1G

    if (target == nullptr) {
    std::cout << "\n| buffer was nil!";
    return 0;
    }

    if (target_end == nullptr) {
    std::cout << "\n| buffer_strand_end was nil!";
    return 0;
    }
    if (target_end <= target) {
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
    target[i++] = digit % 10 + '0';
    digit = digit / 10;
    }

    target[i] = '\0';

    for(int t = 0; t < i / 2; t++) {
    buffer_i_minus_t_minus_1 = &target[i - t - 1],
    buffer_t = &target[t];

    *buffer_t ^= *buffer_i_minus_t_minus_1;
    *buffer_i_minus_t_minus_1 ^= *buffer_t;
    *buffer_t ^= target[i-t-1];
    //buffer[t    ] ^= *buffer[i-t-1];
    //buffer[i-t-1] ^= buffer[t];
    //buffer[t    ] ^= buffer[i-t-1];
    }

    if(n == 0) {
    *target = '0';
    *(target + 1) = '\0';
}
return target + i;
}

const char* StrandRead (const char* buffer, int32_t& result) {
    // Algorithm:
    // 1. Check for sign char and store as variable named sign.
    // 2. Count length of int string and handle error in case of overflow.
    // 3. Get pointer to last char.
    // 4. Set variable named value to the first char and multiplier to 1.
    // 4. Word backwards in a for loop from strand_end of int string to
    //    calculate each decimal place by multiplying (c - '0') times a
    //    multiplier times 10 for each loop.
    // 5. Return sign times the resulting value above.
    enum {
        kMaxLetters = sizeof (int) == 32 ? 10 : 6, //< int can be 16 or 32-bit.
    };
    if (buffer == nullptr) {
    std::cout << "\n| Buffer was nil!";
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
*/

const char* StrandLineEnd (const char* strand, const char* strand_end,
                         int num_columns) {
    if (!strand) {
        return nullptr;
    }
    if (strand_end > strand) {
        return nullptr;
    }
    char c;
    // Scroll to the end of the line.
    c = *strand++;
    while (c) {
        if (strand_end > strand) {
            return nullptr;
        }
        if (num_columns-- < 0) { // We've reached the end.

            // Scroll left till we hit whitespace (if any).
            while (!IsSpace (c)) {
                c = *(--strand);
            }
            // Then scroll past the whitespace.
            while (IsSpace (c)) {
                c = *(--strand);
            }
            return strand;
            c = *strand++;
        }
        c = *strand++;
    }
    return strand - 1;
}

const char*StrandNextNonNumber (const char* strand, const char* strand_end) {
    if (strand > strand_end) {
        return nullptr;
    }
    char c = *strand;
    if (c == '-') {  // It might be negative.
        if (strand > strand_end) {
            return nullptr;
        }
        c = *(++strand);
        if (!isdigit (c)) {  // it's not negative.
            return strand - 1;
        }
        c = *(++strand);
    }
    while (isdigit (c)) {
        if (strand > strand_end) {
            return nullptr;
        }
        c = *(++strand);
    }
    return strand;
}

const char* StrandRead (const char* strand, const char* strand_end,
                      char* token, char* token_end) {
    return StrandWrite (token, token_end, strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      char* token, char* token_end, char delimiter) {
    return StrandWrite (token, token_end, strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (strand, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (strand, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (strand, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (strand, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (strand, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (strand, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    __int64 extra_copy;
    if (!sscanf_s (strand, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    unsigned __int64 extra_copy;
    if (!sscanf_s (strand, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                        float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (strand, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

const char* StrandRead (const char* strand, const char* strand_end,
                      double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (strand, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (strand, strand_end);
}

Strand::Strand (size_t size, char* buffer) :
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

Strand::Strand (const Strand& other) :
    is_dynamic_ (other.is_dynamic_) {
    if (is_dynamic_) { // Clone the dynamic memory.
        size_t size = other.end_ - other.begin_;
        char* buffer = new char[size];
        begin_ = buffer;
        if (!StrandWrite (buffer, buffer + size, other.begin_)) {
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

Strand::~Strand () {
    if (!is_dynamic_) {
        return;
    }
    delete cursor_;
}

bool Strand::IsDynamic () {
    return is_dynamic_;
}

const char* Strand::SetSize (size_t new_size) {
    if (!is_dynamic_) {
        return "Strand is not dynamic";
    }
    if (new_size < kSizeMin) {
        return "new_size too small";
    }
    delete begin_;
    begin_ = new char[new_size];
    return nullptr;
}

const char* Strand::Resize (size_t new_size) {
    if (!is_dynamic_) {
        return "Can't resize this Strand";
    }
    if (!new_size) {
        return "new_size can't be nil";
    }
    char* new_buffer = StrandClone (begin_);
    if (!new_buffer) {
        return "Error cloning strand";
    }
    delete begin_;
    begin_ = new_buffer;
    return nullptr;
}

Strand& Strand::Clear () {
    *begin_ = 0;
    cursor_ = begin_;
    return *this;
}

int Strand::Length () const {
    return (int)(cursor_ - begin_);
}

int Strand::Space () const {
    return (int)(end_ - cursor_);
}

Strand& Strand::SetEquals (const Strand& other) {
    if (this == &other) {
        return *this;
    }
    int other_count = other.Length ();
    MemoryCopy (begin_, begin_ + other_count, other.begin_,
                other.begin_ + other_count);
    cursor_ = begin_ + other_count;
    return *this;
}

char* Strand::GetBegin () {
    return begin_;
}

bool Strand::SetCursor (char* new_cursor) {
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

char* Strand::GetCursor () {
    return cursor_;
}

char* Strand::GetEnd () {
    return end_;
}

Strand& Strand::operator= (const Strand& other) {
    return SetEquals (other);
}

Strand& Strand::Write (const char* strand) {
    char* cursor = StrandWrite (cursor_, GetEnd (), strand);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (int8_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int8_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (uint8_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint8_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (int16_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int16_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (uint16_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint16_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (int32_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int32_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (uint32_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint32_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (int64_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int64_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (uint64_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint64_t:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (float value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    //std::cout << "\n| Write float:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (double value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    //std::cout << "\n| Write double:" << begin_;
    cursor_ = cursor;
    return *this;
}

Strand& Strand::Write (Strand& strand) {
    return Write (strand.GetBegin ());
}

Strand& Strand::Line (char token, const char* header, int length) {
    //std::cout << header << "Writing Line with length " << length;
    char* cursor = cursor_,
        *stop = cursor,
        *end = GetEnd ();
    stop = StrandWrite (cursor, end, header);
    if (!stop) {
        //std::cout << "\n| Error writing header!";
        return *this;
    }
    //std::cout << "... wrote " << StrandLength (cursor) << " chars.";
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    //std::cout << "\n| new_stop_length:" << stop - cursor;

    while (cursor < stop) {
        *cursor++ = token;
    }
    *cursor = 0;
    //std::cout << "\n| Wrote " << StrandLength (cursor_) << " chars: " 
    //          << cursor_ << '\n';
    cursor_ = cursor;
    return *this;
}

Strand& Strand::StringLine (const char* string, int num_columns) {
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

Strand& Strand::Error (const char* message, const char* end_string) {
    return Write ("\n| Error: ").Write (message).Write (end_string);
}

Strand& Strand::LineBreak (const char* message, int top_bottom_margin,
                           char c, int num_columns) {
    Lines (top_bottom_margin);
    *this << "\n| " << message;
    return Line (c, "\n|", num_columns);
}

Strand& Strand::Lines (int num_rows) {
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

Strand& Strand::Pointer (const void* pointer) {
    // @todo Replace with PrintHex.
    int bytes_written = sprintf_s (cursor_, end_ - cursor_,
                                   "0x%p", pointer);
    cursor_ += bytes_written;
    return *this;
}

Strand& Strand::Hex (byte c) {
    uint16_t chars = MemoryByteToUpperCaseHex (c);
    return *this << (char)chars << ((char)(chars >> 8)) << ' ';
}

Strand& Strand::Params (const uint_t* params) {
    return BsqPrint (params, *this);
}

Strand& Strand::Print (char c) {
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

/*
char CreateKeyValueFormatText (char* string, char column_width,
                               char type) {
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
}*/

Strand& Strand::NumberLine (int index) {
    Write ('\n');
    enum {
        kMaxBufferSize = (sizeof (int) == 2)
        ? 7
        : (sizeof (int) == 4)
        ? 11
        : 128
    };
    char buffer[kMaxBufferSize];
    StrandWrite (buffer, buffer + kMaxBufferSize - 1, index);
    int length = StrandLength (buffer),
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

Strand& Strand::NewLine () {
    return *this << "\n| ";
}

Strand& Strand::Heading (const char* input) {
    return *this << "\n| " << input << '\n';
}

Strand& Strand::Break (const char* header, char c, int num_lines,
                       int console_width) {
    for (int i = 0; i < num_lines; ++i) {
        Write ('\n');
    }
    *this << header;
    int length = StrandLength (header);
    for (int i = 0; i < console_width - length; ++i) {
        *this << c;
    }
    return Write ('\n');
}

Strand& Strand::Centered (const char* string, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return *this;
    }
    // We need to leave at least one space to the left and right of
    int length = StrandLength (string);
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
        const char* end_line = StrandLineEnd (input, num_columns);
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

Strand& Strand::Centered (const char* input, int width, bool is_last,
                          char column_delimeter) {
    if (width < 1) {
        return *this;
    }
    *this << column_delimeter;
    int length = StrandLength (input),
        num_spaces;

    if (width < length) {
        char format[32];
        format[0] = '%';
        StrandWrite (&format[1], format + 32, (is_last ? width - 2 : width - 1));
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

Strand& Strand::ColumnBreak (int num_columns, char column_delimeter,
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

Strand& Strand::Right (const char* strand, int num_columns) {
    char c;  //< The current char.

    if (num_columns <= 4) {// We need at least 4 columns for this to work.
        return *this;
    }
    if (!strand) {
        return *this;
    }
    do {
        const char* end_line = StrandLineEnd (strand, num_columns);
        size_t row_length = end_line - strand,
            num_left_spaces = num_columns - row_length,
            i;
        //  left whitespace.
        for (i = num_left_spaces; i != 0; --i) {
            Write (' ');
        }
        //  input.
        for (i = row_length; i != 0; --i) {
            *this << *strand++;
        }
        // Increment the input pointer and scroll past the whitespace
        strand += row_length;
        while (IsSpace (c = *strand++));
    } while (c);
    return *this;
}

Strand& Strand::Right (int8_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (uint8_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (int16_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%i", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (uint16_t value, int num_columns) {
    char buffer[8];
    sprintf_s (buffer, 8, "%u", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (int32_t value, int num_columns) {
    char buffer[16];
    sprintf_s (buffer, 16, "%i", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (uint32_t value, int num_columns) {
    char buffer[16];
    sprintf_s (buffer, 16, "%u", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (int64_t value, int num_columns) {
    char buffer[24];
    sprintf_s (buffer, 24, "%lld", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (uint64_t value, int num_columns) {
    char buffer[24];
    sprintf_s (buffer, 24, "%llu", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (float value, int num_columns) {
    char buffer[kMaxDigitsFloat];
    sprintf_s (buffer, kMaxDigitsFloat, "%f", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Right (double value, int num_columns) {
    char buffer[kMaxDigitsDouble];
    sprintf_s (buffer, kMaxDigitsDouble, "%f", value);
    return Right (buffer, num_columns);
}

Strand& Strand::Page (const char* input, int indentation,
                      char bullet, int index, int tab_size,
                      int num_columns) {
    num_columns -= 4;
    *this << "\n| ";
    int cursor; //< The column number of the cursor.
    char c = *input++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || input == nullptr) { //< It's an empty input.
        for (int i = num_columns; i > 0; --i) {
            Write (' ');
        }
        *this << "\n|\n";
        return *this;
    }

    // Make the input for the bullet.
    if (isdigit (bullet)) { // Then we have to print a number bullet.
        StrandWrite (buffer, buffer + 15, index);
        //< + 2 for "\n| " - 2 for the bullet offset.
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
}

Strand& Strand::Memory (const void* address, const void* stop) {
    *this << "\n| " << 0;
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
        *this << "| " << Pointer (chars + MemoryVector (address, stop));// (chars - 64);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 64; ++i) {
        Write ('_');
    }
    return *this << "| " << Pointer (chars + MemoryVector (address, stop));
}

Strand& Strand::Memory (const void* address, int size) {
    return Memory (address, reinterpret_cast<const char*> (address) + size);
}

Strand& Strand::Token (const char* strand) {
    if (!strand) {
        return *this;
    }
    char c = *strand;
    while (c && !IsSpace (c)) {
        *this << c;
    }
    return *this;
}

Strand& Strand::Token (const char* strand, const char* strand_end) {
    if (!strand) {
        return *this;
    }
    if (strand > strand_end) {
        return *this;
    }
    char c = *strand;
    while (c && !IsSpace (c)) {
        *this << c;
        if (++strand > strand_end) {
            return *this;
        }
    }
    return *this;
}

Strand& Strand::TimeStruct (tm* std_tm) {
    if (std_tm == nullptr) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1
        << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec;
}

Strand& Strand::TimeStruct (std::tm* std_tm, int32_t microseconds) {
    if (std_tm == nullptr) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1
        << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
        << std_tm->tm_min << ":" << std_tm->tm_sec << ":"
        << microseconds;
}

Strand& Strand::Time (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << TimeStruct (&std_tm);
}

Strand& Strand::Timestamp (time_us_t timestamp) {
    time_t t = ClockGetSeconds (timestamp);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    int32_t microseconds = ClockGetMicroseconds (timestamp);
    return TimeStruct (&std_tm, microseconds);
}

Strand& Strand::DateTime (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << std_tm.tm_hour << ":" << std_tm.tm_min << ":"
        << std_tm.tm_sec;
}

#endif  //< USING_SCRIPT_TEXT
}       //< namespace _
