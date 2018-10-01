/* Kabuki Toolkit @version 0.x
@file    ~/seam_00/00/00_test_f2/sprint_01/seam.h
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

#include "../seam.h"

#include "sprint_00/seam.h"

#define SEAM_00_00_00__00_00 1
#define SEAM_00_00_00__00_02 2
#define SEAM_00_00_00__00_03 3

namespace _ {
const char* Seam_00_00_00__00_00(char* seam_begin, char* seam_end, const char* args);
const char* Seam_00_00_00__00_01(char* seam_begin, char* seam_end, const char* args);
const char* Seam_00_00_00__00_02(char* seam_begin, char* seam_end, const char* args);

static const char* Seam_00_00_00__00(char* seam_begin, char* seam_end,
                                     const char* args) {
  return TestTreeNode<Seam_00_00_00__00_00, Seam_00_00_00__00_01,
                      Seam_00_00_00__00_02>(test_result, args);
}

}  // namespace _
