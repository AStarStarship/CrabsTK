/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/cconsole.h
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

#ifndef INCLUDED_KABUKI_F2_CONSOLE
#define INCLUDED_KABUKI_F2_CONSOLE 1

namespace _ {

/* Converts int main(int,char**) arguments back into a string.
@return nil if there are no arguments or the args string upon success.
@param arg_count The number of arguments.
@param args      The arguments. */
API const char* ArgsToString(int arg_count, char** args);

/* Prints a single char to the console. */
API inline void Print(char c);

/* Prints a single char to the console. */
API void Print(char first, char second);

/* Prints a single char to the console. */
API void Print(char first, char second, char third);

/* Prints a single string to the console. */
API void Print(const char* string);

/* Prints a single string to the console. */
API void Print(const char16_t* string);

/* Prints a single string to the console. */
API void Print(const char32_t* string);

/* Prints a single string to the console followed by the given delimiter. */
API void Print(const char* string, char delimiter);

/* Prints two strings. */
API void Print(const char* a, const char* b);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API void Print(int64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API void Print(uint64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API void Print(int32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API void Print(uint32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API void Print(float value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API void Print(double value);

/* Prints a new line followed by a single char to the console. */
API inline void PrintLn(char c = '\n');

/* Prints a new line followed by a single char to the console. */
API inline void PrintLn(char first, char second);

/* Prints a new line followed by a single string to the console. */
API inline void PrintLn(const char* string);

/* Prints a formatted string to the console. */
API void Printf(const char* format, ...);

/* Prints a new line followed by a formatted string to the console. */
API void PrintfLn(const char* format, ...);

/* Prints a new line followed by the given number of spaces.
@param The number of spaces to indent. */
API void PrintIndent(int count = 1);

/* Prints the given token char a specified number of times. */
API void PrintLine(int width = 80, char token = '-', char first_token = '-');

/* Prints a the given line_count of new lines followed by a PrintLine, the
heading, followed by another PrintLine.
@param heading The heading to print.
@param line_count The number of new lines before the heading to print.
@param width The width of the line to print.
@param first_token The first token to print. */
API void PrintHeading(const char* heading = nullptr, int line_count = 5,
                      int width = 80, char token = '-', char first_token = '+');

/* Prints a the given line_count of new lines followed by a PrintLine, the
heading, followed by another PrintLine.
@param heading The heading to print.
@param line_count The number of new lines before the heading to print.
@param width The width of the line to print.
@param first_token The first token to print. */
API void PrintHeading(const char* heading_a, const char* heading_b,
                      int line_count = 5, int width = 80, char token = '-',
                      char first_token = '+');

/* Prints a message then pauses the application until a key is pressed. */
API void Pause(const char* message = "");

/* Prints a message then pauses the application until a key is pressed. */
API void Pausef(const char* message, ...);

/* Prints a 8-bit binary value to the console. */
API void PrintBinary(uint8_t value);

/* Prints a 8-bit binary value to the console. */
API void PrintBinary(int8_t value);

/* Prints a 16-bit binary value to the console. */
API void PrintBinary(uint16_t value);

/* Prints a 16-bit binary value to the console. */
API void PrintBinary(int16_t value);

/* Prints a 32-bit binary value to the console. */
API void PrintBinary(uint32_t value);

/* Prints a 32-bit binary value to the console. */
API void PrintBinary(int32_t value);

/* Prints a 64-bit binary value to the console. */
API void PrintBinary(uint64_t value);

/* Prints a 64-bit binary value to the console. */
API void PrintBinary(int64_t value);

/* Prints a 32-bit floating-point number to the console in binary. */
API void PrintBinary(float value);

/* Prints a 64-bit floating-point number to the console in binary. */
API void PrintBinary(double value);

/* Prints a pointer to the console in binary. */
API void PrintBinary(const void* value);

/* Prints a 8-bit binary value to the console to hex. */
API void PrintHex(uint8_t value);

/* Prints a 8-bit binary value to the console to hex. */
API void PrintHex(int8_t value);

/* Prints a 16-bit binary value to the console to hex. */
API void PrintHex(uint16_t value);

/* Prints a 16-bit binary value to the console to hex. */
API void PrintHex(int16_t value);

/* Prints a 32-bit binary value to the console to hex. */
API void PrintHex(uint32_t value);

/* Prints a 32-bit binary value to the console to hex. */
API void PrintHex(int32_t value);

/* Prints a 64-bit binary value to the console to hex. */
API void PrintHex(uint64_t value);

/* Prints a 64-bit binary value to the console to hex. */
API void PrintHex(int64_t value);

/* Prints a 32-bit floating-point number to the console in hex. */
API void PrintHex(float value);

/* Prints a 64-bit floating-point number to the console in hex. */
API void PrintHex(double value);

/* Prints a pointer to the console in hex. */
API void PrintHex(const void* value);

/* Reads the current key depressed on the keyboard.
@return A negative number (typically 1) if no keys are pressed. */
API int CInKey();

/* Prints the given socket to the stdout. */
API void PrintSocket(const char* begin, const char* end);

/* Prints the given socket to the stdout. */
API void PrintSocket(const void* begin, intptr_t size);

}  // namespace _

#endif  //< #ifndef INCLUDED_KABUKI_F2_CONSOLE
