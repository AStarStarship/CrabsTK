/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_text.cc
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

#if CRABS_SEAM >= 1

// Some of the Text functions will be required but not all of them.

#if USING_CRABS_TEXT

namespace _ {

/*
const char* Scan (const char* text, const char* text_end,
                       char* text, char* text_end) {
    return Print (text, text_end, text, text_end);
}

const char* Scan (const char* text, const char* text_end,
                      char* text, char* text_end, char delimiter) {
    return Print (text, text_end, text, text_end);
}*/

const char* Scan (const char* text, const char* text_end,
                      int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (text, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end,
                       uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (text, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end,
                       int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (text, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (text, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (text, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (text, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    __int64 extra_copy;
    if (!sscanf_s (text, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    unsigned __int64 extra_copy;
    if (!sscanf_s (text, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (text, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, const char* text_end, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (text, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextNextNonNumber (text, text_end);
}

const char* Scan (const char* text, int8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (text, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int8_t)extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, uint8_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (text, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint8_t)extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, int16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (text, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int16_t)extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, uint16_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (text, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint16_t)extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, int32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int extra_copy;
    if (!sscanf_s (text, "%i", &extra_copy)) {
        return nullptr;
    }
    result = (int32_t)extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, uint32_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint extra_copy;
    if (!sscanf_s (text, "%u", &extra_copy)) {
        return nullptr;
    }
    result = (uint32_t)extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, int64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    int64_t extra_copy;
    if (!sscanf_s (text, "%lli", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, uint64_t& result) {
    // @todo Rewrite with custom string-to-integer function.
    uint64_t extra_copy;
    if (!sscanf_s (text, "%llu", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* text, float& result) {
    // @todo Rewrite with custom string-to-float function.
    float extra_copy;
    if (!sscanf_s (text, "%f", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextSkipNumbers (text);
}

const char* Scan (const char* string, double& result) {
    // @todo Rewrite with custom string-to-float function.
    double extra_copy;
    if (!sscanf_s (string, "%lf", &extra_copy)) {
        return nullptr;
    }
    result = extra_copy;
    return TextSkipNumbers (string);
}
/*
const char* Scan (const char* text, const char* text_end,
                        char* target, char* target_end) {
    text = TextSkipSpaces (text, text_end);
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
    while (c && !IsWhitespace (c)) {
        #if CRABS_SEAM == 1
        std::cout << c;
        #endif
        if (++text > text_end) {
            #if CRABS_SEAM == 1
            std::cout << " but text buffer overflowed!";
            #endif
            return nullptr;
        }
        if (++target > target_end) {
            #if CRABS_SEAM == 1
            std::cout << " but target buffer overflowed!";
            #endif
            return nullptr;
        }
        *target = c;
        c = *text;
    }
    #if CRABS_SEAM == 1
    std::cout << '\"';
    #endif
    *target = 0;
    return text;

}

const char* Scan (const char* target, char* text,
                         char* text_end, char delimiter) {
    #if CRABS_SEAM == 1
    std::cout << "\nparse_string buffer_size: " << text_end - text
              << " delimiter " << delimiter << "\n";
    #endif
    if (!target) {
        std::cout << "\n!target";
        return nullptr;
    }
    if (!text) {
        std::cout << "\n!text";
        return nullptr;
    }
    if (text > text_end) {
        std::cout << "\ndestination > text_end";
        return nullptr;
    }
    //std::cout << delimiter;
    char c = *target; //< Current char from target.
    while (c > delimiter) {
        if (c <= delimiter) {
            //std::cout << delimiter << " parsed.\n";
            *text = 0;
            if (delimiter == 0) {
                return target;
            }
            return target + 1;
        }
        //std::cout << c;
        if (target > text_end) {
            std::cout << "\nBuffer overflow!";
            return nullptr;
        }
        *text++ = c;
        c = *(++target);
    }
    //std::cout << delimiter << " done.\n";
    *text = 0;
    if (delimiter == 0) {
        return target;
    }
    return target + 1;
}*/
}       //< namespace _
#endif  //< USING_CRABS_TEXT
#endif  //< CRABS_SEAM >= 1
