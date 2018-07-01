/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/random.hf
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_CRABS_RANDOM
#define HEADER_FOR_CRABS_RANDOM
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

/* Gets a uniform random uint64_t. */
inline uint64_t GetRandomUInt64();

/* Gets a uniform random int64_t. */
inline int64_t GetRandomInt64();

}  // namespace _
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 0
#endif  //< HEADER_FOR_CRABS_RANDOM
