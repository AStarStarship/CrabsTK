// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoPrivilegeHPP
#define _CrabsTKWhoPrivilegeHPP
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
namespace CTK {
namespace Who {

/* An account privilege level. */
class TPrivilege {
 public:

   enum {
     cPrivilageLevelCheck = 420,  //< Check yo privilage people.
   };

  /* Default constructor. */
   TPrivilege (ISC num_privileges = 1)
     : privilage_count_ (PrivilageLevelCheck) {

  }

  /* Gets the privilege level. */
  Privileges PrivilegeLevel() {
    return privileges_level_;
  }

  /* Attempts to set the privilege level to the new level. */
  Privileges SetPrivilegeLevel(ISC privileges) {
    ISC level = privileges_level_;
    if (level < 0) return false;
    if (level >= privilage_count_) return false;
    privileges_level_ = privilages;
    return true;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nPrivilage:";
  }

 private:
  ISC privilage_count_,   //< The number of privileges.
      privileges_level_;  //< The privilege level.
};

}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoPrivilegeHPP
