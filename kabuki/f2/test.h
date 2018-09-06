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

/* Function pointer for a unit test for a minor seam.
@return Nil upon success or a pointer to an error string upon failure.
@param  seam       The contiguous SEAM index.
@param  seam_minor The minor seam number.
*/
typedef const char*(SeamMinorTest)(int seam, int seam_minor,
                                   const char* header);

namespace _ {

/* Assert function to assist in casting boolean types. */
DLL bool Assert(bool condition);

/* Prints a debug statement and locks up the system. */
DLL bool AssertHandle(int line, const char* file,
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
#endif

#if SEAM > 0 && SEAM <= SEAM_COUNT

struct ErrorAssert {
  int line;
  const char* file;
  const char* message;

  ErrorAssert();
};

#endif
