/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/text.h
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

#ifndef HEADER_FOR_SCRIPT_TEXT
#define HEADER_FOR_SCRIPT_TEXT

#include "operation.h"

#if SCRIPT_USING_TEXT

namespace _ {

/** Group of functions for working with strings with explicitly declared end
    of buffer.
    Currently the read-write functions use sprintf. This is not the optimal 
    behavior and the functions needs to bypass the formating string.
    @ingroup Text
*/

enum {
    kMaxDigitsFloat  = FLT_MAX_10_EXP + 2,
    kMaxDigitsDouble = DBL_MAX_10_EXP + 2,
};

/** Empty string. */
KABUKI const char* TextEmpty ();

/** New line string. */
KABUKI const char* TextCR ();

/** Error header string. */
KABUKI const char* TextErrorHeader ();

/** New line and vertical bar "\n| " string. */
KABUKI const char* TextNewLine ();

/** Skips all the spaces at the start of the char.
    @param  text      Beginning address of the input buffer.
    @param  text_end  End address of the input buffer.
    @return A pointer to the end of the token read or if no token read. */
KABUKI const char* TextSkipSpaces (const char* text, const char* text_end);

/** Reads a whitespace-delineated token from the given text buffer.
    @param  text      Beginning address of the input buffer.
    @param  text_end  End address of the input buffer.
    @param  token     Beginning address of the token buffer.
    @param  token_end End address of the token buffer.
                      strands.
    @return A pointer to the end of the token read or if no token read. */
KABUKI const char* TokenRead (const char* text, const char* text_end, 
                                  char* token, char* token_end);

/** Gets the end of the current whitespace-delineated token.
    @param  strand A UTF-8 or ASCII string.
    @return Returns a pointer to the end of the token. */
KABUKI const char* TokenEnd (const char* target, const char* target_end);

/** Gets the end of the current whitespace-delineated token.
    @param  strand A UTF-8 or ASCII string.
    @return Returns a pointer to the end of the token. */
KABUKI const char* TokenEnd (const char* target, const char* target_end);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI int TokenCompare (const char* target, const char* target_end,
                         const char* token);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI int TokenCompare (const char* target, const char* target_end,
                         const char* token, const char* token_end);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* TokenEquals (const char* target, const char* target_end,
                                const char* token);

/** Compares the source and query char using the delimiter to terminate the query. */
KABUKI const char* TokenEquals (const char* target, const char* target_end,
                                const char* token, const char* token_end);

/** Checks if the given char is a token.
    @param strand The char to check.
    @returns Returns true if the given char is a valid token. */
KABUKI bool TokenQualifies (const char* target, const char* target_end);

/** Writes the given strand to the text buffer.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The potentially unsafe string to write. */
KABUKI char* TextWrite (char* target, char* target_end, const char* strand);

/** Copies a char from the source to the text.
    @param text      Beginning address of the buffer.
    @param text_end  End address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI char* TextWrite (char* target, char* target_end, const char* strand,
                        char delimiter);

/** Copies a char from the source to the text.
    @param text      Beginning address of the buffer.
    @param text_end  End address of the buffer.
    @param value     The potentially unsafe string to write.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI char* TextWrite (char* target, char* target_end, const char* text,
                        const char* text_end, char delimiter);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, int8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, uint8_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, int16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, uint16_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, int32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, uint32_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, int64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, uint64_t value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, float value);

/** Writes the give value  to the given buffer as an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param value    The value to write. */
KABUKI char* TextWrite (char* target, char* target_end, double value);

/** Returns a pointer to the char at the end of the row? */
KABUKI const char* TextLineEnd (const char* target, const char* target_end,
                                  int num_columns);

/** Returns the pointer to the next char in the char that is not an ASCII
    number.
    @return A pointer to the next non-number in the strand char. */
KABUKI const char* TextNextNonNumber (const char* target, const char* target_end);

/** Reads a whitespace-delineated token from the given text buffer.
    @param text      Beginning address of the input buffer.
    @param text_end  End address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    strands. */
KABUKI const char* TextRead (const char* text, const char* text_end, 
                             char* token, char* token_end);

/** Reads a explicitly-delineated token from the given text buffer.
    @param text      Beginning address of the input buffer.
    @param text_end  End address of the input buffer.
    @param token     Beginning address of the token buffer.
    @param token_end End address of the token buffer.
    @param delimiter A NON-ZERO delimiter to separate the string text into
    strands. */
KABUKI const char* TextRead (const char* text, const char* text_end, 
                             char* token, char* token_end, char delimiter);

/** Reads a int8_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             int8_t& result);

/** Reads a uint8_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             uint8_t& result);

/** Reads a int16_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             int16_t& result);

/** Reads a uint16_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             uint16_t& result);

/** Reads a int32_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             int32_t& result);

/** Reads a uint32_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             uint32_t& result);

/** Reads a int64_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end, 
                             int64_t& result);

/** Reads a uint64_t from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             uint64_t& result);

/** Reads a float from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             float& result);

/** Reads a double from the given buffer in the form of an ASCII string.
    @param text     Beginning address of the buffer.
    @param text_end End address of the buffer.
    @param result   The number read. */
KABUKI const char* TextRead (const char* target, const char* target_end,
                             double& result);

/** A group of zero or more UTF-8 or ASCII strings.
    Text is intended to work similar to the std::cout using overloaded
    operator<<. The class uses a statically allocated buffer allowing you to
    put it on the stack. The default size is 141 to allow for 140 char_t(s).
*/
class Text {
    public:

