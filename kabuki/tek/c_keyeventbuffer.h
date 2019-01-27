/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/tek/c_keyeventbuffer.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TEK_1
#ifndef KABUKI_TEK_KEYEVENTBUFFER
#define KABUKI_TEK_KEYEVENTBUFFER

namespace _ {

/* A computer keyboard event buffer.
Keyboard events are stored as delegates.
*/
class SDK KeyEventBuffer {
  enum {
    DefaultBufferSize = 256,   //< The default buffer size.
    MinBufferSize = 64,        //< Min buffer size.
    MaxBufferSize = 1024 * 10  //< Max buffer size.
  };

  /* Default constructor. */
  KeyEventBuffer(SI4 a = DefaultBufferSize);

  /* Resets the event buffer. */
  void Clear();

  /* Inserts an event into the buffer. */
  void Insert(Event a);

  /*  */
  void Update();

 private:
  SI4 begin, end, start, stop, bufferSize;
  Event* buffer;

};  //< class KeyEventBuffer
}  // namespace _

#endif  //< KABUKI_TEK_KEYEVENTBUFFER
#endif  //< #if SEAM >= KABUKI_TEK_1
