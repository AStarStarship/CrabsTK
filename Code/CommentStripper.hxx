// Copyright AStarship <https://astarship.net>.
#include <_Config.h>
//
//#include <ASCIICrabs/cin.h>
//
#include "CommentStripper.h"
//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//
#ifdef _WIN32
#include <Windows.h>
#endif
using namespace std;

using namespace _;
namespace CT {
namespace Code {

ISN StripComments(const CHA* directory, const CHA* filename,
                  ISN tab_space_count) {
  if (!directory || !filename) return -1;

  std::string filename_in = std::string(directory) + "/" + filename;

  std::string filename_out(directory);
  filename_out += "/sloth/";
  filename_out += filename;

  std::cout << "\nStripping comments from:" << filename_in
            << " to:" << filename_out << "\n\n\n";

  enum {
    cScanningCode = 0,
    cScanningCommentLine,
    cScanningCommentBlock,
  };

  ISN state = cScanningCode;

  string line, result;
  ifstream file_in(filename_in.c_str());
  // ofstream file_out;
  // file_out.open(filename_out);

  if (file_in.is_open()) {
    // std::cout << "\n\nState:ScanningCode\n\n";
    while (getline(file_in, line)) {
      const CHA* cursor = line.c_str();
      CHA c = *cursor++, last_c = 0;
      while (c) {
        // Check if the rest of the line is whitespace.
        // const CHA* whitespace_cursor = cursor;
        // while (c == ' ' || c == '\t') c = *cursor++;
        // if (!c) break;

        switch (state) {
          case cScanningCode: {
            if (last_c == '/') {
              if (c == '/') {
                std::cout << "//";
                state = cScanningCommentLine;
              } else if (c == '*') {
                std::cout << "/*";
                state = cScanningCommentBlock;
              } else {
                goto PrintC;
              }
            } else {
            PrintC:
              if (last_c == '\t') {
                for (ISN i = tab_space_count; i > 0; --i) {
                  std::cout << ' ';
                  // file_out << ' ';
                }
              } else {
                std::cout << last_c;
                // file_out << last_c;
              }
            }
            last_c = c;
            break;
          }
          case cScanningCommentLine: {
            break;
          }
          case cScanningCommentBlock: {
            if (last_c == '*') {
              if (c == '/') {
                std::cout << "*/";
                state = cScanningCode;
              }
            }
            break;
          }
        }
        last_c = c;
        c = *cursor++;
      }
      c = '\n';
      if (state == cScanningCommentLine) {
        std::cout << '\n';
        // file_out << '\n';
        state = cScanningCode;
      } else if (state != cScanningCommentBlock) {
        if (last_c != '/') {
          std::cout << last_c;
          // file_out << last_c;
        }
        std::cout << '\n';
        // file_out << '\n';
      }
    }
  } else {
    std::cout << "Unable to open file";
  }

  // file_out.close();
  return 0;
}

std::vector<string> FilenamesInFolder(const CHA* directory) {
  std::vector<std::string> names;
  std::string search_path = directory;
#ifdef _WIN32
  search_path += "/*.*";
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        names.push_back(fd.cFileName);
      }
    } while (::FindNextFile(hFind, &fd));
    ::FindClose(hFind);
  }
#endif
  return names;
}

ISN StripComments(const CHA* directory) {
  std::vector<std::string> filenames = FilenamesInFolder(directory);
  return 0;
}

}  //< namespace Code
}  //< namespace CT
