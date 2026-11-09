// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKProWorkspaceHH
#define _CrabsTKProWorkspaceHH
#include <_Config.h>
#if _Seam >= _CrabsTKProWorkspace
#include "Project.hpp"
namespace CTK {
namespace Pro {

/* A workspace folder and settings. */
class Workspace {
 public:
  /* Constructor. */
  Workspace() {}

  /* Clones the other object. */
  Workspace(const Workspace& other);

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

}  // namespace Pro
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKProWorkspace
#endif  //< _CrabsTKProWorkspaceHH
