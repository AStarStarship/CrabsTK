/* CRABSTK
@file    /_Seams/Data/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABSTK_DATA_RECORD
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_DATA_RECORD
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_DATA_DATASET
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_DATA_DATASET
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABSTK_DATA_ASCIIPLOT
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABSTK_DATA_ASCIIPLOT
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Data {
namespace Test {

inline const CHA* DataRecord(const CHA* args) {
#if SEAM >= CRABSTK_DATA_RECORD
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Data record..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* DataDataset(const CHA* args) {
#if SEAM >= CRABSTK_DATA_DATASET
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Data dataset..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* DataAsciiplot(const CHA* args) {
#if SEAM >= CRABSTK_DATA_ASCIIPLOT
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Data ASCII plot..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Data
}  //< namespace CT
