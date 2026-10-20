/* CRABSTK
@file    /_Seams/Touch/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_TOUCH_CORE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_TOUCH_CORE
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Touch {
namespace Test {

inline const CHA* TouchCore(const CHA* args) {
#if SEAM >= CRABSTK_TOUCH_CORE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Touch core..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Touch
}  //< namespace CT
