/* CRABSTK
@file    /_Seams/Crab/00.Core.h
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == CRABS_CORE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_CORE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_RNG
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_RNG
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_ITOS
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_ITOS
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_FTOS
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_FTOS
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_SPRINTER
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_SPRINTER
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_CLOCK
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_CLOCK
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_STACK
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_STACK
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_MATRIX
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_MATRIX
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_STRING
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_STRING
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_LOOM
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_LOOM
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_LIST
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_LIST
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_BOOK
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_BOOK
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_MAP
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_MAP
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_TABLE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_TABLE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_DIC
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_DIC
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_MACHINE
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_MACHINE
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_DOOR
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_DOOR
#include <ASCIICrabs/_Release.h>
#endif
#if SEAM == CRABS_FILECABINET
#include <ASCIICrabs/_Debug.h>
#elif SEAM > CRABS_FILECABINET
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>

using namespace _;
namespace CT {
namespace Crab {
namespace Test {

inline const CHA* CrabsCore(const CHA* args) {
#if SEAM >= CRABS_CORE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab core types..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsRng(const CHA* args) {
#if SEAM >= CRABS_RNG
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab RNG..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsItos(const CHA* args) {
#if SEAM >= CRABS_ITOS
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab ITOS conversion..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsFtos(const CHA* args) {
#if SEAM >= CRABS_FTOS
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab FTOS conversion..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsSprinter(const CHA* args) {
#if SEAM >= CRABS_SPRINTER
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab sprinter..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsClock(const CHA* args) {
#if SEAM >= CRABS_CLOCK
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab clock..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsStack(const CHA* args) {
#if SEAM >= CRABS_STACK
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab stack..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsMatrix(const CHA* args) {
#if SEAM >= CRABS_MATRIX
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab matrix..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsString(const CHA* args) {
#if SEAM >= CRABS_STRING
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab string..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsLoom(const CHA* args) {
#if SEAM >= CRABS_LOOM
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab loom..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsList(const CHA* args) {
#if SEAM >= CRABS_LIST
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab list..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsBook(const CHA* args) {
#if SEAM >= CRABS_BOOK
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab book..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsMap(const CHA* args) {
#if SEAM >= CRABS_MAP
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab map..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsTable(const CHA* args) {
#if SEAM >= CRABS_TABLE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab table..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsDic(const CHA* args) {
#if SEAM >= CRABS_DIC
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab dic..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsMachine(const CHA* args) {
#if SEAM >= CRABS_MACHINE
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab machine..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsDoor(const CHA* args) {
#if SEAM >= CRABS_DOOR
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab door..."));

  // Your test code here.

#endif
  return 0;
}

inline const CHA* CrabsFilecabinet(const CHA* args) {
#if SEAM >= CRABS_FILECABINET
  A_TEST_BEGIN;
  D_COUT(Headingf("Testing Crab filecabinet..."));

  // Your test code here.

#endif
  return 0;
}

}  //< namespace Test
}  //< namespace Crab
}  //< namespace CT
