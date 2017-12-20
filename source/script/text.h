/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/text.h
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

#include "strand.h"

#if SCRIPT_USING_TEXT

namespace _ {

/** A group of zero or more UTF-8 or ASCII strings.
    Text is intended to work similar to the std::cout using overloaded
    operator<<. The class uses a statically allocated buffer allowing you to
    put it on the stack. The default size is 141 to allow for 140 char_t(s).
*/
class Text {
    public:

    enum {
        kSize = SCRIPT_TEXT_SIZE_MAX, //< Size of the Text buffer.
    };

    /** Constructor creates a Text from the given strand. */
    Text ();

    /** Constructor creates a Text from the given strand. */
    Text (const char* strand);

    /** Constructor copies the other Text buffer up the cursor. */
    Text (const Text& other);

    /** Clears the text buffer without overwriting it. */
    Text& Clear ();

    /** Prints the text buffer before clearing it. */
    Text& COut ();

    /** Prints the text buffer before clearing it. */
    Text& CErr ();

    /** Gets the char count of the buffer in bytes. */
    int Length () const;

    /** Gets the char count of the buffer in bytes. */
    int Space () const;

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

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Print (char value);
    
    /** Writes the given strand to the buffer_.
        @param  Strand to write. 
        @return Reference to *this. */
    Text& Write (const char* strand);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (int8_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (uint8_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (int16_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (uint16_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (int32_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (uint32_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (int64_t value);

    /** Writes the given value to the buffer_.
        @param  Value to write.
        @return Reference to *this. */
    Text& Write (uint64_t value);

    /** Writes the given value to the buffer_.
        @param  Strand to write.
        @return Reference to *this. */
    Text& Write (float value);

    /** Writes the given value to the buffer_.
        @param  Strand to write.
        @return Reference to *this. */
    Text& Write (double value);

    /** Writes the given text to the buffer_.
        @param  Strand to write.
        @return Reference to *this. */
    Text& Write (Text& text);

    /** Prints the given token a specified num_times. */
    Text& Line (char token = '-', const char* left_column_text = "\n|",
                int num_columns = 80);

    /** Prints an 80-char line of the char repeating with an underscore
        i.e. s_s_... */
    Text& Line (const char* strand, int num_columns = 80);

    /** Prints a bunch of new lines followed by the given message and a line of
    the specified token. */
    Text& LineBreak (const char* message, int top_bottom_margin,
                          char c = '-', int num_columns = 80);

    /** Prints a vertical tab with the given number of rows. */
    Text& Lines (int numRows = 10);

    /** Print's out the given pointer to the console. */
    Text& Pointer (const void* pointer);

    Text& Error (const char* message, const char* end_string = TextCR ());

    /** Prints the given byte in Hex.
        This function prints the hex in big endian. */
    Text& Hex (byte c);

    /** Print's out the given pointer to the console. */
    Text& Pointer (void* pointer);

    /** Prints the given value in hex format. */
    template<typename Type>
    Text& Hex (Type value) {
        char buffer[sizeof (Type) * 2 + 1];
        sprintf_s (buffer, "%x", &value);
        std::cout << "0x";

        int length = StrandLength (buffer);
        for (int i = 0; i < length; ++i)
            std::cout << '0';
        std::cout << buffer;
    }

    /** Prints an 80-char line of the number repeating with an underscore
    i.e. 1_1_... */
    Text& NumberLine (int index);

    /** Creates the format char for a key-value pair where the key is
        right-aligned to the column_width.
            @depreciated */
    //char PrintCreateKeyValueFormatText (char* string, char column_width,
    //                                    char type);
    /** 
    template<char kColumnWidth>
    Text& Hex (const char* header, Text&* value) {
        char formatText[12],
            nil_term_char = PrintCreateKeyValueFormatText (formatText,
                                                          kColumnWidth, 'p');
        printf (formatText, header, value);
    }*/

    /** 
    template<char kColumnWidth, typename Type>
    Text& SignedHex (const char* header, Type value) {
        char formatText[12],
            nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                          'i');
        printf ("formatText: %s\n", formatText);
        printf (formatText, header, value);
    }*/

    /** 
    template<char kColumnWidth, typename Type>
    Text& UnsignedHex (const char* header, Type value) {
        char formatText[12],
            nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                          'u');
        printf (formatText, header, value);
    }*/

    /** Prints the given value to the console and prompts the user to press any key
        to continue. 
    template<typename Type, const char* format>
    Type PrintReturn (Type value) {
        printf (format, value);
        return value;
    }*/

    /** Prints the array starting at the base_ptr with the given numElements with a
        header and given format. 
    template<typename Type>
    Text& Array (const char* header,
                 const char* format,
                 Type* base_ptr,
                 Type numElements) {
        PrintLine ();
        std::cout << header << ": numElements: " << numElements << '\n';
        for (Type i = 0; i < numElements; ++i) {
            printf (format, i);
            std::cout << ": ";
            printf (format, base_ptr[i]);
            std::cout << '\n';
        }
        std::cout << '\n';
        PrintLine ();
    }*/

    /** Prints an error message and pauses the system. */
    //Text& Pause (const char* string);

    /** Prints a new line. */
    Text& NewLine ();

    /** Prints the given char with a '|' at the beginning followed by a new line */
    Text& Bar (const char* input);

    /** Prints a line break. */
    Text& Break (const char* header = "\n_",
                             char c = '_',
                             int num_lines = 0,
                             int console_width = 80);

    /** Prints a line break with the given number of columns. */
    Text& ColumnBreak (int num_columns,
                                   char column_delimeter = '|',
                                   char break_char = '-',
                                   int width = 80);

    /** Prints the given char centered with a horizontal page bar to the left
        and right of the row.
        @param input The char to print.
        @param num_columns */
    Text& Centered (const char* input, int num_columns);

    /** Prints the given strand centered about the given width in console char
        rows.
        If char is too small to fit in the width, function will print as much of
        the char as it has room for with a "..." If the given width is less than
        5 then only dots will be printed. */

    /** Prints the given char centered. */
    Text& Centered (const char* input, int width, bool is_last,
                    char column_delimeter = '|');

    /** Prints the given char justified right.
        @param input The char to print.
        @param num_columns The number of columns per row. */
    Text& Right (const char* input, int num_columns);

    /** Prints the given input indented and bulleted with and '|' at the beginning
        and end of the line representing a page.

        @code
        print ("List Item 1", 0
        @endcode

        @param input The input to print.
        @param indent The number of tabs indent level.
        @param bullet The type of bullet to print.
        @param index  The index of the bullet in the list.
        @param tab_size The number of spaces per tab.
        @param num_columns The number of columns per line. */
    Text& Page (const char* input = "", int indentation = 0,
                char bullet = '*', int index = 0, int tab_size = 4,
                int num_columns = 80);

    /** Prints out the contents of the address to the debug stream. */
    Text& Memory (const void* address, const void* end);

    /** Prints out the contents of the address to the debug stream. */
    Text& Memory (const void* address, int size);

    /** Prints out the contents of the address to the debug stream. */
    //inline Text& Memory (const void* address, int num_bytes) {
    //    return Memory (address,
    //                   reinterpret_cast<const byte*> (address) + num_bytes);
    //}

    /** Prints out the contents of the address to the debug stream. */
    //inline Text& Memory (const void* address, uint num_bytes) {
    //    return Memory (address,
    //                   reinterpret_cast<const byte*> (address) + num_bytes);
    //}


    /** Prints the first whitespace or nil terminated token from the string.
        @param strand A nil-terminated string in ROM.
        @return A pointer to the end of the token or nil upon failure. */
    Text& Token (const char* strand);

    /** Prints the first whitespace or nil terminated token from the text.
        @param text     A nil-terminated string in RAM or ROM.
        @param strand_end End of the text buffer.
        @return A pointer to the end of the token or nil upon failure. */
    Text& Token (const char* text, const char* strand_end);

    /** Prints the given tm struct to the console. */
    Text& TimeStruct (tm* std_tm);

    /** Prints the given tm struct to the console. */
    Text& TimeStruct (tm* std_tm, int32_t microseconds);

    /** Prints the given time to the console. */
    Text& Time (time_t t);

    /** Prints the given Script Timestamp to the console. */
    Text& Timestamp (time_us_t timestamp);

    /** Prints the given time to the console. */
    Text& DateTime (time_t t);

    /** Prints out an error report to the stdout.
        @param e The error type.
        @param params      The parameter header.
        @param param_index The index in the params where the error occurred.
        @param source      The source buffer address.
    Text& Print (Expression::Error error, const uint_t* params, 
                 byte param_index, void* source); */

    /** Overloaded operator= clones the other Text. */
    Text& operator= (const Text& other);
    
    /** Prints this Text to the stdout. */
    Text& Print ();

    /** Prints this Text to the stdout. */
    Text& Print (Text& text);

    /** Prints this Text to the stdout. */
    Text& Print (const char* text);

    /** Prints this Text to the stderr. */
    Text& Err ();

    /** Prints this Text to the stderr. */
    Text& Err (Text& text);

    /** Prints this Text to the stderr. */
    Text& Err (const char* text);

    private:

    char* cursor_;        //< Buffer write cursor.
    char  buffer_[kSize]; //< Text buffer.

};  //< struct Text

}   //< namespace _

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    text. */
inline std::ostream& operator<< (std::ostream& out_stream, _::Text& text) {
    return out_stream << text.GetBegin ();
}

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    text. */
inline _::Text& operator<< (_::Text& text_a, _::Text& text_b) {
    return text_a.Write (text_b);
}

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    text. */
inline _::Text& operator<< (_::Text& text, const char* strand) {
    return text.Write (strand);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int8_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint8_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int16_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint16_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int32_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint32_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, int64_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, uint64_t value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, float value) {
    return text.Write (value);
}

/** Operation << writes the given value to the text. */
inline _::Text& operator<< (_::Text& text, double value) {
    return text.Write (value);
}

#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_TEXT
