/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/utf16.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_PRINT_UTF16
#define HEADER_FOR_CRABS_PRINT_UTF16
// Dependencies:
#include "config.h"
// End dependencies.
#if USING_UTF16

namespace _ {

/* UTF-8 printing utilities
@ingroup Printer
*/

/* Checks if the given character is whitespace.
 */
KABUKI bool IsWhitespace(char16_t character);

/* Converts the given value to a printable char16_t if it's non-printable. */
KABUKI char16_t PrintableChar(char16_t value);

/* Scrolls over to the next double quote mark.
@warning This function is only safe to use on ROM strings with a nil-term
char16_t. */
KABUKI const char16_t* TextEnd(const char16_t* text, char16_t delimiter = 0);

/* Gets the length of the given char16_t.
@return  Returns -1 if the text char16_t is nil.
@warning This function is only safe to use on ROM strings with a nil-term
char16_t. */
KABUKI int TextLength(const char16_t* text, char16_t delimiter = 0);

/* Clones the given string with given NON-ZERO delimiter.
@param  A nil-terminated string in ROM.
@return Returns a new copy you must delete. */
KABUKI char16_t* TextClone(const char16_t* text);

/* Clones the given string with given NON-ZERO delimiter.
@param  A nil-terminated string in ROM.
@return Returns a new copy you must delete. */
KABUKI char16_t* TextClone(const char16_t* text, char16_t delimiter);

/* Returns a pointer to the char16_t at the end of the line. */
KABUKI const char16_t* TextLineEnd(const char16_t* text, int column_count);

/* Returns the pointer to the next char16_t in the char16_t that is not an ASCII
number.
@return A pointer to the next non-number in the text char16_t. */
KABUKI const char16_t* TextNumberEnd(const char16_t* begin);

/* Returns the pointer to the next char16_t in the char16_t that is not an ASCII
number.
@return A pointer to the next non-number in the text char16_t. */
KABUKI const char16_t* TextNumberEnd(const char16_t* begin);

/* Skips the leading zeros of a number if there are any. */
KABUKI const char16_t* TextSkipChar(const char16_t* text, char16_t skip_char);

/* Skips all the spaces at the start of the char16_t. */
KABUKI const char16_t* TextSkipSpaces(const char16_t* text);

/* Compares the source and query char16_t as nil-terminated strings. */
KABUKI const char16_t* TextEquals(const char16_t* text_a,
                                  const char16_t* text_b,
                                  char16_t delimiter = 0);

/* Searches the given char16_t for the given char16_t.
@param  text      The char16_t to search.
@param  query      The char16_t to search for.
@param  delimiter The delimiter for the text, Example: '\"'
@return Returns nil if the parsing failed and a pointer to the first char16_t
after the end of the text upon success. */
KABUKI const char16_t* TextFind(const char16_t* begin, const char16_t* query,
                                char16_t delimiter = 0);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param  begin  The beginning address of the buffer.
@param  end    The end address of the buffer.
@param  string The potentially unsafe string to write. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, const char16_t* string);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin     Beginning address of the buffer.
@param end       The end address of the buffer.
@param character The value to write. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, char16_t character);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, uint32_t value);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, int32_t value);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, uint64_t value);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, int64_t value);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, float value);

/* Writes the give char16_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, double value);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end,
                             const char16_t* string, int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin Beginning address of the buffer.
@param end The end address of the buffer.
@param character The value to write. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, char16_t character,
                             int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, uint32_t valu,
                             int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, int32_t value,
                             int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, uint64_t value,
                             int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, int64_t value,
                             int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, float value,
                             int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintCenter(char16_t* begin, char16_t* end, double value,
                             int column_count);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end,
                            const char16_t* string, int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin     Beginning address of the buffer.
@param end       The end address of the buffer.
@param character The value to write. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, char16_t character,
                            int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, uint32_t value,
                            int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, int32_t value,
                            int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, uint64_t value,
                            int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, int64_t value,
                            int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, float value,
                            int column_count);

/* Writes the give char16_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintRight(char16_t* begin, char16_t* end, double value,
                            int column_count);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin   The beginning address of the buffer.
@param end     The end address of the buffer.
@param pointer The pointer to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, const void* pointer);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, uint8_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, int8_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, uint16_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, int16_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, uint32_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, int32_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, uint64_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, int64_t value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, float value);

/* Writes the give char16_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintHex(char16_t* begin, char16_t* end, double value);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param pointer The pointer to print to hex. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end,
                             const void* pointer);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, uint8_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, int8_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, uint16_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, int16_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, uint32_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, int32_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, uint64_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, int64_t value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, float value);

/* Writes the give char16_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char16_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char16_t* PrintBinary(char16_t* begin, char16_t* end, double value);

/* Prints out the contents of the address to the printer buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param start The beginning of the read buffer.
@param stop  The end of the read buffer. */
KABUKI char16_t* PrintMemory(char16_t* begin, char16_t* end, const void* start,
                             const void* stop);

/* Prints out the contents of the address to the printer buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param start The beginning of the read buffer.
@param size  The size of the read buffer. */
KABUKI char16_t* PrintMemory(char16_t* begin, char16_t* end, const void* start,
                             size_t size);
