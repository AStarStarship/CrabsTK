/* CrabsTK
@link    https://github.com/AStarship/CT.git
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
/* ASCIICrabs
@link    https://github.com/AStarship/ASCIICrabs.git
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2015-21 AStarship <astarship.net>;
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */
/* 2026-10-12: Fixed TTestTree — all Core entries must be function pointers
matching TestCase (const CHA* (*)(const CHA*)). Removed Pro/Touch (dead
Script2 code). Added Plot (was missing). Image/00.Core.h was a struct, now
a function. */

#include "_Config.h"
#include <ASCIICrabs/COut.h>
#include <ASCIICrabs/COut.hxx>
#include <ASCIICrabs/AType.hxx>
#include <ASCIICrabs/Array.hxx>
#include <ASCIICrabs/Puff.hxx>

#include "../_Package.hxx"

#include <ASCIICrabs/Test.hpp>
#include <ASCIICrabs/Test.hxx>
#include <ASCIICrabs/Stringf.hxx>

#include "Code/00.Core.h"
#include "Data/00.Core.h"
#include "GUI/00.Core.h"
#include "Image/00.Core.h"
#include "Imul/00.Core.h"
#include "Plot/00.Core.h"
#include "Who/00.Core.h"
#include "Release.h"  // CT::Release for the SEAM_N (training-wheels-off) path

using namespace _;
using namespace CT;

ISN main(ISN arg_count, CHA** args) {
#if SEAM == SEAM_N
  return SeamResult(Release(ArgsToString(arg_count, args)));
#else
  // Note: CT::Plot::Core is fully qualified because the real Plot module
  // lives in _::Plot (Plot/Plot.h) which would shadow CT::Plot via
  // `using namespace _`. The other module seams don't have a conflicting
  // _::<Module> namespace, so they resolve unqualified.
  return TTestTree<Code::Core, Database::Core, GUI::Core, Image::Core,
                   IMUL::Core, CT::Plot::Core, Who::Core>(arg_count, args);
#endif
}
