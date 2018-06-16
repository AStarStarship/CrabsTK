/** kabuki::pro
    @file    $kabuki-toolkit/library/kt/id/impl/hit_list.cc
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
#include "hit_list.h"

namespace _ {

HitList::HitList() { LoadTestHits(); }

bool HitList::AddHit(Hit& p) {
  patches.push_back(p);

  /// Check the tags for duplicates and add to tags if no duplicates exist.
  /*
  const char* tagCategory = p.GetCategory ();
  for_each (patches.begin (), patches.end (), [](const char* string) { if
  (string == tagCategory) return true; } tags.push_back (tagCategory); sort
  (tags.begin (), tags.end ()); return true;
  */
  return false;
}

void HitList::SortTags() {
  // sort (tags.begin (), tags.end ());
}

void HitList::LoadFromJSON(const char* json) {
  /*
  try
  {
      patches = new vector<Hit> ();
      JArray a = JArray.Parse (json);
      foreach (JObject o in a.Children<JObject> ())
      {
          Hit p = o.toObject<Hit> ();
      }
  }
  catch (Exception e)
  {
      assert (false, e.print (Log& log));
  }
   */
}

const char* HitList::ToJson() {
  /*
  const char* json = "[";
  int count = patches.size ();
  for (int i = 0; i < count; i++)
  {
      json += patches[i].ToJson ();
      if (i < count - 1) json += ", ";
  }
  json += "]";
  return json_;
  */
  return 0;
}

bool HitList::FindDuplicateName(const char* value) {
  /*
  for_each (patches.begin (), patches.end (), [](Hit& hit)
  {
  if (a.GetName () == value)
      return true;
  });
  */
  return false;
}

const char* HitList::GetCategoryName(int index) {
  if (index >= catagories.size()) return nullptr;

  return catagories[index] _;
}

HitList* HitList::FindTag(const char* tag) {
  /*
  data::Array<Hit> patchList;

  for_each (Hit patch in patches)
  if (patch.ContainsTag (tag))
      patchList.add (patch);
  return patchList;
  */
  return 0;
}

HitList* HitList::FindTags(vector<const char*> tags) {
  /*
  data::Array<Hit> patchList;
  for_each (patches.begin (), patches.end (), [] (Hit& p)
  {
      if (patch.ContainsTags (tags)) patchList.add (patch);
  });
  return patchList;
  */
  return 0;
}

const char* HitList::GetCategoryImageName(const char* string) {
  if (string == "") return nullptr;
  regex regularExpression("\string+");
  const char* result =
      "Category" + regex_replace(string, regularExpression, "");  // + ".png";
  return result_;
}

const char* HitList::GetSubcategoryImageName(const char* string) {
  if (string == "") return nullptr;
  regex regularExpression("\string+");
  const char* result = "Subcategory" + regex_replace(string, regularExpression,
                                                     "");  // + ".png";
  return result_;
}

void HitList::Print(_::Log& log) {}

void HitList::LoadTestHits() {
  patches.Clear();
  /*
  AddHit (new Hit ("Clarinet", "Default clarinet patch.", { "Woodwind",
  "Clarinet", "Reed" })); AddHit (new Hit ("Oboe", "Default oboe patch.", {
  "Woodwind", "Oboe", "Reed" })); AddHit (new Hit ("Tenor Sax", "Default tenor
  sax patch.", { "Woodwind", "Tenor Sax", "Reed", "Tenor" })); AddHit (new Hit
  ("Recorder", "Default recorder patch.", { "Woodwind", "Recorder", "Flute" }));
  AddHit (new Hit ("Flute", "Default flute patch.", { "Woodwind", "Flute" }));

  AddHit (new Hit ("Trumpet", "Default trumpet patch.", { "Brass", "Trumpet"
  })); AddHit (new Hit ("FlugleHorn","Default flugle horn patch.", { "Brass",
  "Flugle Horn" })); AddHit (new Hit ("FrenchHorn", "Default French horn
  patch.", { "Brass", "French Horn" })); AddHit (new Hit ("Trombone", "Default
  trombone patch.", { "Brass", "Trombone" }));
  */
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
