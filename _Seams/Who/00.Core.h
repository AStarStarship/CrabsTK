/* CRABSTK
@file    /_Seams/Who/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_WHO_CORE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_CORE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ACCOUNT
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ACCOUNT
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ADDRESS
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ADDRESS
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ADDRESSEMAIL
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ADDRESSEMAIL
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_AUTHENTICATOR
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_AUTHENTICATOR
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ENTITY
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ENTITY
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ENTITYGROUP
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ENTITYGROUP
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ENTITYLIST
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ENTITYLIST
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_HANDLE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_HANDLE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_NAME
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_NAME
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ORGANIZATION
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ORGANIZATION
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_PASSWORD
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_PASSWORD
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_PERSON
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_PERSON
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_PRIVILEGE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_PRIVILEGE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_PROFILE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_PROFILE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_REGISTRAR
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_REGISTRAR
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_RELATIONSHIP
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_RELATIONSHIP
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ROLE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ROLE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_ROSTER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_ROSTER
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_SYMBOL
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_SYMBOL
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_UIDASSIGNER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_UIDASSIGNER
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_USER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_USER
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_WHO_USERLIST
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_WHO_USERLIST
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Who {
namespace Test {

inline const CHA* WhoCore(const CHA* args) {
#if SEAM >= CRABSTK_WHO_CORE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who core..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoAccount(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ACCOUNT
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who account..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoAddress(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ADDRESS
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who address..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoAddressemail(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ADDRESSEMAIL
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who address email..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoAuthenticator(const CHA* args) {
#if SEAM >= CRABSTK_WHO_AUTHENTICATOR
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who authenticator..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoEntity(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ENTITY
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who entity..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoEntitygroup(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ENTITYGROUP
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who entity group..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoEntitylist(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ENTITYLIST
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who entity list..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoHandle(const CHA* args) {
#if SEAM >= CRABSTK_WHO_HANDLE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who handle..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoName(const CHA* args) {
#if SEAM >= CRABSTK_WHO_NAME
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who name..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoOrganization(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ORGANIZATION
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who organization..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoPassword(const CHA* args) {
#if SEAM >= CRABSTK_WHO_PASSWORD
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who password..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoPerson(const CHA* args) {
#if SEAM >= CRABSTK_WHO_PERSON
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who person..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoPrivilege(const CHA* args) {
#if SEAM >= CRABSTK_WHO_PRIVILEGE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who privilege..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoProfile(const CHA* args) {
#if SEAM >= CRABSTK_WHO_PROFILE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who profile..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoRegistrar(const CHA* args) {
#if SEAM >= CRABSTK_WHO_REGISTRAR
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who registrar..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoRelationship(const CHA* args) {
#if SEAM >= CRABSTK_WHO_RELATIONSHIP
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who relationship..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoRole(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ROLE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who role..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoRoster(const CHA* args) {
#if SEAM >= CRABSTK_WHO_ROSTER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who roster..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoSymbol(const CHA* args) {
#if SEAM >= CRABSTK_WHO_SYMBOL
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who symbol..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoUidassigner(const CHA* args) {
#if SEAM >= CRABSTK_WHO_UIDASSIGNER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who UID assigner..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoUser(const CHA* args) {
#if SEAM >= CRABSTK_WHO_USER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who user..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* WhoUserlist(const CHA* args) {
#if SEAM >= CRABSTK_WHO_USERLIST
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Who user list..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Who
}  //< namespace CT
