/* Kabuki Toolkit @version 0.x @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/test.h
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

#ifndef INCLUDED_CRABS_CONSOLE
#define INCLUDED_CRABS_CONSOLE

namespace _ {

/* Prints a single char to the console. */
DLL void Print(char c);

/* Prints a new line followed by a single char to the console. */
DLL void PrintLn(char c);

/* Prints a single string to the console. */
DLL void Print(const char* string);

/* Prints a new line followed by a single string to the console. */
DLL void PrintLn(const char* string);

#if WORD_SIZE == 32

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
DLL void Print(int32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
DLL void Print(uint32_t value);
#endif

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
DLL void Print(int64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
DLL void Print(uint64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
DLL void Print(float value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
DLL void Print(double value);

/* Prints a formatted string to the console. */
DLL void Printf(const char* format, ...);

/* Prints a new line followed by a formatted string to the console. */
DLL void PrintfLn(const char* format, ...);

/* Prints the given token char a specified number of times. */
DLL void PrintLine(int width = 80, char token = '-', char first_token = '-');

/* Prints a the given line_count of new lines follwed by a PrintLine, the
heading, follwed by another PrintLine. */
DLL void PrintHeading(const char* heading = nullptr, int line_count = 5,
                      int width = 80, char token = '-', char first_token = '-');

/* Pauses the application until a key is presssed. */
DLL void Pause();

/* Prints a message then pauses the application until a key is presssed. */
DLL void Pause(const char* message);

/* Prints a 8-bit binary value to the console. */
DLL void PrintBinary(uint8_t value);

/* Prints a 8-bit binary value to the console. */
DLL void PrintBinary(int8_t value);

/* Prints a 16-bit binary value to the console. */
DLL void PrintBinary(uint16_t value);

/* Prints a 16-bit binary value to the console. */
DLL void PrintBinary(int16_t value);

/* Prints a 32-bit binary value to the console. */
DLL void PrintBinary(uint32_t value);

/* Prints a 32-bit binary value to the console. */
DLL void PrintBinary(int32_t value);

/* Prints a 64-bit binary value to the console. */
DLL void PrintBinary(uint64_t value);

/* Prints a 64-bit binary value to the console. */
DLL void PrintBinary(int64_t value);

/* Prints a 8-bit binary value to the console. */
DLL void PrintHex(uint8_t value);

/* Prints a 8-bit binary value to the console. */
DLL void PrintHex(int8_t value);

/* Prints a 16-bit binary value to the console. */
DLL void PrintHex(uint16_t value);

/* Prints a 16-bit binary value to the console. */
DLL void PrintHex(int16_t value);

/* Prints a 32-bit binary value to the console. */
DLL void PrintHex(uint32_t value);

/* Prints a 32-bit binary value to the console. */
DLL void PrintHex(int32_t value);

/* Prints a 64-bit binary value to the console. */
DLL void PrintHex(uint64_t value);

/* Prints a 64-bit binary value to the console. */
DLL void PrintHex(int64_t value);

/* Reads the current key depressed on the keyboard.
@return A negative number (typically 1) if no keys are pressed. */
DLL int KeyboardRead();

}  // namespace _
#endif  //< #ifndef INCLUDED_F2_CONSOLE
