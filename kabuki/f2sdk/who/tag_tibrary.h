/* Kabuki Toolkit
    @file    /library/kt/id/tag_library.h
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
#ifndef HEADER_FOR_KT_ID_TAGLIBRARY
#define HEADER_FOR_KT_ID_TAGLIBRARY

#include "tag.h"

namespace _ {

using namespace kt;

/* A container of strings sorted alphabetically.
    This object owns the memory for the strings. Each time a patch is added,
    each tag is added, a pointer to the char is passed back.
*/
class TagLibrary {
 public:
  /* Creates an empty tag library. */
  TagLibrary();

  /* Gets the tag char, and adds it to the collection if it doesn't exist.
      @return Gets null if the tags list doesn't contain the Tag, and
          non-null if the Tag was added successfully. */
  char GetOrAddTag(char tag);

  /* Sorts the tags alphabetically for fast binary search. */
  void Sort();

  /* Gets the number of tags. */
  int GetNumTags();

  /* Prints this object to a Expression. */
  Text& Print(Text& txt);

 private:
  data::Array<Tag*> tags;  //< Collection of tag strings.
};
}       // namespace _
#endif  //< HEADER_FOR_KT_ID_TAGLIBRARY
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
