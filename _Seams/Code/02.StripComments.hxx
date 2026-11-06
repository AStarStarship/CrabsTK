// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKCodeStripComments
#define _CrabsTKCodeStripComments
#include "../_Config.h"
#include "../../Code/CommentStripper.h"
namespace CTKCodeTest {
using namespace ::_;
using namespace ::CTK;

inline const CHA* TestStripComments(const CHA* args) {
#if _Seam >= _CrabsTKCodeCommentStripper
  _ATestBegin;

  TestStripComments("who", "UserList.hpp");

  D_COUT("\n\nCompleted successfully.");
#endif
  return 0;
}

}  //< namespace CTKCodeTest
#endif  //< _CrabsTKCodeStripComments
