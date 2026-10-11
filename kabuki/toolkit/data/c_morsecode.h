/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/ccodemore.h
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

#if SEAM >= KABUKI_TOOLKIT_DATA_1
#if SEAM == KABUKI_TOOLKIT_DATA_1
#ifndef INCLUDED_KABUKI_TEK_UTILS_MORSECODE
#define INCLUDED_KABUKI_TEK_UTILS_MORSECODE 1

namespace _ {

/* Gets the Morse code for the given CH1. */
const CH1* ToMorseCode (CH1 value);

}       //< _utils
#endif
#endif
