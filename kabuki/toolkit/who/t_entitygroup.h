/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_entitygroup.h
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
#ifndef KABUKI_TOOLKIT_WHO_ENTITYGROUP
#define KABUKI_TOOLKIT_WHO_ENTITYGROUP

#include "t_entity.h"

namespace _ {

/* A group of entities. */
class TEntityGroup {
 public:
  /* A group of entities such as people or businesses. */
  TEntityGroup(const TStrand<>& name)
    : name_ (StrandClone (name == nullptr ? "" : name)) {}

  /* Gets the name of the entity group. */
  const TStrand<>& GetName() { return name_; }

  /* Sets the name of the entity group. */
  void SetName(const TStrand<>& string) {
    name_ = StrandClone (string);
  }

  /* Applies privileges to the entity group. */
  void ApplyPrivilege(const TStrand<>& privileges) {
    // for (SI4 i = 0; i < base.getNumAccounts (); i++)
    //    accounts[i].Role ().ApplyPrivileges (new_privileges);
  }

  /* Returns true if this list of entities contains the given CH1. */
  virtual SI4 Search(const TStrand<>& string) { return 0; }

  template<typename Printer>
  Printer& Print (Printer& o) { 
    out << "Group: " << name_ << " ";
    return out;
  }

 private:
  TStrand<> name_,             //< The name of the entity group.
       privileges_;            //< A string of privileges the group has.
  TArray<TEntity*>* entities_;  //< A TArray if Entity pointers.
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_ENTITYGROUP
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
