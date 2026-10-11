/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_tag.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_TAG
#define KABUKI_TOOLKIT_AV_TAG


namespace _ {

class Tag {
 public:
  /* Gets the Unique identifier number. */
  virtual SI4 GetUid() = 0;

  /* Sets the Unique identifier number. */
  virtual CH1 SetUid(SI4 value) = 0;

  /* Gets the ID name. */
  virtual CH1 GetName() = 0;

  /* Sets the ID name. */
  virtual CH1 SetName(const TStrand<>& name) = 0;
};
}       // namespace _
#endif  //< KABUKI_TOOLKIT_AV_TAG
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
