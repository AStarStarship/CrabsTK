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

#pragma once
#include <pch.h>
#if SEAM >= SEAM_0_0_4_0__00_00
#ifndef INCLUDED_CRABS_FLOOR
#define INCLUDED_CRABS_FLOOR
#include "config.h"

namespace _ {

/* Cache-aligned group of global system variables.
The Floor is the Floor in the Chinese Room. In Kabuki Crabs, the floor is
designed to store variables without having to include the classes the variable
controls. For this reason only strings and POD types are in the Floor.
*/
struct API Floor {
  int16_t epoch;
};

/* Returns the the global Floor. */
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336
API inline Floor* Global();

}  // namespace _

#endif  // #if INCLUDED_CRABS_FLOOR
#endif  // #if SEAM >= SEAM_0_0_4_0__00_00
