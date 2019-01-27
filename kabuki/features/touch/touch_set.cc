/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_set.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "set.h"

namespace _ {

Set::Set()
//:   controlSurfaces ()
{}

Set::Set(const Set& other)
//:   controlSurfaces  (other.control_surfaces_)
{}

Set::~Set() {
  for (SI4 i = 0; i < projects.size; ++i) delete projects[i];
}

void Set::Add(HMI::Controller* c) {
  if (c == nullptr) return;

  projects.Add(c);
}

SI4 Set::GetNumProject() { return projects.GetCount(); }

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
