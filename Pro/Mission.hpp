// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKI_PRO_PROJECT
#define KABUKI_PRO_PROJECT
#include <_Config.h>
#if SEAM >= CRABSTK_PRO_CORE
#include "Schedule.hpp"
#include "Task.hpp"
namespace _ {

/* . */
class Mission : public Issue {
 public:
  
  /* Constructs a Mission from the given parameters. */
  Mission(CHA* key, CHA* readme)
    : key_ (key), readme_ (readme), task_ (nullptr) {}

  /* Destructor. */
  ~Issue() {}
  
  /* Gets the Issue Id number. */
  ISW Id () { return id_; }
  
  /* Prints the Shopping list to the console.*/
  template<typename Printer>
  Printer& Print(Printer& o) {
    return o << "\nIssue #" << id_;
  }

  /* ASCIICrabs operations. */
  virtual const Op* Star (CHN index, Expr* expr) {
    return nullptr;
  }

 private:
  ISW id_;   //< The unique identification number.
};

}  //< namespace _
#endif
#endif
