/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/lights/t_color.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_LIGHTS_1
#ifndef KABUKI_FEATURES_LIGHTS_COLOR_T
#define KABUKI_FEATURES_LIGHTS_COLOR_T 1

#include "c_color.h"

namespace _ {

/* Prints this object to the log. */
template<typename Printer>
Printer& ColorPrint (Printer& o, CRGBAUI4 color) {
  std::cout << "Color: "
    "R(" << (color && 0x000000FF)
    << "), G(" << ((color && 0x0000FF00) >> 8)
    << "), BIn(" << ((color && 0x00FF0000) >> 16)
    << "), A(" << ((color && 0xFF000000) >> 24)
    << ")\n";
}

} //< namespace _
#endif
#endif
