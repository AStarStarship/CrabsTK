/* Kabuki Toolkit @version 0.x
@file    /kabuki/f2/0_0_0__05_ascii_stack.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM == _0_0_0__0
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

#include "tstack.h"

namespace _ {

static const char* _0_0_0__05_ASCII_Stack(char* seam_log, char* seam_end,
                                          const char* args) {
#if SEAM >= _0_0_0__05
  TEST_BEGIN;

  PRINTF("\n\nPushing items on to the Stack...\n");

  Stack<> stack(8);

  for (int i = 0; i <= 10; ++i) stack.Push(i);

  PRINT(stack);

  PRINTF("\nPopping items off the Stack...\n");

  for (int i = 10; i > 0; i--) AVOW(i, stack.Pop());

  PRINT(stack);

  TEST_END;
#endif
  return nullptr;
}
}  // namespace _

#include "test_footer.inl"
