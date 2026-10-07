/* CrabsTK
@link    https://github.com/KabukiStarship/CT.git
@file    /_Seams/Image/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) Kabuki Starship <kabukistarship.com>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#if SEAM == CRABSTK_GRAPHICS_CORE
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
using namespace _;
namespace CT {
namespace Image {
inline const CHA* Core(const CHA* args) {
#if SEAM >= CRABSTK_PRO_CORE
  A_TEST_BEGIN;

#endif
  return 0;
}
}  //< namespace Image
}  //< namespace CT
