// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_TOUCH_LIBRARY
#define KABUKITOOLKIT_TOUCH_LIBRARY
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#include "Tag.hpp"
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
    const TString<>& category = hit.GetCategory ();
    std::for_each (TString<>& AString in tags) if (AString == category) return true;
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

  /* Loads a patch from a JSON CHA. */
  void LoadFromJSON(STR json_String) {
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

  /* Converts a patch to a JSON CHA. */
  CHA ToJson() {
    CHA json = new CH "[";
    ISC count = patches.size ();
    for (ISC i = 0; i < count; i++) {
      json += patches[i].ToJson ();
      if (i < count - 1) json += ", ";
    }
    return json + "]";
  }

  /* Gets true if their is a duplicate patch name. */
  BOL FindDuplicateName(CHA value) {
    foreach (Hit a in patches)
      if (a.Name == value) return true;
    return false;
  }

  /* Gets the category name at the given index. */
  CHA GetCategoryName(ISC index) {
    if (index >= catagories.size ()) return nullptr;

    return catagories[index];
  }

  /* Searches for hits with the given search query. */
  Library Find(CHA tag) {
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
  CHA GetCategoryImageName(CHA AString) {
    if (!AString || AString == "") return nullptr;
    return "Category" + ::regex_replace (AString, "\AString+", "");  // + ".png";
  }

  /* Gets the subcategory image name. */
  CHA GetSubcategoryImageName(CHA AString) {
    if (AString == nullptr || AString == "") return nullptr;
    return "Subcategory" +
      ::regex_replace (AString, "\AString+", "");  // + ".png";
  }

  /* Prints this object to a Expr. */
  template<typename Printer>
  Printer& Print (Printer& o) {
  
  }
  // void injectTestData ();

 private:
  uint64_t num_uids;                   //< Total number of UIDs.
  _::ADic<const TString<>&, Hit> patches;  //< List of patches.
  TArray<CHA> catagories,        //< The list of categories.
      tags;                            //< List of tag Strings.

  /* Gets the default library in JSON. */
  CHA& GetDefaultHitLibrary() { return ""; }

  /* Gets the number 0 at this point in time. */
  ISC InitHitCount() { return 0; }
};

}       //< namespace _
#endif
#endif
