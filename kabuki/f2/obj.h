/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/obj.h
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
#include <pch.h>
#if SEAM >= SEAM_00_00_01__06
#ifndef INCLUDED_F2_OBJ
#define INCLUDED_F2_OBJ

#include "align.h"
#include "test.h"


typedef void (*Destructor)(uintptr_t* buffer);

namespace _ {

/* C header for an ASCII OBJ with programmable destructor. */
struct CObj {
  uintptr_t* begin;       //< Pointer to the contiguous ASCII OBJ.
  Destructor destructor;  //< Pointer to the destructor.
};

/* Destructor deletes the buffer. */
API void Delete(uintptr_t* buffer);

/* Checks if the value is a valid object index, that it's 7 less than the max
value or less. */
API inline bool ObjCountIsValid(int8_t value, int8_t count_min = 1);

/* Checks if the value is a valid object index, that it's 7 less than the max
value or less. */
API inline bool ObjCountIsValid(int16_t value, int16_t count_min = 1);

/* Checks if the value is a valid index, that it's 7 less than the max
value or less. */
API inline bool ObjCountIsValid(int32_t value, int32_t count_min = 1);

/* Checks if the value is a valid index, that it's 7 less than the max
value or less. */
API inline bool ObjCountIsValid(int64_t value, int64_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
API inline bool ObjSizeIsValid(int16_t value, int16_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
API inline bool ObjSizeIsValid(int32_t value, int32_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
API inline bool ObjSizeIsValid(int64_t value, int64_t count_min = 1);

/* Clones the given ASCII OBJ. */
API uintptr_t* ObjClone(uintptr_t* ascii_object, int8_t size);

/* Clones the given ASCII OBJ. */
API uintptr_t* ObjClone(uintptr_t* ascii_object, int16_t size);

/* Clones the given ASCII OBJ. */
API uintptr_t* ObjClone(uintptr_t* ascii_object, int32_t size);

/* Clones the given ASCII OBJ. */
API uintptr_t* ObjClone(uintptr_t* ascii_object, int64_t size);

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_00_01__06
#endif  //< INCLUDED_F2_OBJ
