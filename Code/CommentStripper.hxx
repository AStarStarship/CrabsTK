// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
#include <ASCIICrabs/String.hpp>
#include "CommentStripper.h"

#include <cstdio>
#include <string>

using namespace _;
namespace CT {
namespace Code {

/* Strips C++ comments from a source file and writes the result to stdout.
Tabs are expanded to `tab_space_count` spaces.
@return 0 on success, -1 on error. */
ISN StripComments(const CHA* directory, const CHA* filename,
                  ISN tab_space_count) {
  if (IsError(directory) || IsError(filename)) return -1;

  std::string path(directory);
  path += "/";
  path += filename;

  FILE* file_in = fopen(path.c_str(), "r");
  if (file_in == 0) return -1;

  enum State { cScanningCode = 0, cScanningCommentLine, cScanningCommentBlock };
  ISN state = cScanningCode;

  char buf[4096];
  while (fgets(buf, sizeof(buf), file_in)) {
    CHA c, last_c = 0;
    for (const CHA* cursor = buf; (c = *cursor++) != 0; ) {
      if (c == '\n') break;  //< stop at end of line
      switch (state) {
        case cScanningCode: {
          if (last_c == '/') {
            if (c == '/') {
              D_COUT("//");
              state = cScanningCommentLine;
            } else if (c == '*') {
              D_COUT("/*");
              state = cScanningCommentBlock;
            } else {
              D_COUT(last_c);
            }
          } else {
            if (last_c == '\t') {
              for (ISN i = tab_space_count; i > 0; --i) D_COUT(' ');
            } else {
              D_COUT(last_c);
            }
          }
          last_c = c;
          break;
        }
        case cScanningCommentLine: {
          break;
        }
        case cScanningCommentBlock: {
          if (last_c == '*' && c == '/') {
            D_COUT("*/");
            state = cScanningCode;
          }
          last_c = c;
          break;
        }
      }
    }
    // End-of-line handling.
    if (state == cScanningCommentLine) {
      D_COUT('\n');
      state = cScanningCode;
    } else if (state != cScanningCommentBlock) {
      if (last_c != '/') D_COUT(last_c);
      D_COUT('\n');
    }
  }
  fclose(file_in);
  return 0;
}

/* Strips comments from every source file in the given directory.
@return 0 on success. */
ISN StripComments(const CHA* directory) {
  (void)directory;  //< TODO: enumerate directory entries and call StripComments.
  return 0;
}

}  //< namespace Code
}  //< namespace CT
