// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef _CrabsTKCodeCommentStripperH
#define _CrabsTKCodeCommentStripperH

namespace CTK {
namespace Code {

/* Strip C++ comments from a source file and write the result to stdout.
The Lexer (Code/Lexer.hxx) decides what a comment is, so string and character
literals are never mistaken for comment delimiters ("http://x" is safe).
Comment-only lines are collapsed to nothing; blank lines are preserved; code
lines keep their indentation and lose trailing horizontal whitespace.
Tabs are expanded to `tab_space_count` spaces.
@param directory       Directory holding the file.
@param filename        File to strip.
@param tab_space_count Spaces per tab in the output.
@return 0 on success, -1 on error. */
ISC StripComments(const CHA* directory, const CHA* filename,
                  ISC tab_space_count = 2);

/* Strip comments from every source file in the given directory.
@param directory  Directory to sweep.
@return 0 on success, -1 on error. */
ISC StripComments(const CHA* directory);

}  //< namespace Code
}  //< namespace CTK
#endif
