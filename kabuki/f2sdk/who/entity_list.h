/* Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/entity_hist.h
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
#ifndef HEADER_FOR_KT_ID_ENTITYLIST
#define HEADER_FOR_KT_ID_ENTITYLIST

#include "entity.h"

namespace _ {

/* A List of entities.
    This is essentially a contact list. */
class KABUKI EntityList {
 public:
  /* Creates an empty list. */
  EntityList();

  /* Gets the number of Accounts in the List. */
  int GetSize();

  /* Adds an Entity to the list. */
  void Add(Entity* entity);

  /* Adds a list of Entity (string) to the list. */
  void Add(EntityList& enities);

  /* Finds an entity in the list by the given search char. */
  Entity* Find(const char* string);

  /* Prints this object to the log. */
  void Print(Log& log);

 private:
  data::Array<Entity*>* entities_;  //< The list of entities.
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_ENTITYLIST
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
