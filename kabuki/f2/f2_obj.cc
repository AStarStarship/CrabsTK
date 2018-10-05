/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/f2_object.h
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
#if SEAM >= SEAM_00_00_01__06

#include "align.h"
#include "obj.h"
#include "socket.h"
#include "tobj.h"


namespace _ {

bool ObjSizeIsValid(int16_t size, int16_t size_min) {
  return ObjSizeIsValid<int16_t>(size, size_min);
}

bool ObjSizeIsValid(int32_t size, int32_t size_min) {
  return ObjSizeIsValid<int32_t>(size, size_min);
}

bool ObjSizeIsValid(int64_t size, int64_t size_min) {
  return ObjSizeIsValid<int64_t>(size, size_min);
}

bool ObjCountIsValid(int8_t index, int8_t count_min) {
  return ObjCountIsValid<int8_t>(index, count_min);
}

bool ObjCountIsValid(int16_t index, int16_t count_min) {
  return ObjCountIsValid<int16_t>(index, count_min);
}

bool ObjCountIsValid(int32_t index, int32_t count_min) {
  return ObjCountIsValid<int32_t>(index, count_min);
}

bool ObjCountIsValid(int64_t index, int64_t count_min) {
  return ObjCountIsValid<int64_t>(index, count_min);
}

uintptr_t* ObjClone(uintptr_t* ascii_object, int8_t size) {
  return ObjClone<int8_t>(ascii_object, size);
}

uintptr_t* ObjClone(uintptr_t* ascii_object, int16_t size) {
  return ObjClone<int16_t>(ascii_object, size);
}

uintptr_t* ObjClone(uintptr_t* ascii_object, int32_t size) {
  return ObjClone<int32_t>(ascii_object, size);
}

uintptr_t* ObjClone(uintptr_t* ascii_object, int64_t size) {
  return ObjClone<int64_t>(ascii_object, size);
}

uintptr_t* ObjNew(int16_t size, size_t header_size) {
  ASSERT(size >= 8);

  if (ObjSizeIsValid(size)) return nullptr;
  size = AlignUp(size);
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
  *reinterpret_cast<int16_t*>(buffer) = size;
  return buffer;
}

void Delete(uintptr_t* buffer) {
  ASSERT(buffer);
  delete buffer;
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_00_01__06
