/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/params.h
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

#ifndef HEADER_FOR_SCRIPT_PARAMS
#define HEADER_FOR_SCRIPT_PARAMS

#include "types.h"
#include "text.h"

namespace _ {

/** Returns a static const Script B-Sequence header.
    C++11 variadic template to ensure only one copy in ROM 
    and to eliminate some redundant typing. */
template<const uint_t... N>
inline const uint_t* Params () {
    static const uint_t kList[sizeof... (N)] = { N... };
    return kList;
}

/*< Returns the requested parameter number. */
KABUKI uint_t ParamsNumber (const uint_t* params, int param_number);

#if SCRIPT_USING_TEXT
/**  Prints out the parameters to the debug console. */
KABUKI Text& ParamsPrint (const uint_t* prams, Text& text);
#endif  //< SCRIPT_USING_TEXT

}       //< namespace _

#if SCRIPT_USING_TEXT
/**  Prints out the parameters to the debug console. */
KABUKI _::Text& operator<< (_::Text& text, const uint_t* params);
#endif  //< SCRIPT_USING_TEXT

#endif  //< HEADER_FOR_SCRIPT_PARAMS
