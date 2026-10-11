// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_ROLE_DECL_DECL
#define CRABSTK_WHO_ROLE_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
namespace _ {

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
}       //< namespace _
#endif
#endif
