/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_text.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2

#include "center.h"
#include "printer.h"
#if USING_PRINTER

#include "type.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_HEADING\
    std::cout << '\n';\
    for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#endif

namespace _ {

Center::Center (const char* string, int num_columns) :
    string (string),
    num_columns (num_columns) {
    assert (string);
    assert (num_columns > 0);
}

Printer& Center::Print (Printer& print) {
    char* cursor = print.cursor,
        * end    = print.end;
    assert (cursor);
    assert (string);
    assert (cursor < end);

    // We need to leave at least one space to the left and right of
    int length = TextLength (string),
        offset;
    PRINTF ("\n\n    Printing \"%s\":%i num_columns:%i", string, length,
            num_columns)

        if (num_columns < length) {
            offset = length - num_columns;
            if (offset > 3) {
                offset = 3;
            }
            num_columns -= offset;
            while (num_columns-- > 0) {
                *cursor++ = *string++;
            }
            while (offset-- > 0) {
                *cursor++ = '.';
            }
            *cursor = 0;
            print.cursor = cursor;
            return print;
        }
    offset = (num_columns - length) >> 1; //< >> 1 to /2
    length = num_columns - length - offset;
    PRINTF ("\n    length:%i offset:%i", length, offset)

    while (length-- > 0)
        *cursor++ = ' ';

    char c = *string++;
    while (c) {
        *cursor++ = c;
        c = *string++;
    }
    while (offset-- > 0)
        *cursor++ = ' ';
    *cursor = 0;
    PRINTF ("\n    Printed:\"%s\"", string);
    print.cursor = cursor;
    return print;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
