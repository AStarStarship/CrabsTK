/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/pro/pro_workspace.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_PRO_1
#include "cworkspace.h"

namespace _ {

Workspace::Workspace()
//:   controlSurfaces ()
{}

Workspace::Workspace(const Workspace& wordspace)
//:   controlSurfaces  (O.controlSurfaces)
{}

Workspace::~Workspace() {
  for (SI4 i = 0; i < controlSurfaces.size; ++i) delete controlSurfaces[i];
}

void Workspace::Add(Controller* controller) {
  if (controller == nullptr) return;

  controlSurfaces.Add(controller);
}

SI4 Workspace::GetNumControllers() { return controlSurfaces.count(); }

}  // namespace _
#endif
