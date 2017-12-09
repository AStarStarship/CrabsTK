/** kabuki::script
#include <script_operation.h>
#include <script_args.h>
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_operation.cc
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
#include "operation.h"
#include "memory.h"
#include "args.h"
#include "text.h"

namespace _ {

const char* OperandName (Operand* operand) {
    if (operand == nullptr) {
        return "null operand";
    }
    const Operation* op = operand->Star ('?', nullptr);
    if (op == nullptr) {
        return "null op";
    }
    return op->name;
}

uintptr_t OperationCount (const uint_t* op) {
    return (char_t)reinterpret_cast<uintptr_t>(op);
}

#if USE_MORE_ROM
void OperationPrint (const Operation* op) {
    if (op == nullptr) {
        std::cout << "\n| Operation:      null";
        return;
    }
    if (OperationCount (op->params) < kMaxNumParams) {
        // Print the Operand Header.
        //const uint_t* result   = op->result;
        uintptr_t     num_ops  = reinterpret_cast<uintptr_t>(op->params),
                      first_op = reinterpret_cast<uintptr_t>(op->result),
                      last_op  = first_op + num_ops - 1;
        //const byte  * eval     = op->evaluation;
        std::cout << op->name
                  << "\n| Num Operations: " << num_ops << " First: " << first_op << '\'';
        putchar (first_op);
        std::cout << "\' Last:" << last_op << '\'';
        putchar (last_op);
        std::cout << "\'\n| Metadata:        " << op->metadata;
        return;
    }

    std::cout << "\n| Operation:   " << op->name
              << "\n| Params:      ";
    ParamsPrint (op->params);
    std::cout << "\n| Result:      ";
    ParamsPrint (op->params);
    std::cout << "\n| Metadata:    " << op->metadata
              << "\n| Evaluation   ";
    const byte* eval = op->evaluation;
    if (eval == nullptr) {
        std::cout << "null";
    } else {
        //PrintMemory (eval)
        std::cout << "@todo Print B-Sequence here";
    }
}
void OperandPrint (Operand* operand) {
    std::cout << "\n| Operand:          ";
    if (operand == nullptr) {
        std::cout << "null";
        return;
    }

    const Operation* op = operand->Star ('?', nullptr);
    if (op == nullptr) {
        std::cout << "\n| null Operation Error!";
        return;
    }
    OperationPrint (op);
    PrintLine (">", '-');
    uintptr_t num_ops = reinterpret_cast<uintptr_t>(op->params),
              op_num  = reinterpret_cast<uintptr_t>(op->result),
              last_op = op_num + num_ops - 1;
    if (num_ops > kMaxNumParams) {
        std::cout << "\n| Error: Too many parameters!";
        return;
    }
    for (; op_num <= last_op; ++op_num) {
        std::cout << "\n| Operation \'";
        putchar (op_num);
        std::cout << "\':" << op_num << ' ';
        op = operand->Star (op_num, nullptr);
        OperationPrint (op);
        PrintLine (">", '-');
    }
}

void PrintAddress (const byte* address, Operand* root) {
    if (address == nullptr)
        return;
    if (root == nullptr)
        return;
    int index = *address++;
    const Operation* ope = root->Star (index, nullptr);
    std::cout << ope->name;
    index = *address++;
    while (index) {
        ope = root->Star (index, nullptr);
        if (ope == nullptr)
            return;
        std::cout << '.' << ope->name;
        index = *address++;
    }
}
#endif  //< USE_MORE_ROM

uintptr_t ToUInt (Operand* operand) {
    if (operand == nullptr) return 0;
    const Operation* ope = operand->Star (0, nullptr);
    return ope == nullptr?0:
        reinterpret_cast<uintptr_t> (ope->params);
}

bool IsGroup (const Operation* ope) {
    if (!ope)
        return false;
    return (reinterpret_cast<uintptr_t> (ope->result) < ' ');
}


const Operation* NilResult () {
static const Operation Result = { "Nil",
        NumOperations (0), FirstOperation (' '),
        nullptr, 0 };
    return &Result;
}


const Operation* InvalidOperation () {
    return reinterpret_cast<const Operation*> (1);
}

}   //< namespace _
