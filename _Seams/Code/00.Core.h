/* CRABSTK
@file    /_Seams/Code/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_CODE_CORE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_CODE_CORE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_CODE_CODEMODULE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_CODE_CODEMODULE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_CODE_COMMENTSTRIPPER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_CODE_COMMENTSTRIPPER
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_CODE_LEXER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_CODE_LEXER
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_CODE_FITTER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_CODE_FITTER
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Code {
namespace Test {

inline const CHA* CodeCore(const CHA* args) {
#if SEAM >= CRABSTK_CODE_CORE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Code core..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CodeCodemodule(const CHA* args) {
#if SEAM >= CRABSTK_CODE_CODEMODULE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Code code module..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CodeCommentstripper(const CHA* args) {
#if SEAM >= CRABSTK_CODE_COMMENTSTRIPPER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Code comment stripper..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CodeLexer(const CHA* args) {
#if SEAM >= CRABSTK_CODE_LEXER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Code lexer..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CodeFitter(const CHA* args) {
#if SEAM >= CRABSTK_CODE_FITTER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Code fitter..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Code
}  //< namespace CT
