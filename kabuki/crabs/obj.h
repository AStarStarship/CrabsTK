/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/

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
#ifndef HEADER_FOR_CRABS_OBJ
#define HEADER_FOR_CRABS_OBJ
// Dependencies:
#include "align.h"
#include "debug.h"
// End dependencies.

typedef void (*Destructor)(uintptr_t* buffer);

namespace _ {

/* C header for an ASCII OBJ with programmable destructor. */
struct CObj {
  uintptr_t* begin;       //< Pointer to the contiguous ASCII OBJ.
  Destructor destructor;  //< Pointer to the destructor.
};

/* Destructor deletes the buffer. */
KABUKI void Delete(uintptr_t* buffer);

/* Checks if the value is a valid object index, that it's 7 less than the max
value or less. */
KABUKI inline bool ObjCountIsValid(int8_t value, int8_t count_min = 1);

/* Checks if the value is a valid object index, that it's 7 less than the max
value or less. */
KABUKI inline bool ObjCountIsValid(int16_t value, int16_t count_min = 1);

/* Checks if the value is a valid index, that it's 7 less than the max
value or less. */
KABUKI inline bool ObjCountIsValid(int32_t value, int32_t count_min = 1);

/* Checks if the value is a valid index, that it's 7 less than the max
value or less. */
KABUKI inline bool ObjCountIsValid(int64_t value, int64_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
KABUKI inline bool ObjSizeIsValid(int16_t value, int16_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
KABUKI inline bool ObjSizeIsValid(int32_t value, int32_t count_min = 1);

/* Checks if the value is a valid object size, that it's an even multiple of
8. */
KABUKI inline bool ObjSizeIsValid(int64_t value, int64_t count_min = 1);

/* Clones the given ASCII OBJ. */
KABUKI uintptr_t* ObjClone(uintptr_t* ascii_object, int8_t size);

/* Clones the given ASCII OBJ. */
KABUKI uintptr_t* ObjClone(uintptr_t* ascii_object, int16_t size);

/* Clones the given ASCII OBJ. */
KABUKI uintptr_t* ObjClone(uintptr_t* ascii_object, int32_t size);

/* Clones the given ASCII OBJ. */
KABUKI uintptr_t* ObjClone(uintptr_t* ascii_object, int64_t size);

}  // namespace _
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#endif  //< HEADER_FOR_CRABS_OBJ
