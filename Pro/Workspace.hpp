// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_PRO_WORKSPACE
#define KABUKITOOLKIT_PRO_WORKSPACE
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_PRO_WORKSPACE
#include "Project.hpp"
namespace _ {

/* A workspace folder and settings. */
class LIB_MEMBER Workspace {
 public:
  /* Constructor. */
  Workspace() {}

  /* Clones the other object. */
  Project(const Project& p);

  /* Destructor. */
  virtual ~Workspace();

  /* Adds the given controller to the workspace. */
  void Add(const Project& p);

  /* Gets the number of widgets in the project. */
  ISC GetNumWidgets();

  template <typename Printer>
  Printer& Print(Printer& o) {
    o << "\nWorkspace:";
  }

 private:
  _::TArray<TProject> projects_;  //< Workspace Projects.
};

}  //< namespace _
#endif
#endif
