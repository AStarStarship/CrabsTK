/* kabuki::pro
    @file    /library/kt/id/id_privilage.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "privilage.h"

namespace _ {

static Privilages num_privilage_levels = kDefaultMaxPrivilageLevels;

Privilages SetMinPrivilageLevel() { return num_privilage_levels; }

bool SetPrvilageLevelRange(Privilages min, Privilages max) {}

Privilage::Privilage() {}

Privilages Privilage::GetPrivilageLevel() { return privilages_; }

bool Privilage::SetPrivilageLevel(Privilages privilages) {
  if (privilages < 0) return false;
  if (privilages > num_privilage_levels) return false;
  privilages_ = privilages;
  return true;
}

void Privilage::Print(Log& log) {}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
