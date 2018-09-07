/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/crabs_operand.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM >= SEAM_0_0_4
// Dependencies:
#include "test.h"
#include "op.h"
#include "operand.h"
#include "str1.h"
// End dependencies.
#if SEAM == SEAM_0_0_4
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#else
#define PRINTF(x, ...)
#define PRINT(c)
#endif

namespace _ {

const char* OperandName(Operand* operand) {
  ASSERT(operand);
  const Op* op = operand->Star('?', nullptr);
  ASSERT(op);

  return op->name;
}

uintptr_t OperandCount(Operand* operand) {
  ASSERT(operand);
  const Op* op = operand->Star(0, nullptr);
  return (op == nullptr) ? 0 : reinterpret_cast<uintptr_t>(op->in);
}

wchar_t OperandIndex(Operand* operand, char* begin, char* end) {
  ASSERT(operand);
  const Op* op = operand->Star('?', nullptr);
  ASSERT(op);
  wchar_t index = OpFirst(op), last = OpLast(op);
  ASSERT(index);
  for (; index <= last; ++index) {
    if (StringEquals(begin, end, operand->Star(index, nullptr)->name)) {
      return index;
    }
  }
  return 0;
}

#if CRABS_TEXT
/*
Utf& Print (Utf& print, const Operand* op) {
    print << "\n Op:\n" << op->name << "\nparams_in:"
          << Bsq (op->in)
          << "\nparams_out:" << op->out
          << "\npop:" << op->pop  << " close:" << op->close
          << " default_op:"       << op->default_op
          << "\nignore_chars :"   << op->ignore_chars
          << "\nallowed_chars:"   << op->allowed_chars
          << "\n description :\"" << op->description;
    return print;
}*/

Utf8& PrintOperand(Utf8& print, Operand* operand) {
  ASSERT(operand);

  const Op* op = operand->Star('?', nullptr);

  ASSERT(op);

  uintptr_t num_ops = reinterpret_cast<uintptr_t>(op->in),
            op_num = reinterpret_cast<uintptr_t>(op->out),
            last_op = op_num + num_ops - 1;
  if (num_ops > kParamsMax) {
    return print << "\nError: Too many parameters!";
  }
  print << "\nOperand         :" << op->name << Line('-', 80);
  for (; op_num <= last_op; ++op_num) {
    op = operand->Star((wchar_t)op_num, nullptr);
    print << "\nOp \'" << op_num << "\':" << op_num << ' ' << op
          << Line('-', 80);
  }
  return print;
}

Slot& OperandQuery(Operand* root, const char* address, Slot& slot) {
  ASSERT(address);
  ASSERT(root);

  int index = *address++;
  const Op* op = root->Star(index, nullptr);
  char buffer[1024];
  PRINTF("%s", op->name)
  index = *address++;
  while (index) {
    op = root->Star(index, nullptr);
    ASSERT(op);
    PRINTF(".%s", op->name)
    index = *address++;
  }
  slot.Write(buffer);
  return slot;
}
#endif
}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //> #if SEAM >= SEAM_0_0_4
