#include <ASCIICrabs/String.hpp>
// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_CODE_COMMENTSTRIPPER_DECL_DECL
#define CRABSTK_CODE_COMMENTSTRIPPER_DECL_DECL

namespace _ {
ISN StripComments(const CHA* directory, const CHA* filename,
                  ISN tab_space_count = 2);

ISN StripComments(const CHA* directory);

}  //< namespace _
#endif
