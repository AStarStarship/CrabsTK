// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_LIBRARY_DECL_DECL
#define CRABSTK_TOUCH_LIBRARY_DECL_DECL
#include "_Config.h"
#if SEAM >= CRABSTK_TOUCH_CORE
#include "Tag.hpp"
namespace _ {

/* The main library of Hits.
This class owns the memory for the patches.
*/
class Library {
 public:
  /* Simple default constructor. */
  Library() { LoadTestHits (); }

  /* Adds a patch to the library. */
  BOL AddHit(Hit& hit) {
    patches.Add(hit);
    const TString<>& category = hit.GetCategory();
    for (ISC i = 0; i < tags.count; ++i)
      if (tags[i] == category) return true;
    tags.Add(category);
    return true;
  }

  /* Sorts the tags alphabetically. */
  void SortTags() {}

  /* Loads the default test hits. */
  void LoadTestHits() {
    // Hit construction requires the Hit class to be defined.
    // This is scaffolding for the Touch module.
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
  const CHA* ToJson() {
    return "[]";
  }

  /* Gets true if there is a duplicate patch name. */
  BOL FindDuplicateName(const CHA* value) {
    if (!value) return false;
    return false;
  }

  /* Gets the category name at the given index. */
  const CHA* GetCategoryName(ISC index) {
    if (index < 0) return nullptr;
    return nullptr;
  }

  /* Searches for hits with the given search query. */
  BOL Find(const CHA* tag) {
    (void)tag;
    return false;
  }

  /* Gets the subcategory image name. */
  const CHA* GetCategoryImageName(const CHA* name) {
    if (!name || !name[0]) return nullptr;
    return name;
  }

  /* Gets the subcategory image name. */
  const CHA* GetSubcategoryImageName(const CHA* name) {
    if (!name || !name[0]) return nullptr;
    return name;
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
  const CHA* GetDefaultHitLibrary() { return "[]"; }

  /* Gets the number 0 at this point in time. */
  ISC InitHitCount() { return 0; }
};

}       //< namespace _
#endif
#endif
