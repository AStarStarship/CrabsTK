/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/crabs_object.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
// Dependencies:
#include "object.h"
#include "socket.h"
// End dependencies.

namespace _ {

bool ObjectSizeIsValid(int16_t size, int16_t size_min) {
  return ObjectSizeIsValid<int16_t>(size, size_min);
}

bool ObjectSizeIsValid(int32_t size, int32_t size_min) {
  return ObjectSizeIsValid<int32_t>(size, size_min);
}

bool ObjectSizeIsValid(int64_t size, int64_t size_min) {
  return ObjectSizeIsValid<int64_t>(size, size_min);
}

bool ObjectCountIsValid(int8_t index, int8_t count_min) {
  return ObjectCountIsValid<int8_t>(index, count_min);
}

bool ObjectCountIsValid(int16_t index, int16_t count_min) {
  return ObjectCountIsValid<int16_t>(index, count_min);
}

bool ObjectCountIsValid(int32_t index, int32_t count_min) {
  return ObjectCountIsValid<int32_t>(index, count_min);
}

bool ObjectCountIsValid(int64_t index, int64_t count_min) {
  return ObjectCountIsValid<int64_t>(index, count_min);
}

uintptr_t* ObjectClone(uintptr_t* ascii_object, int8_t size) {
  return ObjectClone<int8_t>(ascii_object, size);
}

uintptr_t* ObjectClone(uintptr_t* ascii_object, int16_t size) {
  return ObjectClone<int16_t>(ascii_object, size);
}

uintptr_t* ObjectClone(uintptr_t* ascii_object, int32_t size) {
  return ObjectClone<int32_t>(ascii_object, size);
}

uintptr_t* ObjectClone(uintptr_t* ascii_object, int64_t size) {
  return ObjectClone<int64_t>(ascii_object, size);
}

uintptr_t* ObjectNew(int16_t size, size_t header_size) {
  ASSERT(size >= 8);

  if (ObjectSizeIsValid(size)) return nullptr;
  size = AlignUp<8, uint16_t, int16_t>(size);
  uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];
  *reinterpret_cast<int16_t*>(buffer) = size;
  return buffer;
}

void Delete(uintptr_t* buffer) {
  ASSERT(buffer);
  delete[] buffer;
}

void Delete(CObj object) {
  if (!object.destructor_) return;
  object.destructor_(object.ascii_obj_);
}

}  // namespace _
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
