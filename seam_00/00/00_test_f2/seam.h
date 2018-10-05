/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/eam_00/00/00_test_f2/seam.h
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

#include "00/seam_itos_and_stoi.h"
#include "01/seam_ftos_and_stof.h"
#include "02/seam_core_text_functions.h"
#include "03/seam.h"

namespace _ {

static bool Seam_00_00_00__00(char* seam_log, char* log_end, const char* args) {
  if (!TestBegin(seam_log, log_end, args, __FUNCTION__)) return false;
  return TestTree<Seam_00_00_00__00, Seam_00_00_00__01>(seam_log, log_end,
                                                        args);
}

}  // namespace _
