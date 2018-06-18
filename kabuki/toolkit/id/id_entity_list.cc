/** kabuki::pro
    @file    $kabuki-toolkit/library/kt/id/id_entity_list.cc
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
#include "../entity_list.h"

namespace _ {

EntityList::EntityList() {}

int EntityList::GetSize() { return entities_->GetSize(); }

void EntityList::Add(Entity* a) { entities_->Push(a); }

void EntityList::Add(EntityList& l) {
  // entities.push_back (l);
}

Entity* EntityList::Find(const char* query) {
  size_t length = strlen(query);
  if (length == 0) return nullptr;

  Entity* ptr;

  for (int i = 0; i < entities_->GetSize(); i++) {
    ptr = entities_->Element(i);
    if (ptr->Query(query)) return ptr;
  }

  return nullptr;  // static website guest entities
}

void EntityList::Print(Log& log) {
  log << "Number of Accounts: " << entities_->GetCount() << (char)13;

  for (int i = 0; i < entities_->GetSize(); i++) {
    log << "Account " << (i + 1) << ": "
        << entities_->Element(i)->GetFirstName() << (char)13;
  }
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
