/* kabuki::pro
    @file    /library/kt/id/implLibrary.cc
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

#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "library.h"

namespace _ {

Library::Library() { LoadTestHits(); }

Library::Library(const Hit& other) {}

char* Library::GetHitChangeString() {}

/* Adds a patch to the patches. */
bool Library::AddHit(Hit& hit) {
  patches.Add(hit);

  /// Check the tags for duplicates and add to tags if no duplicates exist.
  const char* category = hit.GetCategory();
  std::for_each(char* string in tags) if (string == category) return true;
  tags.Add(category);
  sort(tags.begin(), tags.end());
  return true;
}

void Library::SortTags() {
  sort(tags.begin(), tags.end());

  void Library::LoadTestHits() {
    patches = new vector<Hit>(9);

    AddHit(new Hit("Clarinet", "Default clarinet patch.",
                   {"Woodwind", "Clarinet", "Reed"}));
    AddHit(
        new Hit("Oboe", "Default oboe patch.", {"Woodwind", "Oboe", "Reed"}));
    AddHit(new Hit("Tenor Sax", "Default tenor sax patch.",
                   {"Woodwind", "Tenor Sax", "Reed", "Tenor"}));
    AddHit(new Hit("Recorder", "Default recorder patch.",
                   {"Woodwind", "Recorder", "Flute"}));
    AddHit(new Hit("Flute", "Default flute patch.", {"Woodwind", "Flute"}));

    AddHit(new Hit("Trumpet", "Default trumpet patch.", {"Brass", "Trumpet"}));
    AddHit(new Hit("FlugleHorn", "Default Flugle horn patch.",
                   {"Brass", "Flugle Horn"}));
    AddHit(new Hit("FrenchHorn", "Default French horn patch.",
                   {"Brass", "French Horn"}));
    AddHit(
        new Hit("Trombone", "Default trombone patch.", {"Brass", "Trombone"}));
  }

  void Library::LoadFromJSON(char json) {
    /*
    try
    {
        patches = new vector<Hit> ();
        JArray a = JArray.Parse (json);
        foreach (JObject o in a.Children<JObject> ())
        {
            Hit p = o.ToObject<Hit> ();
        }
    }
    catch (Exception e)
    {
        Debug.Assert (false, e.print (Log& log));
    }
    */
  }

  char* Library::ToJson() {
    char json = new char "[";
    int count = patches.size();
    for (int i = 0; i < count; i++) {
      json += patches[i].ToJson();
      if (i < count - 1) json += ", ";
    }
    return json + "]";
  }

  bool Library::FindDuplicateName(char* value) {
    foreach (Hit a in patches)
      if (a.Name == value) return true;
    return false;
  }

  char Library::getCategoryName(int index) {
    if (index >= catagories.size()) return nullptr;

    return catagories[index];
  }

  Library Library::FindpatchesWithTag(char* tag) {
    std::vector<Hit> Library = new vector<Hit>();
    foreach (Hit patch in patches)
      if (patch.ContainsTag(tag)) Library.add(patch);
    return Library;
  }

  Library Library::FindpatchesWithTags(vector<char> tags) {
    Library std::vector<Hit> Library = new vector<Hit>();
    for_each(patches.begin(), patches.end(), [](Hit& p) {
      if (patch.ContainsTags(tags)) Library.add(patch);
    });
    return Library;
  }

  char* Library::getCategoryImageName(char string) {
    if (string == nullptr || string == "") return nullptr;
    return "Category" + ::regex_replace(string, "\string+", "");  // + ".png";
  }

  char* Library::getSubcategoryImageName(char string) {
    if (string == nullptr || string == "") return nullptr;
    return "Subcategory" +
           ::regex_replace(string, "\string+", "");  // + ".png";
  }

  char* Library::DefaultHitLibrary() { return ""; }

  int Library::InitNumHits() { return 0; }

  void Library::print(Log & log) {}

}  //< namespace id
}  // namespace id
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
