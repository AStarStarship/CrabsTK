/* Kabuki Toolkit
    @file    /library/kt/id/Person.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
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
#ifndef HEADER_FOR_KT_ID_PERSON
#define HEADER_FOR_KT_ID_PERSON

#include "Entity.h"

namespace _ {

/* Class that represents a person/human.
    @todo Load a person from social media account using Facebook and OAuth APIs.
*/
class API Person : public Entity {
 public:
  Person();
  /*Creates a person with no name. */

  void Print(Log& log);
  /* Prints this object to a expression. */

 private:
  char* lastName;  //< The last name of the entity.
};
}       // namespace _
#endif  //< HEADER_FOR_KT_ID_PERSON
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
