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
#include <pch.h>

#if SEAM >= SEAM_00_00_00__01
#ifndef INCLUDED_F2_CONSOLE
#define INCLUDED_F2_CONSOLE 1

namespace _ {

/* Converts int main(int,char**) arguments back into a string.
@return nil if there are no arguments or the args string upon success.
@param arg_count The number of arguments.
@param args      The arguments. */
const char* ConsoleArgs(int arg_count, char** args);

class API Console {
 public:
  /* Default constructor does nothing. */
  Console();

  /* Prints a single char to the console. */
  Console(char c);

  /* Prints a single char to the console. */
  Console(char first, char second);

  /* Prints a single char to the console. */
  Console(char first, char second, char third);

  /* Prints a single string to the console. */
  Console(const char* string);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  Console(int64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  Console(uint64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  Console(int32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  Console(uint32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  Console(float value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  Console(double value);

  /* Prints a single char to the console. */
  inline Console& Print(char c = '\n');

  /* Prints a single char to the console. */
  inline Console& Print(char first, char second);

  /* Prints a single char to the console. */
  inline Console& Print(char first, char second, char third);

  /* Prints a single string to the console. */
  inline Console& Print(const char* string);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline Console& Print(int64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline Console& Print(uint64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline Console& Print(int32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline Console& Print(uint32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline Console& Print(float value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline Console& Print(double value);

  /* Prints a new line followed by a single char to the console. */
  inline Console& PrintLn(char c = '\n');

  /* Prints a new line followed by a single char to the console. */
  inline Console& PrintLn(char first, char second);

  /* Prints a new line followed by a single string to the console. */
  inline Console& PrintLn(const char* string);

  /* Prints a formatted string to the console. */
  inline Console& Printf(const char* format, ...);

  /* Prints a new line followed by a formatted string to the console. */
  inline Console& PrintfLn(const char* format, ...);

  /* Prints the given token char a specified number of times. */
  inline Console& Line(int width = 80, char token = '-',
                       char first_token = '-');

  /* Prints a the given line_count of new lines followed by a PrintLine, the
  heading, followed by another PrintLine. */
  inline Console& Heading(const char* heading = nullptr, int line_count = 5,
                          int width = 80, char token = '-',
                          char first_token = '-');

  /* Prints a 8-bit binary value to the console. */
  inline Console& Binary(uint8_t value);

  /* Prints a 8-bit binary value to the console. */
  inline Console& Binary(int8_t value);

  /* Prints a 16-bit binary value to the console. */
  inline Console& Binary(uint16_t value);

  /* Prints a 16-bit binary value to the console. */
  inline Console& Binary(int16_t value);

  /* Prints a 32-bit binary value to the console. */
  inline Console& Binary(uint32_t value);

  /* Prints a 32-bit binary value to the console. */
  inline Console& Binary(int32_t value);

  /* Prints a 64-bit binary value to the console. */
  inline Console& Binary(uint64_t value);

  /* Prints a 64-bit binary value to the console. */
  inline Console& Binary(int64_t value);

  /* Prints a 8-bit binary value to the console. */
  inline Console& Hex(uint8_t value);

  /* Prints a 8-bit binary value to the console. */
  inline Console& Hex(int8_t value);

  /* Prints a 16-bit binary value to the console. */
  inline Console& Hex(uint16_t value);

  /* Prints a 16-bit binary value to the console. */
  inline Console& Hex(int16_t value);

  /* Prints a 32-bit binary value to the console. */
  inline Console& Hex(uint32_t value);

  /* Prints a 32-bit binary value to the console. */
  inline Console& Hex(int32_t value);

  /* Prints a 64-bit binary value to the console. */
  inline Console& Hex(uint64_t value);

  /* Prints a 64-bit binary value to the console. */
  inline Console& Hex(int64_t value);

  /* Prints a pointer to the console. */
  inline Console& Hex(const void* ptr);

  /* Prints a 32-bit floating-point number to the console. */
  inline Console& Hex(float value);

  /* Prints a 64-bit floating-point number to the console. */
  inline Console& Hex(double value);

  /* Prints a message then pauses the application until a key is pressed. */
  inline Console& Pause(const char* message = "");
};

struct API CHex {
  /* Prints a pointer to the console in hex. */
  CHex(const void* pointer);

  /* Prints a 8-bit hex value to the console in hex. */
  CHex(uint8_t value);

  /* Prints a 8-bit hex value to the console in hex. */
  CHex(int8_t value);

  /* Prints a 16-bit hex value to the console in hex. */
  CHex(uint16_t value);

  /* Prints a 16-bit hex value to the console in hex. */
  CHex(int16_t value);

  /* Prints a 32-bit hex value to the console in hex. */
  CHex(uint32_t value);

  /* Prints a 32-bit hex value to the console in hex. */
  CHex(int32_t value);

  /* Prints a 64-bit hex value to the console in hex. */
  CHex(uint64_t value);

  /* Prints a 64-bit hex value to the console in hex. */
  CHex(int64_t value);

  /* Prints a 32-bit floating-point number to the console in hex. */
  CHex(float value);

  /* Prints a 64-bit floating-point number to the console in hex. */
  CHex(double value);
};

struct API CBinary {
  /* Prints a 8-bit binary value to the console in binary. */
  CBinary(uint8_t value);

  /* Prints a 8-bit binary value to the console in binary. */
  CBinary(int8_t value);

  /* Prints a 16-bit binary value to the console in binary. */
  CBinary(uint16_t value);

  /* Prints a 16-bit binary value to the console in binary. */
  CBinary(int16_t value);

  /* Prints a 32-bit binary value to the console in binary. */
  CBinary(uint32_t value);

  /* Prints a 32-bit binary value to the console in binary. */
  CBinary(int32_t value);

  /* Prints a 64-bit binary value to the console in binary. */
  CBinary(uint64_t value);

  /* Prints a 64-bit binary value to the console in binary. */
  CBinary(int64_t value);

  /* Prints a 32-bit floating-point number to the console in binary. */
  CBinary(float value);

  /* Prints a 64-bit floating-point number to the console in binary. */
  CBinary(double value);

  /* Prints a pointer to the console in binary. */
  CBinary(const void* pointer);
};

/* Prints a single char to the console. */
API inline Console Print(char c);

/* Prints a single char to the console. */
API Console Print(char first, char second);

/* Prints a single char to the console. */
API Console Print(char first, char second, char third);

/* Prints a single string to the console. */
API Console Print(const char* string);

/* Prints a single string to the console followed by the given delimiter. */
API Console Print(const char* string, char delimiter);

/* Prints two strings. */
API Console Print(const char* a, const char* b);

/* Prints three strings. */
API Console Print(const char* a, const char* b, const char* c);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API Console Print(int64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API Console Print(uint64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API Console Print(int32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API Console Print(uint32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API Console Print(float value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param value The value to print. */
API Console Print(double value);

/* Prints a new line followed by a single char to the console. */
API inline Console PrintLn(char c = '\n');

/* Prints a new line followed by a single char to the console. */
API inline Console PrintLn(char first, char second);

/* Prints a new line followed by a single string to the console. */
API inline Console PrintLn(const char* string);

/* Prints a formatted string to the console. */
API Console Printf(const char* format, ...);

/* Prints a new line followed by a formatted string to the console. */
API Console PrintfLn(const char* format, ...);

/* Prints a new line followed by the given number of spaces.
@param The number of spaces to indent. */
API Console PrintIndent(int count = 1);

/* Prints the given token char a specified number of times. */
API Console PrintLine(int width = 80, char token = '-', char first_token = '-');

/* Prints a the given line_count of new lines followed by a PrintLine, the
heading, followed by another PrintLine.
@param heading The heading to print.
@param line_count The number of new lines before the heading to print.
@param width The width of the line to print.
@param first_token The first token to print. */
API Console PrintHeading(const char* heading = nullptr, int line_count = 5,
                         int width = 80, char token = '-',
                         char first_token = '+');

/* Prints a the given line_count of new lines followed by a PrintLine, the
heading, followed by another PrintLine.
@param heading The heading to print.
@param line_count The number of new lines before the heading to print.
@param width The width of the line to print.
@param first_token The first token to print. */
API Console PrintHeading(const char* heading_a, const char* heading_b,
                         int line_count = 5, int width = 80, char token = '-',
                         char first_token = '+');

/* Prints a message then pauses the application until a key is pressed. */
API void Pause(const char* message = "");

/* Prints a message then pauses the application until a key is pressed. */
API void Pausef(const char* message, ...);

/* Prints a 8-bit binary value to the console. */
API Console PrintBinary(uint8_t value);

/* Prints a 8-bit binary value to the console. */
API Console PrintBinary(int8_t value);

/* Prints a 16-bit binary value to the console. */
API Console PrintBinary(uint16_t value);

/* Prints a 16-bit binary value to the console. */
API Console PrintBinary(int16_t value);

/* Prints a 32-bit binary value to the console. */
API Console PrintBinary(uint32_t value);

/* Prints a 32-bit binary value to the console. */
API Console PrintBinary(int32_t value);

/* Prints a 64-bit binary value to the console. */
API Console PrintBinary(uint64_t value);

/* Prints a 64-bit binary value to the console. */
API Console PrintBinary(int64_t value);

/* Prints a 32-bit floating-point number to the console in binary. */
API Console PrintBinary(float value);

/* Prints a 64-bit floating-point number to the console in binary. */
API Console PrintBinary(double value);

/* Prints a pointer to the console in binary. */
API Console PrintBinary(const void* value);

/* Prints a 8-bit binary value to the console to hex. */
API Console PrintHex(uint8_t value);

/* Prints a 8-bit binary value to the console to hex. */
API Console PrintHex(int8_t value);

/* Prints a 16-bit binary value to the console to hex. */
API Console PrintHex(uint16_t value);

/* Prints a 16-bit binary value to the console to hex. */
API Console PrintHex(int16_t value);

/* Prints a 32-bit binary value to the console to hex. */
API Console PrintHex(uint32_t value);

/* Prints a 32-bit binary value to the console to hex. */
API Console PrintHex(int32_t value);

/* Prints a 64-bit binary value to the console to hex. */
API Console PrintHex(uint64_t value);

/* Prints a 64-bit binary value to the console to hex. */
API Console PrintHex(int64_t value);

/* Prints a 32-bit floating-point number to the console in hex. */
API Console PrintHex(float value);

/* Prints a 64-bit floating-point number to the console in hex. */
API Console PrintHex(double value);

/* Prints a pointer to the console in hex. */
API Console PrintHex(const void* value);

/* Reads the current key depressed on the keyboard.
@return A negative number (typically 1) if no keys are pressed. */
API int CInKey();

}  // namespace _

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@The cout.
@param  cout  The cout.
@param  value The value to print. */
API inline _::Console& operator<<(_::Console& cout, const char* string);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value   The value to print. */
API inline _::Console& operator<<(_::Console& cout, char c);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, uint8_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, int16_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, uint16_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, int32_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, uint32_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, int64_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, uint64_t value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, float value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  value The value to write to the print. */
API inline _::Console& operator<<(_::Console& cout, double value);

/* Writes the given value to the print.
@The cout.
@param  cout The cout.
@param  item The item to write to print. */
API inline _::Console& operator<<(_::Console& cout, _::CHex& item);

/* Writes the given value to the print justified right.
@The cout.
@param  cout The cout.
@param  item The item to print. */
API inline _::Console& operator<<(_::Console& cout, _::CBinary& item);

#endif  //< #ifndef INCLUDED_KABUKI_F2_CONSOLE
#endif  //< #if SEAM >= SEAM_00_00_00__01
