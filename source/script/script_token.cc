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
#if USING_SCRIPT_TEXT
#include "text.h"

namespace _ {

bool TokenQualifies (const char* strand) {
    if (strand == nullptr) {
        return false;
    }
    char c = *strand;
    while (c) {
        if (!IsSpace (c)) {
            return true;
        }
        c = *(++strand);
    }
    return false;
}

bool TokenQualifies (const char* text, const char* strand_end) {
    if (!text) {
        return false;
    }
    if (text > strand_end) {
        return false;
    }
    char c = *text;
    while (c) {
        if (!IsSpace (c)) {
            // The token must end at or before the target_end.
            do {
                if (++text > strand_end) {
                    return false;
                }
                c = *text;
                if (!IsSpace (c)) {
                    return true;
                }
            } while (c);
            return true;
        }
        if (++text > strand_end) {
            return false;
        }
        c = *text;
    }
    return false;
}

const char* TokenEquals (const char* strand, const char* token) {
    strand = StrandSkipSpaces (strand);
    if (!strand) {
        return nullptr;
    }
    if (!token) {
        return nullptr;
    }

    char a = *strand,
         b = *token;
    int  result;

    #if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << "\n| Comparing \"" << strand << "\" to \"" << token << "\"";
    #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN

    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << "\n| b - a = " << b << " - " << a << " = " << result;
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        if (result) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is not a hit.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        if (!a) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is a partial match but !a.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        ++strand;
        ++token;
        a = *strand;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << " is only a partial match but found " << (a ? "a" : "space");
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        return nullptr;
    }
    #if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << " is a match!";
    #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
    return strand;
}

const char* TokenEquals (const char* text, const char* strand_end,
                         const char* token) {
    text = StrandSkipSpaces (text, strand_end);
    if (!text) {
        return nullptr;
    }
    if (!token) { // We're assuming token is in ROM and has no whitespace.
        return nullptr;
    }
    char a = *text,
         b = *token;
    int  result;

#if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << "\n| Comparing \"" << text << "\" to \"" << token << "\"";
#endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN

    a = *text;
    b = *token;
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        //std::cout << "\n| b - a = " << b << " - " << a << " = " << result;
        if (result) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " has unmatched chars b - a = " << b << " - " << a << " = "
                 << result;
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        if (!a) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is a partial match but !a.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        if (++text > strand_end) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " but text buffer overflowed!";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << " is only a partial match but found " << (a ? "a" : "space");
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        return nullptr;
    }
    #if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << " is a match!";
    #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
    return text;
}

const char* TokenRead (const char* text, const char* strand_end,
                       char* target, char* target_end) {
    text = StrandSkipSpaces (text, strand_end);
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
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << c;
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        if (++text > strand_end) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " but text buffer overflowed!";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        if (++target > target_end) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " but target buffer overflowed!";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return nullptr;
        }
        *target = c;
        c = *text;
    }
    #if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << '\"';
    #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
    *target = 0;
    return text;

}

int TokenCompare (const char* strand, const char* token) {
    int a,
        b,
        result;
    if (!strand) {
        if (!token) {
            return 0;
        }
        return *token;
    }
    if (!token) {
        return 0 - *strand;
    }

    #if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << "\n| Comparing \"" << text << "\" to \"" << token << "\"";
    #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
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
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << "\n| b - a = " << b << " - " << a << " = " << result;
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        if (result) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is not a hit.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return result;
        }
        if (!a) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is a partial match but !a.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return result;
        }
        ++strand;
        ++token;
        a = *strand;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << " is only a partial match but found " << (a?"a":"space");
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        return b - a;
    }
    return 0;
}

int TokenCompare (const char* text, const char* strand_end,
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
    if (text > strand_end) {
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
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << "\n| b - a = " << b << " - " << a << " = " << result;
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        if (result) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is not a hit.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return result;
        }
        if (!a) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " is a partial match but !a.";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return result;
        }
        if (++text > strand_end) {
            #if SCRIPT_DEBUG == SCRIPT_TOKEN
            std::cout << " but buffer overflowed!";
            #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
            return result;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (a && !IsSpace (a)) {
        #if SCRIPT_DEBUG == SCRIPT_TOKEN
        std::cout << " is only a partial match but found " << (a?"a":"space");
        #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
        return b - a;
    }
    #if SCRIPT_DEBUG == SCRIPT_TOKEN
    std::cout << " is a match!";
    #endif  //< SCRIPT_DEBUG == SCRIPT_TOKEN
    return 0;
}

const char* TokenEnd (const char* target) {
    if (!target) {
        return nullptr;
    }
    target = StrandSkipSpaces (target);

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

const char* TokenEnd (const char* text, const char* strand_end) {
    if (!text) {
        return nullptr;
    }
    if (text > strand_end) {
        return nullptr;
    }
    text = StrandSkipSpaces (text, strand_end);

    char c = *text;
    while (c) {
        if (IsSpace (c)) {
            return text;
        }
        if (++text > strand_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
}

#endif  //< USING_SCRIPT_TEXT
}       //< namespace _
