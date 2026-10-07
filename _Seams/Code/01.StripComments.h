/* CrabsTK
@link    https://github.com/AStarship/CT.git
@file    /_Seams/Code/01.StripComments.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#include "../../Code/CommentStripper.h"
using namespace _;

namespace CT {
namespace Code {

inline const CHA* StripComments(const CHA* args) {
#if SEAM >= CRABSTK_CODE_COMMENTSTRIPPER
  A_TEST_BEGIN;

  StripComments("who", "UserList.hpp");

  D_COUT("\n\nCompleted successfully.");
#endif
  return 0;
}

}  //< namespace Code
}  //< namespace CT
