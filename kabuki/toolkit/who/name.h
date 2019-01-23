/* Kabuki Toolkit
    @file    /library/kt/id/name.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
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
#ifndef HEADER_FOR_KT_ID_NAME
#define HEADER_FOR_KT_ID_NAME

#include "config.h"

namespace _ {

/* A name of an entity. */
class API Name {
 public:
  /* Default constructor. */
  Name();

  /* Writes this object to the given text. */
  void Write(Text& text);
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_NAME
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
