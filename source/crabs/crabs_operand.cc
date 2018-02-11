/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_operand.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if CRABS_SEAM >= 2

namespace _ {

const char* OperandName (Operand* operand) {
    if (!operand) {
        return "nil operand";
    }
    const Op* op = operand->Star ('?', nullptr);
    if (op == nullptr) {
        return "nil op";
    }
    return op->name;
}

uintptr_t OperandCount (Operand* operand) {
    if (!operand) {
        return 0;
    }
    const Op* op = operand->Star (0, nullptr);
    return (op == nullptr)?0:
        reinterpret_cast<uintptr_t> (op->in);
}

wchar_t OperandIndex (Operand* operand, char* begin, char* end) {
    if (!operand) {
        return 0;
    }
    const Op* op = operand->Star ('?', nullptr);
    if (!op) {
        return 0;
    }
    wchar_t index = OpFirst (op),
            last  = OpLast  (op);
    if (!index) {
        return 0;
    }
    for (; index <= last; ++index) {
        if (OpEquals (operand->Star (index, nullptr), begin, end)) {
            return index;
        }
    }
    return 0;
}

#if USING_CRABS_TEXT

Slot& OperandPrint (Operand* operand, Slot& slot) {
    if (!operand) {
        return slot << "\nError: Operand can't be nil";
    }

    /*
    if (OpCount (op->params) < kMaxNumParams) {
    // Print the Operand Header.
    //const uint_t* result   = op->result;
    uintptr_t     num_ops = reinterpret_cast<uintptr_t>(op->params),
    first_op = reinterpret_cast<uintptr_t>(op->result),
    last_op = first_op + num_ops - 1;
    //const byte  * eval     = op->evaluation;
    text << op->name
    << "\nOp Count: " << num_ops << " First: " << first_op
    << '\'' << Char (first_op) << "\' Last:" << last_op << '\''
    << Char (last_op)
    << "\'\nMetadata:        " << op->description;
    return;
    }*/

    const _::Op* op = operand->Star ('?', nullptr);
    if (!op) {
        return slot << "\nError: invalid Op!";
    }
    uintptr_t num_ops = reinterpret_cast<uintptr_t>(op->in),
              op_num = reinterpret_cast<uintptr_t>(op->out),
              last_op = op_num + num_ops - 1;
    if (num_ops > _::kParamsMax) {
        return slot << "\nError: Too many parameters!";
    }
    slot << "\nOperand         :" << op->name
         << Line (slot, '-', "\n>");
    for (; op_num <= last_op; ++op_num) {
        op = operand->Star (op_num, nullptr);
        slot << "\nOp \'" << slot.Write (op_num) << "\':" 
             << op_num << ' ' << op
             << Line (slot, '-', "\n>");
    }
    return slot;
}

Slot& OperandQuery (Operand* root, const char* address, Slot& slot) {
    if (!address) {
        return slot;
    }
    if (!root) {
        return slot;
    }
    int index = *address++;
    const Op* op = root->Star (index, nullptr);
    slot << op->name;
    index = *address++;
    while (index) {
        op = root->Star (index, nullptr);
        if (!op) {
            return slot;
        }
        slot << '.' << op->name;
        index = *address++;
    }
    return slot;
}
#endif
}       //< namespace _
#endif  //< CRABS_SEAM >= 2
