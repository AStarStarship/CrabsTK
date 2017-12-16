/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/script_strand.cc
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
#include "text.h"

using namespace std;

namespace _ {

#if SCRIPT_USING_STRAND
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
    //cout << "Checking string length for " << string << '\n';
    char c = *strand;
    //cout << c;
    while (c != 0) {
        ++count;
        c = *(++strand);
        //cout << c;
    }
    //cout << '\n';
    return count;
}

int StrandLength (const char* strand, char delimiter) {
    if (strand == nullptr)
        return -1;
    int count = 0;
    //cout << "Checking string length for " << string << '\n';
    char c = *strand;
    //cout << c;
    while (!c) {
        if (c == delimiter) {
            return count;
        }
        ++count;
        c = *(++strand);
        //cout << c;
    }
    //cout << '\n';
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

const char* StrandRead (const char* strand, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (strand, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (strand);
}

const char* StrandEquals (const char* strand, const char* query) {
    if (strand == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;
    //cout << "\n| Comparing \"" << target << "\" to \"" << query << "\"";

    char a = *strand,
         b = *query;
    while (a) {
        cout << a;
        if (a != b) { // Not a hit.
            //cout << "\n| But it's not a hit\n";
            return nullptr;
        }
        if (b == 0) { // Hit!
            //cout << "\n| Found hit at ";
            //PrintPointerNL (target);
            return strand;
        }
        a = *(++strand);
        b = *(++query);
    }
    if (b != 0) {
        cout << "\n| Not a hit: no nil-term char found";
        return nullptr;
    }
    cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return strand; //< Find hit!
}

const char* StrandEquals (const char* target, const char* query,
                            char delimiter) {
    //cout << "Comparing \"" << target << "\" to \"" << query
    //    << "\"\n";
    if (target == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    //target = StrandSkipSpaces (target); //< I think this is token compare
    //query = StrandSkipSpaces (query);

    char i = *target,
         q = *query;
    while (i != delimiter) {
        if (i != q) { // Not a hit.
            //cout << "\n| not a hit";
            return nullptr;
        }
        //cout << i;
        i = *(++target);
        q = *(++query);
    }
    if (q != delimiter) {
        //cout << "\n| reached nil-term char but no q:\'" << q
        //    << "\' is not the delimiter.";
        return nullptr;
    }
    //cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return target; //< Find hit!
}

/*
char* StrandWrite (char* strand, int value) {
    // Stolen from https://goo.gl/waaF1G
    // @todo Convert to pointer arithmetic.

    if (strand == nullptr) {
        std::cout << "\n| null buffer!";
        return 0;
    }

    if (strand_end == nullptr) {
        std::cout << "\n| null strand_end!";
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
        std::cout << "\n| Buffer was null!";
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
            while (!isspace (c)) {
                c = *(--target);
            }
            // Then scroll past the whitespace.
            while (isspace (c)) {
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
    while (isspace (c)) {
        //cout << '.';
        if (!c) { //< This isn't an error as far as I can see.
            return strand;
        }
        ++strand;
        c = *strand;
    }
    return strand;
}

const char* StrandRead (const char* target, char* text,
                         char* strand_end, char delimiter) {
    //cout << "\n| parse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n| ";
    if (!target) {
        cout << "\n| !target";
        return nullptr;
    }
    if (!text) {
        cout << "\n| !text";
        return nullptr;
    }
    if (text > strand_end) {
        cout << "\n| destination > strand_end";
        return nullptr;
    }
    //cout << delimiter;
    char c = *target; //< Current char from target.
    while (c) {
        if (c == delimiter) {
            //cout << delimiter << " parsed.\n";
            *text = 0;
            if (delimiter == 0) {
                return target;
            }
            return target + 1;
        }
        //cout << c;
        if (target > strand_end) {
            cout << "\n| Buffer overflow!";
            return nullptr;
        }
        *text++ = c;
        c = *(++target);
    }
    //cout << delimiter << " done.\n";
    *text = 0;
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

const char* TextEmpty () {
    return "";
}

const char* TextCR () {
    return "\n";
}

const char* TextErrorHeader () {
    return "\n| Error: ";
}

const char* TextNewLine () {
    return "\n| ";
}

const char* TextSkipSpaces (const char* text, const char* strand_end) {
    if (!text) {
        return nullptr;
    }
    if (text > strand_end) {
        return nullptr;
    }
    //cout << "\n| Skipping spaces: ";
    char c = *text;
    while (isspace (c)) {
        //cout << '.';
        if (!c) {
            return nullptr;
        }
        if (++text > strand_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
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
    char c = *strand;
    ++strand;
    while (c) {
        if (target > target_end) {
            return nullptr;
        }
        *target = c;
        ++target;
        c = *strand;
        ++strand;
    }
    *target = 0;
    return target;
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
    char s = *strand;
    while (s != delimiter) {
        if (!s) {
            return nullptr;
        }
        *target = s;
        if (++target > target_end) {
            return nullptr;
        }
        ++strand;
        s = *strand;
    }
    *target = s;
    return target;
}

char* StrandWrite (char* target, char* target_end, const char* text,
                 const char* strand_end) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > strand_end) {
        return nullptr;
    }
    char t = *text;
    while (t) {
        *target = t;
        if (++target > target_end) {
            return nullptr;
        }
        if (++text > strand_end) {
            return nullptr;
        }
        t = *text;
    }
    *target = t;
    return target;
}

char* StrandWrite (char* target, char* target_end, const char* text,
                 const char* strand_end, char delimiter) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > strand_end) {
        return nullptr;
    }
    char t = *text;
    while (t != delimiter) {
        if (!t) {
            //cout << "\n| There was a !t error in StrandWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            return nullptr;
        }
        if (++text > strand_end) {
            return nullptr;
        }
        t = *text;
    }
    *target = t;
    return target;
}

char* StrandWrite (char* target, char* target_end, int8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, uint8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, int16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, uint16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, int32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, uint32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, int64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%lli", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, uint64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int result = sprintf_s (target, target_end - target, "%llu", value);
    if (result < 0) {
        return nullptr;
    }
    cout << "\n| !!! " << target << " result:" << result;
    return target + result;
}

char* StrandWrite (char* target, char* target_end, float value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* StrandWrite (char* target, char* target_end, double value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

/*
char* StrandWrite (char* target, char* target_end, int32_t value) {

// Stolen from https://goo.gl/waaF1G
// @todo Convert to pointer arithmetic.

if (target == nullptr) {
std::cout << "\n| null buffer!";
return 0;
}

if (target_end == nullptr) {
std::cout << "\n| null buffer_strand_end!";
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
std::cout << "\n| buffer was null!";
return 0;
}

if (target_end == nullptr) {
std::cout << "\n| buffer_strand_end was null!";
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
*/

const char* StrandLineEnd (const char* target, const char* target_end,
                         int num_columns) {
    if (target_end > target) {
        return nullptr;
    }
    char c;
    // Scroll to the end of the line.
    c = *target++;
    while (c) {
        if (target_end > target) {
            return nullptr;
        }
        if (num_columns-- < 0) { // We've reached the end.
                                 // Scroll left till we hit whitespace (if any).
            while (!isspace (c)) {
                c = *(--target);
            }
            // Then scroll past the whitespace.
            while (isspace (c)) {
                c = *(--target);
            }
            return target;
            c = *target++;
        }
        c = *target++;
    }
    return target - 1;
}

const char*StrandNextNonNumber (const char* target, const char* target_end) {
    if (target > target_end) {
        return nullptr;
    }
    char c = *target;
    if (c == '-') {  // It might be negative.
        if (target > target_end) {
            return nullptr;
        }
        c = *(++target);
        if (!isdigit (c)) {  // it's not negative.
            return target - 1;
        }
        c = *(++target);
    }
    while (isdigit (c)) {
        if (target > target_end) {
            return nullptr;
        }
        c = *(++target);
    }
    return target;
}

const char* TextRead (const char* text, const char* strand_end,
                      char* token, char* token_end) {
    return StrandWrite (token, token_end, text, strand_end);
}

const char* TextRead (const char* text, const char* strand_end,
                      char* token, char* token_end, char delimiter) {
    return StrandWrite (token, token_end, text, strand_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    __int64 extra_copy;
    if (!sscanf_s (target, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    unsigned __int64 extra_copy;
    if (!sscanf_s (target, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (target, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (target, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandNextNonNumber (target, target_end);
}

#endif  //< SCRIPT_USING_STRAND
}       //< namespace _
