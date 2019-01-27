/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_registrar.h
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
#ifndef KABUKI_TOOLKIT_WHO_IDSERVER
#define KABUKI_TOOLKIT_WHO_IDSERVER

namespace _ {

/* Manages user keys and assigns unique ids to events. */
class TRegistrar {
 public:
  enum {
    kMaxKeyLength = 32,  //< The max key length.
  };

  /* Creates an empty server. */
  TRegistrar() : event_count_ (0) {}

  /* Destructor. */
  ~TRegistrar() {}

  /* Registers an unique key with the server without cloning the name. */
  SI4 RegisterKey(TStrand<>& key) {
    return RegisterKey (StrandClone (key));
  }

  /* Registers an unique key the server and clones the name CH1. */
  SI4 RegisterKey(const TStrand<>& key) {
    if (StrandLength (key) > kMaxKeyLength) return -1;
    return ids_.Push (StrandClone (key));
  }

  /* Searches for the given key.
  @return Returns a pointer to a key CH1 upon success and null
  if the key does not exist. */
  SI4 Find(const TStrand<>& key) {
    // @todo Replace me with hash table!
    for (SI4 i = 0; i < ids_.GetCount (); ++i)
      if (!strcmp (key, ids_.Element (i))) return i;
    return -1;
  }
  

  /* Gets the number of events. */
  SI4 NumEvents() { return event_count_; }

  /* Registers a new Event UID by incrementing and returning the num_events. */
  SI4 RegisterEvent();

  /* Prints this object to the console. */
   Printer& Print (Printer& o) {
     o << "\nId Server:"
       << "\n  EventCount: " << event_count_
       << "\n  Registered Handles: \n";

     for (SI4 i = 0; i < ids_.GetCount (); ++i)
       o << "| " << i <<

 private:
  UID event_count_;        //< The global number of event created.
  TArray<TStrand<>&> ids_;  //< A the id keys.
};                          //< Array class
}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_IDSERVER
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
