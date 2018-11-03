/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/0_0_0__00_rng.h
@author  Cale McCollough <calemccollough.github.io>
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

#include "cconsole.h"
#include "crng.h"

#if SEAM == _0_0_0__00
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

namespace _ {

inline const char* _0_0_0__00_RNG(char* seam_log, char* seam_end,
                                  const char* args) {
#if SEAM >= _0_0_0__00
  TEST_BEGIN;

  PRINT_HEADING("Testing ArgsToString");

  char arg_string[] = "C:\\Windows\0Foo\0\0Bar    \0\0\0   420    \0";
  char* test_args[] = {arg_string, arg_string + 11, arg_string + 16,
                       arg_string + 26};
  const int kArgCount = 4;

  PRINT("\nArguments:\n");
  for (int i = 0; i < kArgCount; ++i) {
    char* arg = test_args[i];
    if (arg) {
      PRINTF("\ni:%i\"%s\" 0x%p", i, arg, arg);
    } else {
      PRINT("\nNil arg.");
    }
  }
  PRINT("\n\nRunning ArgsToString...\n");
  ASSERT(ArgsToString(kArgCount, test_args));

  PRINT("\n\nPrinting argument string...\n");
  PRINT(test_args[1]);

  PRINT("\n\nDone testing const char* ArgsToString(int, char**);");

  PRINT_HEADING("Testing RNG");

  for (int i = 0; i < 10000; ++i) {
    RandomizeSeed();
    auto value = RandomUI2();
    PRINT('\n');
    PRINT(value);
  }

  TEST_END;
#endif
  return 0;
}
}  // namespace _

#include "test_footer.inl"
