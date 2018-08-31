/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/crabs_wall.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM >= SEAM_0_0_4
// Dependencies:
#include "align.h"
#include "debug.h"
#include "tstk.h"
#include "wall.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 4
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#else
#define PRINTF(x, ...)
#define PRINT(c)
#endif

namespace _ {

Wall::~Wall() {
  if (is_dynamic_) {
    char* buffer = reinterpret_cast<char*>(doors_);
    delete[] buffer;
  }
}

Wall::Wall(size_t size_bytes) : is_dynamic_(true) {
  size_bytes = size_bytes < kMinSizeBytes ? (uint_t)kMinSizeBytes : size_bytes;
  size_bytes = AlignUpUnsigned<int64_t, size_t>(size_bytes);
  size_t size_words = (size_bytes >> sizeof(void*)) + 3;
  uintptr_t *buffer = new uintptr_t[size_words],
            *aligned_buffer = AlignUpPointer8<uintptr_t>(buffer);
  //< Shift 3 to divide by 8. The extra 3 elements are for aligning memory
  //< on 16 and 32-bit systems.
  size_bytes -= sizeof(uintptr_t) * (aligned_buffer - buffer);
  begin = buffer;
  doors_ = reinterpret_cast<CArray<Door*>*>(aligned_buffer);
  StackInit(buffer, size_bytes >> sizeof(uintptr_t));
}

Wall::Wall(uintptr_t* buffer, size_t size_bytes) {
  // char* ptr     = reinterpret_cast<char*> (buffer);//,
  //    * new_ptr = ptr + AlignOffset<uint64_t> (ptr),
  //    * end_ptr = ptr + size_bytes;
  enum {
    kBitsShift = sizeof(uintptr_t) == 2 ? 1 : sizeof(uintptr_t) == 2 ? 2 : 3,
  };
  // uint_t size_words = (size_bytes >> kBitsShift) + 3;
  //< Computer engineering voodoo for aligning to 64-bit boundary.

  uintptr_t* aligned_buffer = AlignUpPointer8<uintptr_t>(buffer);
  //< Shift 3 to divide by 8. The extra 3 elements are for aligning memory
  //< on 16 and 32-bit systems.
  size_bytes -= sizeof(uintptr_t) * (aligned_buffer - buffer);
  begin = buffer;
  doors_ = reinterpret_cast<CArray<Door*>*>(aligned_buffer);
  StackInit(buffer, size_bytes >> sizeof(uintptr_t));
}

Wall::Wall(CArray<Door*>* doors) {}

uintptr_t Wall::GetSizeBytes() { return size_bytes_; }

CArray<Door*>* Wall::Doors() { return doors_; }

Door* Wall::GetDoor(int index) { return 0; }

int Wall::OpenDoor(Door* door) { return 0; }

bool Wall::CloseDoor(int index) { return false; }

Slot& Wall::Print(Slot& slot) {
  // Printf ("\nDoor:\nis_dynamic %s\nnum_doors: %u\nmax_num_doors: %u\n",
  //        is_dynamic ? "true" : "false", num_doors, max_num_doors);
  return slot;
}

}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //< #if SEAM >= SEAM_0_0_4