    enum {
        kMixSize = 16,                   //< Min buffer size.
        kSize    = SCRIPT_TEXT_SIZE_MAX, //< Size of the Text buffer.
    };

    /** Constructor creates a Text from the given strand. */
    Text (const char* strand = "");

    /** Constructor copies the other Text buffer up the cursor. */
    Text (const Text& other);

    /** Clears the text buffer. */
    void Clear ();

    /** Deep copies the state of the other object. */
    void Clone (const Text& other);
    
    /** Gets a pointer to the beginning of the buffer. */
    char* GetBegin ();

    /** Gets a pointer to the current write location in the buffer_. */
    char* GetCursor ();

    /** Gets a pointer to the end of the buffer. */
    char* GetEnd ();

    /** Sets the cursor to the new value. 
        @return Returns false if the new_cursor is out of bounds. */
    bool SetCursor (char* new_cursor);

    /** Gets the char count of the buffer in bytes. */
    int GetCount () const;

    /** Prints this Text to the stdout. */
    void Print (Text& txt = Text ());

    /** Overloaded operator= clones the other Text. */
    Text& operator= (const Text& other);

    private:

    char* cursor_;        //< Buffer write cursor.
    char  buffer_[kSize]; //< Text buffer.

};  //< struct Text

}   //< namespace _

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    text. */
inline _::Text& operator<< (_::Text& text, const char* strand) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), strand);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int8_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given int8_t to the text. 
inline _::Text& operator>> (_::Text& text, int8_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint8_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given uint8_t from the text. 
inline _::Text& operator>> (_::Text& text, uint8_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int16_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given int16_t from the text. 
inline _::Text& operator>> (_::Text& text, int16_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint16_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given uint16_t from the text. 
inline _::Text& operator>> (_::Text& text, uint16_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int32_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given int32_t from the text. 
inline _::Text& operator>> (_::Text& text, int32_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint32_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given uint32_t from the text. 
inline _::Text& operator>> (_::Text& text, uint32_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int64_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given int64_t from the text. 
inline _::Text& operator>> (_::Text& text, int64_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint64_t value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given uint64_t from the text. 
inline _::Text& operator>> (_::Text& text, uint64_t value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, float value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given float from the text. 
inline _::Text& operator>> (_::Text& text, float value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, double value) {
    char* cursor = _::TextWrite (text.GetBegin (), text.GetEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}

/** Operation >> reads the given double from the text. 
inline _::Text& operator>> (_::Text& text, double value) {
    char* cursor = _::TextRead (text.GetText (), text.GetTextEnd (), value);
    if (cursor == nullptr) {
        return text;
    }
    text.SetCursor (cursor + 1);
    return text;
}*/

#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_TEXT
