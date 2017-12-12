/** kabuki::script
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
#include "print.h"
#include "strand.h"
#include "expression.h"

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

const Operation* OperandQuery (Expression* expr,
                               const Operation* operation) {
    if (expr) {
        if (!operation) {
            return OperationInvalid ();
        }
        void* args[2];
        uintptr_t num_operations = (uintptr_t)operation->params,
                 first_operation = (uintptr_t)operation->result;
        return ExprResult (expr, Params<4, STR, kOperationMaxNameLength,
                                        UV8, UV8, STR,
                                        kOperationMaxDescriptionLength> (),
                           Args (args, operation->name,
                                 &num_operations, &first_operation,
                                 operation->metadata));
    }
    return operation;
}

uintptr_t OperationCount (const uint_t* b_sequence) {
    return (char_t)reinterpret_cast<uintptr_t>(b_sequence);
}

uintptr_t ToUInt (Operand* operand) {
    if (operand == nullptr) {
        return 0;
    }
    const Operation* operation = operand->Star (0, nullptr);
    return (operation == nullptr) ? 0 :
        reinterpret_cast<uintptr_t> (operation->params);
}

bool IsGroup (const Operation* operation) {
    if (!operation) {
        return false;
    }
    return (reinterpret_cast<uintptr_t> (operation->result) < ' ');
}

uint OperationFind (Operand* operand, const char* name) {
    if (!name) {
        return 0;
    }
    if (!operand) {
        return 0;
    }
    const Operation* operation = operand->Star ('?', nullptr);
    if (!operation) {
        return 0;
    }
    for (uint i = OperationFirst (operation); i <= OperationLast (operation);
         ++i) {
        operation = operand->Star (i, nullptr);
        if (!operation) {
            return 0;
        }
        if (StrandEquals (name, operation->name)) {
            return i;
        }
    }
    return 0;
}

const Operation* OperationNil () {
    static const Operation Result = { "Nil",
        NumOperations (0), OperationFirst (' '),
        nullptr, 0 };
    return &Result;
}

const Operation* OperationInvalid () {
    static const Operation Result = { "Invalid",
        NumOperations (1), OperationFirst (' '),
        nullptr, 0 };
    return &Result;
}

#if USE_MORE_ROM
void OperationPrint (const Operation* operation) {
    if (!operation) {
        std::cout << "\n| Operation:      null";
        return;
    }
    if (OperationCount (operation->params) < kMaxNumParams) {
        // Print the Operand Header.
        //const uint_t* result   = op->result;
        uintptr_t     num_ops  = reinterpret_cast<uintptr_t>(operation->params),
                      first_op = reinterpret_cast<uintptr_t>(operation->result),
                      last_op  = first_op + num_ops - 1;
        //const byte  * eval     = op->evaluation;
        std::cout << operation->name
                  << "\n| Num Operations: " << num_ops << " First: " << first_op
                  << '\'';
        putchar (first_op);
        std::cout << "\' Last:" << last_op << '\'';
        putchar (last_op);
        std::cout << "\'\n| Metadata:        " << operation->metadata;
        return;
    }

    std::cout << "\n| Operation:   " << operation->name
              << "\n| Params:      ";
    ParamsPrint (operation->params);
    std::cout << "\n| Result:      ";
    ParamsPrint (operation->params);
    std::cout << "\n| Metadata:    " << operation->metadata
              << "\n| Evaluation   ";
    const byte* eval = operation->evaluation;
    if (eval == nullptr) {
        std::cout << "null";
    } else {
        //PrintMemory (eval)
        std::cout << "@todo Print B-Sequence here";
    }
}
void OperandPrint (Operand* operand) {
    std::cout << "\n| Operand:          ";
    if (!operand) {
        std::cout << "null";
        return;
    }

    const Operation* operation = operand->Star ('?', nullptr);
    if (!operation) {
        std::cout << "\n| null Operation Error!";
        return;
    }
    OperationPrint (operation);
    PrintLine ('-', "\n>");
    uintptr_t num_ops = reinterpret_cast<uintptr_t>(operation->params),
              op_num  = reinterpret_cast<uintptr_t>(operation->result),
              last_op = op_num + num_ops - 1;
    if (num_ops > kMaxNumParams) {
        std::cout << "\n| Error: Too many parameters!";
        return;
    }
    for (; op_num <= last_op; ++op_num) {
        std::cout << "\n| Operation \'";
        putchar (op_num);
        std::cout << "\':" << op_num << ' ';
        operation = operand->Star (op_num, nullptr);
        OperationPrint (operation);
        PrintLine ('-', "\n>");
    }
}

void PrintAddress (const byte* address, Operand* root) {
    if (!address) {
        return;
    }
    if (!root) {
        return;
    }
    int index = *address++;
    const Operation* operation = root->Star (index, nullptr);
    std::cout << operation->name;
    index = *address++;
    while (index) {
        operation = root->Star (index, nullptr);
        if (!operation) {
            return;
        }
        std::cout << '.' << operation->name;
        index = *address++;
    }
}
#endif  //< USE_MORE_ROM

}   //< namespace _
