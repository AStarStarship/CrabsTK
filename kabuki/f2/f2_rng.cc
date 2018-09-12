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

#if SEAM >= SEAM_0_0_1
#ifndef INCLUDED_CRABS_RANDOM
#define INCLUDED_CRABS_RANDOM

#include "trng.h"

#include <random>

namespace _ {

inline uint RandomizeSeed() { return std::random_device()(); }

template <typename I>
class MersenneTwister : public RNG<I> {
 public:
  MersenneTwister(I min, I max) : rng(RandomizeSeed()), dist(min, max) {}

  MersenneTwister() : rng(RandomizeSeed()), dist() {}

  virtual void Seed(uint32_t seed) { rng.seed(seed); }

  virtual I Next() { return dist(rng); }

  static inline MersenneTwister& Static() {
    static MersenneTwister<I> rng();
    return rng;
  }

 private:
  std::mt19937 rng;
  std::uniform_int_distribution<std::mt19937::result_type> dist;
};

template <typename I>
RNG<I>& RNGStatic() {
  return MersenneTwister<I>::Static();
}

template <typename I>
I Random() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<I> dist();
  return dist(rng);
}

template <typename I>
I Random(I min, I max) {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<I> dist(min, max);
  return dist(rng);
}

uint8_t Random(uint8_t min, uint8_t max) { return Random<uint8_t>(min, max); }

uint8_t Random(int8_t min, int8_t max) { return Random<int8_t>(min, max); }

uint16_t Random(uint16_t min, uint16_t max) {
  return Random<uint16_t>(min, max);
}

uint16_t Random(int16_t min, int16_t max) { return Random<int16_t>(min, max); }

uint32_t Random(uint32_t min, uint32_t max) {
  return Random<uint32_t>(min, max);
}

uint64_t Random(uint64_t min, uint64_t max) {
  return Random<uint64_t>(min, max);
}

uint64_t Random(int64_t min, int64_t max) { return Random<int64_t>(min, max); }

uint8_t RandomUI1() { return Random<uint8_t>(); }

int8_t RandomSI1() { return Random<int8_t>(); }

uint16_t RandomUI2() { return Random<uint16_t>(); }

int16_t RandomSI2() { return Random<int16_t>(); }

uint32_t RandomUI4() { return Random<uint32_t>(); }

int32_t RandomSI4() { return Random<int32_t>(); }

uint64_t RandomUI8() { return Random<uint64_t>(); }

uint64_t RandomSI8() { return Random<uint64_t>(); }

}  // namespace _
#endif  //< INCLUDED_CRABS_RANDOM

#endif  //< #if SEAM >= SEAM_0_0_1
