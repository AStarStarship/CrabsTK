// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#ifndef CRABSTK_WHO_ENTITYGROUP_DECL_DECL
#define CRABSTK_WHO_ENTITYGROUP_DECL_DECL
#include "Entity.hpp"
namespace _ {

/* A group of entities. */
class TEntityGroup {
 public:
  /* A group of entities such as people or businesses. */
  TEntityGroup(const TString<>& name)
    : name_ (StringClone (name == nullptr ? "" : name)) {}

  /* Gets the name of the entity group. */
  const TString<>& GetName() { return name_; }

  /* Sets the name of the entity group. */
  void SetName(const TString<>& AString) {
    name_ = StringClone (AString);
  }

  /* Applies privileges to the entity group. */
  void ApplyPrivilege(const TString<>& privileges) {
    // for (ISC i = 0; i < base.getNumAccounts (); i++)
    //    accounts[i].Role ().ApplyPrivileges (new_privileges);
  }

  /* Returns true if this list of entities contains the given CHA. */
  virtual ISC Search(const TString<>& AString) { return 0; }

  template<typename Printer>
  Printer& Print (Printer& o) { 
    out << "Group: " << name_ << " ";
    return out;
  }

 private:
  TString<> name_,             //< The name of the entity group.
       privileges_;            //< A AString of privileges the group has.
  TArray<TEntity*>* entities_; //< A TArray if Entity pointers.
};

}  //< namespace _
#endif
#endif
