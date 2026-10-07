// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKI_PRO_ISSUE
#define KABUKI_PRO_ISSUE
#include <_Config.h>
#if SEAM >= CRABSTK_PRO_1
#include "Schedule.hpp"
#include "Task.hpp"
namespace _ {

/* And 'issue ticket'. */
class Issue : public Operand {
 public:
  /* Default constructor initializes list with the given or left key. */
  Issue(const CHA* key = "Unnamed", const CHA* readme = "")
      : key_(key == nullptr ? StringClone("") : key),
        readme_(readme == nullptr ? StringClone("") : readme),
        task_(nullptr) {}

  /* Constructor initializes with stolen key and readme. */
  Issue(CHA* key, CHA* readme) : key_(key), readme_(readme), task_(nullptr) {}

  /* Destructor. */
  ~Issue() {}

  /* Gets the Issue Id number. */
  ISW Id() { return id_; }

  /* Prints the Shopping list to the console.*/
  template <typename Printer>
  Printer& Print(Printer& o) {
    return o << "\nIssue #" << id_;
  }

  /* ASCIICrabs operations. */
  virtual const Op* Star(CHN index, Expr* expr) { return nullptr; }

 private:
  ISW id_;  //< The unique identification number.
};

}  // namespace _
#endif
#endif
