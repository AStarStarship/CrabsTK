/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/bsq.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
#ifndef HEADER_FOR_CRABS_BSQ
#define HEADER_FOR_CRABS_BSQ
// Dependencies:
#include "printer.h"
#include "type.h"
// End dependencies.

namespace _ {

/* Utility class for printing B-Sequences. */
struct Bsq {
  const uint_t* params;  //< Bsq params.

  Bsq(const uint_t* params) : params(params) {
    // Nothing to do here! ({:-)-+=<
  }
};

#if WORD_SIZE == 16
inline int16_t UnpackSVI(int16_t value) {
  if (value < 0) {
    uint16_t result = 0x8000 return result | ~(value - 1);
  }
  return value;
}

inline int16_t PackSVI(int16_t value) {
  if (value < 0) {
    uint16_t result = 1 << 15;
    return result | ((~value + 1) << 1);
  }
  return value;
}
#else
inline int32_t UnpackSVI(int32_t value) {
  if (value < 0) {
    uint32_t result = 0x80000000;
    return result | ~(value - 1);
  }
  return value;
}

inline int32_t PackSVI(int32_t value) {
  if (value < 0) {
    int32_t result = 0x80000000;
    return result | ((~value + 1) << 1);
  }
  return value;
}
#endif

inline int64_t UnpackSV8(int64_t value) {
  if (value < 0) {
    int64_t result = 0x8000000000000000;
    return result | ~(value - 1);
  }
  return value;
}

inline int64_t PackSV8(int64_t value) {
  if (value < 0) {
    int64_t result = 0x8000000000000000;
    return result | ((~value + 1) << 1);
  }
  return value;
}

// template<typename T>
// T UnpackSVI (T value) {
//    T temp = value << sizeof (T) * 8 - 1;
//    temp |= value;
//    return temp;
//}

constexpr uint_t BsqSize(const uint_t* params) {
  if (!params) {
    return 0;
  }
  uint_t size_bytes = sizeof(uint_t), count = *params++;

  if (count > kParamsMax) {
    return 0;
  }

  for (; count > 0; --count) {
    uint_t param = *params++;

    if (param == NIL) {  // This is illegal.
      return 0;
    }
    if (param <= TKN) {
      size_bytes += sizeof(uint_t);
      ++params;
    }
    if (param == SIH) {
      size_bytes += sizeof(uint_t);
      ++params;
    }
    if (param == UIH) {
      size_bytes += sizeof(uint_t);
      ++params;
    }
    if (param >= LST && param <= MAP) {  // This is illegal.
      return 0;
    }
    if (param > MAP) {
      if (param >> 8) {  // This is an error.
        return 0;
      }
      if (param & 0x20) {     // It's a multi-dimensional array so
        param = *params;      // add the number of dimensions + 1;
        params += param + 1;  // for the dimension count.
      }
    }
    size_bytes += sizeof(uint_t);
  }
  return size_bytes;
}

/* Creates a immutable Script B-Sequence.
    C++11 variadic template to ensure only one copy in ROM
    and to eliminate some redundant typing. */
template <const uint_t... N>
inline const uint_t* Params() {
  static const uint_t kSize = 0,  // BsqSize ({ N... })
      kList[sizeof...(N)] = {N...};
  return &kSize;
}

/* Prints out the BSQ parameters. */
KABUKI Printer& PrintBsq(Printer& printer, const uint_t* params);

/*  Prints out the parameters. */
// KABUKI Printer& PrintBsq (const uint_t* bsq, Printer& print);

/*< Returns the requested parameter number. */
KABUKI uint_t BsqParamNumber(const uint_t* bsq, int param_number);

}  // namespace _

/*  Prints out the parameters. */
inline _::Printer& operator<<(_::Printer& printer, _::Bsq bsq) {
  return _::PrintBsq(printer, bsq.params);
}

#endif  //< HEADER_FOR_CRABS_BSQ
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
