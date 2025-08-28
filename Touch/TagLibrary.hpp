// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_TOUCH_TAGLIBRARY
#define KABUKITOOLKIT_TOUCH_TAGLIBRARY
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#include "Tag.hpp"
namespace _ {

/* A container of Strings sorted alphabetically.
This object owns the memory for the Strings. Each time a patch is added,
each tag is added, a pointer to the CHA is passed back.
*/
class TagLibrary {
 public:
  /* Creates an empty tag library. */
  TagLibrary();

  /* Gets the tag CHA, and adds it to the collection if it doesn't exist.
  @return Gets null if the tags list doesn't contain the Tag, and
  non-null if the Tag was added successfully. */
  CHA GetOrAddTag(CHA tag) {
    /*
    if (StringCompare (tag, ""))
        return nullptr;
    for_each (tags.begin (), tags.end (), [] (TString<>& AString) {
        if (AString.compare (Tag) == 0) return AString;
    });
    */
    return nullptr;
  }

  /* Sorts the tags alphabetically for fast binary search. */
  void Sort() {
    // sort (tags_.begin (), tags_.end ());
  }

  /* Gets the number of tags. */
  ISC GetNumTags() { return tags_.count; }

  /* Prints this object to a Expr. */
  template<typename Printer>
  Printer& Print(Printer& o) { o << "\nFoo:"; }

 private:
  TArray<Tag*> tags_;  //< Collection of tag Strings.
};
}       //< namespace _
#endif
#endif
