/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/console.h
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
#ifndef HEADER_FOR_CRABS_CONSOLE
#define HEADER_FOR_CRABS_CONSOLE
#include "config.h"

namespace _ {

/* Prints a single char to the console. */
KABUKI void Print(char_t c);

/* Prints a new line followed by a single char to the console. */
KABUKI void PrintLn(char_t c);

/* Prints a single string to the console. */
KABUKI void Print(const char_t* string);

/* Prints a new line followed by a single string to the console. */
KABUKI void PrintLn(const char_t* string);

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
KABUKI void Printf(const char_t* format, ...);

/* Prints a new line followed by a formatted string to the console. */
KABUKI void PrintfLn(const char_t* format, ...);

}  // namespace _

#endif  //< #if HEADER_FOR_CRABS_CONSOLE
