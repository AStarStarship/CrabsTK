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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

#include "text.h"
#include "memory.h"

#if USING_TEXT_SCRIPT

#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

// Some of the Text functions will be required but not all of them.


namespace _ {

const char* TextEmpty () {
    return "";
}

const char* TextCR () {
    return "\n";
}

const char* TextErrorHeader () {
    return "\nError: ";
}

const char* TextNewLine () {
    return "\n";
}

byte TextNibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    if (b > 9)
        return b + ('a' - 10);
    return b + '0';
}

byte TextNibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b > 9) return b + ('A' - 10);
    return b + '0';
}

uint16_t TextByteToLowerCaseHex (byte b) {
    uint16_t value = TextNibbleToLowerCaseHex (b & 0xf);
    value = value << 8;
    value |= TextNibbleToLowerCaseHex (b >> 4);
    return value;
}

uint16_t TextByteToUpperCaseHex (byte b) {
    uint16_t value = TextNibbleToUpperCaseHex (b & 0xf);
    PRINTF ("\n    First nibble:%c", value)
    value = value << 8;
    uint16_t second_nibble = TextNibbleToUpperCaseHex (b >> 4);
    PRINTF (" second nibble:%c", second_nibble)
    value |= second_nibble;
    return value;
}

int TextHexToByte (byte c) {
    if (c < '0') {
        return -1;
    }
    if (c >= 'a') {
        if (c > 'f') return -1;
        return c - ('a' - 10);
    }
    if (c >= 'A') {
        if (c > 'F') return -1;
        return c - ('A' - 10);
    }
    if (c > '9') return -1;
    return c - '0';
}

int TextHexToByte (uint16_t h) {
    int lowerValue = TextHexToByte ((byte)(h >> 8));

    if (lowerValue < 0) return -1;

    int upper_value = TextHexToByte ((byte)h);
    if (upper_value < 0) return -1;

    return lowerValue | (upper_value << 4);
}

const char* TextEnd (const char* text, char delimiter) {
    char c = *text;
    while (c > delimiter) {
        c = *(++text);
    }
    return text;
}

const char* TextEnd (const char* text, const char* text_end, char delimiter) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    text = TextSkipSpaces (text, text_end);

    char c = *text;
    while (c > delimiter) {
        if (IsWhitespace (c)) {
            return text;
        }
        if (++text > text_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
}

int TextLength (const char* text, char delimiter) {
    if (!text) {
        return -1;
    }
    int count = 0;
    char c = *text;
    while (c > delimiter) {
        ++count;
        c = *(++text);
    }
    return count;
}

char* TextClone (const char* text, char delimiter) {
    if (!text) {
        text = "";
    }
    int length = TextLength (text, delimiter) + 1;
    if (length < 0) {
        return nullptr;
    }
    char* clone = new char[length];
    MemoryCopy (clone, clone + length, text, text + length + 1);
    return clone;
}

const char* TextSkipNumbers (const char* text, char delimiter) {
    if (!text) {
        return nullptr;
    }
    char c = *text;
    if (c <= delimiter) {
        return text;
    }
    if (c == '-') {  // It might be negative.
        c = *(++text);
        if (!isdigit (c)) {  // It's not negative.
            return text - 1;
        }
        c = *(++text);
    }
    while (isdigit (c)) {
        if (c <= delimiter) {
            return text;
        }
        c = *(++text);
    }
    return text;
}

const char* TextEquals (const char* text_a, const char* text_b, char delimiter) {
    if (!text_a) {
        return nullptr;
    }
    if (!text_b) {
        return nullptr;
    }
    PRINTF ("\nComparing \"%s\" to \"%s\"", text_a, text_b)

    char a = *text_a,
         b = *text_b;
    while (a > delimiter) {
        PUTCHAR (a)
        if (a != b) { // Not a hit.
            PRINTF ("\nBut it's not a hit")
            return nullptr;
        }
        if (b <= delimiter) { // Hit!
            PRINTF ("\nFound hit at 0x%p", text_a)
            return text_a;
        }
        a = *(++text_a);
        b = *(++text_b);
    }
    if (b > delimiter) {
        PRINTF ("\nNot a hit: no nil-term char found")
        return nullptr;
    }
    PRINTF ("\nFound hit at 0x%p", text_a);
    return text_a; //< Find hit!
}

const char* TextEquals (const char* text, const char* text_end,
                        const char* string, char delimiter) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (!string) {
        return nullptr;
    }
    PRINTF ("\nComparing \"%s\" to \"%s\"", text, string)

    char a = *text,
         b = *string;
    while (a > delimiter) {
        PUTCHAR (a)
        if (a != b) { // Not a hit.
            PRINTF ("\nBut it's not a hit")
            return nullptr;
        }
        if (b <= delimiter) { // Hit!
            PRINTF ("\nFound hit at 0x%p", text)
            return text;
        }
        if (text > text_end) {
            return nullptr;
        }
        a = *(++text);
        b = *(++string);
    }
    if (b > delimiter) {
        PRINTF ("\nNot a hit: no nil-term char found")
        return nullptr;
    }
    PRINTF ("\nFound hit at 0x%p.", text)
    return text;
}

