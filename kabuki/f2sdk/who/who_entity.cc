/* kabuki::pro
    @file    $kabuki-toolkit/library/kt/id/id_entity.cc
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

#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "entity.h"

namespace _ {

Entity::Entity() {}

const char* Entity::GetFirstName() { return first_name_; }

void Entity::SetFirstName(const char* string) {
  delete first_name_;
  first_name_ = StringClone(string);
}

const char* Entity::GetLastName() { return last_name_; }

void Entity::SetLastName(const char* string) {
  delete last_name_;
  last_name_ = StringClone(string);
}

bool Entity::Query(const char* query) {
  /*
  for_each (tags.begin (), tags.end (), [](char& string(
  {
      if (string == query) return 1;
  });

  for_each (tags.begin (), tags.end (), [](Address& a(
  {
      if (a == query) return 1;
  });

  for_each (tags.begin (), tags.end (), [](Entity& e(
  {
      if (e == query) return 1;
  });

  for_each (tags.begin (), tags.end (), [](Profile& p(
  {
      if (p == query) return 1;
  });

  for_each (tags.begin (), tags.end (), [](char& string(
  {
      if (string == query) return 1;
  });
  */
  return false;
}

void Entity::Print(Log& log) { log << "Entity: "; }

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
