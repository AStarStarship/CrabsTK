/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    kabuki-toolkit.git/kabuki/crabs/KabukiSDK/data/iterator.h
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
#if SEAM >= SEAM_0_0_00
#ifndef CRABS_ITERATOR_H
#define CRABS_ITERATOR_H
// Dependencies:
#include "config.h"
// End dependencies.

namespace _ {

/* Interface for a generic iterator. */
struct API Iterator {
  /* Virtual destructor. */
  virtual ~Iterator() = 0;

  /* Iterates over the next item.
      @return Pointer to the next object and nil when iteration is
              complete. */
  virtual void* GetNext() = 0;

  /* Flags when there are not more objects to iterate through.
      @return True when there are no more objects to iterate through. */
  virtual bool IsDone() = 0;
};

}  // namespace _
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >=5
#endif  //< CRABS_ITERATOR_H
