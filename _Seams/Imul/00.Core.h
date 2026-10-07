/* CrabsTK
@link    https://github.com/AStarship/CT.git
@file    /_Seams/Imul/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_IMUL
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include "../../IMUL/Parser.h"
using namespace _;
namespace CT {
namespace IMUL {
inline const CHA* Core(const CHA* args) {
#if SEAM >= CRABSTK_IMUL_CORE
  A_TEST_BEGIN;
  A_RUN_TEST(T01S01);
#endif
  return 0;
}
}  //< namespace IMUL
}  //< namespace CT
