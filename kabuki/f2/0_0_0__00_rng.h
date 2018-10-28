/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/00/itos_and_stoi.h
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

  for (int i = 0; i < 10000; ++i) {
    Print("\n");
    Print(RandomUI4());
  }

  TEST_END;
#endif
  return 0;
}
}  // namespace _

#include "test_footer.inl"
