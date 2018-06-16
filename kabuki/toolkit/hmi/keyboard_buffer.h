/* Kabuki Toolkit
    @file       ~/kabuki_tek/logging/real-time_keyboard.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
namespace _ {

/* A computer keyboard.
 */
class KeyboardBuffer {
  enum {
    DefaultBufferSize = 256,    //< The default buffer size.
    MinBufferSize = 64,         //< Min buffer size.
    MaxBufferSize = 1024 * 10,  //< Max buffer size.
  };

  /* Default constructor. **/
  KeyboardBuffer(int a = DefaultBufferSize) {
    if (a < MinBufferSize)
      bufferSize = MinBufferSize;
    else if (a > MaxBufferSize)
      bufferSize = MaxBufferSize;
    else
      bufferSize = a;

    start = stop = begin;
  }

  /* Resets the event buffer. */
  void Clear() { start = stop = begin; }

  /* Inserts an event into the buffer. */
  void Insert(Event a) {
    if (stop + 1 > end) {
      Debug.Assert(start != begin, "Insert  (byte): Buffer overflow.");
      stop = begin;
      buffer[stop] = a;
    }
    Debug.Assert(stop + 1 == start, "Insert  (byte): Buffer overflow.");

    buffer[++stop] = a;
  }

  /* Updates the  */
  void Update() {
    if (start > stop) {
      for (int i = start; i <= end; ++i) buffer[i]();
      for (int i = begin; i <= stop; ++i) buffer[i]();
      start = stop = begin;
      return;
    }

    for (int i = start; i <= end; ++i) buffer[i]();
    start = stop = begin;
  }

 private:
  int begin,       //< The begning of the
      end,         //< The end of the
      start,       //< The start of the events.
      stop,        //< The stop of the events.
      bufferSize;  //< The buffer size.

  Event buffer[];  //< An ring-buffer of real-time events from the keyboard.
};                 //< KeyboardBuffer
}  // namespace logging
}  // namespace _ {tek
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
