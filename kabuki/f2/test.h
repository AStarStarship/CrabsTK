/* Kabuki Toolkit @version 0.x @version 0.x
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

#ifndef INCLUDED_CRABS_TEST
#define INCLUDED_CRABS_TEST

namespace _ {
/* Clas for storing information about a failed assertion. */
struct Assertion {
  int line;             //< The line the assertion failed at.
  const char* file;     //< The file the assertion occurred at.
  const char* message;  //< An optional message to print.

  /* Constructs all of the data fields the given arguments.
  @param line    The line the assertion failed at.
  @param file    The file the assertion occurred at.
  @param message An optional message to print. */
  Assertion(int line = 0, const char* file = nullptr,
            const char* message = nullptr);

  /* Checks if the assertion failed.
  @return True if the assertion failed.
  @desc   Algorithm works by checking if the file pointer is null. */
  bool Failed();
};
}  // namespace _

typedef _::Assertion (*SeamMinorTest)(const char* args, int arg_count);

namespace _ {

/* Assert function to assist in casting boolean types. */
API bool Assert(bool condition);

/* Handles an assert by printing a debug statement and locks up the system.
@return True upon failure.
@param  line    The line the program failed at.
@param  file    The file the error occured at.
@param  message An optional message to print. */
API bool AssertHandle(int line, const char* file,
                      const char* message = nullptr);

}  // namespace _

#if CRABS_ASSERT_PRINT || CRABS_ASSERT_PAUSE || CRABS_ASSERT_FREEZE
#define ASSERT(condition)
#else
#define ASSERT(condition) \
  if (Assert(condition)) AssertHandle(__LINE__, __FILE__)
#endif
#if CRABS_ASSERT_PRINT || CRABS_ASSERT_PAUSE || CRABS_ASSERT_FREEZE
#define ASSERT(condition)
#else
#define ASSERT(condition) \
  if (Assert(condition)) AssertHandle(__LINE__, __FILE__)
#endif

#define TEST(condition, message) \
  if (Assert(condition))         \
  AssertHandle(__LINE__, __FILE__, message == nullptr ? "" : message)

#endif  //< #ifndef INCLUDED_CRABS_TEST
