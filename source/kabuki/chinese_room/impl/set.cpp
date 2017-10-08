/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/set.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <chinese_room/include/set.h>

namespace _ {

uint_t* NumMembers (std::uintptr_t value) {
    return reinterpret_cast<uint_t*> (value);
}

const uint_t* FirstMember (uint_t value) {
    return reinterpret_cast<uint_t*>(value);
}

uintptr_t Index (const void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr);
}

uintptr_t GetNumMembers (const Set* m) {
    return Index (m->params);
}

const Set* ReadError () {
    //return DeviceMember<"Read"> ();
    static const Set error = { "Read",
        0,
        0,
        0
    };
    return &error;
}

const Set* WriteError () {
    //return DeviceMember<"Write"> ();
    static const Set error = { "Write",
        0,
        0,
        0
    };
    return &error;
}

const Set* DeviceStackOverflowError () {
    //return DeviceMember<"Star stack overflow"> ();
    static const Set error = { "Star stack overflow",
        0,
        0,
        0
    };
    return &error;
}

const Set* InvalidMember () {
    //return DeviceMember<"Invalid member index"> ();
    static const Set error = { "Invalid member index",
        0,
        0,
        0
    };
    return &error;
}

void Print (const Set* m) {
    if (m == nullptr)
        return;

    const uint_t* result = m->result;

    if (Index (result) < 256) {
        // Print Set Star
        std::cout << "\n| Num Members: " << GetNumMembers (m)
            << "\n| Metadata:    " << m->metadata << '\n';
    }

    std::cout << "| Name:       " << m->name;
    PrintEsc (m->params);
    std::cout << '\n';
    PrintEsc (m->result);
    std::cout << "\n| Metadata:   " << m->metadata << '\n';
}

}   //< namespace _