/* Converts the given string to a 8-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, int8_t& result);

/* Converts the given string to a 8-bit unsigned integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, uint8_t& result);

/* Converts the given string to a 16-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, int16_t& result);

/* Converts the given string to a 16-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, uint16_t& result);

/* Converts the given string to a 32-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, int32_t& result);

/* Converts the given string to a 32-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, uint32_t& result);

/* Converts the given string to a 64-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, int64_t& result);

/* Converts the given string to a 64-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, uint64_t& result);

/* Converts the given string to a 32-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, float& result);

/* Converts the given string to a 64-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char16_t after the end
of the read number or nil upon failure. */
KABUKI const char16_t* TextScan(const char16_t* text, double& result);

/* ASCII printing utilities
@ingroup Printer
*/

/* Utility class for printing strings.
This class only stores the end of buffer pointer and a pointer to the write
begin. It is up the user to store start of buffer pointer and if they would
like to replace the begin with the beginning of buffer pointer when they
are done printing.
*/
struct KABUKI Printer2 {
  char16_t *begin,  //< Write begin pointer.
      *end;         //< End of buffer pointer.

  /* Initializes the Printer& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Printer2(char16_t* begin, size_t size);

  /* Initializes the Printer& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Printer2(char16_t* begin, char16_t* end);

  /* Clones the other print. */
  Printer2(const Printer2& other);

  /* Sets the begin pointer to the new_pointer. */
  inline Printer2& Set(char16_t* new_pointer);

  /* Prints the given value as hex. */
  inline Printer2& Hex(int8_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(uint8_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(int16_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(uint16_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(int32_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(uint32_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(int64_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(uint64_t value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(float value);

  /* Prints the given value as hex. */
  inline Printer2& Hex(double value);

  /* Prints the given pointer as hex. */
  inline Printer2& Hex(const void* pointer);

  /* Prints the given value as binary. */
  inline Printer2& Binary(int8_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(uint8_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(int16_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(uint16_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(int32_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(uint32_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(int64_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(uint64_t value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(float value);

  /* Prints the given value as binary. */
  inline Printer2& Binary(double value);

  /* Prints the given pointer as binary. */
  inline Printer2& Binary(const void* pointer);
};

/* Utility class for printing numbers. */
class Text2 {
 public:
  /* Default constructor does nothing. */
  Text2();

  /* Prints the value to the text buffer. */
  Text2(char16_t character);

  /* Prints the value to the text buffer. */
  Text2(int32_t value);

  /* Prints the value to the text buffer. */
  Text2(uint32_t value);

  /* Prints the value to the text buffer. */
  Text2(int64_t value);

  /* Prints the value to the text buffer. */
  Text2(uint64_t value);

  /* Prints the value to the text buffer. */
  Text2(float value);

  /* Prints the value to the text buffer. */
  Text2(double value);

  /* Gets the number string. */
  const char16_t* GetString();

 private:
  enum { kSize = 24 };

  char16_t string[kSize * sizeof(char16_t) + 1];  //< String buffer.
};

/* Utility class for printing hex with operator<<. */
class Center2 {
 public:
  /* Prints the value to the text buffer. */
  Center2(const char16_t* string, int column_count);

  /* Prints the value to the text buffer. */
  Center2(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center2(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center2(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center2(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center2(float value, int column_count);

  /* Prints the value to the text buffer. */
  Center2(double value, int column_count);

  /* Gets the number string. */
  const char16_t* GetString();

  /* Gets the column_count. */
  int GetColumnCount();

 private:
  const char16_t* string;  //< Pointer to the string.
  Text2 number;            //< Pointer to a pointer to print.
  int column_count;        //< Number of columns to center.
};

/* Utility class for printing hex with operator<<. */
class Right2 {
 public:
  /* Prints the value to the text buffer. */
  Right2(const char16_t* string, int column_count);

  /* Prints the value to the text buffer. */
  Right2(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right2(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right2(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right2(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right2(float value, int column_count);

  /* Prints the value to the text buffer. */
  Right2(double value, int column_count);

  /* Gets the number string. */
  const char16_t* GetString();

  /* Gets the column_count. */
  int GetColumnCount();

 private:
  const char16_t* string;  //< Pointer to the string.
  Text2 number;            //< Pointer to a pointer to print.
  int column_count;        //< Number of columns to center.
};

}  // namespace _

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@param  printer The printer.
@param  value   The value to print.
@return The printer. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, const char16_t* string);

/* Writes the given value to the print.
@param  printer The printer.
@param  value   The value to print.
@return The printer. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, char16_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, uint8_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, int16_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, uint16_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, int32_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, uint32_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, int64_t value);

/* Writes the given value to the print.
@return The printer.
@desc
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, uint64_t value);

/* Writes the given value to the print.
@return The printer.
@desc
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, float value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, double value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print justified center. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, _::Center2 item);

/* Writes the given value to the print jusified right.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer2& operator<<(_::Printer2& printer, _::Right2 item);

#endif  //< #if USING_UTF16
#endif  //< #if HEADER_FOR_CRABS_PRINT_UTF16
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
