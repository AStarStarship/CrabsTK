/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/buffer.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_TEXT
#define HEADER_FOR_CRABS_TEXT
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

/* Creates/Gets a static buffer of the specified size. */
template <size_t kBufferSize = kBufferSizeDefault>
inline char* Buffer() {
  enum {
    kBufferSizeWords =
        (kBufferSize / sizeof(uintptr_t)) + kBufferSize % sizeof(uintptr_t) ? 1
                                                                            : 0
  };
  static uintptr_t buffer[kBufferSizeWords];
  return reinterpret_cast<char*>(buffer);
}

}  // namespace _
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#endif  //< HEADER_FOR_CRABS_TEXT
