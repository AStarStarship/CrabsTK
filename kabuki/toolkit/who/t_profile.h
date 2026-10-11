/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_profile.h
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
#if SEAM >= KABUKI_TOOLKIT_WHO_1
#ifndef KABUKI_TOOLKIT_WHO_PROFILE
#define KABUKI_TOOLKIT_WHO_PROFILE


#include "t_entity.h"

namespace _ {

/* An online profile of a person. */
class TProfile {
 public:
  /* Constructor an anonymous Profile. */
  TProfile();

  /* Prints this object to the console. */
  Printer& Print(Printer& print);

 private:
};
}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_PROFILE
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
