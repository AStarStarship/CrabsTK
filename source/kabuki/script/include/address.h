/** kabuki::script
    @version 0.x
    @file    ~/kabuki/source/include/kabuki/address.h
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

#ifndef SCRIPT_ADDRESS_H
#define SCRIPT_ADDRESS_H

#include "module_config.h"

namespace _ {
    
/** C++11 constexpr struct KABUKI for creating parameter headers with less typing. */

template<const char... N>
KABUKI const char* Address () {
    static const char path[sizeof... (N)] = { N... };
    static const char term_char = 0;
    return path;
}

/** Returns a pointer to static variable for writing the address { a, \0 } to.
*/
KABUKI uint16_t PackAddress (byte a);

/** Returns a pointer to static variable for writing the address { a, b, \0, \0
    } to. */
KABUKI uint32_t PackAddress (uint32_t a, uint32_t b);

/** Returns a pointer to static variable for writing the address { a, b, c, \0 }
to. */
KABUKI uint32_t PackAddress (uint32_t a, uint32_t b, uint32_t c);

/** Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
KABUKI uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c,
                             uint64_t d);

/** Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
KABUKI uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                             uint64_t e);

/* Returns a pointer to static variable for writing the address { a, b, c, d, e,
   f, g, \0 } to. */
KABUKI uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                             uint64_t e, uint64_t f);

/** Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
KABUKI uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                             uint64_t e, uint64_t f, uint64_t g);

}       //< namespace _
#endif  //< SCRIPT_ADDRESS_H
