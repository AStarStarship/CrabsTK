/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/music/t_library.h
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
#ifndef KABUKI_TOOLKIT_WHO_LIBRARY
#define KABUKI_TOOLKIT_WHO_LIBRARY

#include "t_tag.h"

namespace _ {

/* The main library of Hits.
This class owns the memory for the patches.
*/
class Library {
 public:
  /* Simple default constructor. */
  Library() { LoadTestHits (); }

  /* Adds a patch to the Hits. */
  BOL AddHit(Hit& p) {
    patches.Add (hit);

    /// Check the tags for duplicates and add to tags if no duplicates exist.
    const TStrand<>& category = hit.GetCategory ();
    std::for_each (TStrand<>& string in tags) if (string == category) return true;
    tags.Add (category);
    sort (tags.begin (), tags.end ());
    return true;
  }

  /* Sorts the tags alphabetically. */
  void SortTags() {
    sort (tags.begin (), tags.end ());

    void Library::LoadTestHits () {
      patches = new TArray<Hit> (9);

      AddHit (new Hit ("Clarinet", "Default clarinet patch.",
        { "Woodwind", "Clarinet", "Reed" }));
      AddHit (
        new Hit ("Oboe", "Default oboe patch.", { "Woodwind", "Oboe", "Reed" }));
      AddHit (new Hit ("Tenor Sax", "Default tenor sax patch.",
        { "Woodwind", "Tenor Sax", "Reed", "Tenor" }));
      AddHit (new Hit ("Recorder", "Default recorder patch.",
        { "Woodwind", "Recorder", "Flute" }));
      AddHit (new Hit ("Flute", "Default flute patch.", { "Woodwind", "Flute" }));

      AddHit (new Hit ("Trumpet", "Default trumpet patch.", { "Brass", "Trumpet" }));
      AddHit (new Hit ("FlugleHorn", "Default Flugle horn patch.",
        { "Brass", "Flugle Horn" }));
      AddHit (new Hit ("FrenchHorn", "Default French horn patch.",
        { "Brass", "French Horn" }));
      AddHit (
        new Hit ("Trombone", "Default trombone patch.", { "Brass", "Trombone" }));
  }

  /* Loads a patch from a JSON CH1. */
  void LoadFromJSON(STR json_string) {
    /*
    try
    {
      patches = new TArray<Hit> ();
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

  /* Converts a patch to a JSON CH1. */
  CH1 ToJson() {
    CH1 json = new CH "[";
    SI4 count = patches.size ();
    for (SI4 i = 0; i < count; i++) {
      json += patches[i].ToJson ();
      if (i < count - 1) json += ", ";
    }
    return json + "]";
  }

  /* Gets true if their is a duplicate patch name. */
  BOL FindDuplicateName(CH1 value) {
    foreach (Hit a in patches)
      if (a.Name == value) return true;
    return false;
  }

  /* Gets the category name at the given index. */
  CH1 GetCategoryName(SI4 index) {
    if (index >= catagories.size ()) return nullptr;

    return catagories[index];
  }

  /* Searches for hits with the given search query. */
  Library Find(CH1 tag) {
    Library TArray<Hit> Library = new TArray<Hit> ();
    for_each (patches.begin (), patches.end (), [](Hit& p) {
      if (patch.ContainsTags (tags)) Library.add (patch);
    });
    return Library;
  }

  Library Find(TLoomy<>& tags) {
    TArray<Hit> Library = new TArray<Hit> ();
    foreach (Hit patch in patches)
      if (patch.ContainsTag (tag)) Library.add (patch);
    return Library;
  }

  /* Gets the subcategory image name. */
  CH1 GetCategoryImageName(CH1 string) {
    if (!string || string == "") return nullptr;
    return "Category" + ::regex_replace (string, "\string+", "");  // + ".png";
  }

  /* Gets the subcategory image name. */
  CH1 GetSubcategoryImageName(CH1 string) {
    if (string == nullptr || string == "") return nullptr;
    return "Subcategory" +
      ::regex_replace (string, "\string+", "");  // + ".png";
  }

  /* Prints this object to a Expr. */
  template<typename Printer>
  Printer& Print (Printer& o) {
  
  }
  // void injectTestData ();

 private:
  uint64_t num_uids;                   //< Total number of UIDs.
  std::map<const TStrand<>&, Hit> patches;  //< List of patches.
  TArray<CH1> catagories,        //< The list of categories.
      tags;                            //< List of tag strings.

  /* Gets the default library in JSON. */
  CH1& GetDefaultHitLibrary() { return ""; }

  /* Gets the number 0 at this point in time. */
  SI4 InitNumHits() { return 0; }
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_LIBRARY
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
