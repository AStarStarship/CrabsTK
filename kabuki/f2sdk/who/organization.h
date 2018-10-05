/* Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/organization.h
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

#pragma once
#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_ORGANIZATION
#define HEADER_FOR_KT_ID_ORGANIZATION

#include "entity.h"

namespace _ {

/* An entity that is not a person such as a business or non-profit
   organization. Roster - list or plan showing turns of duty or leave for
   individuals or groups in an organization.
*/
class KABUKI Organization : public Entity {
 public:
  /* Default constructor. */
  Organization();

  /* Prints this object to a expression. */
  void Print(Log& log);

 private:
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_ORGANIZATION
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
