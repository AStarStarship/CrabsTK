/* Kabuki Toolkit
    @file    /library/kt/id/tag.h
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
#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_TAG
#define HEADER_FOR_KT_ID_TAG
#include "config.h"

namespace _ {

class API Tag {
 public:
  /* Gets the Unique identifier number. */
  virtual int GetUid() = 0;

  /* Sets the Unique identifier number. */
  virtual char SetUid(int value) = 0;

  /* Gets the ID name. */
  virtual char GetName() = 0;

  /* Sets the ID name. */
  virtual char SetName(const char* name) = 0;
};
}       // namespace _
#endif  //< HEADER_FOR_KT_ID_TAG
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
