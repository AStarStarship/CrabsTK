
/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/scan.h
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

#ifndef HEADER_FOR_CRABS_SCAN
#define HEADER_FOR_CRABS_SCAN

#include "config.h"

#if CRABS_SEAM >= 1
#if USING_CRABS_TEXT

namespace _ {

/** String (STR) and Address (ADR) Text (TKN) scanning utilities
    @ingroup Scan

    Scanning specifically refers to converting from strings, or
    the opposite of print.
*/

/** Converts the given string to a 8-bit signed integer.
    @param  text A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, int8_t& result);

/** Converts the given string to a 8-bit unsigned integer.
    @param  text A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, uint8_t& result);

/** Converts the given string to a 16-bit signed integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, int16_t& result);

/** Converts the given string to a 16-bit unsigned integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, uint16_t& result);

/** Converts the given string to a 32-bit signed integer.
    @param  text A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, int32_t& result);

/** Converts the given string to a 32-bit unsigned integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, uint32_t& result);

/** Converts the given string to a 64-bit signed integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, int64_t& result);

/** Converts the given string to a 64-bit unsigned integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, uint64_t& result);

/** Converts the given string to a 32-bit floating-point number.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, float& result);

/** Converts the given string to a 64-bit floating-point number.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (const char* text, double& result);

/** Reads a int8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             int8_t& result);

/** Reads a uint8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             uint8_t& result);

/** Reads a int16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             int16_t& result);

/** Reads a uint16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             uint16_t& result);

/** Reads a int32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             int32_t& result);

/** Reads a uint32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             uint32_t& result);

/** Reads a int64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end, 
                               int64_t& result);

/** Reads a uint64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             uint64_t& result);

/** Reads a float from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                             float& result);

/** Reads a double from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (const char* text, const char* text_end,
                         double& result);

}   //< namespace _
#endif  //< USING_CRABS_TEXT
#endif  //< CRABS_SEAM >= 1
#endif  //< HEADER_FOR_CRABS_SCAN
