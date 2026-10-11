/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /project/theater/01_bar.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM == THEATER_0
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

using namespace _;

namespace project { namespace theater { 
inline const CH1* _1_Bar (CH1* seam_log, CH1* seam_end, const CH1* args) {
#if SEAM >= THEATER_0
  TEST_BEGIN;

  PRINT_HEADING ("Testing Foo fun.");

#endif
  return 0;
}
}  //< namespace theater
}  //< namespace project
