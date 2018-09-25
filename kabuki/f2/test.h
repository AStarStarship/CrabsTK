/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/test.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>

#ifndef INCLUDED_KABUKI_F2_TEST
#define INCLUDED_KABUKI_F2_TEST 1

namespace _ {

/* Class for storing information about a failed assertion. */
struct API TestResult {
  const char *name,  //< The file the assertion occurred at.
      *description;  //< An optional description to print.
  int line;          //< The line the assertion failed at.

  /* Constructs all of the data fields the given arguments.
  @param name    The file the assertion occurred at.
  @param line    The line the assertion failed at.
  @param description An optional description to print. */
  TestResult(const char* name = nullptr, int line = 0,
             const char* description = nullptr);

  /* Copy constructor clones the other object. */
  TestResult(const TestResult& other);

  /* Checks if the assertion failed.
  @return True if the assertion failed.
  @desc   Algorithm works by checking if the file pointer is null. */
  bool Failed();

  /* Returns app exit code for when tests fail. */
  int Quit();
};
}  // namespace _

typedef _::TestResult (*TestCase)(const char* args);

namespace _ {

/* A Major Seam in a Layer-Major-Minor Seam Tree. */
class API SeamMajor {
 public:
  /* Constructs a SeamMajor from the an array of count TestCase function
  pointers.
  @param seam_layer  The page seam number.
  @param seam_major The major seam number.
  @param major_seams Array of pointers to SeamMajor(s).
  @param major_seam_count The number of major seams. */
  SeamMajor(TestCase* minor_seams, int minor_count);

  /* Runs a unit minor_seams with the given global parameter list.
  @return Nil upon success or an TestCase with debug info upon failure.
  @param  args      An array of string arguments.
  @param  arg_count The number of arguments
  @param  seam_layer The seam layer index.
  @praam  seam_major The seam major index. */
  TestResult Run(const char* args, int seam_layer, int seam_major);

  /* Gets a reference to the major_seam_. */
  int& GetMinorSeamCount();

 private:
  TestCase* minor_seams_;  //< Array of function pointers.
  int minor_count_;        //< Minor seam count.
};

class API SeamLayer {
 public:
  /* Constructs a SeamMajor from the an array of count SeamMajor(s).
  @param page_seam The major seam index that is zero or more.
  @param major_seams Array of pointers to SeamMajor(s).
  @param major_seam_count The number of major seams. */
  SeamLayer(SeamMajor** major_seams, int major_seam_count);

  /* Runs a page seam unit test with the given global parameter list.
  @return Nil upon success or an TestCase with debug info upon failure.
  @param  args      An array of string arguments.
  @param  arg_count The number of arguments. */
  TestResult Run(const char* args, int seam_layer);

  /* Gets a reference to the major_count. */
  int& GetMajorSeamCount();

 private:
  SeamMajor** major_seams_;  //< Array of SeamMajor(s).
  int major_count;           //< Major seam count.
};

class API UnitTest {
 public:
  /* Constructs a UnitTest from the an array of count SeamLayer(s).
  @param page_seams Array of pointers to SeamLayer(s).
  @param layer_count The number of page seams. */
  UnitTest(SeamLayer** page_seams_, int layer_count);

  /* Runs a unit test with the given global parameter list.
  @return APP_EXIT_SUCCESS upon success or APP_EXIT_FAILURE upon failure.
  @param  args      An array of string arguments.
  @param  arg_count The number of arguments. */
  int Run(const char* args, const char* name = nullptr,
          const char* description = nullptr);

  /* Gets a reference to the layer_count_. */
  int& GetPageSeamCount();

 private:
  SeamLayer** page_seams_;  //< Array of SeamLayer pointers.
  int layer_count_;         //< Page seam count.
};

/* Assert check if the given condition is true
@return false if the condition is false.
@param condition The condition to check. */
API bool Assert(bool condition);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(const char* a, const char* b);

/* Tests if the two pointers are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(const void* a, const void* b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(uint8_t a, uint8_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(int8_t a, int8_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(uint8_t a, uint8_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(int16_t a, int16_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(uint16_t a, uint16_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(int32_t a, int32_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(uint32_t a, uint32_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(int64_t a, int64_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(uint64_t a, uint64_t b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(float a, float b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(double a, double b);

/* Tests if the two pointers are equal.
@return false if the condition is false.
@param  value The value to test.  */
API bool Test(const void* value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test.
@param  b Parameter b. */
API bool Test(uint8_t value, uint8_t b);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(int8_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(uint8_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(int16_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(uint16_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(int32_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(uint32_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(int64_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(uint64_t value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(float value);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param  value The value to test. */
API bool Test(double value);

/* Handles an assert by printing a debug statement and locks up the system.
@return True upon failure.
@param  line    The line the program failed at.
@param  file    The file the error occurred at.
@param  message An optional message to print. */
API bool AssertHandle(const char* file, int line,
                      const char* message = nullptr);
}  // namespace _

typedef _::TestResult (*TestCase)(const char* args);

#if SEAM > 0 && SEAM <= SEAM_N
#define ASSERT(condition) \
  if (Assert(condition)) _::AssertHandle(__FILE__, __LINE__)
#define CHECK(item) \
  if (Test(item)) _::AssertHandle(__FILE__, __LINE__)
#define TEST(a, b) \
  if (Test(a, b)) _::AssertHandle(__FILE__, __LINE__)
#else
#define ASSERT(condition)
#define CHECK(item)
#define TEST(a, b)
#endif

#endif  //< #ifndef INCLUDED_KABUKI_F2_TEST
