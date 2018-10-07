/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/str1.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_0_0_0__03
#ifndef INCLUDED_KABUKI_F2_UTF8
#define INCLUDED_KABUKI_F2_UTF8

#include "ascii_data.h"
#include "clock.h"

#if USING_UTF8

namespace _ {

/* UTF-8 printing utilities.
@ingroup Utf8
*/

/* Checks if the given character is whitespace. */
API bool IsWhitespace(char character);

/* Converts the given value to a printable char if it's non-printable. */
API char PrintableChar(char value);

/* Scrolls over to the next double quote mark.
@warning This function is only safe to use on ROM strings with a nil-term
char. */
API const char* TextEnd(const char* text);

/* Gets the length of the given char.
@return  Returns -1 if the text char is nil.
@warning This function is only safe to use on ROM strings with a nil-term
char. */
API int TextLength(const char* text);

/* Clones the given string.
@param  A nil-terminated string in ROM.
@return Returns a new copy you must delete. */
API char* TextClone(const char* text);

/* Returns a pointer to the char at the end of the line. */
API const char* TextLineEnd(const char* text, int column_count);

/* Returns a pointer to the char at the end of the row. */
API const char* TextLineEnd(const char* text, const char* text_end,
                            int column_count);

/* Returns the pointer to the next char in the char that is not an ASCII
number.
@return A pointer to the next non-number in the text char. */
API const char* TextNumberStop(const char* text, const char* text_end);

/* Returns the pointer to the next char in the char that is not an ASCII
number.
@return A pointer to the next non-number in the text char. */
API const char* TextNumberStop(const char* text);

/* Skips the leading zeros of a number if there are any. */
API const char* TextSkipChar(const char* text, char skip_char);

/* Skips all the spaces at the start of the char. */
API const char* TextSkipSpaces(const char* text);

/* Skips all the spaces at the start of the char.
@param  begin Beginning address of the read buffer.
@param  end   The end address of the input buffer.
@return A pointer to the end of the text read or if no text read. */
API const char* TextSkipSpaces(const char* text, const char* text_end);

/* Compares the source and query char as nil-terminated strings. */
API const char* StringEquals(const char* text_a, const char* text_b);

/* Compares the source and query char as nil-terminated strings. */
API const char* StringEquals(const char* text, const char* text_end,
                             const char* query);

/* Searches the given char for the given char.
@param  text      The char to search.
@param  query      The char to search for.
@return Returns nil if the parsing failed and a pointer to the first char
after the end of the text upon success. */
API const char* StringFind(const char* text, const char* query);

/* Prints the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param  begin  The beginning address of the buffer.
@param  end    The end address of the buffer.
@param  string The potentially unsafe string to write. */
API char* Print(char* begin, char* end, const char* string);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin     Beginning address of the buffer.
@param end       The end address of the buffer.
@param character The value to write. */
API char* Print(char* begin, char* end, char character);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* Print(char* begin, char* end, uint32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* Print(char* begin, char* end, int32_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* Print(char* begin, char* end, uint64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* Print(char* begin, char* end, int64_t value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* Print(char* begin, char* end, float value);

/* Writes the give char to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* Print(char* begin, char* end, double value);

/* Prints the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
API char* PrintCenter(char* begin, char* end, const char* string,
                      int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin Beginning address of the buffer.
@param end The end address of the buffer.
@param character The value to write. */
API char* PrintCenter(char* begin, char* end, char character, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintCenter(char* begin, char* end, uint32_t valu, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintCenter(char* begin, char* end, int32_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintCenter(char* begin, char* end, uint64_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintCenter(char* begin, char* end, int64_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintCenter(char* begin, char* end, float value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintCenter(char* begin, char* end, double value, int column_count);

/* Prints the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
API char* PrintRight(char* begin, char* end, const char* string,
                     int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin     Beginning address of the buffer.
@param end       The end address of the buffer.
@param character The value to write. */
API char* PrintRight(char* begin, char* end, char character, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintRight(char* begin, char* end, uint32_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintRight(char* begin, char* end, int32_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintRight(char* begin, char* end, uint64_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintRight(char* begin, char* end, int64_t value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintRight(char* begin, char* end, float value, int column_count);

/* Writes the give char to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintRight(char* begin, char* end, double value, int column_count);

/* Prints the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
API char* PrintHex(char* begin, char* end, const void* pointer);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, int8_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, uint16_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, int16_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, uint32_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, int32_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, uint64_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, int64_t value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, float value);

/* Writes the give char to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintHex(char* begin, char* end, double value);

/* Prints the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param pointer The pointer to print to hex. */
API char* PrintBinary(char* begin, char* end, const void* pointer);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, uint8_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, int8_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, uint16_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, int16_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, uint32_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, int32_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, uint64_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, int64_t value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, float value);

/* Writes the give char to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
API char* PrintBinary(char* begin, char* end, double value);

/* Prints the given memory socket to the text buffer. */
API char* PrintMemory(char* begin, char* end, const void* start, size_t size);

/* Prints out the contents of the address to the utf buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param start The beginning of the read buffer.
@param stop  The end of the read buffer. */
API char* PrintMemory(char* begin, char* end, const void* start,
                      const void* stop);

/* Prints a line of the given column_count.
@return Returns a pointer to the next char after the end of the read number or
nil upon failure.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param token The token to print.
@param column_count The number of tokens to print. */
API char* PrintLine(char* cursor, char* end, char token, int column_count);

/* Prints a line of the given column_count.
@return Returns a pointer to the next char after the end of the read number or
nil upon failure.
@param begin  The beginning of the write buffer.
@param end    The end of the write buffer.
@param string The string to print.
@param column_count The number of columns. */
API char* PrintLineString(char* cursor, char* end, const char* string,
                          int column_count);

/* Converts the given string to a 8-bit signed integer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion. */
API const char* Scan(const char* begin, int8_t& result);

/* Converts the given string to a 8-bit signed integer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion. */
API const char* Scan(const char* text, int8_t& result);

/* Converts the given string to a 8-bit unsigned integer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion. */
API const char* Scan(const char* text, uint8_t& result);

/* Converts the given string to a 16-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, int16_t& result);

/* Converts the given string to a 16-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, uint16_t& result);

/* Converts the given string to a 32-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, int32_t& result);

/* Converts the given string to a 32-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, uint32_t& result);

/* Converts the given string to a 64-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, int64_t& result);

/* Converts the given string to a 64-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, uint64_t& result);

/* Converts the given string to a 32-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, float& result);

/* Converts the given string to a 64-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure. */
API const char* Scan(const char* text, double& result);

/* Prints the buffer to the console as a UTF-8 string. */
void COutUtf8(uintptr_t* buffer);

/* Prints the buffer to the console as a UTF-8 string. */
void COutAutoUtf8(uintptr_t* buffer);

/* Utility class for printing strings.
This class only stores the end of buffer pointer and a pointer to the write
begin. It is up the user to store start of buffer pointer and if they would
like to replace the begin with the beginning of buffer pointer when they
are done printing.
*/
struct API Utf8 {
  char *begin,  //< Write begin pointer.
      *end;     //< End of buffer pointer.

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Utf8(char* begin, size_t size);

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Utf8(char* begin, char* end);

  /* Clones the other print. */
  Utf8(const Utf8& other);

  /* Sets the begin pointer to the new_pointer. */
  inline Utf8& Set(char* new_pointer);

  /* Prints the given value as hex. */
  inline Utf8& Hex(int8_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(uint8_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(int16_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(uint16_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(int32_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(uint32_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(int64_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(uint64_t value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(float value);

  /* Prints the given value as hex. */
  inline Utf8& Hex(double value);

  /* Prints the given pointer as hex. */
  inline Utf8& Hex(const void* pointer);

  /* Prints the given value as binary. */
  inline Utf8& Binary(int8_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(uint8_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(int16_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(uint16_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(int32_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(uint32_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(int64_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(uint64_t value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(float value);

  /* Prints the given value as binary. */
  inline Utf8& Binary(double value);

  /* Prints the given pointer as binary. */
  inline Utf8& Binary(const void* pointer);
};

/* Utility class for printing numbers. */
class Utf8Text {
 public:
  /* Default constructor does nothing. */
  Utf8Text();

  /* Prints the value to the text buffer. */
  Utf8Text(char character);

  /* Prints the value to the text buffer. */
  Utf8Text(int32_t value);

  /* Prints the value to the text buffer. */
  Utf8Text(uint32_t value);

  /* Prints the value to the text buffer. */
  Utf8Text(int64_t value);

  /* Prints the value to the text buffer. */
  Utf8Text(uint64_t value);

  /* Prints the value to the text buffer. */
  Utf8Text(float value);

  /* Prints the value to the text buffer. */
  Utf8Text(double value);

  /* Gets the number string. */
  const char* String();

 private:
  enum { kSize = 24 };

  char string[kSize * sizeof(char) + 1];  //< String buffer.
};

/* Utility class for printing hex with operator<<. */
class Utf8Center {
 public:
  /* Prints the value to the text buffer. */
  Utf8Center(const char* string, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Center(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Center(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Center(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Center(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Center(float value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Center(double value, int column_count);

  /* Gets the number string. */
  const char* String();

  /* Gets the column_count. */
  int GetColumnCount();

 private:
  const char* string;  //< Pointer to the string.
  Utf8Text number;     //< Pointer to a pointer to print.
  int column_count;    //< Number of columns to center.
};

/* Utility class for printing hex with operator<<. */
class Utf8Right {
 public:
  /* Prints the value to the text buffer. */
  Utf8Right(const char* string, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Right(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Right(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Right(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Right(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Right(float value, int column_count);

  /* Prints the value to the text buffer. */
  Utf8Right(double value, int column_count);

  /* Gets the number string. */
  const char* String();

  /* Gets the column_count. */
  int GetColumnCount();

 private:
  const char* string;  //< Pointer to the string.
  Utf8Text number;     //< Pointer to a pointer to print.
  int column_count;    //< Number of columns to center.
};

/* Utility class for printing a single char token line with operator<<. */
struct API Utf8Line {
  char token;        //< Character to print.
  int column_count;  //< Column count.

  /* Constructor. */
  Utf8Line(char token, int column_count);
};

/* Utility class for printing a string line with operator<<. */
struct API Utf8LineString {
  const char* string;  //< Character to print.
  int column_count;    //< Column count.

  /* Constructor. */
  Utf8LineString(const char* string, int column_count);
};

class API Utf8String {
 public:
  /* Constructs a bank UTF-8 string with dynamic memory. */
  Utf8String();

  /* Calls the destructor if there is one. */
  ~Utf8String();

 private:
};

}  // namespace _

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
API _::Utf8& operator<<(_::Utf8& utf, const char* string);

/* Writes the given value to the print.
@param  utf The utf.
@param  value   The value to print.
@return The utf. */
API _::Utf8& operator<<(_::Utf8& utf, char value);

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
API _::Utf8& operator<<(_::Utf8& utf, uint8_t value);

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
API _::Utf8& operator<<(_::Utf8& utf, int16_t value);

/* Writes the given value to the print.
@param  utf The utf.
@param  value The value to write to the print.
@return The utf. */
API _::Utf8& operator<<(_::Utf8& utf, uint16_t value);

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
API _::Utf8& operator<<(_::Utf8& utf, int32_t value);

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
API _::Utf8& operator<<(_::Utf8& utf, uint32_t value);

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
API _::Utf8& operator<<(_::Utf8& utf, int64_t value);

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
API _::Utf8& operator<<(_::Utf8& utf, uint64_t value);

/* Writes the given value to the print.
@return The utf.
@desc
@param  utf The utf.
@param  value The value to write to the print. */
API _::Utf8& operator<<(_::Utf8& utf, float value);

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  value The value to write to the print. */
API _::Utf8& operator<<(_::Utf8& utf, double value);

/* Writes the given value to the print.
@return The utf.
@param  utf The utf.
@param  item The item to write to print. */
API _::Utf8& operator<<(_::Utf8& utf, _::Utf8Center item);

/* Writes the given value to the print justified right.
@return The utf.
@param  utf The utf.
@param  item The item to print. */
API _::Utf8& operator<<(_::Utf8& utf, _::Utf8Right item);

/* Prints a line of the given column_count to the utf. */
API _::Utf8& operator<<(_::Utf8& utf, _::Utf8Line line);

/* Prints a line string of the given column_count to the utf. */
API _::Utf8& operator<<(_::Utf8& utf, _::Utf8LineString line);

#endif  //< #if USING_UTF8
#endif  //< #if INCLUDED_KABUKI_F2_UTF8
#endif  //< #if SEAM >= SEAM_0_0_0__03