const char* TextLineEnd (const char* target, int num_columns,
                         char delimiter) {
    char c;
    // Scroll to the end of the line.
    c = *target++;
    while (c > delimiter) {
        if (num_columns-- < 0) { // We've reached the end.
            // Scroll left till we hit whitespace (if any).
            while (!IsWhitespace (c)) {
                c = *(--target);
            }
            // Then scroll past the whitespace.
            while (IsWhitespace (c)) {
                c = *(--target);
            }
            return target;
            c = *target++;
        }
        c = *target++;
    }
    return target - 1;
}

const char* TextSkipChar (const char* text, char skip_char, char delimiter) {
    if (text == nullptr)
        return nullptr;
    char c = *text,
        d;
    if (c != skip_char) {
        return text;
    }
    d = *text;
    // We know the first char is a skip_char so just loop till c and d aren't
    // the skip_char.
    while (c == d) {
        c = d;
        d = *(++text);
    }
    return text;
}

const char* TextSkipSpaces (const char* text) {
    if (!text) {
        return nullptr;
    }
    char c = *text;
    while (IsWhitespace (c)) {
        PUTCHAR ('.')
        if (!c) { //< This isn't an error as far as I can see.
            return text;
        }
        ++text;
        c = *text;
    }
    return text;
}

const char* TextSkipSpaces (const char* text, const char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    PRINTF ("\nSkipping spaces: ")
    char c = *text;
    while (IsWhitespace (c)) {
        PUTCHAR ('.')
        if (!c) {
            return nullptr;
        }
        if (++text > text_end) {
            return nullptr;
        }
        c = *text;
    }
    return text;
}

