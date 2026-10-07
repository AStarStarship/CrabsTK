/* CrabsTK
@link    https://github.com/KabukiStarship/CT.git
@file    /_Seams/Touch/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019 (C) AStarship <astarship.net>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == KT_TOUCH_CORE
#include "_Debug.h"
#else
#include "_Release.h"
#endif
using namespace _;
namespace CT {
namespace Touch {

inline const CHA* Core(const CHA* args) {
#if SEAM >= KT_TOUCH_CORE
  A_TEST_BEGIN;

#endif
  return 0;
}
}  //< namespace Touch
}  //< namespace CT
