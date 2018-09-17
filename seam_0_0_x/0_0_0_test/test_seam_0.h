/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/seams/0_seam_tests/test_seam_0.h
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

#include <assembly.h>

#ifndef INCLUDED_SEAMS_0_0_SEAM_TESTS_TEST_SEAM_0
#define INCLUDED_SEAMS_0_0_SEAM_TESTS_TEST_SEAM_0

#include "../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../cpputest/include/CppUTest/TestHarness.h"

/* Converts int main(int,char**) arguments back into a string.
@return false if there are no args to convert.
@param arg_count The number of arguments.
@param args      The arguments. */
bool ArgsToString(int args_count, char** args) {
  if (args_count <= 1) return false;
  if (args_count == 2) return true;
  for (int i = 2; i < args_count; ++i) {
    char* cursor = args[i];
    while (*cursor) cursor--;
    *cursor = ' ';
  }
  return true;
}
}  // namespace _

int main(int args_count, char** args) {
  cout << "\n\nTesting ArgsToString...\n";

  if (args_count <= 1) return 1;
  cout << "\nArguments:\n";
  for (int i = 0; i < args_count; ++i) {
    char* arg = args[i];
    printf("\ni:%i\"%s\" 0x%p", i, arg, arg);
  }
  cout << "\n\nContiguous Args:\n";
  char* end = args[args_count - 1];
  while (*end) ++end;
  cout << "\n\nContiguous Args:\n";
  char* cursor = args[0];
  while (cursor != end) {
    char c = *cursor++;
    if (c == 0)
      cout << '`';
    else if (c < ' ')
      cout << '~';
    else
      cout << c;
  }
  cout << "\n\nPrinting argument string...\n";
  _::ArgsToString(args_count, args);
  cout << "\n" << args[1];
  return 0;
}

#endif  //< INCLUDED_SEAMS_0_0_SEAM_TESTS_TEST_SEAM_0
