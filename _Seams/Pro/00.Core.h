/* CRABSTK
@file    /_Seams/Pro/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_PRO_CORE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_CORE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_GIT
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_GIT
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_ISSUE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_ISSUE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_MISSION
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_MISSION
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_PROJECT
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_PROJECT
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_SCHEDULE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_SCHEDULE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_TASK
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_TASK
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_PRO_WORKSPACE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_PRO_WORKSPACE
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Pro {
namespace Test {

inline const CHA* ProCore(const CHA* args) {
#if SEAM >= CRABSTK_PRO_CORE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro core..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProGit(const CHA* args) {
#if SEAM >= CRABSTK_PRO_GIT
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro git..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProIssue(const CHA* args) {
#if SEAM >= CRABSTK_PRO_ISSUE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro issue..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProMission(const CHA* args) {
#if SEAM >= CRABSTK_PRO_MISSION
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro mission..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProProject(const CHA* args) {
#if SEAM >= CRABSTK_PRO_PROJECT
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro project..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProSchedule(const CHA* args) {
#if SEAM >= CRABSTK_PRO_SCHEDULE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro schedule..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProTask(const CHA* args) {
#if SEAM >= CRABSTK_PRO_TASK
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro task..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* ProWorkspace(const CHA* args) {
#if SEAM >= CRABSTK_PRO_WORKSPACE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Pro workspace..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Pro
}  //< namespace CT
