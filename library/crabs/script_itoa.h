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

#include "config.h"

namespace _ {

/** Writes the give value to the given buffer as an ASCII string.
    @param value The value to write.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer. */
KABUKI char* Print (char* text, char* text_end, uint32_t value);

/** Writes the give value to the given buffer as an ASCII string.
    @param value The value to write.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer. */
KABUKI char* Print (char* text, char* text_end, uint32_t value, char delimiter);

/** Writes the give value to the given buffer as an ASCII string.
    @param value The value to write.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer. */
KABUKI char* Print (char* text, char* text_end, uint64_t value);

/** Writes the give value to the given buffer as an ASCII string.
    @param value The value to write.
    @param begin Beginning address of the buffer.
    @param end   The end address of the buffer. */
KABUKI char* Print (char* text, char* text_end, uint64_t value, char delimiter);

}       //< namespace _

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
