// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_WHO_ROLE
#define KABUKITOOLKIT_WHO_ROLE
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_CORE
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
