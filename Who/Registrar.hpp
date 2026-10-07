// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#ifndef CRABSTK_WHO_IDSERVER_DECL_DECL
#define CRABSTK_WHO_IDSERVER_DECL_DECL

namespace _ {

/* Manages user keys and assigns unique ids to events. */
class TRegistrar {
 public:
  enum {
    cMaxKeyLength = 32,  //< The max key length.
  };

  /* Creates an empty server. */
  TRegistrar() : event_count_ (0) {}

  /* Destructor. */
  ~TRegistrar() {}

  /* Registers an unique key with the server without cloning the name. */
  ISC RegisterKey(TString<>& key) {
    return RegisterKey (StringClone (key));
  }

  /* Registers an unique key the server and clones the name CHA. */
  ISC RegisterKey(const TString<>& key) {
    if (StringLength (key) > kMaxKeyLength) return -1;
    return ids_.Push (StringClone (key));
  }

  /* Searches for the given key.
  @return Returns a pointer to a key CHA upon success and null
  if the key does not exist. */
  ISC Find(const TString<>& key) {
    // @todo Replace me with hash table!
    for (ISC i = 0; i < ids_.GetCount (); ++i)
      if (!strcmp (key, ids_.Element (i))) return i;
    return -1;
  }
  
  /* Gets the number of events. */
  ISC NumEvents() { return event_count_; }

  /* Registers a new Event IUD by incrementing and returning the num_events. */
  ISC RegisterEvent();

  /* Prints this object to the console. */
   Printer& Print (Printer& o) {
     o << "\nId Server:"
       << "\n  EventCount: " << event_count_
       << "\n  Registered Handles: \n";

     for (ISC i = 0; i < ids_.GetCount (); ++i)
       o << "| " << i <<

 private:
  IUD event_count_;        //< The global number of event created.
  TArray<TString<>&> ids_;  //< A the id keys.
};                          //< Array class
}       //< namespace _
#endif
#endif
