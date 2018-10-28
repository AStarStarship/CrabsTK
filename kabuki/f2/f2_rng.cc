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

#include "crng.h"

#include <random>

namespace _ {

static std::default_random_engine rng;

inline uint32_t RandomUI4() { return rng(); }

inline uint RandomSeed() { return std::random_device()(); }

inline void RandomizeSeed() { return rng.seed(RandomSeed()); }

inline uint8_t RandomUI1() { return (uint8_t)RandomUI4(); }

inline int8_t RandomSI1() { return (int8_t)RandomUI1(); }

inline uint16_t RandomUI2() { return (uint16_t)RandomUI4(); }

inline int16_t RandomSI2() { return (int16_t)RandomUI2(); }

inline int32_t RandomSI4() { return (int32_t)RandomUI4(); }

inline uint64_t RandomUI8() {
  uint64_t a = RandomUI4(), b = RandomUI4();
  return (int64_t)(a | (b << 32));
}

inline int64_t RandomSI8() { return (int64_t)RandomUI8(); }

inline void RandomNumber(uint8_t& result) { result = RandomUI1(); }

inline void RandomNumber(int8_t& result) { result = RandomSI1(); }

inline void RandomNumber(uint16_t& result) { result = RandomUI2(); }

inline void RandomNumber(int16_t& result) { result = RandomSI2(); }

inline void RandomNumber(uint32_t& result) { result = RandomUI4(); }

inline void RandomNumber(int32_t& result) { result = RandomSI4(); }

inline void RandomNumber(uint64_t& result) { result = RandomUI8(); }

inline void RandomNumber(int64_t& result) { result = RandomSI8(); }

template <typename I>
inline I Random(I min, I max) {
  std::uniform_int_distribution<I> dist(min, max);
  return dist(rng);
}

inline uint16_t Random(uint16_t min, uint16_t max) {
  return Random<uint16_t>(min, max);
}

inline uint16_t Random(int16_t min, int16_t max) {
  return Random<int16_t>(min, max);
}

inline uint32_t Random(uint32_t min, uint32_t max) {
  return Random<uint32_t>(min, max);
}

inline uint64_t Random(uint64_t min, uint64_t max) {
  return Random<uint64_t>(min, max);
}

inline uint64_t Random(int64_t min, int64_t max) {
  return Random<int64_t>(min, max);
}

}  // namespace _
