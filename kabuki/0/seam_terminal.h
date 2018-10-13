/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\0\seam.h
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

#include "0_f2/seam_f2.h"
#include "1_hal/seam_hal.h"
#include "2_drivers/seam_drivers.h"
#include "3_platform/seam_platform.h"
#include "4/seam_features.h"
#include "5_data/seam_data.h"
#include "6_pro/seam_pro.h"
#include "7_ai_math/seam_ai_math.h"

namespace _ {

static const char* Seam_0(char* seam_log, char* log_end, const char* args) {
  return TestTree<Seam_0_0_0, Seam_0_0_1, Seam_0_0_2, Seam_0_0_3, Seam_0_0_4>(
      seam_log, log_end, args);
}

}  // namespace _
