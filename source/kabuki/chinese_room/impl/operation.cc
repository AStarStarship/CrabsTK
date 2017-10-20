/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/operation.cc
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
#include "../include/operation.h"

namespace _ {

uint_t* NumOperations (std::uintptr_t value) {
    return reinterpret_cast<uint_t*> (value);
}

const uint_t* FirstOperation (uint_t value) {
    uintptr_t index = value;
    return reinterpret_cast<uint_t*>(index);
}

uintptr_t Index (const void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr);
}

uintptr_t CountCoperations (const Operation* op) {
    return Index (op->params);
}

const Operation* ReadError () {
    //return DeviceOperation<"Read"> ();
    static const Operation error = { "Read",
        0,
        0,
        0
    };
    return &error;
}

const Operation* WriteError () {
    //return DeviceOperation<"Write"> ();
    static const Operation error = { "Write",
        0,
        0,
        0
    };
    return &error;
}

const Operation* ErrorTicket (ticket_t error) {
    // @todo Fix me!
    return nullptr;
}

const Operation* StackOverflow () {
    //return DeviceOperation<"Star stack overflow"> ();
    static const Operation error = { "Star stack overflow",
        0,
        0,
        0
    };
    return &error;
}

const Operation* InvalidOperation () {
    static const Operation error = { "Invalid operation",
        0, 0, 0, 0
    };
    return &error;
}

void OperationPrint (const Operation* op) {
    if (op == nullptr)
        return;

    const uint_t* result = op->result;

    if (Index (result) < 256) {
        // Print Operation Star
        std::cout << "\n| Num Operations:  " << CountCoperations (op)
                  << "\n| Metadata:     " << op->metadata << '\n';
    }

    std::cout << "| Name:         " << op->name;
    EscPrint (op->params);
    std::cout << '\n';
    EscPrint (op->result);
    std::cout << "\n| Metadata:   " << op->metadata << '\n';
}
uintptr_t ToUInt (Operable* op) {
    if (op == nullptr) return 0;
    const Operation* operation = op->Star (0, nullptr);
    return operation == nullptr?0:
        reinterpret_cast<uintptr_t> (operation->params);
}

bool IsOperation (const Operation* op) {
    if (!op)
        return false;
    return (reinterpret_cast<uintptr_t> (op->result) < ' ');
}

void OperationPrint (Operation* op) {
    std::cout << "| Operation:        ";
    if (op == nullptr) {
        std::cout << "null";
        PrintLine ("|", '_');
        return;
    }
    printf ("0x%p ", d);
    const Operation* operation = op->Star (0, nullptr);   //< Get Star header.
    std::cout << operation->name
        << "\n| NumOperations:  " << CountCoperations (m)
        << "\n| Description:   " << operation->metadata << '\n';
    byte i = Index (operation->result),
        stop_index = i + Index (operation->params);
    std::cout << "| FirstOperation: " << i << " LastOperation: "
        << stop_index;
    PrintLine ("|", '-');
    for (; i < stop_index; ++i) {
        operation = op->Star (i, nullptr);
        if (operation != nullptr) {
            std::cout << "| " << i << ": " << operation->name << '\n'
                << "| input:   ";
            EscPrint (operation->params);
            std::cout << "| result:   ";
            EscPrint (operation->result);
            std::cout << "| Description: " << operation->metadata;
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

void PrintAddress (const byte* address, Operation* root) {
    if (address == nullptr)
        return;
    if (root == nullptr)
        return;
    char_t index = *address++;
    const Operation* op = root->Star (index, nullptr);
    std::cout << op->name;
    index = *address++;
    while (index) {
        m = root->Star (index, nullptr);
        if (m == nullptr)
            return;
        std::cout << '.' << op->name;
        index = *address++;
    }
}

}   //< namespace _
