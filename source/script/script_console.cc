/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_console.cc
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

#include "console.h"
#include "text.h"

namespace _ {

#if USING_SCRIPT_TEXT

char ConsoleReadChar (const char* header) {
    std::cout << header;
    char c = getchar ();
    std::cout << '\n';
    return c;
}

int ConsoleReadInt (const char* header) {
    int number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

float ConsoleReadFloat (const char* header) {
    float number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

void ConsoleReadStrand (const char* header, char* target, char* target_end) {
    if (!target) {
        return;
    }
    if (target > target_end) {
        return;
    }
    std::cout << header;
    std::cin.get (target, target_end - target, '\n');
    std::cin.clear ();
    std::cin.ignore (target_end - target, '\n');
}

void ConsoleWrite (const char* strand) {
    std::cout << strand;
}

void ConsoleWrite (Strand& strand) {
    std::cout << strand.GetBegin ();
}

void ConsoleDump (Strand& strand) {
    std::cout << strand.GetBegin ();
    strand.Clear ();
}

#endif  //< USING_SCRIPT_TEXT
}       //< namespace _
