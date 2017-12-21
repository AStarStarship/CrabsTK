/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_text.cc
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

#if SCRIPT_USING_TEXT

#include "memory.h"
#include "strand.h"
#include "clock.h"

namespace _ {

Text::Text () :
    cursor_ (buffer_) {
#if SCRIPT_DEBUG == CLASS___TEXT
    MemoryClear (buffer_, kSize);
#endif  //< SCRIPT_DEBUG == CLASS___TEXT
    *buffer_ = 0;
}

Text::Text (const char* strand) :
    cursor_ (buffer_) {
#if SCRIPT_DEBUG == CLASS___TEXT
    MemoryClear (buffer_, kSize);
#endif  //< SCRIPT_DEBUG == CLASS___TEXT
    char* cursor = StrandWrite (buffer_, buffer_ + kSize, strand);
    if (!cursor) {
        *buffer_ = 0;
    }
    cursor_ = cursor;
}

Text::Text (const Text& other) :
    cursor_ (buffer_ + other.Length ()) {
    char* cursor = cursor_;
    const char* other_buffer_end = other.buffer_ + (cursor - buffer_);
    MemoryCopy (buffer_, cursor, other.buffer_, other_buffer_end);
}

Text& Text::Clear () {
    *buffer_ = 0;
    cursor_ = buffer_;
    return *this;
}

int Text::Length () const {
    return (int)(cursor_ - buffer_);
}

int Text::Space () const {
    return (int)((buffer_ + kSize) - cursor_);
}

void Text::Clone (const Text& other) {
    int other_count = other.Length ();
    MemoryCopy (buffer_, buffer_ + other_count, other.buffer_,
                other.buffer_ + other_count);
    cursor_ = buffer_ + other_count;
}

char* Text::GetBegin () {
    return buffer_;
}

bool Text::SetCursor (char* new_cursor) {
    if (new_cursor < buffer_) {
        return false;
    }
    if (new_cursor >= buffer_ + kSize) {
        return false;
    }
    cursor_ = new_cursor;
    return true;
}

char* Text::GetCursor () {
    return cursor_;
}

char* Text::GetEnd () {
    return buffer_ + kSize;
}

Text& Text::operator= (const Text& other) {
    if (this == &other) {
        return *this;
    }
    Clone (other);
    return *this;
}

Text& Text::Write (const char* strand) {
    char* cursor = StrandWrite (cursor_, GetEnd (), strand);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (int8_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int8_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (uint8_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint8_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (int16_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int16_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (uint16_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint16_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (int32_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int32_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (uint32_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint32_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (int64_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write int64_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (uint64_t value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        return *this;
    }
    //std::cout << "\n| Write uint64_t:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (float value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    //std::cout << "\n| Write float:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (double value) {
    char* cursor = StrandWrite (cursor_, GetEnd (), value);
    if (!cursor) {
        *cursor_ = 0;
        return *this;
    }
    //std::cout << "\n| Write double:" << buffer_;
    cursor_ = cursor;
    return *this;
}

Text& Text::Write (Text& text) {
    return Write (text.GetBegin ());
}

Text& Text::Line (char token, const char* header, int length) {
    char* cursor = cursor_,
        * stop   = cursor,
        * end    = GetEnd ();
    if (cursor + length + 1 > end) {
        return *this;
    }
    stop = StrandWrite (cursor, end, header);
    if (!stop) {
        //std::cout << "\n| Error writting header!";
        return *this;
    }
    //if (!end) { // This will never happen because we just checked the bounds.
    //    return *this;
    //}
    length -= (int)(stop - cursor);
    stop = ((cursor + length) > end) ? end : cursor + length;
    
    while (cursor <= stop) {
        *cursor++ = token;
    }
    //std::cout << "\n| Wrote: " << cursor_;
    cursor_ = cursor;
    return *this;
}

Text& Text::StringLine (const char* string, int num_columns) {
    *this << Text ().Line ();
    char* cursor     = cursor_,
        * stop       = cursor + num_columns + 1, //< +1 for nil-term char.
        * end        = GetEnd ();
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

Text& Text::Error (const char* message, const char* end_string) {
    return Write ("\n| Error: ").Write (message).Write (end_string);
}

Text& Text::LineBreak (const char* message, int top_bottom_margin,
                      char c, int num_columns) {
    Lines (top_bottom_margin);
    *this << "\n| " << message;
    return Line (c, "\n|", num_columns);
}

Text& Text::Lines (int num_rows) {
    char* cursor = cursor_,
        * end    = GetEnd (),
        * stop   = cursor + num_rows + 1;
    while (cursor < stop) {
        *cursor++ = '\n';
    }
    *cursor = 0;
    cursor_ = cursor + 1;
    return *this;
}

Text& Text::Pointer (const void* pointer) {
    // @todo Replace with PrintHex.
    int bytes_written = sprintf_s (cursor_, (buffer_ + kSize) - cursor_, 
                                   "0x%p", pointer);
    cursor_ += bytes_written;
    return *this;
}

Text& Text::Hex (byte c) {
    uint16_t chars = MemoryByteToUpperCaseHex (c);
    return *this << (char)chars << ((char)(chars >> 8)) << ' ';
}

Text& Text::Pointer (void* pointer) {
    // @todo Replace with PrintHex.
    if (Space () < (2 + sizeof (void*) * 2)) {
        return *this;
    }
    cursor_ += sprintf_s (GetCursor (), Space (), "0x%p", pointer);
    return *this;
}

Text& Text::Print (char c) {
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

Text& Text::NumberLine (int index) {
    Write ('\n');
    enum {
        kMaxBufferSize = (sizeof (int) == 2)
        ?7
        :(sizeof (int) == 4)
        ?11
        :128
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

Text& Text::NewLine () {
    return *this << "\n| ";
}

Text& Text::Bar (const char* input) {
    return *this << "\n| " << input << '\n';
}

Text& Text::Break (const char* header, char c, int num_lines,
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

Text& Text::Centered (const char* string, int width) {
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
        while (isspace (c = *input++));
    } while (c);
    return *this;
}*/

Text& Text::Centered (const char* input, int width, bool is_last,
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
        StrandWrite (&format[1], format + 32, (is_last?width - 2:width - 1));
        *this << column_delimeter;
        sprintf_s (GetBegin (), Space (), format, input);
        if (is_last) {
            *this << column_delimeter << '\n';
        }
    }
    num_spaces = width - length - 1 - (is_last?1:0);
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

Text& Text::ColumnBreak (int num_columns, char column_delimeter,
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

Text& Text::Right (const char* strand, int num_columns) {
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
        while (isspace (c = *strand++));
    } while (c);
    return *this;
}

Text& Text::Page (const char* input, int indentation,
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
    } else if (isalpha (bullet)) { // Then it's a lower case bullet.
        cursor = 0;
        bullet += index % 26;  //<
        for (; index > 0; index -= 26)
            buffer[cursor++] = bullet;
        buffer[cursor] = '\0';
    } else {
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

Text& Text::Memory (const void* address, const void* end) {
    *this << "\n| " << 0;
    //  columns
    for (int i = 8; i <= 66; i += 8) {
        //Right (i, 8);
        sprintf_s (cursor_, (buffer_ + kSize) - cursor_, "%i", 8);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 65; ++i) {
        *this << '_';
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < end) {
        Write ('\n') << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            putchar (temp);
            ++chars;
        }
        *this << "| " << Pointer (chars + Diff (address, end));// (chars - 64);
    }
    Write ('\n') << '|';
    for (int i = 0; i < 64; ++i) {
        Write ('_');
    }
    return *this << "| " << Pointer (chars + Diff (address, end));
}

Text& Text::Memory (const void* address, int size) {
    return Memory (address, reinterpret_cast<const byte*> (address) + size);
}

Text& Text::Token (const char* strand) {
    if (!strand) {
        return *this;
    }
    char c = *strand;
    while (c && !isspace (c)) {
        *this << c;
    }
    return *this;
}

Text& Text::Token (const char* text, const char* strand_end) {
    if (!text) {
        return *this;
    }
    if (text > strand_end) {
        return *this;
    }
    char c = *text;
    while (c && !isspace (c)) {
        *this << c;
        if (++text > strand_end) {
            return *this;
        }
    }
    return *this;
}

Text& Text::TimeStruct (tm* std_tm) {
    if (std_tm == nullptr) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1 
                 << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
                 << std_tm->tm_min << ":" << std_tm->tm_sec;
}

Text& Text::TimeStruct (std::tm* std_tm, int32_t microseconds) {
    if (std_tm == nullptr) {
        return *this << "Null tm*\n";
    }
    return *this << std_tm->tm_year + kTimeEpoch << "-" << std_tm->tm_mon + 1 
                 << "-" << std_tm->tm_mday << "@" << std_tm->tm_hour << ":"
                 << std_tm->tm_min << ":" << std_tm->tm_sec << ":"
                 << microseconds;
}

Text& Text::Time (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << TimeStruct (&std_tm);
}

Text& Text::Timestamp (time_us_t timestamp) {
    time_t t = ClockGetSeconds (timestamp);
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    int32_t microseconds = ClockGetMicroseconds (timestamp);
    return TimeStruct (&std_tm, microseconds);
}

Text& Text::DateTime (time_t t) {
    tm std_tm;
    ClockLocalTime (&std_tm, t);
    return *this << std_tm.tm_hour << ":" << std_tm.tm_min << ":"
                 << std_tm.tm_sec;
}

#endif  //< SCRIPT_USING_TEXT
}       //< namespace _
