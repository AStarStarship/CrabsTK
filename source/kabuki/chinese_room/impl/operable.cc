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
#include "../include/operator.h"
#include "../include/args.h"

namespace _ {

uintptr_t ToUInt (Operable* op) {
    if (op == nullptr) return 0;
    const Operation* operation = op->Star (0, nullptr);
    return operation == nullptr ? 0 : 
                        reinterpret_cast<uintptr_t> (operation->params);
}

bool IsOperator (const Operation* op) {
    if (!op)
        return false;
    return (reinterpret_cast<uintptr_t> (op->result) < ' ');
}

void OperatorPrint (Operator* op) {
    std::cout << "| Operator:        ";
    if (op == nullptr) {
        std::cout << "null";
        PrintLine ("|", '_');
        return;
    }
    printf ("0x%p ", d);
    const Operation* operation = op->Star (0, nullptr);   //< Get Star header.
    std::cout << operation->name
        << "\n| NumOperators:  " << CountCoperations (m)
        << "\n| Description:   " << operation->metadata << '\n';
    byte i = Index (operation->result),
        stop_index = i + Index (operation->params);
    std::cout << "| FirstOperator: " << i << " LastOperator: "
        << stop_index;
    PrintLine ("|", '-');
    for (; i < stop_index; ++i) {
        op = op->Star (i, nullptr);
        if (op != nullptr) {
            std::cout << "| " << i << ": " << op->name << '\n'
                << "| input:   ";
            EscPrint (operation->params);
            std::cout << "| result:   ";
            EscPrint (operation->result);
            std::cout << "| Description: " << op->metadata;
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

void PrintAddress (const byte* address, Operator* root) {
    if (address == nullptr)
        return;
    if (root == nullptr)
        return;
    char_t index = *address++;
    const Operator* op = root->Star (index, nullptr);
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

}       //< namespace _
