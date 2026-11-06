// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWhoRoleHPP
#define _CrabsTKWhoRoleHPP
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
namespace CTK {
namespace Who {

/* A role that an entity plays in an organization.
Examples of a role are:
1. Owner
2. Executive
3. Manager
4. Employee
5. Volunteer
*/
struct LIB_MEMBER TRole {
  IUD GetRole() = 0;
};
}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoRoleHPP
