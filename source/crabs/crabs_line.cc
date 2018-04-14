/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_line.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

namespace _ {

Line::Line (char token, int column_count) :
    token (token),
    column_count (column_count) {
    // Nothing to do here. ({:-)-+=<
}

Printer& Line::Print (Printer& printer) {
    char* cursor = printer.cursor,
        * end    = printer.end;
    int l_column_count = column_count;
    char l_token = token;
    assert (cursor);
    assert (cursor + l_column_count + 1 < end);

    *cursor++ = '\n';

    while (l_column_count-- > 0) {
        *cursor++ = l_token;
    }
    *cursor = 0;
    printer.cursor = cursor;
    return printer;
}

StringLine::StringLine (const char* string, int column_count) :
    string       (string),
    column_count (column_count) {
    // Nothing to do here. ({:-)-+=<
}

Printer& StringLine::Print (Printer& printer) {
    char* cursor = printer.cursor,
        * end    = printer.end;
    int l_column_count = column_count;
    assert (cursor);
    assert (cursor + l_column_count + 1 < end);

    const char* cursor = string;
    while (l_column_count-- > 0) {
        char c = *string++;
        if (!c) {
            cursor = string;
        }
        *cursor++ = c;
    }
    *cursor = 0;
    return printer;
}

}
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
