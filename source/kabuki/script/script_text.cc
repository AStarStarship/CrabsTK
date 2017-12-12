/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_text.cc
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

#include "text.h"

#if USING_SCRIPT_TEXT

#include "print.h"
#include "memory.h"
#include "strand.h"

using namespace std;

namespace _ {

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

const char* TextSkipSpaces (const char* text, const char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text >= text_end) {
        return nullptr;
    }
    //cout << "\n| Skipping spaces: ";
    char c = *text;
    while (!isspace (c)) {
        //cout << '.';
        if (!c) {
            // We're skipping spaces because we're expecting good data so this
            // is an error.
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
}

const char* TextTokenEnd (const char* text, const char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    text = TextSkipSpaces (text, text_end);

    char c = *text;
    while (c) {
        if (isspace (c)) {
            return text;
        }
        if (++text > text_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
}

int TextTokenCompare (const char* text, const char* text_end,
                      const char* token) {
    int a,
        b,
        comparison;
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
    if (text > text_end) {
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
    while (a && !isspace (a)) {
        //cout << "\n| s: " << s << " t: " << t;
        comparison = b - a;
        if (comparison) {
            //cout << "\n| Found unmatched chars t:\'" << t <<"\' s:\'" << s << "comparison:" << comparison;
            return comparison;
        }
        if (++text > text_end) {
            return *token;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (!isspace (b)) {
        //cout << "\n| token still has some chars so it's is greater.";
        return b;
    }
    //cout << "\n| Find match!";
    return 0;
}

int TextTokenCompare (const char* text, const char* text_end,
                      const char* token, const char* token_end) {
    int a,
        b,
        comparison;
    if (!text) {
        if (!token) {
            return 0;
        }
        return *token;
    }
    if (!token) {
        return 0 - *text;
    }
    if (text > text_end) {
        return *token;
    }
    if (token > token_end) {
        return *text;
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
    while (a && !isspace (a)) {
        //cout << "\n| s: " << s << " t: " << t;
        comparison = b - a;
        if (comparison) {
            //cout << "\n| Found unmatched chars t:\'" << t <<"\' s:\'" << s << "comparison:" << comparison;
            return comparison;
        }
        if (++text > text_end) {
            return b;
        }
        if (++token > token_end) {
            return a;
        }
        a = *text;
        b = *token;
    }
    if (!isspace (b)) {
        //cout << "\n| token still has some chars so it's is greater.";
        return b;
    }
    //cout << "\n| Find match!";
    return 0;
}

const char* TextTokenEquals (const char* text, const char* text_end,
                             const char* token) {
    int a,
        b,
        comparison;
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }
    // Algorithm combines loops for better performance.

    a = *text;
    b = *token;
    if (!a) {
        return nullptr;
    }
    if (!b) {
        return nullptr;
    }
    while (a && !isspace (a)) {
        //cout << "\n| s: " << s << " t: " << t;
        comparison = b - a;
        if (comparison) {
            //cout << "\n| Found unmatched chars t:\'" << t <<"\' s:\'" << s << "comparison:" << comparison;
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (!isspace (b)) {
        //cout << "\n| token still has some chars so it's is greater.";
        return nullptr;
    }
    //cout << "\n| Find match!";
    return 0;
}

const char* TextTokenEquals (const char* text, const char* text_end,
                             const char* token, const char* token_end) {
    int a,
        b,
        comparison;
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }
    if (token > token_end) {
        return nullptr;
    }
    // Algorithm combines loops for better performance.

    a = *text;
    b = *token;
    while (a && !isspace (a)) {
        //cout << "\n| s: " << s << " t: " << t;
        comparison = b - a;
        if (comparison) {
            //cout << "\n| Found unmatched chars t:\'" << t <<"\' s:\'" << s << "comparison:" << comparison;
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        if (++token > token_end) {
            return nullptr;
        }
        a = *text;
        b = *token;
    }
    if (!isspace (b)) {
        //cout << "\n| token still has some chars so it's is greater.";
        return nullptr;
    }
    //cout << "\n| Find match!";
    return text;
}

bool TextTokenQualifies (const char* text, const char* text_end) {
    if (!text) {
        return false;
    }
    if (text > text_end) {
        return false;
    }
    char c = *text;
    while (c) {
        if (!isspace (c)) { 
            // The token must end at or before the target_end.
            do {
                if (++text > text_end) {
                    return false;
                }
                c = *text;
                if (!isspace (c)) {
                    return true;
                }
            } while (c);
            return true;
        }
        if (++text > text_end) {
            return false;
        }
        c = *text;
    }
    return false;
}

char* TextWrite (char* target, char* target_end, const char* strand) {
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

char* TextWrite (char* target, char* target_end, const char* strand,
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

char* TextWrite (char* target, char* target_end, const char* text,
                 const char* text_end) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    char t = *text;
    while (t) {
        *target = t;
        if (++target > target_end) {
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        t = *text;
    }
    *target = t;
    return target;
}

char* TextWrite (char* target, char* target_end, const char* text,
                 const char* text_end, char delimiter) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    char t = *text;
    while (t != delimiter) {
        if (!t) {
            //cout << "\n| There was a !t error in TextWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        t = *text;
    }
    *target = t;
    return target;
}

char* TextWrite (char* target, char* target_end, int8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, uint8_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, int16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, uint16_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, int32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, uint32_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, int64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%lli", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, uint64_t value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int result = sprintf_s (target, target_end - target, "%llu", value);
    if (result < 0) {
        return nullptr;
    }
    cout << "\n| !!! " << target << " result:" << result;
    return target + result;
}

char* TextWrite (char* target, char* target_end, float value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    int buffer_size = target_end - target,
        result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

char* TextWrite (char* target, char* target_end, double value) {
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
char* TextWrite (char* target, char* target_end, int32_t value) {
    
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

char* TextWrite (char* target, char* target_end, uint value) {
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
    // 4. Word backwards in a for loop from strand_end of int string to calculate each
    //    decimal place by multiplying (c - '0') times a multiplier times 10
    //    for each loop.
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

const char* TextLineEnd (const char* target, const char* target_end,
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

const char*TextNextNonNumber (const char* target, const char* target_end) {
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

const char* TextRead (const char* text, const char* text_end,
                      char* token, char* token_end) {
    return TextWrite (token, token_end, text, text_end);
}

const char* TextRead (const char* text, const char* text_end,
                       char* token, char* token_end, char delimiter) {
    return TextWrite (token, token_end, text, text_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (target, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end,
                      uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (target, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextTokenRead (const char* text, const char* text_end,
                           char* token, char* token_end) {
    if (!token) {
        return nullptr;
    }
    if (token > token_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    char t = *text;
    while (t && !isspace (t)) {
        *token = t;
        if (++token > token_end) {
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        t = *text;
    }
    *token = t;
    return text;
}

const char* TextRead (const char* target, const char* target_end, int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    __int64 extra_copy;
    if (!sscanf_s (target, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end, uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    unsigned __int64 extra_copy;
    if (!sscanf_s (target, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end, float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (target, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (target, target_end);
}

const char* TextRead (const char* target, const char* target_end, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (target, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (target, target_end);
}

#endif  //< USING_SCRIPT_TEXT
}       //< namespace _