const char* TextFind (const char* target, const char* query, char delimiter) {
    if (target == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    char string = *target, //< Current target char.
         t      = *query, //< Current query char.
         c      = t;      //< The first char of the query we're searching for.
    if (c <= delimiter)   //< We're not allowing empty queries.
        return nullptr;
    const char* start_of_query,
              * cursor = target;
    query = TextSkipSpaces (query);

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

const char* TextLineEnd (const char* text, const char* text_end,
                         int num_columns, char delimiter) {
    if (!text) {
        return nullptr;
    }
    if (text_end > text) {
        return nullptr;
    }
    char c;
    // Scroll to the end of the line.
    c = *text++;
    while (c > delimiter) {
        if (text_end > text) {
            return nullptr;
        }
        if (num_columns-- < 0) { // We've reached the end.

                                 // Scroll left till we hit whitespace (if any).
            while (!IsWhitespace (c)) {
                c = *(--text);
            }
            // Then scroll past the whitespace.
            while (IsWhitespace (c)) {
                c = *(--text);
            }
            return text;
            c = *text++;
        }
        c = *text++;
    }
    return text - 1;
}

const char*TextNextNonNumber (const char* text, const char* text_end,
                              char delimiter) {
    if (text > text_end) {
        return nullptr;
    }
    char c = *text;
    if (c == '-') {  // It might be negative.
        if (text > text_end) {
            return nullptr;
        }
        c = *(++text);
        if (!isdigit (c)) {  // it's not negative.
            return text - 1;
        }
        c = *(++text);
    }
    while (isdigit (c)) {
        if (text > text_end) {
            return nullptr;
        }
        c = *(++text);
    }
    return text;
}

bool TextQualifies (const char* text, char delimiter) {
    if (text == nullptr) {
        return false;
    }
    char c = *text;
    while (c > delimiter) {
        if (!IsWhitespace (c)) {
            return true;
        }
        c = *(++text);
    }
    return false;
}

bool TextQualifies (const char* text, const char* text_end, char delimiter) {
    if (!text) {
        return false;
    }
    if (text > text_end) {
        return false;
    }
    char c = *text;
    while (c > delimiter) {
        if (!IsWhitespace (c)) {
            // The text must end at or before the target_end.
            do {
                if (++text > text_end) {
                    return false;
                }
                c = *text;
                if (!IsWhitespace (c)) {
                    return true;
                }
            } while (c > delimiter);
            return true;
        }
        if (++text > text_end) {
            return false;
        }
        c = *text;
    }
    return false;
}

const char* TextEquals (const char* text_a, const char* text_b) {
    text_a = TextSkipSpaces (text_a);
    if (!text_a) {
        return nullptr;
    }
    if (!text_b) {
        return nullptr;
    }

    char a = *text_a,
         b = *text_b;
    int  result;
    
    PRINTF ("\nComparing \"%s\" to \"%s\"", text_a, text_b)

    // text SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        PRINTF ("\nb - a = %i - %i = %i", b, a, result)
        if (result) {
            PRINTF (" is not a hit.")
            return nullptr;
        }
        if (!a) {
            PRINTF (" is a partial match but !a.")
            return nullptr;
        }
        ++text_a;
        ++text_b;
        a = *text_a;
        b = *text_b;
    }
    if (a && !IsWhitespace (a)) {
        PRINTF (" is only a partial match but found %s", (a ? "a" : "space"))
        return nullptr;
    }
    PRINTF (" is a match!")
    return text_a;
}

int TextCompare (const char* text_a, const char* text_b) {
    int a,
        b,
        result;
    if (!text_a) {
        if (!text_b) {
            return 0;
        }
        return *text_a;
    }
    if (!text_b) {
        return 0 - *text_a;
    }

    PRINTF ("\nComparing \"%s\" to \"%s\"", text_a, text_b)
    a = *text_a;
    b = *text_b;
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
    // text SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        PRINTF ("\nb - a = %i - %i = %i", b, a, result)
        if (result) {
            PRINTF (" is not a hit.")
            return result;
        }
        if (!a) {
            PRINTF (" is a partial match but !a.")
            return result;
        }
        ++text_a;
        ++text_b;
        a = *text_a;
        b = *text_b;
    }
    if (a && !IsWhitespace (a)) {
        PRINTF (" is only a partial match but found %s", (a ? "a" : "space"))
        return b - a;
    }
    return 0;
}

int TextCompare (const char* text_a, const char* text_end,
                 const char* text_b) {
    char a = *text_a,
        b = *text_b;
    int result;

    if (!text_a) {
        if (!text_b) {
            return 0;
        }
        a = 0;
        b = *text_b;
        return b - a;
    }
    if (!text_b) {
        a = *text_a;
        b = 0;
        return b - a;
    }
    if (text_a > text_end) {
        return *text_b;
    }

    // Algorithm combines loops for better performance.
    a = *text_a;
    b = *text_b;
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
    // text SHOULD be a nil-terminated string without whitespace.
    while (b) {
        result = b - a;
        PRINTF ("\nb - a = %c - %c = %i", b, a, result)
        if (result) {
            PRINTF (" is not a hit.")
            return result;
        }
        if (!a) {
            PRINTF (" is a partial match but !a.")
            return result;
        }
        if (++text_a > text_end) {
            PRINTF (" but buffer overflowed!")
            return result;
        }
        ++text_b;
        a = *text_a;
        b = *text_b;
    }
    if (a && !IsWhitespace (a)) {
        PRINTF (" is only a partial match but found %s", (a ? "a" : "space"))
        return b - a;
    }
    PRINTF (" is a match!")
    return 0;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_TEXT_SCRIPT
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2
