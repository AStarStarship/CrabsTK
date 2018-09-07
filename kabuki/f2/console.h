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

#ifndef INCLUDED_CRABS_CONSOLE
#define INCLUDED_CRABS_CONSOLE

namespace _ {

struct API COut {
  /* Default constructor does nothing. */
  COut();

  /* Prints a single char to the console. */
  COut(char c);

  /* Prints a single char to the console. */
  COut(char first, char second);

  /* Prints a single char to the console. */
  COut(char first, char second, char third);

  /* Prints a single string to the console. */
  COut(const char* string);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  COut(int64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  COut(uint64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  COut(int32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  COut(uint32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  COut(float value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  COut(double value);

  /* Prints a single char to the console. */
  inline COut Print(char c = '\n');

  /* Prints a single char to the console. */
  inline COut Print(char first, char second);

  /* Prints a single char to the console. */
  inline COut Print(char first, char second, char third);

  /* Prints a single string to the console. */
  inline COut Print(const char* string);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline COut Print(int64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline COut Print(uint64_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline COut Print(int32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline COut Print(uint32_t value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline COut Print(float value);

  /* Writes the give char to the given buffer.
  @return Returns nil upon buffer overflow and a pointer to the nil-term char
  upon success.
  @param value The value to print. */
  inline COut Print(double value);

  /* Prints a new line followed by a single char to the console. */
  inline COut PrintLn(char c = '\n');

  /* Prints a new line followed by a single char to the console. */
  inline COut PrintLn(char first, char second);

  /* Prints a new line followed by a single string to the console. */
  inline COut PrintLn(const char* string);

  /* Prints a formatted string to the console. */
  inline COut Printf(const char* format, ...);

  /* Prints a new line followed by a formatted string to the console. */
  inline COut PrintfLn(const char* format, ...);

  /* Prints the given token char a specified number of times. */
  inline COut Line(int width = 80, char token = '-', char first_token = '-');

  /* Prints a the given line_count of new lines follwed by a PrintLine, the
  heading, follwed by another PrintLine. */
  inline COut Heading(const char* heading = nullptr, int line_count = 5,
                      int width = 80, char token = '-', char first_token = '-');

  /* Prints a 8-bit binary value to the console. */
  inline COut Binary(uint8_t value);

  /* Prints a 8-bit binary value to the console. */
  inline COut Binary(int8_t value);

  /* Prints a 16-bit binary value to the console. */
  inline COut Binary(uint16_t value);

  /* Prints a 16-bit binary value to the console. */
  inline COut Binary(int16_t value);

  /* Prints a 32-bit binary value to the console. */
  inline COut Binary(uint32_t value);

  /* Prints a 32-bit binary value to the console. */
  inline COut Binary(int32_t value);

  /* Prints a 64-bit binary value to the console. */
  inline COut Binary(uint64_t value);

  /* Prints a 64-bit binary value to the console. */
  inline COut Binary(int64_t value);

  /* Prints a 8-bit binary value to the console. */
  inline COut Hex(uint8_t value);

  /* Prints a 8-bit binary value to the console. */
  inline COut Hex(int8_t value);

  /* Prints a 16-bit binary value to the console. */
  inline COut Hex(uint16_t value);

  /* Prints a 16-bit binary value to the console. */
  inline COut Hex(int16_t value);

  /* Prints a 32-bit binary value to the console. */
  inline COut Hex(uint32_t value);

  /* Prints a 32-bit binary value to the console. */
  inline COut Hex(int32_t value);

  /* Prints a 64-bit binary value to the console. */
  inline COut Hex(uint64_t value);

  /* Prints a 64-bit binary value to the console. */
  inline COut Hex(int64_t value);

  /* Prints a 32-bit floating-point number to the console. */
  inline COut Hex(float value);

  /* Prints a 64-bit floating-point number to the console. */
  inline COut Hex(double value);

  /* Pauses the application until a key is presssed. */
  inline COut Pause();

  /* Prints a message then pauses the application until a key is presssed. */
  inline COut Pause(const char* message);
};

struct API CHex {
  /* Prints a 8-bit binary value to the console in hex. */
  CHex(uint8_t value);

  /* Prints a 8-bit binary value to the console in hex. */
  CHex(int8_t value);

  /* Prints a 16-bit binary value to the console in hex. */
  CHex(uint16_t value);

  /* Prints a 16-bit binary value to the console in hex. */
  CHex(int16_t value);

  /* Prints a 32-bit binary value to the console in hex. */
  CHex(uint32_t value);

  /* Prints a 32-bit binary value to the console in hex. */
  CHex(int32_t value);

  /* Prints a 64-bit binary value to the console in hex. */
  CHex(uint64_t value);

  /* Prints a 64-bit binary value to the console in hex. */
  CHex(int64_t value);

  /* Prints a 32-bit floating-point number to the console in hex. */
  CHex(float value);

  /* Prints a 64-bit floating-point number to the console in hex. */
  CHex(double value);

  /* Prints a pointer to the console in hex. */
  CHex(void* pointer);
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
  CBinary(void* pointer);
};

/* Prints a single char to the console. */
inline void Print(char c);

/* Prints a single char to the console. */
API void Print(char first, char second);

/* Prints a single char to the console. */
API void Print(char first, char second, char third);

/* Prints a single string to the console. */
API void Print(const char* string);

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
inline void PrintLn(char c = '\n');

/* Prints a new line followed by a single char to the console. */
inline void PrintLn(char first, char second);

/* Prints a new line followed by a single string to the console. */
inline void PrintLn(const char* string);

/* Prints a formatted string to the console. */
API void Printf(const char* format, ...);

/* Prints a new line followed by a formatted string to the console. */
API void PrintfLn(const char* format, ...);

/* Prints the given token char a specified number of times. */
API void PrintLine(int width = 80, char token = '-', char first_token = '-');

/* Prints a the given line_count of new lines follwed by a PrintLine, the
heading, follwed by another PrintLine. */
API void PrintHeading(const char* heading = nullptr, int line_count = 5,
                      int width = 80, char token = '-', char first_token = '-');

/* Pauses the application until a key is presssed. */
API void Pause();

/* Prints a message then pauses the application until a key is presssed. */
API void Pause(const char* message);

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
API void PrintBinary(void* value);

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
API void PrintHex(void* value);

/* Reads the current key depressed on the keyboard.
@return A negative number (typically 1) if no keys are pressed. */
API int KeyboardRead();

}  // namespace _
#endif  //< #ifndef INCLUDED_F2_CONSOLE
