/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_set.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_SET
#define KABUKI_TOOLKIT_AV_SET

#include "../pro/t_project.h"

namespace _ {

/* A collection of shared objects, . */
class SDK Set {
 public:
  /* Constructor. */
   Set (const CH1* name, UID uid) 
   : name_ (name),
     uid_ (uid) {}

  /* Copy constructor copies the other object. */
  Set(const Set& s) {}

  /* Destructor. */
  virtual ~Set() {}

  /* Adds a new Project to the set. */
  void Add(const Project& workspace) {
    workspaces_.Add (workspace);
  }

  /* Gets the number of projects in the set. */
  SI4 WorkspaceCount() { return workspaces_.GetCount (); }

  /* Prints this object to the terminal. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nSet:" << name << " UID:" << uid_;
  }

 private:
   TStrand<> name_;           //< Set name.
   UID uid_;                  //< UID of this set. 
   TArray<Workspace> workspaces_; //< Array of Project(s).
};                            //< class Set
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_SET
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
