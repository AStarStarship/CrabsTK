/** Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/hit_list.h
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
#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_HITLIST
#ifndef HEADER_FOR_KT_ID_HITLIST

#include "hit.h"

namespace _ {

/** A collection of Hit(string).
 */
class KABUKI HitList {
 public:
  /** Simple default constructor. */
  HitList();

  /** Adds a patch to the Hits. */
  bool AddHit(Hit& p);

  /** Sorts the tags alphabetically. */
  void SortTags();

  /** Loads a patch from a JSON char. */
  void LoadFromJSON(const char* json);

  /** Converts a patch to a JSON char. */
  const char* ToJson();

  /** Gets true if their is a duplicate patch name. */
  bool FindDuplicateName(const char* value);

  /** Gets the category name at the given index. */
  const char* GetCategoryName(int index);

  /** Gets a HitList of the given tag. */
  HitList* FindTag(const char* tag);

  /** Gets a HitList of the given tags. */
  HitList* FindTags(data::Array<const char*>& tags);

  /** Gets the category image name. */
  const char* GetCategoryImageName(const char* string);

  /** Gets the subcategory image name. */
  const char* GetSubcategoryImageName(const char* string);

  /** Prints this object to the log. */
  void Print(_::Log& log);

 private:
  uint32_t uid = 0;

  data::Array<Hit&> patches;            //< The list of patches.
  data::Array<const char*> catagories,  //< The list of catagories.
      tags;                             //< List of tag strings.

  void loadTestHits();
  /** Loads up some test data: delete me and make unit test! */
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_HITLIST
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
