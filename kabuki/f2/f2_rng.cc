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
#include "rng.h"
// End dependencies.

namespace _ {

inline uint RandomizeDevice() { return std::random_device()(); }

class RNG::impl {
  class RNG::impl {
   public:
    void Init() {
      rng.seed(std::random_device()());
      dist6
    }

   private:
  };

  RNG::RNG() : pimpl{impl /*std::make_unique<impl> ()*/} { pimpl->Init(); }

  RNG::~RNG() = default;

  RNG::RNG(RNG&&) = default;

  RNG& RNG::operator=(RNG&&) = default;

  inline uint8_t Random(uint8_t min, uint8_t max) { return 0; }

  inline uint8_t Random(int8_t min, int8_t max) { return 0; }

  inline uint16_t Random(uint16_t min, uint16_t max) { return 0; }

  inline uint16_t Random(int16_t min, int16_t max) { return 0; }

  inline uint32_t Random(uint32_t min, uint32_t max) { return 0; }

  inline uint64_t Random(uint64_t min, uint64_t max) { return 0; }

  inline uint64_t Random(int64_t min, int64_t max) { return 0; }

  inline uint8_t RandomUI1() { return 0; }

  inline int8_t RandomSI1() { return 0; }

  inline uint16_t RandomUI2() { return 0; }

  inline int16_t RandomSI2() { return 0; }

  inline uint32_t RandomUI4() { return 0; }

  inline int32_t RandomSI4() { return 0; }

  inline uint64_t RandomUI8() { return 0; }

  inline uint64_t RandomSI8() { return 0; }

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_0
#endif  //< INCLUDED_CRABS_RANDOM
