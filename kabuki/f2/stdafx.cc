/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/stdafx.cc
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
#include "global.h"

#if SEAM > 0 && SEAM <= SEAM_COUNT

using namespace _;

/* Main program entry.
@return APP_EXIT_SUCCESS upon success and APP_EXIT_FAILURE upon failure. */
int main(const char* args, int arg_count) {
  return RunUnitTests<"F2 Foundation Framework">(args, arg_count);
}
#endif
