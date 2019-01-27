/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_person.h
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
#ifndef KABUKI_TOOLKIT_WHO_PERSON
#define KABUKI_TOOLKIT_WHO_PERSON

#include "t_entity.h"

namespace _ {

/* Class that represents a person/human.
@todo Load a person from social media account using Facebook and OAuth APIs.
*/
class TPerson : public TEntity {
 public:
  /*Creates a person with no name. */
  TPerson();

  /* Prints this object to a expression. */
  Printer& Print (Printer& o) { o << "Person: "; }

 private:
  TStrand<>& lastName;  //< The last name of the entity.
};
}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_PERSON
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
