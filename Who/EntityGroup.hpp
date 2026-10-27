// Copyright AStarship <https://astarship.net>.
#pragma once
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#ifndef CRABSTK_WHO_ENTITYGROUP_DECL_DECL
#define CRABSTK_WHO_ENTITYGROUP_DECL_DECL
#include "Entity.hpp"
namespace _ {

/* A group of entities. */
class TEntityGroup {
 public:
  /* A group of entities such as people or businesses. */
  TEntityGroup(const CHR* name)
    : name_(""),
      privileges_(""),
      entities_(nullptr) {
    if (name) SetName(name);
  }

  /* Gets the name of the entity group. */
  const CHR* GetName() const { return name_; }

  /* Sets the name of the entity group. */
  void SetName(const CHR* AString) {
    if (!AString) AString = "";
    const IUB NameMax = 63;
    IUB i = 0;
    while (i < NameMax && AString[i]) name_[i] = AString[i++];
    name_[i] = '\0';
  }

  /* Applies privileges to the entity group. */
  void ApplyPrivilege(const CHR* privileges) {
    // for (ISC i = 0; i < base.getNumAccounts (); i++)
    //    accounts[i].Role ().ApplyPrivileges (new_privileges);
    (void)privileges;
  }

  /* Returns true if this list of entities contains the given CHA. */
  virtual ISC Search(const CHR* AString) {
    (void)AString;
    return 0;
  }

 private:
  CHA name_[64];            //< The name of the entity group.
  CHA privileges_[64];      //< A string of privileges the group has.
  TEntity** entities_;      //< Array of Entity pointers.
};

}  //< namespace _
#endif
#endif
