/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/display.h
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

#ifndef HEADER_FOR_SCRIPT_PRINTER
#define HEADER_FOR_SCRIPT_PRINTER

#include "text.h"

#if SCRIPT_USING_TEXT

namespace _ {

/** Utility for printing text to the stdout.
*/
struct Printer {

};  //< struct Printer

    /** Prints the Text to the stdout. */
inline Printer I () {
    return Printer ();
}

}   //< namespace _

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    printer. */
inline _::Printer& operator<< (_::Printer& printer, _::Text& txt) {
    std::cout << txt.GetBegin ();
    return printer;
}

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    printer. */
inline _::Printer& operator<< (_::Printer& printer, const char* strand) {
    std::cout << strand;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, int8_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, uint8_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, int16_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, uint16_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, int32_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, uint32_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, int64_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, uint64_t value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, float value) {
    std::cout << value;
    return printer;
}

/** Operation << writes the given value to the printer. */
inline _::Printer& operator<< (_::Printer& printer, double value) {
    std::cout << value;
    return printer;
}

#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_PRINTER
