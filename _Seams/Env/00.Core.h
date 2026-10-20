/* CRABSTK
@file    /_Seams/Env/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_ENV_CORE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_ENV_CORE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_ENV_VAR
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_ENV_VAR
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Env {
namespace Test {

inline const CHA* EnvCore(const CHA* args) {
#if SEAM >= CRABSTK_ENV_CORE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Env core..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* EnvVar(const CHA* args) {
#if SEAM >= CRABSTK_ENV_VAR
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Env var..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Env
}  //< namespace CT
