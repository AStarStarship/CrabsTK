/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/tbenchmark.h
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
#include <stdafx.h>

#if SEAM >= SEAM_00_00_00__00_01
#ifndef INCLUDED_KABUKI_F2_TBENCHMARK
#define INCLUDED_KABUKI_F2_TBENCHMARK

#include "benchmark.h"

namespace _ {

template <typename UI>
TestResult BenchmarkCaseLoop(const char* args) {
  PrintHeading((sizeof(UI) == 8) ? "Testing 64-bit use case"
                                 : "Testing 32-bit use case");
}

}  // namespace _

#endif  //< INCLUDED_KABUKI_F2_TBENCHMARK
#endif  //< #if SEAM >= SEAM_00_00_00__00_01
