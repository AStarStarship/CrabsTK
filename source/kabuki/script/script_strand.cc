/** kabuki::script
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

#if USE_SCRIPT_STRAND
const char* StrandEnd (const char* target) {
    char c = *target;
    while (c) {
        c = *(++target);
    }
    return target;
}

const char* StrandEnd (const char* target, char delimiter) {
    char c = *target;
    while (c) {
        if (c == delimiter) {
            return target;
        }
        c = *(++target);
    }
    return target;
}

int StrandLength (const char* target) {
    if (target == nullptr)
        return -1;
    int count = 0;
    //cout << "Checking string length for " << string << '\n';
    char c = *target;
    //cout << c;
    while (c != 0) {
        ++count;
        c = *(++target);
        //cout << c;
    }
    //cout << '\n';
    return count;
}

int StrandLength (const char* target, char delimiter) {
    if (target == nullptr)
        return -1;
    int count = 0;
    //cout << "Checking string length for " << string << '\n';
    char c = *target;
    //cout << c;
    while (!c) {
        if (c == delimiter) {
            return count;
        }
        ++count;
        c = *(++target);
        //cout << c;
    }
    //cout << '\n';
    return -1;
}

char* StrandClone (const char* input) {
    if (input == nullptr)
        input = "";
    int length = StrandLength (input);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    TextWrite (clone, clone + length, input);
    return clone;
}

char* StrandClone (const char* input, char delimiter) {
    if (input == nullptr)
        input = "";
    int length = StrandLength (input);
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length + 1];
    TextWrite (clone, clone + length, input, delimiter);
    return clone;
}

void StrandText (char* destination, const char* source) {
    if (destination == nullptr) {
        return;
    }
    if (source == nullptr) {
        return;
    }
    char c = *source;
    ++source;
    while (c) {
        *destination = c;
        ++destination;
        c = *source;
        ++source;
    }
    *destination = 0;
}

char* StrandWrite (char* target, char* end, int8_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, uint8_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, int16_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, uint16_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, int32_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, uint32_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, int64_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%lli", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, uint64_t value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%llu", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, float value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%f", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

char* StrandWrite (char* target, char* end, double value) {
    // @todo Don't use sprintf_s.
    int result = sprintf_s (target, end - target, "%f", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result + 1;
}

const char* StrandSkipNumbers (const char* target) {
    if (target == nullptr)
        return nullptr;
    char c = *target;
    if (c == '-') {  // It might be negative.
        c = *(++target);
        if (!isdigit (c)) {  // it's not negative.
            return target - 1;
        }
        c = *(++target);
    }
    while (isdigit (c)) {
        c = *(++target);
    }
    return target;
}

const char* StrandRead (const char* target, int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int64_t extra_copy;
    if (!sscanf_s (target, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint64_t extra_copy;
    if (!sscanf_s (target, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (target, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandRead (const char* target, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (target, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return StrandSkipNumbers (target);
}

const char* StrandEquals (const char* target, const char* query) {
    if (target == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;
    //cout << "\n| Comparing \"" << target << "\" to \"" << query << "\"";

    char i = *target,
        q = *query;
    while (i) {
        cout << i;
        if (i != q) { // Not a hit.
            //cout << "\n| But it's not a hit\n";
            return nullptr;
        }
        if (q == 0) { // Hit!
            //cout << "\n| Found hit at ";
            //PrintPointerNL (target);
            return target;
        }
        i = *(++target);
        q = *(++query);
    }
    if (q != 0) {
        cout << "\n| Not a hit: no nil-term char found";
        return nullptr;
    }
    cout << "\n| Found hit at ";
    //PrintPointerNL (target);
    return target; //< Find hit!
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

const char* StrandSkipSpaces (const char* target) {
    if (target == nullptr)
        return nullptr;
#if SCRIPT_DEBUG
    cout << "\nSkipping spaces: ";
#endif    //< SCRIPT_DEBUG
    char c = *target;
    while (c) {
#if SCRIPT_DEBUG
        cout << '.';
#endif    //< SCRIPT_DEBUG
        if (!isspace (c))
            return target;
        ++target;
        c = *target;
    }
    return target;
}

bool StrandTokenQualifies (const char* target) {
    if (target == nullptr) {
        return false;
    }
    char c = *target;
    while (c) {
        if (!isspace (c)) {
            return true;
        }
        c = *(++target);
    }
    return false;
}

const char* StrandTokenEnd (const char* target) {
    if (!target) {
        return nullptr;
    }
    target = StrandSkipSpaces (target);

    char c = *target;
    while (c) {
        if (isspace (c))
            return target + 1;
        c = *(++target);
    }
    // We're skipping spaces because we're expecting good data so this is an
    // error.
    return nullptr;
}

int StrandTokenCompare (const char* strand, const char* token) {
    int a,
        b,
        comparison;
    if (!strand) {
        if (!token) {
            return 0;
        }
        return *token;
    }
    if (!token) {
        return 0 - *strand;
    }

    // Algorithm combines loops for better performance.

    a = *strand;
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
    while (a && !isspace (a)) {
        //cout << "\n| s: " << s << " t: " << t;
        comparison = b - a;
        if (comparison) {
            //cout << "\n| Found unmatched chars t:\'" << t <<"\' s:\'" << s << "comparison:" << comparison;
            return comparison;
        }
        ++strand;
        ++token;
        a = *strand;
        b = *token;
    }
    if (!isspace (b)) {
        //cout << "\n| token still has some chars so it's is greater.";
        return b;
    }
    //cout << "\n| Find match!";
    return 0;
}

const char* StrandRead (const char* target, char* text,
                         char* text_end, char delimiter) {
    //cout << "> parse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n| ";
    if (!target) {
        cout << "\n| !target";
        return nullptr;
    }
    if (!text) {
        cout << "\n| !text";
        return nullptr;
    }
    if (text > text_end) {
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
        if (target > text_end) {
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

char* StrandParseToken (char* target) {
    if (target == nullptr)
        return nullptr;

    target = (char*)StrandSkipSpaces (target);

    char c = *target;
    while (c && !isspace (c)) {
        c = *(++target);
    }
    *target = 0;   //< Mark the null-term char.
    return target; //< This is the start of the next token or whitespace.
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
/*
const char* StrandRead (const char* target, int* value) {
    return nullptr;
}

char* StrandRead (char* target, int* value) {
    return nullptr;
}

const char* StrandParseFloat (const char* target, float* value) {
    return nullptr;
}

char* StrandReadFloat (char* target, float* value) {
    return nullptr;
} */

#endif  //< USE_SCRIPT_STRAND
}       //< namespace _
