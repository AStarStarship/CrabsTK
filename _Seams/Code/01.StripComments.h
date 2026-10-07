/* CrabsTK
@link    https://github.com/KabukiStarship/CT.git
@file    /_Seams/Code/00.StripComments.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#include "../CommentStripper.h"
#if SEAM >= KABUKI_DOWNLOAD_API
#include "../Code/CodeModule.h"
using namespace _;
#if SEAM == CRABSTK_CODE_STRIPCOMMENTS
#include "_Debug.h"
#else
#include "_Release.h"
#endif
#endif
namespace CT {
namespace Code {
inline const CHA* StripComments(const CHA* args) {
#if SEAM >= CRABSTK_CODE_STRIPCOMMENTS
  TEST_BEGIN;

  StripComments("who", "UserList.hpp");

  std::cout << "\n\nCompleted successfully.";
/*
  static const CHA* cHeader = "";

  CodeModule module("https://github.com/SFML/SFML.git", kHeader,
                    "C:/workspace/KabukiStarship/CT/_Seams/bin");
  module.Clone();
  module.RepoAddressSet("https://github.com/KabukiStarship/kabuki.vm.git");
  module.Import();

#if DEBUG_THIS
  module.COut();
#endif
*/
  return 0;
}
}  //< namespace Code
}  //< namespace CT

#include <_Config.h>
#include "../Code/CommentStripper.h"
using namespace _;
#include <fstream>
#include <iostream>
#include <string>
ISN main(const CHA **args, ISN arg_count) {

  return 0;
}
