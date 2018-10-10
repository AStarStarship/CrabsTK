/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
<<<<<<< HEAD:seam_0/0/seam_terminal.h
@file    /\seam_0/0\seam.h
=======
@file    /\seam_00\00\seam.h
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/seam_terminal.h
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

#include "0_seam_f2/seam.h"
#include "1_test_crabs/seam.h"
#include "2_test_hal/seam.h"
#include "3_test_drivers/seam.h"
#include "4_platform/seam.h"
#include "5_features/seam.h"
#include "6_test_data/seam.h"
#include "7_test_pro/seam.h"

namespace _ {

static const char* Seam_0_0(char* seam_log, char* log_end, const char* args) {
  return TestTree<Seam_0_0_0, Seam_0_0_1, Seam_0_0_2, Seam_0_0_4, Seam_0_0_5>(
      seam_log, log_end, args);
}

}  // namespace _
