/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_operand.cc
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
#include "operand.h"

namespace _ {

const char* OperandName (Operand* operand) {
    if (!operand) {
        return "null operand";
    }
    const Operation* op = operand->Star ('?', nullptr);
    if (op == nullptr) {
        return "null op";
    }
    return op->name;
}

uintptr_t OperandCount (Operand* operand) {
    if (!operand) {
        return 0;
    }
    const Operation* operation = operand->Star (0, nullptr);
    return (operation == nullptr)?0:
        reinterpret_cast<uintptr_t> (operation->params);
}

#if SCRIPT_USING_TEXT

Text& OperandPrint (Operand* operand, Text& text) {
    if (!operand) {
        return text << "\n| Error: Operand can't be nil";
    }

    /*
    if (OperationCount (operation->params) < kMaxNumParams) {
    // Print the Operand Header.
    //const uint_t* result   = op->result;
    uintptr_t     num_ops = reinterpret_cast<uintptr_t>(operation->params),
    first_op = reinterpret_cast<uintptr_t>(operation->result),
    last_op = first_op + num_ops - 1;
    //const byte  * eval     = op->evaluation;
    text << operation->name
    << "\n| Operation Count: " << num_ops << " First: " << first_op
    << '\'' << Char (first_op) << "\' Last:" << last_op << '\''
    << Char (last_op)
    << "\'\n| Metadata:        " << operation->description;
    return;
    }*/

    const _::Operation* operation = operand->Star ('?', nullptr);
    if (!operation) {
        return text << "\n| Error: invalid Operation!";
    }
    uintptr_t num_ops = reinterpret_cast<uintptr_t>(operation->params),
        op_num = reinterpret_cast<uintptr_t>(operation->result),
        last_op = op_num + num_ops - 1;
    if (num_ops > _::kMaxNumParams) {
        return text << "\n| Error: Too many parameters!";
    }
    text << "\n| Operand         :" << operation->name
        << text.Line ('-', "\n>");
    for (; op_num <= last_op; ++op_num) {
        operation = operand->Star (op_num, nullptr);
        text << "\n| Operation \'" << text.Write (op_num) << "\':" 
             << op_num << ' ' << operation
             << text.Line ('-', "\n>");
    }
    return text;
}

Text& OperandPrint (Operand* root, const char_t* address, Text& text) {
    if (!address) {
        return text;
    }
    if (!root) {
        return text;
    }
    int index = *address++;
    const Operation* operation = root->Star (index, nullptr);
    text << operation->name;
    index = *address++;
    while (index) {
        operation = root->Star (index, nullptr);
        if (!operation) {
            return text;
        }
        text << '.' << operation->name;
        index = *address++;
    }
    return text;
}
#endif  //< SCRIPT_USING_TEXT

}   //< namespace _


#if SCRIPT_USING_TEXT

_::Text& operator<< (_::Text& text, _::Operand* operand) {
    return text << OperandPrint (operand, text);
}
#endif  //< SCRIPT_USING_TEXT
