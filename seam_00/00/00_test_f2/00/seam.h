/* Kabuki Toolkit @version 0.x
@file    ~/seam_00/00/00_test_f2/00/seam.h
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

#include "seam_header.h"

#include "00/seam.h"

namespace _ {

static const char* Seam_00_00_00__00(char* seam_log, char* seam_end,
                                     const char* args) {
  if (!TestBegin(seam_log, seam_end, args)) return __FUNCTION__;
  return TestTreeNode<Seam_00_00_00__00_00>(seam_log, seam_end, args);
}
}  // namespace _

#include "seam_footer.h"
