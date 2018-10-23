/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_random.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if SEAM >= SEAM_0_0_0__00
#ifndef INCLUDED_KABUKI_F2_RNG
#define INCLUDED_KABUKI_F2_RNG

#include "trng.h"

#include <random>

namespace _ {

static std::mt19937 rng;

inline uint RandomSeed() { return std::random_device()(); }

inline void RandomizeSeed() { return rng.seed(RandomSeed()); }

inline void RandomNumber(int8_t& result) { result = (int8_t)rng(); }

inline void RandomNumber(uint8_t& result) { result = (uint8_t)rng(); }

inline void RandomNumber(int16_t& result) { result = (int16_t)rng(); }

inline void RandomNumber(uint16_t& result) { result = (uint16_t)rng(); }

inline void RandomNumber(int32_t& result) { result = (int32_t)rng(); }

inline void RandomNumber(uint32_t& result) { result = rng(); }

inline void RandomNumber(int64_t& result) {
  uint64_t a = rng(), b = rng();
  result = (int64_t)(a | (b << 32));
}

inline void Random(uint64_t& result) {
  uint64_t a = rng(), b = rng();
  result = a | (b << 32);
}

template <typename I>
I Random(I min, I max) {
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

uint8_t RandomUI1() {
  std::mt19937 rng;
  return (uint8_t)rng();
}

int8_t RandomSI1() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  return (uint8_t)rng();
}

uint16_t RandomUI2() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  return (uint8_t)rng();
}

int16_t RandomSI2() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  return (uint8_t)rng();
}

uint32_t RandomUI4() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  return (uint8_t)rng();
}

int32_t RandomSI4() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  return (uint8_t)rng();
}

uint64_t RandomUI8() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  return ((uint64_t)rng() << 32) | rng();
}

uint64_t RandomSI8() { return (uint64_t)RandomUI8(); }

/* Hidden MersenneTwister wrapper class. */
template <typename I>
class MersenneTwister : public TRNG<I> {
 public:
  virtual void Seed(uint32_t seed) { rng.seed(seed); }

  virtual I Next() {
    I random;
    RandomNumbber(random);
    return I;
  }
};

}  // namespace _
#endif  //< INCLUDED_KABUKI_F2_RNG

#endif  //< #if SEAM >= SEAM_0_0_0__00
