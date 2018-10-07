/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/crabs/floor.h
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
#if SEAM >= SEAM_0_0_2
#include "clock.h"
#include "floor.h"
#include "tsocket.h"

namespace _ {

char FloorInit(uintptr_t* buffer) { return 0; }

Floor* FloorInit(Floor* floor) {
  floor->epoch = kClockEpochInit;
  return floor;
}

inline Floor* Global() {
  static TSocket<kFloorSize, kCpuCacheLineSize> floor_socket;
  return floor_socket.Start<Floor>();
}

}  // namespace _
#endif  // #if SEAM >= SEAM_0_0_2
