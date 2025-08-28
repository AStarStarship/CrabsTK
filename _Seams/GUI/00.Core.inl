/* Kabuki Toolkit
@link    https://github.com/KabukiStarship/KT.git
@file    /_Seams/GUI/00.Core.inl
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) Kabuki Starship <kabukistarship.com>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#if SEAM == KABUKITOOLKIT_GUI_CORE
#include <Script2/_Debug.inl>
#else
#include <Script2/_Release.inl>
#endif
using namespace _;
namespace KT {
namespace GUI {
  
inline const CHA* Core(CHA* seam_log, CHA* seam_end, const CHA* args) {
#if SEAM >= KABUKITOOLKIT_GUI_CORE
  A_TEST_BEGIN;

#endif
  return 0;
}
}  //< namespace GUI
}  //< namespace KT
