/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/Member.h
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

#include "../include/member.h"
#include "../include/error.h"

namespace _ {

KABUKI uint_t* NumMembers (std::uintptr_t value) {
    return reinterpret_cast<uint_t*> (value);
}

KABUKI const uint_t* FirstMember (uint_t value) {
    return reinterpret_cast<uint_t*>(value);
}

KABUKI uintptr_t Index (const void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr);
}

KABUKI uintptr_t GetNumMembers (const Member* m) {
    return Index (m->rx_header);
}

KABUKI const Member* ReadError () {
    //return DeviceMember<"Read"> ();
    static const Member error = { "Read",
        0,
        0,
        0
    };
    return &error;
}

KABUKI const Member* WriteError () {
    //return DeviceMember<"Write"> ();
    static const Member error = { "Write",
        0,
        0,
        0
    };
    return &error;
}

KABUKI const Member* DeviceStackOverflowError () {
    //return DeviceMember<"Device stack overflow"> ();
    static const Member error = { "Device stack overflow",
        0,
        0,
        0
    };
    return &error;
}

KABUKI const Member* InvalidMember () {
    //return DeviceMember<"Invalid member index"> ();
    static const Member error = { "Invalid member index",
        0,
        0,
        0
    };
    return &error;
}

KABUKI void Print (const Member* m) {
    if (m == nullptr)
        return;

    const uint_t* tx_header = m->tx_header;

    if (Index (tx_header) < 256) {
        // Print Member Device
        std::cout << "\n| NumMembers:  " << GetNumMembers (m)
            << "\n| Description: " << m->description << '\n';
    }

    std::cout << "| Name:          " << m->key;
    PrintEsc (m->rx_header);
    std::cout << '\n';
    PrintEsc (m->tx_header);
    std::cout << "\n| Description: " << m->description << '\n';
}

}   //< namespace _
