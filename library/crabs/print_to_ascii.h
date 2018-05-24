/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/text.h
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

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1

#include "config.h"

#if USING_PRINTER

#ifndef HEADER_FOR_CRABS_PRINT_TO_ASCII
#define HEADER_FOR_CRABS_PRINT_TO_ASCII

namespace _ {

/** Writes the give value to the given buffer as an ASCII string.
    @param value The value to write.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer. */
KABUKI char* Out (char* cursor, char* end, uint32_t value);

/** Writes the give value to the given buffer as an ASCII string.
    @param  print The Printer& to print to.
    @param value The value to write. */
KABUKI char* Out (char* cursor, char* end, int32_t value);

/** Writes the give value to the given buffer as an ASCII string.
    @param  print The Printer& to print to.
    @param end   The end address of the buffer. */
KABUKI char* Out (char* cursor, char* end, uint64_t value);

/** Writes the give value to the given buffer as an ASCII string.
    @param  print The Printer& to print to.
    @param value The value to write. */
KABUKI char* Out (char* cursor, char* end, int64_t value);

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Out (char* cursor, char* end, float value);

/** Writes the give value to the given buffer as an ASCII string.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer.
    @param value The value to write. */
KABUKI char* Out (char* cursor, char* end, double value);

}   //< namespace _

#endif  //< HEADER_FOR_CRABS_PRINT_TO_ASCII
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
