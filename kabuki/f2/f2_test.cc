/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/f2_test.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if SEAM >= SEAM_0_0_0__00
#include "ttest.h"

#include "console.h"

namespace _ {

const char* TestTree(char* seam_log, char* seam_end, const char* args,
                     TestCase* tests, int count) {
  for (int i = 0; i < count; ++i) {
    TestCase test = tests[i];
    if (test) {
      const char* seam = test(seam_log, seam_end, nullptr);
      PrintHeading("Testing ", seam);
      const char* error = test(seam_log, seam_end, args);
      if (error) return error;
      Print("\n\nDone testing ", seam);
    } else {
      Print("\n Test ").Print(i) << " missing!";
    }
  }
  return nullptr;
}

bool TestBegin(char* seam_log, char* log_end, const char* args) {
  return ((intptr_t)seam_log) || ((intptr_t)seam_log) || ((intptr_t)seam_log);
}

void TestEnd(const char* function_name) {
  Print("\n  Done testing ", function_name);
}

bool Assert(bool condition) { return !condition; }

bool Test(const char* a, const char* b) { return Test<char>(a, b); }

bool Test(const char16_t* a, const char16_t* b) { return Test<char16_t>(a, b); }

bool Test(const char32_t* a, const char32_t* b) { return Test<char32_t>(a, b); }

bool Test(const void* a, const void* b) {
  if (a == b) return true;
  CHex test(b);
  Print("\nERROR: Expecting:").Hex(a)
      << "\n           Found:" << test;  // CHex (b);
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

bool AssertHandle(const char* function, const char* file, int line) {
  Printf("\nAssertion failed in function %s at line %d in \"%s\"", function,
         line, file);
  Pause();
  return true;
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_0__00
