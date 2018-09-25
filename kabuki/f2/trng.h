/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/trng.h
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

#if SEAM >= SEAM_0_0_02
#ifndef INCLUDED_CRAB_TRNG
#define INCLUDED_CRAB_TRNG

namespace _ {

/* Interface for a random number generator.
@theory Random number generation takes enough CPU cycles that use of an
interface does not effect performance but also allows for hiding of the
C++ standard library implementation. */
template <typename T>
struct RNG {
  /* Sets the RNG. */
  virtual void Seed(uint32_t seed) = 0;

  /* Returns the next random number. */
  virtual T Next() = 0;
};

}  // namespace _
#endif  //< INCLUDED_CRAB_TRNG

#endif  //< #if SEAM >= SEAM_0_0_02
