/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_line.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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
#include "line.h"
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {

Line::Line (char token, int column_count) :
    token (token),
    column_count (column_count) {
    // Nothing to do here. ({:-)-+=<
}

Printer& Line::Print (Printer& print) {
    char* cursor = print.cursor;
    ASSERT (cursor);
    char* end = print.end;
    int l_column_count = column_count;
    if (cursor + l_column_count + 1 >= end)
        return print;

    *cursor++ = '\n';

    char l_token = token;
    while (l_column_count-- > 0)
        *cursor++ = l_token;

    *cursor = 0;
    print.cursor = cursor;
    return print;
}

StringLine::StringLine (const char* string, int column_count) :
    string       (string),
    column_count (column_count) {
    // Nothing to do here. ({:-)-+=<
}

Printer& StringLine::Print (Printer& print) {
    char* cursor = print.cursor;
    ASSERT (cursor);
    char* end    = print.end;
    int l_column_count = column_count;
    if (cursor + l_column_count + 1 > end)
        return print;

    const char* l_string = string;
    while (l_column_count-- > 0) {
        char c = *l_string++;
        if (!c) {
            cursor = 0;
        }
        *cursor++ = c;
    }
    *cursor = 0;
    return print;
}

}
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
