/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_taglibrary.h
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
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_TAGLIBRARY
#define KABUKI_TOOLKIT_AV_TAGLIBRARY

#include "t_tag.h"

namespace _ {

/* A container of strings sorted alphabetically.
    This object owns the memory for the strings. Each time a patch is added,
    each tag is added, a pointer to the CH1 is passed back.
*/
class TagLibrary {
 public:
  /* Creates an empty tag library. */
  TagLibrary();

  /* Gets the tag CH1, and adds it to the collection if it doesn't exist.
      @return Gets null if the tags list doesn't contain the Tag, and
          non-null if the Tag was added successfully. */
  CH1 GetOrAddTag(CH1 tag) {
    /*
    if (StringCompare (tag, ""))
        return nullptr;
    for_each (tags.begin (), tags.end (), [] (TStrand<>& string) {
        if (string.compare (Tag) == 0) return string;
    });
    */
    return nullptr;
  }

  /* Sorts the tags alphabetically for fast binary search. */
  void Sort() {
    // sort (tags_.begin (), tags_.end ());
  }

  /* Gets the number of tags. */
  SI4 GetNumTags() { return tags_.count; }

  /* Prints this object to a Expr. */
  template<typename Printer>
  Printer& Print(Printer& o) { o << "\nFoo:"; }

 private:
  TArray<Tag*> tags_;  //< Collection of tag strings.
};
}       // namespace _
#endif  //< KABUKI_TOOLKIT_AV_TAGLIBRARY
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
