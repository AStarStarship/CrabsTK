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

#if SEAM >= SEAM_0_0_0
#ifndef INCLUDED_CRABS_TEST
#define INCLUDED_CRABS_TEST 1

namespace _ {

/* Class for storing information about a failed assertion. */
struct API TestResult {
  const char* name;     //< The file the assertion occurred at.
  const char* message;  //< An optional message to print.
  int line;             //< The line the assertion failed at.

  /* Constructs all of the data fields the given arguments.
  @param name    The file the assertion occurred at.
  @param line    The line the assertion failed at.
  @param message An optional message to print. */
  TestResult(const char* name = nullptr, int line = 0,
             const char* message = nullptr);

  /* Copy constructor clones the other object. */
  TestResult(const TestResult& other);

  /* Checks if the assertion failed.
  @return True if the assertion failed.
  @desc   Algorithm works by checking if the file pointer is null. */
  bool Failed();
};

/* */
class SeamMajor {
 public:
  /* Constructs a SeamMajor from the an array of count TestCase function
  pointers.
  @param seam_page  The page seam number.
  @param seam_major The major seam number.
  @param major_seams Array of pointers to SeamMajor(s).
  @param major_seam_count The number of major seams. */
  SeamMajor(int seam_page, int seam_major, TestCase* minor_seams,
            int minor_seam_count);

  /* Runs a unit minor_seams with the given global parameter list.
  @return Nil upon success or an TestCase with debug info upon failure.
  @param  args      An array of string argumetns.
  @param  arg_count The number of arguments. */
  TestResult Run(const char* args);

  /* Gets a reference to the major_seam_. */
  int& GetMinorSeamCount();

 private:
  /* Array of unit tests for each minor seam. */
  int seam_page_,          //< Page seam number.
      seam_major_,         //< Major seam number.
      minor_seam_count_;   //< Minor seam count.
  TestCase* minor_seams_;  //< Array of function pointers.
};

class SeamPage {
 public:
  /* Constructs a SeamMajor from the an array of count SeamMajor(s).
  @param page_seam The major seam index that is zero or more.
  @param major_seams Array of pointers to SeamMajor(s).
  @param major_seam_count The number of major seams. */
  SeamPage(int seam_page, SeamMajor** major_seams, int major_seam_count);

  /* Runs a page seam unit test with the given global parameter list.
  @return Nil upon success or an TestCase with debug info upon failure.
  @param  args      An array of string argumetns.
  @param  arg_count The number of arguments. */
  TestResult Run(const char* args);

  /* Gets a reference to the major_seam_count_. */
  int& GetMajorSeamCount();

 private:
  /* Array of unit tests for each minor seam. */
  int seam_page_,            //< Page seam number.
      major_seam_count_;     //< Major seam count.
  SeamMajor** major_seams_;  //< Array of SeamMajor(s).
};

class UnitTest {
 public:
  /* Constructs a UnitTest from the an array of count SeamPage(s).
  @param page_seams Array of pointers to SeamPage(s).
  @param page_seam_count The number of page seams. */
  UnitTest(SeamPage** page_seams_, int page_seam_count, const char* name,
           const char* description);

  /* Runs a unit test with the given global parameter list.
  @return APP_EXIT_SUCCESS upon success or APP_EXIT_FAILURE upon failure.
  @param  args      An array of string argumetns.
  @param  arg_count The number of arguments. */
  int Run(const char* args);

  /* Gets a reference to the page_seam_count_. */
  int& GetPageSeamCount();

 private:
  int page_seam_count_;    //< Page seam count.
  SeamPage** page_seams_;  //< Array of SeamPage pointers.
  const char *name_,       //< The name of the PMMS unit test.
      *description_;       //< Description of the PMMS unit test.
};

API TestResult TestRun(TestCase test_case, const char* args);

/* Assert function to assist in casting boolean types. */
API bool Assert(bool condition);

/* Handles an assert by printing a debug statement and locks up the system.
@return True upon failure.
@param  line    The line the program failed at.
@param  file    The file the error occured at.
@param  message An optional message to print. */
API bool AssertHandle(const char* file, int line,
                      const char* message = nullptr);
}  // namespace _

typedef _::TestResult (*TestCase)(const char* args);

#if CRABS_ASSERT_PRINT || CRABS_ASSERT_PAUSE || CRABS_ASSERT_FREEZE
#define ASSERT(condition)
#else
#define ASSERT(condition) \
  if (Assert(condition)) AssertHandle(__FILE__, __LINE__)
#endif
#if CRABS_ASSERT_PRINT || CRABS_ASSERT_PAUSE || CRABS_ASSERT_FREEZE
#define ASSERT(condition)
#else
#define ASSERT(condition) \
  if (Assert(condition)) AssertHandle(__FILE__, __LINE__)
#endif

#define TEST(condition, message) \
  if (Assert(condition))         \
  AssertHandle(__FILE__, __LINE__, message == nullptr ? "" : message)

#endif  //< #ifndef INCLUDED_CRABS_TEST

#endif  //< #if SEAM >= SEAM_0_0_0
