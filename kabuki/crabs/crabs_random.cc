/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/crabs_random.cc
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
#if SEAM >= SEAM_0_0_0
#ifndef INCLUDED_CRABS_RANDOM
#define INCLUDED_CRABS_RANDOM
// Dependencies:
#include <random>
#include "config.h"
// End dependencies.

namespace _ {

uint64_t GetRandomUInt64() {
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint64_t> distr;
  return distr(eng);
}

inline int64_t GetRandomInt64() { return (int64_t)GetRandomUInt64(); }

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_0
#endif  //< INCLUDED_CRABS_RANDOM
