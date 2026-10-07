/* CrabsTK
@link    https://github.com/KabukiStarship/CT.git
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
/* ASCIICrabs
@link    https://github.com/KabukiStarship/ASCIICrabs.git
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2015-21 AStarship <astarship.net>;
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */

#include <_Config.h>
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
#include "Pro/00.Core.h"
#include "Touch/00.Core.h"
#include "Who/00.Core.h"

using namespace _;
using namespace CT;

ISN main(ISN arg_count, CHA** args) {
#if SEAM == SEAM_N
  return SeamResult(Release(ArgsToString(arg_count, args)));
#else
  return TTestTree<Code::Core, Database::Core, GUI::Core, Image::Core, 
                   IMUL::Core, Pro::Core, Touch::Core, Who::Core>(arg_count, args);
#endif
}
