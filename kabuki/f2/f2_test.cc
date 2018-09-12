/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/f2_test.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>

#if SEAM >= SEAM_0_0_0
#include "ttest.h"

#include "console.h"

namespace _ {

SeamMajor::SeamMajor(int seam_page, int seam_major, TestCase* minor_seams,
                     int minor_seam_count)
    : seam_page_(seam_page),
      seam_major_(seam_major),
      minor_seam_count_(minor_seam_count),
      minor_seams_(minor_seams) {}

TestResult SeamMajor::Run(const char* args) {
  enum { kSize = 128 };
  char buffer[kSize];
  char *cursor, *end = buffer + kSize - 1;
  int seam_page = seam_page_, seam_major = seam_major_;
  cursor = Print(buffer, end, "Testing Major Seam ");
  cursor = Print(cursor, end, seam_major);
  PrintHeading(buffer);
  for (int seam_minor = 0; seam_minor < kSeamCount; seam_minor++) {
    cursor = Print(buffer, end, "Testing SEAM_");
    cursor = Print(cursor, end, seam_page);
    cursor = Print(cursor, end, '_');
    cursor = Print(cursor, end, seam_major);
    cursor = Print(cursor, end, '_');
    cursor = Print(cursor, end, seam_minor);
    PrintHeading(buffer);
    TestResult result = minor_seams_[seam_minor](args);
    if (result.Failed()) return result;
    Printf("\n\nDone testing SEAM_%i_%i_%i", seam_page, seam_major, seam_minor);
  }
  Print("\n\nDone testing Major Seam");
  Print(seam_major);
  Print('\n', '\n');
  return TestResult();
}

int& SeamMajor::GetMinorSeamCount() { return minor_seam_count_; }

SeamPage::SeamPage(int seam_page, SeamMajor** seam_majors, int seam_major_count)
    : seam_page_(seam_page),
      major_seam_count_(seam_major_count),
      major_seams_(seam_majors) {}

TestResult SeamPage::Run(const char* args) {
  Print("\n\nTesting SEAM_").Print(seam_page_);
  int seam_major_count = major_seam_count_;
  if (seam_major_count <= 1) return major_seams_[0]->Run(args);

  SeamMajor **cursor = major_seams_, **end = cursor + seam_major_count - 1;
  while (cursor < end) (cursor++)[0]->Run(args);
  Print("\n\nDone testing SEAM_").Print(seam_page_);
  return TestResult();
}

int& SeamPage::GetMajorSeamCount() { return major_seam_count_; }

UnitTest::UnitTest(SeamPage** page_seams, int page_seam_count, const char* name,
                   const char* description)
    : page_seams_(page_seams),
      page_seam_count_(page_seam_count),
      name_(name),
      description_(description) {}

int UnitTest::Run(const char* args) {
  PrintHeading(name_);
  Print('\n');
  Print(description_);
  Print('\n');
  PrintLine();
  PrintLn();
  for (int i = 0; i < page_seam_count_; ++i) page_seams_[i]->Run(args);
  TestResult result = ;
  if (result.Failed()) {
    Print("\nUnit tests failed.");
    return APP_EXIT_FAILURE;
  }
  Pause("Completed running unit tests successsfully! :-)");
  return APP_EXIT_SUCCESS;
}

int& UnitTest::GetPageSeamCount() { return page_seam_count_; }

TestResult TestRun(TestCase test_case, const char* args) {
  TestResult result = test_case(nullptr);
  if (!args) args = "";
  return TestResult();
}

bool Assert(bool condition) { return !condition; }

bool AssertHandle(const char* file, int line, const char* message) {
  if (message)
    Printf("\n%s\n,TestResult failed at line %d in \"%s\" with message:\n%s",
           line, file, message);
  else
    Printf("\nAssertion failed at line %d in \"%s\"", line, file);
  Pause();
  return true;
}

TestResult::TestResult(const char* name, int line, const char* message)
    : name(name), message(message), line(line) {
  // Nothing to do here! ({:-)-+=<
}

TestResult::TestResult(const TestResult& other)
    : name(other.name), message(other.message), line(other.line) {}

bool TestResult::Failed() { return name != nullptr; }

}  // namespace _

#endif  //< #if SEAM >= SEAM_0_0_0
