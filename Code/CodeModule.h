// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABSTK_CODE_CODEMODULE_DECL
#define CRABSTK_CODE_CODEMODULE_DECL

namespace CT {
namespace Code {

/* A code module wraps a Git repository and its local checkout. */
class CodeModule {
 public:
  CodeModule();

  const CHA* Header() const { return header_; }
  const CHA* RepoAddress() const { return repo_address_; }
  const CHA* OutputPath() const { return output_path_; }

 private:
  const CHA* header_ = 0;
  const CHA* repo_address_ = 0;
  const CHA* output_path_ = 0;
};

}  //< namespace Code
}  //< namespace CT
#endif
