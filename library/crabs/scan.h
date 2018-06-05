
/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/scan.h
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
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_CRABS_SCAN
#define HEADER_FOR_CRABS_SCAN
// Dependencies:
#include "config.h"
// End dependencies.
#if USING_PRINTER

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
KABUKI const char* Scan (int8_t& result, const char* text);

/** Converts the given string to a 8-bit unsigned integer.
    @param  text A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (uint8_t& result, const char* text);

/** Converts the given string to a 16-bit signed integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (int16_t& result, const char* text);

/** Converts the given string to a 16-bit unsigned integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (uint16_t& result, const char* text);

/** Converts the given string to a 32-bit signed integer.
    @param  text A nil-terminated string in ROM.
    @param  result  The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (int32_t& result, const char* text);

/** Converts the given string to a 32-bit unsigned integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (uint32_t& result, const char* text);

/** Converts the given string to a 64-bit signed integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (int64_t& result, const char* text);

/** Converts the given string to a 64-bit unsigned integer.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (uint64_t& result, const char* text);

/** Converts the given string to a 32-bit floating-point number.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (float& result, const char* text);

/** Converts the given string to a 64-bit floating-point number.
    @param  text  A nil-terminated string in ROM.
    @param  result The result of the conversion.
    @return Returns a pointer to the next char after the end
            of the read number or nil upon failure. */
KABUKI const char* Scan (double& result, const char* text);

/** Reads a int8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (int8_t& result, const char* text,
                         const char* text_end);

/** Reads a uint8_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (uint8_t& result, const char* text,
                         const char* text_end);

/** Reads a int16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (int16_t& result, const char* text,
                         const char* text_end);

/** Reads a uint16_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (uint16_t& result, const char* text,
                         const char* text_end);

/** Reads a int32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (int32_t& result, const char* text,
                         const char* text_end);

/** Reads a uint32_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (uint32_t& result, const char* text,
                         const char* text_end);

/** Reads a int64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (int64_t& result, const char* text, const char* text_end);

/** Reads a uint64_t from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (uint64_t& result, const char* text,
                         const char* text_end);

/** Reads a float from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (float& result, const char* text,
                         const char* text_end);

/** Reads a double from the given buffer in the form of an ASCII string.
    @param begin  Beginning address of the buffer.
    @param end    The end address of the buffer.
    @param result The number read. */
KABUKI const char* Scan (double& result, const char* text,
                         const char* text_end);

}   //< namespace _
#endif  //< USING_PRINTER
#endif  //< HEADER_FOR_CRABS_SCAN
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
