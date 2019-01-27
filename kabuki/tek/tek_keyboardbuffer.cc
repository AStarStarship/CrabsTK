/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/tek/tek_keyboardbuffer.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_TEK_1

#include "c_keyboardbuffer.h"

#if SEAM == KABUKI_TEK_1
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

namespace _ {

KeyboardBuffer::KeyboardBuffer(SI4 a) {
  if (a < MinBufferSize)
    bufferSize = MinBufferSize;
  else if (a > MaxBufferSize)
    bufferSize = MaxBufferSize;
  else
    bufferSize = a;

  start = stop = begin;
}

void KeyboardBuffer::Clear() { start = stop = begin; }

void KeyboardBuffer::Insert(Event a) {
  if (stop + 1 > end) {
    DASSERT (start != begin);
    stop = begin;
    buffer[stop] = a;
  }
  DASSERT(stop + 1 == start);

  buffer[++stop] = a;
}

void KeyboardBuffer::Update() {
  if (start > stop) {
    for (SI4 i = start; i <= end; ++i) buffer[i]();
    for (SI4 i = begin; i <= stop; ++i) buffer[i]();
    start = stop = begin;
    return;
  }

  for (SI4 i = start; i <= end; ++i) buffer[i]();
  start = stop = begin;
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_TEK_1
