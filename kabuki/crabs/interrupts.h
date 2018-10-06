/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    kabuki-toolkit.git/kabuki/crabs/interrupts.h
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
#ifndef INCLUDED_CRABS_EXCEPTIONS
#define INCLUDED_CRABS_EXCEPTIONS
// Dependencies:
#include "config.h"
// End dependencies.
#if USING_CRABS_INTERRUPTS

namespace _ {

#if USING_TEXT_SCRIPT
/* Exception thrown to interrupt and crash the program. */
struct API RoomCrashException : public std::exception {
  const char* what() const throw();
};
#endif
}  // namespace _
#endif  //< USING_CRABS_INTERRUPTS
#endif  //< INCLUDED_CRABS_EXCEPTIONS
