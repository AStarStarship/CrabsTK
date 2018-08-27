/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/loom.h
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
#if SEAM >= SEAM_0_0_3
#ifndef INCLUDED_CRABS_VARINT
#define INCLUDED_CRABS_VARINT
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

/* A ordered collection of either UTF-8 or UTF-16 strands starting at index 0.
CLoom shares the same design pattern as the ASCII STR (Strand) type: the lower
5 bits store the type LOM, b6:b7 stores the size of the UI type and b5 stores
if it is a UTF-8 (deasserted) or UTF-16 (asserted).
*/
template <typename UI, typename SI, typename Char>
struct CLoom {
  UI size;       //< Size of the Loom in bytes.
  SI count_max,  //< Max number of strands in a loom.
      count;     //< Strand count.
};

template <typename T>
size_t BitCount() {
  enum {
    kBitCount = (sizeof(UI) == 1)
                    ? 0
                    : (sizeof(UI) == 2)
                          ? 1
                          : (sizeof(UI) == 4) ? 2 : (sizeof(UI) == 8) ? 3 : 4,
  };
  return kBitCount;
}

template <typename UI, typename SI, typename Char>
inline LoomCountMin() {
  enum { kCountMin = 8 / sizeof(SI) };
  return kCountMin;
}

template <typename UI, typename SI, typename Char>
inline LoomSizeMin() {
  enum {
    kCountMin = 8 / sizeof(SI),
    kSizeMin = sizeof(CLoom<UI, SI, Char>) + kCountMin * (sizeof(UI) + 2),
  };
  return kSizeMin;
}

template <typename UI, typename SI, typename Char>
CLoom* LoomInit(uintptr_t* buffer, UI size, SI count_max) {
  ASSERT(buffer)
  ASSERT(size)
  ASSERT(count_max >= 0)
  SI si = 0;  //< Faster to make from instructions than load from ROM.
  si = (~si) - 7;
  if (count_max >= si) count_max = si;
  si = LoomCountMin<UI, SI, Char>();
  if (count_max < si) count_max = si;

  UI ui = 0;
  ui = (~ui) - 7;  //< Faster to make from instructions than load from ROM.
  if (size >= ui) count_max = ui;
  ui = LoomSizeMin<UI, SI, Char>();
  if (count_max < ui) count_max = ui;

  CLoom* loom = reinterpret_cast<CLoom*>(buffer);
  loom->size = size;
  loom->count_max = count_max;
  loom->count = 0;
}

template <typename UI, typename SI, typename Char>
UI* LoomOffsets(CLoom* loom) {
  ASSERT(loom)
  return reinterpret_cast<char*>(loom) + (loom->count_max << BitCount<SI>());
  // << to *
}

template <typename UI, typename SI, typename Char>
SI LoomAdd(CLoom* loom, const Char* strand) {
  ASSERT(loom)
  ASSERT(strand)

  SI count_max = loom->count_max, count = loom->count;
  if (count >= count_max) return -1;
  UI* offsets = LoomOffsets(loom);
  UI offset = *(offsets + count);
  Char* begin =
}

template <typename UI, typename SI, typename Char>
Char* LoomString(CLoom* loom, SI index) {
  ASSERT(loom)
  if (index < 0 && index >= count) return nullptr;
  UI* offsets = LoomOffsets<UI, SI, Char>(loom);
  uintptr_t offset = reinterpret_cast<uintptr_t>(loom) + offsets[index];
  return offset;
}

template <typename UI, typename SI, typename Char>
SI LoomPrint(Utf8& print, CLoom* loom, const Char* strand) {
  ASSERT(loom)
  ASSERT(strand)
  SI count = loom->count;
  print << "\nLoom:" << loom->size << "b count_max:" << loom->count_max
        << " count:" << count;
  UI* offsets = LoomOffsets<UI, SI, Char>(loom);
  uintptr_t offset = reinterpret_cast<uintptr_t>(loom) + offsets[index];
  for (SI i = 0; i < count; ++i) {
    print << '\n' << i << ".) \"" <<
  }
}

}  // namespace _
#endif  //< INCLUDED_CRABS_VARINT
#endif  //< #if SEAM >= SEAM_0_0_3
