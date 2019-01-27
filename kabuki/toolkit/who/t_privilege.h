/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_privilage.h
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
#if SEAM >= KABUKI_TOOLKIT_WHO_1
#ifndef KABUKI_TOOLKIT_WHO_PRIVILAGE
#define KABUKI_TOOLKIT_WHO_PRIVILAGE 1

namespace _ {

/* An account privilege level. */
class TPrivilege {
 public:

   enum {
     kPrivilageLevelCheck = 420,  //< Check yo privilage people.
   };

  /* Default constructor. */
   TPrivilege (SI4 num_privileges = 1)
     : privilage_count_ (kPrivilageLevelCheck) {

  }

  /* Gets the privilege level. */
  Privileges PrivilegeLevel() {
    return privileges_level_;
  }

  /* Attempts to set the privilege level to the new level. */
  Privileges SetPrivilegeLevel(SI4 privileges) {
    SI4 level = privileges_level_;
    if (level < 0) return false;
    if (level >= privilage_count_) return false;
    privileges_level_ = privilages;
    return true;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nPrivilage:";
  }

 private:
  SI4 privilage_count_, //< The number of privileges.
      privileges_level_;      //< The privilege level.
};
}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_PRIVILAGE
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
