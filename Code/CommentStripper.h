// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef _CrabsTKCodeCommentStripperH
#define _CrabsTKCodeCommentStripperH

namespace CTK {
namespace Code {
    
ISN StripComments(const CHA* directory, const CHA* filename,
                  ISN tab_space_count = 2);

ISN StripComments(const CHA* directory);
    
}  //< namespace Code
}  //< namespace CTK
#endif
