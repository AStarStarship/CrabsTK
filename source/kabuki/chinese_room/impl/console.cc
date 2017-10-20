/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/console.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
#include "../include/console.h"

namespace _ {

char ReadChar (const char* header) {
    std::cout << header;
    char c = getchar ();
    std::cout << '\n';
    return c;
}

int ReadInt (const char* header) {
    int number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

float ReadFloat (const char* header) {
    float number;
    std::cout << header;
    std::cin.clear ();
    std::cin >> number;
    return number;
}

char* ReadString (const char* header, int buffer_size) {
    if (buffer_size < 0)
        return nullptr;
    char* buffer = new char[buffer_size];
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');

    return buffer;
}

void ReadString (const char* header, char* buffer, int buffer_size) {
    if (buffer == nullptr)
        return;
    if (buffer_size < 0)
        return;
    std::cout << header;
    std::cin.get (buffer, buffer_size, '\n');
    std::cin.clear ();
    std::cin.ignore (buffer_size, '\n');
}

Console::Console () :
    Room (0)
{
    
}

Console::~Console () {

}

ticket_t Console::Init () {
    return 0;
}

ticket_t Console::Loop () {
    return 0;
}

const Operation* Console::Star (index index, Expression* a) {
    return 0;
}

}       //< namespace _
