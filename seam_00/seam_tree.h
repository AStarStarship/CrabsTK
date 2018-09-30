/* Kabuki Toolkit @version 0.x
@file    ~/seam_00/seam_tree_node.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>

#include <kabuki/f2/global.h>

#include "00/seam_tree.h"
#include "01/seam_tree.h"
#include "02/seam_tree.h"
#include "03/seam_tree.h"
#include "04/seam_tree.h"

namespace _ {
const char* Seam_00_00(TestResult& test_result, const char* args);
const char* Seam_00_01(TestResult& test_result, const char* args);
const char* Seam_00_02(TestResult& test_result, const char* args);
const char* Seam_00_04(TestResult& test_result, const char* args);

static const char* Seam_00(TestResult& test_result, const char* args) {
  return TestTree<Seam_00_00, Seam_00_01, Seam_00_02, Seam_00_03, Seam_00_04>(
      test_result, args);
}
}  // namespace _
