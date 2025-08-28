/* Kabuki Toolkit
@link    https://github.com/KabukiStarship/KT.git
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) Kabuki Starship <kabukistarship.com>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
/* Script2 ™
@link    https://github.com/KabukiStarship/Script2.git
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2015-21 Kabuki Starship ™ <kabukistarship.com>;
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */

#include <_Config.h>

#include "../_Package.inl"
#include "Code/00.Core.inl"
#include "Database/00.Core.inl"
#include "GUI/00.Core.inl"
#include "Image/00.Core.inl"
#include "IMUL/00.Core.inl"
#include "Pro/00.Core.inl"
#include "Touch/00.Core.inl"
#include "Who/00.Core.inl"

using namespace _;
using namespace KT;

ISN main(ISN arg_count, CHA** args) {
#if SEAM == SEAM_N
  return SeamResult(Release(ArgsToString(arg_count, args)));
#else
  return TTestTree<Code::Core, Database::Core, GUI::Core, Image::Core, 
                   IMUL::Core, Pro::Core, Touch::Core, Who::Core);
#endif
}
