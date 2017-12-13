/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_token.cc
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

#if SCRIPT_USING_TOKEN
#include "strand.h"
#include "text.h"

using namespace std;

namespace _ {

const char* TokenPrint (const char* strand) {
    if (!strand) {
        return nullptr;
    }
    char c = *strand;
    while (c && !isspace (c)) {
        cout << c;
    }
    return "";
}

const char* TokenPrint (const char* text, const char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    char c = *text;
    while (c && !isspace (c)) {
        cout << c;
        if (++text > text_end) {
            return nullptr;
        }
    }
    return "";
}

bool TokenQualifies (const char* strand) {
    if (strand == nullptr) {
        return false;
    }
    char c = *strand;
    while (c) {
        if (!isspace (c)) {
            return true;
        }
        c = *(++strand);
    }
    return false;
}

bool TokenQualifies (const char* text, const char* text_end) {
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

    //cout << "\n| Comparing \"" << strand << "\" to \"" << token << "\"";

    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        //cout << "\n| b - a = " << b << " - " << a << " = " << result;
        if (result) {
            //cout << " is not a hit.";
            return nullptr;
        }
        if (!a) {
            //cout << " is a partial match but !a.";
            return nullptr;
        }
        ++strand;
        ++token;
        a = *strand;
        b = *token;
    }
    if (a && !isspace (a)) {
        //cout << " is only a partial match but found " << (a ? "a" : "space");
        return nullptr;
    }
    //cout << " is a match!";
    return strand;
}

const char* TokenEquals (const char* text, const char* text_end,
                         const char* token) {
    text = TextSkipSpaces (text, text_end);
    if (!text) {
        return nullptr;
    }
    if (!token) { // We're assuming token is in ROM and has no whitespace.
        return nullptr;
    }
    char a = *text,
         b = *token;
    int  result;

    cout << "\n| Comparing \"" << text << "\" to \"" << token << "\"";

    a = *text;
    b = *token;
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        //cout << "\n| b - a = " << b << " - " << a << " = " << result;
        if (result) {
            cout << " has unmatched chars b - a = " << b << " - " << a << " = "
                 << result;
            return nullptr;
        }
        if (!a) {
            cout << " is a partial match but !a.";
            return nullptr;
        }
        if (++text > text_end) {
            cout << " but text buffer overflowed!";
            return nullptr;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (a && !isspace (a)) {
        cout << " is only a partial match but found " << (a ? "a" : "space");
        return nullptr;
    }
    cout << " is a match!";
    return text;
}

const char* TokenRead (const char* text, const char* text_end,
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

    cout << "\n| Reading token \"" << TokenPrint (text, text_end) << "\" and writing \"";

    char c = *text;
    while (c && !isspace (c)) {
        cout << c;
        if (++text > text_end) {
            cout << " but text buffer overflowed!";
            return nullptr;
        }
        if (++target > target_end) {
            cout << " but target buffer overflowed!";
            return nullptr;
        }
        *target = c;
        c = *text;
    }
    cout << '\"';
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

    //cout << "\n| Comparing \"" << text << "\" to \"" << token << "\"";
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
        //cout << "\n| b - a = " << b << " - " << a << " = " << result;
        if (result) {
            //cout << " is not a hit.";
            return result;
        }
        if (!a) {
            //cout << " is a partial match but !a.";
            return result;
        }
        ++strand;
        ++token;
        a = *strand;
        b = *token;
    }
    if (a && !isspace (a)) {
        //cout << " is only a partial match but found " << (a?"a":"space");
        return b - a;
    }
    return 0;
}

int TokenCompare (const char* text, const char* text_end,
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
    // token SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        //cout << "\n| b - a = " << b << " - " << a << " = " << result;
        if (result) {
            //cout << " is not a hit.";
            return result;
        }
        if (!a) {
            //cout << " is a partial match but !a.";
            return result;
        }
        if (++text > text_end) {
            //cout << " but buffer overflowed!";
            return result;
        }
        ++token;
        a = *text;
        b = *token;
    }
    if (a && !isspace (a)) {
        //cout << " is only a partial match but found " << (a?"a":"space");
        return b - a;
    }
    //cout << " is a match!";
    return 0;
}

const char* TokenEnd (const char* target) {
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

const char* TokenEnd (const char* text, const char* text_end) {
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

#endif  //< SCRIPT_USING_TOKEN
}       //< namespace _
