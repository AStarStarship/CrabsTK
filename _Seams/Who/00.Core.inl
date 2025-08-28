/* Kabuki Toolkit
@link    https://github.com/KabukiStarship/KT.git
@file    /_Seams/Who/00.Core.inl
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019 (C) Kabuki Starship <kabukistarship.com>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == KT_WHO
#include "_Debug.inl"
#else
#include "_Release.inl"
#endif
using namespace _;
namespace KT {
namespace Who {

inline const CHA* Core(const CHA* args) {
#if SEAM >= KT_WHO_CORE
  A_TEST_BEGIN;

#endif
  return 0;
}
}  //< namespace Who
}  //< namespace KT
