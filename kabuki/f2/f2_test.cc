/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/f2_test.cc
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

#include "console.h"
#include "ttest.h"

namespace _ {

bool AssertDisplay(int line, const char* file) {
  Printf("\nError at line %d in \"%s\"", line, file);
  Pause();
  return true;
}

bool Assert(bool condition) { return !condition; }

bool AssertHandle(int line, const char* file, const char* message) {
#if CRABS_ASSERT_PRINT
  Printf("\n%s\n,Assertion failed at line %d in \"%s\"", line, file);
#endif
#if CRABS_ASSERT_FREEZE
  while (1)
    ;
#elif CRABS_ASSERT_PAUSE
  Pause();
#endif
}

ErrorAssert::ErrorAssert() {}

}  // namespace _
