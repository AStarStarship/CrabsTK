/* Kabuki Toolkit
@link    https://github.com/KabukiStarship/KT.git
@file    /_Seams/release.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) Kabuki Starship <kabukistarship.com>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == KABUKITOOLKIT_RELEASE
#include "_Debug.inl"
#else
#include "_Release.inl"
#endif
using namespace _;
namespace KT {
inline const CHA* Release(const CHA* args) {
#if SEAM >= KABUKITOOLKIT_RELEASE
  TEST_BEGIN;

#endif
  return 0;
}
}  //< namespace KT
