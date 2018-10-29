/* Kabuki Toolkit @version 0.x
@file    /kabuki/features/crabs/seam_0_0_4_0_crabs.h
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

#include <kabuki/f2/ttest.h>

#include "00/project_room.h"

namespace _ {

static const char* Kabuki__Terminal___Kabuki__Features__Crabs(char* seam_log, char* log_end,
                                 const char* args) {
#if SEAM >= _0_0_4
  if (!TestBegin(seam_log, log_end, args)) return __FUNCTION__;
#endif
  return TestTree<Seam_0_0_4_0__00>(seam_log, log_end, args);
}
}  //< namespace _
