/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/stdafx.h
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

#ifndef INCLUDED_SEAMS_0_0_TEST_SEAMS
#define INCLUDED_SEAMS_0_0_TEST_SEAMS 1

#include <f2/test.h>

TestResult TestSEAM_0_0_0(const char* args);

static SeamMajor seam_major_1 =
    MajorSeams<&TestSEAM_0_0_0>(0, 0, seam_major_1.GetMinorSeamCount());

}  // namespace _

int main(int arg_count, char** args) { return seam_major_1.Run(args); }

#endif  //< #if INCLUDED_SEAMS
