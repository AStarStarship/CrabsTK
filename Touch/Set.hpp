// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKTouchSetHPP
#define _CrabsTKTouchSetHPP
#include <_Config.h>
#if _Seam >= _CrabsTKTouchCore
#include "../Pro/project.hpp"
namespace CTK {
namespace Touch {

/* A collection of shared objects, . */
class Set {
 public:
  /* Constructor. */
  Set(const CHA* name, IUD uid) : name_(name), uid_(uid) {}

  /* Copy constructor copies the other object. */
  Set(const Set& s) {}

  /* Destructor. */
  virtual ~Set() {}

  /* Adds a new Project to the set. */
  void Add(const Project& workspace) { workspaces_.Add(workspace); }

  /* Gets the number of projects in the set. */
  ISC WorkspaceCount() { return workspaces_.GetCount(); }

  /* Prints this object to the terminal. */
  template <typename Printer>
  Printer& Print(Printer& o) {
    o << "\nSet:" << name << " IUD:" << uid_;
  }

 private:
  TString<> name_;                //< Set name.
  IUD uid_;                       //< IUD of this set.
  AArray<Workspace> workspaces_;  //< Array of Project(s).
};
}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKTouchCore
#endif  //< _CrabsTKTouchSetHPP
