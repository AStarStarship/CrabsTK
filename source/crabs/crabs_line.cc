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
    column_count (column_count),
    token (token) {
    // Nothing to do here. ({:-)-+=<
}

char* PrintLine (char c, int num_columns, char* buffer, char* buffer_end,
                 char delimiter) {
    if (!buffer) {
        return nullptr;
    }
    if (buffer + num_columns > buffer_end) {
        return nullptr;
    }
    while (num_columns-- > 0) {
        *buffer++ = c;
    }
    *buffer = delimiter;
    return buffer;
}

char* PrintLine (const char* string, int num_columns, char* buffer,
                 char* buffer_end, char delimiter) {
    if (!buffer) {
        return nullptr;
    }
    if (buffer + num_columns > buffer_end) {
        return nullptr;
    }
    const char* cursor = string;
    while (num_columns-- > 0) {
        char c = *string++;
        if (!c) {
            cursor = string;
        }
        *buffer++ = c;
    }
    *buffer = delimiter;
    return buffer;
}

}
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
