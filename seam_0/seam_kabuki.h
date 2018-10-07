/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /seam_0/seam_tree_node.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include <kabuki/f2/global.h>

#include "00/seam_terminal.h"
#include "01/seam_web.h"
#include "02/seam_audio.h"
#include "03/seam_visual.h"
#include "04/seam_multimedia.h"

namespace _ {

static const char* Seam_0(char* seam_log, char* log_end, const char* args) {
  enum { kSeamLogSize = 1024 };
  char seam_log[kSeamLogSize];
  return TestTree<Seam_00_00, Seam_00_01, Seam_00_02, Seam_00_03, Seam_00_04>(
      seam_log, args);
}
}  // namespace _
