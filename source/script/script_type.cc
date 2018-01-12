/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_types.cc
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

#include <stdafx.h>
#include "type.h"
#include "text.h"

namespace _ {

uint_t TypeSize (uint_t type) {
    static const int8_t kWidths[] = {
        0, //< NIL: 0 
        0, //< ADR: 1 
        0, //< STR: 2
        1, //< SI1: 3 
        1, //< UI1: 4 
        1, //< BOL: 5 
        0, //< ST2: 6 
        2, //< SI2: 7 
        2, //< UI2: 8
        2, //< HLF: 9
        3, //< SVI: 10
        3, //< UVI: 11
        0, //< SI4: 12
        4, //< UI4: 13
        4, //< FLT: 14
        4, //< TMU: 17
        4, //< DBL: 18
        8, //< TMU: 19
        8, //< SV8: 20
        8, //< UV8: 21
        8, //< DEC: 22
        9, //< SIN: 23
        9, //< UIN: 24
        0, //< OBJ: 25
        0, //< BSQ: 26
        0, //< ESC: 27
        0, //< LST: 28
        0, //< BOK: 29
        0, //< DIC: 30
        0, //< MAP: 31
    };
    if (type >= 32) {
        return 0;
    }
    return kWidths[type];
}

bool TypeIsValid (uint_t type) {
    return type > 31 ? false : true;
}

const char** TypeStrings () {
    static const char* kNames[] = {
        "NIL",  //<  0
        "ADR",  //<  1
        "STR",  //<  2
        "TKN",  //<  3
        "BOL",  //<  4
        "SI1",  //<  5
        "UI1",  //<  6
        "SI2",  //<  7
        "UI2",  //<  8
        "HLF",  //<  9
        "SVI",  //< 10
        "UVI",  //< 11
        "SI4",  //< 12
        "UI4",  //< 13
        "FLT",  //< 14
        "TMS",  //< 15
        "SI8",  //< 16
        "UI8",  //< 17
        "DBL",  //< 18
        "TMU",  //< 19
        "SV8",  //< 20
        "UV8",  //< 21
        "DEC",  //< 22
        "SIN",  //< 23
        "UIN",  //< 24
        "OBJ",  //< 25
        "BSQ",  //< 26
        "ESC",  //< 27
        "LST",  //< 28
        "BOK",  //< 29
        "DIC",  //< 30
        "MAP",  //< 31
    };
    return kNames;
}

}       //< namespace _
