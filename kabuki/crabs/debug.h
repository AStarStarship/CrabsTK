/* Kabuki Toolkit @version 0.x
@file    $kabuki-toolkit/kabuki/crabs/debug.h
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

#ifndef INCLUDED_CRABS_DEBUG
#define INCLUDED_CRABS_DEBUG

#include "config.h"

namespace _ {

/* Prints a single char to the console. */
KABUKI void Print(char c);

/* Prints a new line followed by a single char to the console. */
KABUKI void PrintLn(char c);

/* Prints a single string to the console. */
KABUKI void Print(const char* string);

/* Prints a new line followed by a single string to the console. */
KABUKI void PrintLn(const char* string);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
KABUKI void Print(int32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
KABUKI void Print(uint32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
KABUKI void Print(int64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
KABUKI void Print(uint64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
KABUKI void Print(float value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
KABUKI void Print(double value);

/* Prints a formatted string to the console. */
KABUKI void Printf(const char* format, ...);

/* Prints a new line followed by a formatted string to the console. */
KABUKI void PrintfLn(const char* format, ...);

/* Prints the given token char a specified number of times. */
KABUKI void PrintLine(int width = 80, char token = '-', char first_token = '-');

/* Prints a the given line_count of new lines follwed by a PrintLine, the
heading, follwed by another PrintLine. */
KABUKI void PrintHeading(const char* heading = nullptr, int line_count = 5,
                         int width = 80, char token = '-',
                         char first_token = '-');

/* Pauses the application until a key is presssed. */
KABUKI void Pause();

/* Prints a message then pauses the application until a key is presssed. */
KABUKI void Pause(const char* message);

/* Assert function to assist in casting boolean types. */
KABUKI bool Assert(bool condition);

/* Prints a debug statement and locks up the system. */
KABUKI bool AssertHandle(int line, const char* file,
                         const char* message = nullptr);

/* Prints a 8-bit binary value to the console. */
KABUKI void PrintBinary(uint8_t value);

/* Prints a 8-bit binary value to the console. */
KABUKI void PrintBinary(int8_t value);

/* Prints a 16-bit binary value to the console. */
KABUKI void PrintBinary(uint16_t value);

/* Prints a 16-bit binary value to the console. */
KABUKI void PrintBinary(int16_t value);

/* Prints a 32-bit binary value to the console. */
KABUKI void PrintBinary(uint32_t value);

/* Prints a 32-bit binary value to the console. */
KABUKI void PrintBinary(int32_t value);

/* Prints a 64-bit binary value to the console. */
KABUKI void PrintBinary(uint64_t value);

/* Prints a 64-bit binary value to the console. */
KABUKI void PrintBinary(int64_t value);

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

#define DEBUG(condition, message) \
  if (Assert(condition)) AssertHandle(__LINE__, __FILE__, message)
#endif  //< INCLUDED_CRABS_DEBUG
