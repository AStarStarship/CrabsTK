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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#include "op.h"
#include "printer.h"
#include "text.h"
#include "line.h"


#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

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
        if (TextEquals (begin, end, operand->Star (index, nullptr)->name)) {
            return index;
        }
    }
    return 0;
}

#if USING_TEXT_SCRIPT

Printer& PrintOperand (Operand* operand, Printer& print) {
    if (!operand) {
        return print << "\nError: Operand can't be nil";
    }

    const _::Op* op = operand->Star ('?', nullptr);
    if (!op) {
        return print << "\nError: invalid Op!";
    }
    uintptr_t num_ops = reinterpret_cast<uintptr_t>(op->in),
              op_num = reinterpret_cast<uintptr_t>(op->out),
              last_op = op_num + num_ops - 1;
    if (num_ops > _::kParamsMax) {
        return print << "\nError: Too many parameters!";
    }
    print << "\nOperand         :" << op->name
          << Line ('-', 80);
    for (; op_num <= last_op; ++op_num) {
        op = operand->Star ((wchar_t)op_num, nullptr);
        print << "\nOp \'" << op_num << "\':"
             << op_num << ' ' << op
             << Line ('-', 80);
    }
    return print;
}

Printer& OperandQuery (Operand* root, const char* address, Printer& print) {
    if (!address) {
        return print;
    }
    if (!root) {
        return print;
    }
    int index = *address++;
    const Op* op = root->Star (index, nullptr);
    print << op->name;
    index = *address++;
    while (index) {
        op = root->Star (index, nullptr);
        if (!op) {
            return print;
        }
        print << '.' << op->name;
        index = *address++;
    }
    return print;
}
#endif
}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< #if MAJOR_SEAM == 1 && MINOR_SEAM >= 4
