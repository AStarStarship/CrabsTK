/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/seam_0_0/seam.h
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

#include "00_test_f2/seam.h.h"
#include "01_test_crabs/seam.h"
#include "02_test_hal/seam.h"
#include "03_test_drivers/seam.h"
#include "04_test_platform/seam.h"
#include "05_test_features/seam.h"
#include "06_test_data/seam.h"
#include "07_test_pro/seam.h"

namespace _ {

static const char* Seam_00_00_00(char* test_result, const char* args) {
  return TestTree<Seam_00_00_00__00>(test_result, args);
}

}  // namespace _
