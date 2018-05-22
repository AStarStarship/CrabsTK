/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/script_itoa.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1

#include "print_to_ascii.h"

namespace _ {

/** ASCII printing utilities
    @ingroup Printer
*/

/** Utility class for printing strings.
    This class only stores the end of buffer pointer and a pointer to the write 
    cursor. It is up the user to store start of buffer pointer and if they would 
    like to replace the cursor with the beginning of buffer pointer when they 
    are done printing.
*/
struct KABUKI Printer {

    char* cursor,  //< Write cursor pointer.
        * end;     //< End of buffer pointer.

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin, size_t size_bytes);

    /** Initializes the Printer from the given buffer pointers.
        @param begin The beginning of the buffer.
        @param end   The end of the buffer. */
    Printer (char* begin, char* end);

    /** Clones the other print. */
    Printer (const Printer& other);
};

/** Utility class for printing numbers. */
template<typename T>
struct Number {
    enum {
        kSize = sizeof (T) == 2 ? 8 :
        sizeof (T) == 4 ? 16 : 24, //< Size of the text buffer in bytes.
    };

    T value;            //< Value to print.
    char string[kSize]; //< String buffer.

                        /** Constructor copies value and prints it to the string buffer. */
    Number (T value) :
        value (value) {
        Print (value, string, string + kSize - 1);
    }
};

/** Utility class for printing to the CRABS Output. */
template<size_t kSize = kBufferSizeDefault>
class Console {
    public:

    /** Constructs the Printer pointers to the buffer_. */
    Console () :
        out_ (buffer_, kSize) {
        *buffer_ = 0;
    }

    /** Prints the buffer_. */
    ~Console () {
        std::cerr << buffer_;
    }

    /** Gets the begin of the Console buffer. */
    inline char* GetBegin () {
        return buffer_;
    }

    /** Gets the Printer. */
    inline Printer Print () {
        return out_;
    }

    private:

    Printer out_;        //< Printer.
    char buffer_[kSize]; //< Console buffer.
};

#define COUT ::_::Console<> ().Print ()

/** Copies a char from the source to the text.
    @param text   Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param string The potentially unsafe string to write. */
KABUKI char* Print (char* cursor, char* end, const char* string);

/** Copies a char from the source to the text.
    @param text   Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param value The potentially unsafe string to write. */
KABUKI char* Print (char* cursor, char* end, const char* string, 
                    const char* string_end);

/** Writes the give char to the given buffer.
    @param begin     Beginning address of the buffer.
    @param end       The end address of the buffer.
    @param character The value to write. */
KABUKI char* Print (char* cursor, char* end, char character);

}       //< namespace _

/** Writes a nil-terminated UTF-8 or ASCII string to the print.
    @param  printer The print.
    @param  value   The value to print. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& p, const char* string) {
    char* cursor = _::Print (p.cursor, p.end, string);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value   The value to print. 
    @return The printer. */
inline _::Printer& operator<< (_::Printer& p, char value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  print The Printer to print to.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, int32_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, uint32_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, int64_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, uint64_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, int32_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, uint32_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, int64_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, uint64_t value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, float value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

/** Writes the given value to the print.
    @param  printer The print.
    @param  value The value to write to the print. 
    @return The print. */
inline _::Printer& operator<< (_::Printer& p, double value) {
    char* cursor = _::Print (p.cursor, p.end, value);
    if (!cursor)
        return p;
    p.cursor = cursor;
    return p;
}

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
