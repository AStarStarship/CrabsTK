/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/operation.cpp
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

#include "../include/operation.h"

namespace _ {

KABUKI uintptr_t ToUInt (Operation* d) {
    if (d == nullptr) return 0;
    const Set* m = d->Star (0, nullptr);
    return m == nullptr ? 0 : reinterpret_cast<uintptr_t> (m->params);
}

KABUKI bool IsDevice (const Set* member) {
    if (!member)
        return false;
    return (reinterpret_cast<uintptr_t> (member->result) < ' ');
}

KABUKI void Print (Operation* d) {
    std::cout << "| Star:        ";
    if (d == nullptr) {
        std::cout << "null";
        PrintLine ("|", '_');
        return;
    }
    printf ("0x%p ", d);
    const Set* m = d->Star (0, nullptr);   //< Get Star header.
    std::cout << m->name
        << "\n| NumMembers:  " << GetNumMembers (m)
        << "\n| Description: " << m->metadata << '\n';
    byte i = Index (m->result),
        stop_index = i + Index (m->params);
    std::cout << "| FirstMember: " << i << " LastMember: "
        << stop_index;
    PrintLine ("|", '-');
    for (; i < stop_index; ++i) {
        m = d->Star (i, nullptr);
        if (m != nullptr) {
            std::cout << "| " << i << ": " << m->name << '\n'
                << "| input:   ";
            PrintEsc (m->params);
            std::cout << "| result:   ";
            PrintEsc (m->result);
            std::cout << "| Description: " << m->metadata;
            if (i == stop_index)
                PrintLine ("|", '_');
            else
                PrintLine ("|", '-');
        } else {
            std::cout << "| " << i << ": null\n";
        }
    }
    //system ("PAUSE");
}

KABUKI void PrintAddress (const byte* address, Operation* root) {
    if (address == nullptr)
        return;
    if (root == nullptr)
        return;
    char_t index = *address++;
    const Set* m = root->Star (index, nullptr);
    std::cout << m->name;
    index = *address++;
    while (index) {
        m = root->Star (index, nullptr);
        if (m == nullptr)
            return;
        std::cout << '.' << m->name;
        index = *address++;
    }

}

}       //< namespace _
