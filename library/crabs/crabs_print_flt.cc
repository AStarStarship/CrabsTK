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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1

#include "print_to_ascii.h"

#if USING_PRINTER

#if MAJOR_SEAM == 1 && MINOR_SEAM == 1
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

char* Out (char* cursor, char* end, float value) {
    // @todo Replace with GrisuX algorithm that uses the Script ItoS Algorithm.
    intptr_t buffer_size = end - cursor,
             result = sprintf_s (cursor, buffer_size, "%f", value);
    if (result < 0) {
        *cursor = 0;
        return nullptr;
    }
    cursor += result;
    *cursor = 0;
    return cursor;
}

char* Out (char* cursor, char* end, double value) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = end - cursor,
             result = sprintf_s (cursor, buffer_size, "%f", value);
    if (result < 0) {
        *cursor = 0;
        return nullptr;
    }
    cursor += result;
    *cursor = 0;
    return cursor;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
