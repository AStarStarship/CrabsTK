/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\f2\ctest.h
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
#include <pch.h>

#ifndef INCLUDED_KABUKI_F2_COMPARE
#define INCLUDED_KABUKI_F2_COMPARE 1

/* Function pointer prototype for a test case with command line argument
string.
@return Pointer to a string to print to the stdout.
@param seam_log Beginning of the seam log buffer.
@param seam_end End of the seam log buffer.
@param args     Command line argument string. */
typedef const char* (*TestCase)(char* seam_log, char* seam_end,
                                const char* args);

namespace _ {

/* Tests an array of TestCase(s).
@return 0 upon success or an app exit code upon failure. */
const char* TestTree(char* seam_log, char* seam_end, const char* args,
                     TestCase* seams, int node_count);

/* Prints a message when a TestCase completes without failure. */
bool TestBegin(char* seam_log, char* seam_end, const char* args);

/* Test function to assist in casting boolean types.
@return false if the condition is false.
@param condition The condition to check. */
API bool Test(bool condition);

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

/* Tests if the pointer is nil.
@return false if the condition is false.
@param  value The value to test.  */
API bool Test(const void* value);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(const char* a, const char* b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(const char16_t* a, const char16_t* b);

/* Tests if the given arguments are equal.
@return false if the condition is false.
@param  a Parameter a.
@param  b Parameter b. */
API bool Test(const char32_t* a, const char32_t* b);

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

/* Handles an assert.
@return True upon failure.
@param  line    The line the program failed at.
@param  file    The file the error occurred at.
@param  message An optional message to print. */
API bool TestWarn(const char* funciton, const char* file, int line);

/* Handles an assert.
@return True upon failure.
@param  line    The line the program failed at.
@param  file    The file the error occurred at.
@param  message An optional message to print. */
API bool ErrorFreeze(const char* funciton, const char* file, int line);
}  // namespace _

#endif  //< #ifndef INCLUDED_KABUKI_F2_COMPARE
