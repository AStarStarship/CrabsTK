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

#include "ttest.h"

#include "console.h"

namespace _ {

SeamMajor::SeamMajor(TestCase* minor_seams, int minor_count)
    : minor_seams_(minor_seams), minor_count_(minor_count) {}

TestResult SeamMajor::Run(const char* args, int seam_layer, int seam_major) {
  enum { kSize = 128 };
  char buffer[kSize];
  char *end = buffer + kSize - 1,
       *cursor = Print<>(buffer, end, "Testing Major SEAM_");
  cursor = Print<uint, int>(cursor, end, seam_layer);
  cursor = Print<>(cursor, end, '_');
  cursor = Print<uint, int>(cursor, end, seam_major);
  cursor = Print<>(cursor, end, "_x");
  PrintHeading(buffer);
  for (int seam_minor = 0; seam_minor < kSeamCount; ++seam_minor) {
    cursor = Print<>(buffer, end, "Testing SEAM_");
    cursor = Print<uint, int>(cursor, end, seam_layer);
    cursor = Print<>(cursor, end, '_');
    cursor = Print<uint, int>(cursor, end, seam_major);
    cursor = Print<>(cursor, end, '_');
    cursor = Print<uint, int>(cursor, end, seam_minor);
    PrintHeading(buffer);
    TestResult result = minor_seams_[seam_minor](args);
    if (result.Failed()) return result;
    Printf("\n\nDone testing SEAM_%i_%i_%i", seam_layer, seam_major,
           seam_minor);
  }
  Print("\n\nDone testing Major Seam");
  Print(seam_major);
  Print('\n', '\n');
  return TestResult();
}

int& SeamMajor::GetMinorSeamCount() { return minor_count_; }

SeamLayer::SeamLayer(SeamMajor** seam_majors, int seam_major_count)
    : major_seams_(seam_majors), major_count(seam_major_count) {}

TestResult SeamLayer::Run(const char* args, int seam_layer) {
  Print("\n\nTesting SEAM_").Print(seam_layer);
  int seam_major_count = major_count;
  ASSERT(seam_major_count >= 1);
  for (int i = 0; i < seam_major_count; ++i)
    major_seams_[i]->Run(args, seam_layer, i);
  Print("\n\nDone testing SEAM_").Print(seam_layer);
  return TestResult();
}

int& SeamLayer::GetMajorSeamCount() { return major_count; }

UnitTest::UnitTest(SeamLayer** page_seams, int layer_count)
    : page_seams_(page_seams), layer_count_(layer_count) {}

int UnitTest::Run(const char* args, const char* name, const char* description) {
  if (!name) name = "Foo";
  if (!description) description = "Bar";
  PrintHeading(name).Print('\n') << description << '\n';
  PrintLine();
  PrintLn();
  for (int i = 0; i < layer_count_; ++i) {
    TestResult result = page_seams_[i]->Run(args, i);
    if (result.Failed()) return APP_EXIT_FAILURE;
  }
  Pause("Completed running unit tests successfully! :-)");
  return APP_EXIT_SUCCESS;
}

int& UnitTest::GetPageSeamCount() { return layer_count_; }

TestResult TestRun(TestCase test_case, const char* args) {
  TestResult result = test_case(nullptr);
  if (!args) args = "";
  return TestResult();
}

bool Assert(bool condition) { return !condition; }

bool Test(const char* a, const char* b) { return Test<char>(a, b); }

bool Test(const char16_t* a, const char16_t* b) { return Test<char16_t>(a, b); }

bool Test(const char32_t* a, const char32_t* b) { return Test<char32_t>(a, b); }

bool Test(const void* a, const void* b) {
  if (a == b) return true;
  CHex test(b);
  Print("\nERROR: Expecting:").Hex(a) << "\n           Found:" << CHex(b);
  return false;
}

bool Test(uint8_t a, uint8_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(int8_t a, int8_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(uint16_t a, uint16_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(int16_t a, int16_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(uint32_t a, uint32_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(int32_t a, int32_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(uint64_t a, uint64_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(int64_t a, int64_t b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(float a, float b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(double a, double b) {
  if (a == b) return true;
  Print("\nERROR: Expecting:").Print(a) << "\n           Found:" << b;
  return false;
}

bool Test(const void* value) {
  if (value) return true;
  Print("\nERROR: Pointer was nil!");
  return false;
}

bool Test(uint8_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(int8_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(uint16_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(int16_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(uint32_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(int32_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(uint64_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(int64_t value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(float value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

bool Test(double value) {
  if (value) return true;
  Print("\nERROR: value was nil!");
  return false;
}

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
    : name(name), description(message), line(line) {
  // Nothing to do here! ({:-)-+=<
}

TestResult::TestResult(const TestResult& other)
    : name(other.name), description(other.description), line(other.line) {}

bool TestResult::Failed() { return name != nullptr; }

int TestResult::Quit() {
  return Failed() ? APP_EXIT_FAILURE : APP_EXIT_SUCCESS;
}

}  // namespace _
