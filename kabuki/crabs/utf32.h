/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/utf32.h
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
#ifndef HEADER_FOR_CRABS_PRINT_UTF32
#define HEADER_FOR_CRABS_PRINT_UTF32
// Dependencies:
#include "config.h"
// End dependencies.
#if USING_UTF32

namespace _ {

/* UTF-8 printing utilities
@ingroup Printer
*/

/* Checks if the given character is whitespace.
 */
KABUKI bool IsWhitespace(char32_t character);

/* Converts the given value to a printable char32_t if it's non-printable. */
KABUKI char32_t Char(char32_t value);

/* Scrolls over to the next double quote mark.
@warning This function is only safe to use on ROM strings with a nil-term
char32_t. */
KABUKI const char32_t* TextEnd(const char32_t* begin, char32_t delimiter = 0);

/* Gets the length of the given char32_t.
@return  Returns -1 if the text char32_t is nil.
@warning This function is only safe to use on ROM strings with a nil-term
char32_t. */
KABUKI int TextLength(const char32_t* begin, char32_t delimiter = 0);

/* Clones the given string with given NON-ZERO delimiter.
@param  A nil-terminated string in ROM.
@return Returns a new copy you must delete. */
KABUKI char32_t* TextClone(const char32_t* begin, char32_t delimiter = 0);

/* Returns a pointer to the char32_t at the end of the line. */
KABUKI const char32_t* TextLineEnd(const char32_t* begin, int column_count);

/* Returns the pointer to the next char32_t in the char32_t that is not an ASCII
number.
@return A pointer to the next non-number in the text char32_t. */
KABUKI const char32_t* TextNumberEnd(const char32_t* begin);

/* Skips the leading zeros of a number if there are any. */
KABUKI const char32_t* TextSkipChar(const char32_t* begin, char32_t skip_char);

/* Skips all the spaces at the start of the char32_t. */
KABUKI const char32_t* TextSkipSpaces(const char32_t* begin);

/* Skips all the spaces at the start of the char32_t.
@param  begin Beginning address of the read buffer.
@param  end   The end address of the input buffer.
@return A pointer to the end of the text read or if no text read. */
KABUKI const char32_t* TextSkipSpaces(const char32_t* begin,
                                      const char32_t* text_end);

/* Compares the source and query char32_t as nil-terminated strings. */
KABUKI const char32_t* TextEquals(const char32_t* text_a,
                                  const char32_t* text_b,
                                  char32_t delimiter = 0);

/* Compares the source and query char32_t as nil-terminated strings. */
KABUKI const char32_t* TextEquals(const char32_t* begin,
                                  const char32_t* text_end,
                                  const char32_t* query,
                                  char32_t delimiter = 0);

/* Searches the given char32_t for the given char32_t.
@param  text      The char32_t to search.
@param  query      The char32_t to search for.
@param  delimiter The delimiter for the text, Example: '\"'
@return Returns nil if the parsing failed and a pointer to the first char32_t
after the end of the text upon success. */
KABUKI const char32_t* TextFind(const char32_t* begin, const char32_t* query,
                                char32_t delimiter = 0);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param  begin  The beginning address of the buffer.
@param  end    The end address of the buffer.
@param  string The potentially unsafe string to write. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, const char32_t* string);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin     Beginning address of the buffer.
@param end       The end address of the buffer.
@param character The value to write. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, char32_t character);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, uint32_t value);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, int32_t value);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, uint64_t value);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, int64_t value);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, float value);

/* Writes the give char32_t to the given buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, double value);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end,
                             const char32_t* string, int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin Beginning address of the buffer.
@param end The end address of the buffer.
@param character The value to write. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, char32_t character,
                             int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, uint32_t valu,
                             int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, int32_t value,
                             int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, uint64_t value,
                             int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, int64_t value,
                             int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, float value,
                             int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintCenter(char32_t* begin, char32_t* end, double value,
                             int column_count);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param string The potentially unsafe string to write. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end,
                            const char32_t* string, int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin     Beginning address of the buffer.
@param end       The end address of the buffer.
@param character The value to write. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, char32_t character,
                            int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, uint32_t value,
                            int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, int32_t value,
                            int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, uint64_t value,
                            int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, int64_t value,
                            int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, float value,
                            int column_count);

/* Writes the give char32_t to the given buffer center.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintRight(char32_t* begin, char32_t* end, double value,
                            int column_count);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin   The beginning address of the buffer.
@param end     The end address of the buffer.
@param pointer The pointer to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, const void* pointer);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, uint8_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, int8_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, uint16_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, int16_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, uint32_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, int32_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, uint64_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, int64_t value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, float value);

/* Writes the give char32_t to the given buffer in hex form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintHex(char32_t* begin, char32_t* end, double value);

/* Printrs the given string to the print buffer.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin  The beginning address of the buffer.
@param end    The end address of the buffer.
@param pointer The pointer to print to hex. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end,
                             const void* pointer);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, uint8_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, int8_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, uint16_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, int16_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, uint32_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, int32_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, uint64_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, int64_t value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, float value);

/* Writes the give char32_t to the given buffer in binary form.
@return Returns nil upon buffer overflow and a pointer to the nil-term char32_t
upon success.
@param begin The beginning address of the buffer.
@param end The end address of the buffer.
@param value The value to print. */
KABUKI char32_t* PrintBinary(char32_t* begin, char32_t* end, double value);

/* Prints the given memory socket to the text buffer. */
KABUKI char32_t* PrintMemory(char32_t* begin, char32_t* end, const void* start,
                             size_t size);

/* Prints out the contents of the address to the printer buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param start The beginning of the read buffer.
@param stop  The end of the read buffer. */
KABUKI char32_t* PrintMemory(char32_t* begin, char32_t* end, const void* start,
                             const void* stop);

/* Prints out the contents of the address to the printer buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param start The beginning of the read buffer.
@param size  The size of the read buffer. */
KABUKI char32_t* PrintMemory(char32_t* begin, char32_t* end, const void* start,
                             size_t size);

/* Converts the given string to a 8-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, int8_t& result);

/* Converts the given string to a 8-bit unsigned integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, uint8_t& result);

/* Converts the given string to a 16-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, int16_t& result);

/* Converts the given string to a 16-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, uint16_t& result);

/* Converts the given string to a 32-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, int32_t& result);

/* Converts the given string to a 32-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, uint32_t& result);

/* Converts the given string to a 64-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, int64_t& result);

/* Converts the given string to a 64-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, uint64_t& result);

/* Converts the given string to a 32-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, float& result);

/* Converts the given string to a 64-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* begin, double& result);
/* Converts the given string to a 8-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, int8_t& result);

/* Converts the given string to a 8-bit unsigned integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, uint8_t& result);

/* Converts the given string to a 16-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, int16_t& result);

/* Converts the given string to a 16-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, uint16_t& result);

/* Converts the given string to a 32-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, int32_t& result);

/* Converts the given string to a 32-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, uint32_t& result);

/* Converts the given string to a 64-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, int64_t& result);

/* Converts the given string to a 64-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, uint64_t& result);

/* Converts the given string to a 32-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, float& result);

/* Converts the given string to a 64-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* TextScan(const char32_t* text, double& result);

/* Reads a time or time delta from a a char starting with an '@' sign..
@param input  The char to parse.
@param hour   The location to write the number of hours to.
@param minute The location to write the number of minutes to.
@param Second The location to write the number of seconds to.
@return The offset The end of where the parser exited successfully at.

@code
@4        (This is 4AM)
@4PM      (No space required)
@4:20P    (Or M)
@4:20 PM
@16:20
@4:20 am
@4:20a.m.
@4:20:00
@4:20:00AM
@16:20:00
@endcode
*/
KABUKI const char* TextScanTime(const char32_t* input, int& hour, int& minute,
                                int& second);

/* Converts a keyboard input to char and deletes the char.
 */
KABUKI const char* TextScanTime(const char32_t* input, TimeSeconds& time);

/* Converts a keyboard input to a time_t. */
KABUKI const char* TextScanTime(const char32_t* input, time_t& result);

/* ASCII printing utilities
@ingroup UTF
*/

/* Utility class for printing strings.
This class only stores the end of buffer pointer and a pointer to the write
begin. It is up the user to store start of buffer pointer and if they would
like to replace the begin with the beginning of buffer pointer when they
are done printing.
*/
struct KABUKI Printer4 {
  char32_t *begin,  //< Write begin pointer.
      *end;         //< End of buffer pointer.

  /* Initializes the Printer& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Printer4(char32_t* begin, size_t size);

  /* Initializes the Printer& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Printer4(char32_t* begin, char32_t* end);

  /* Clones the other print. */
  Printer4(const Printer4& other);

  /* Sets the begin pointer to the new_pointer. */
  inline Printer4& Set(char32_t* new_pointer);

  /* Prints the given value as hex. */
  inline Printer4& Hex(int8_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(uint8_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(int16_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(uint16_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(int32_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(uint32_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(int64_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(uint64_t value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(float value);

  /* Prints the given value as hex. */
  inline Printer4& Hex(double value);

  /* Prints the given pointer as hex. */
  inline Printer4& Hex(const void* pointer);

  /* Prints the given value as binary. */
  inline Printer4& Binary(int8_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(uint8_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(int16_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(uint16_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(int32_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(uint32_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(int64_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(uint64_t value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(float value);

  /* Prints the given value as binary. */
  inline Printer4& Binary(double value);

  /* Prints the given pointer as binary. */
  inline Printer4& Binary(const void* pointer);
};

/* Utility class for printing numbers. */
class Text4 {
 public:
  /* Default constructor does nothing. */
  Text4();

  /* Prints the value to the text buffer. */
  Text4(char32_t character);

  /* Prints the value to the text buffer. */
  Text4(int32_t value);

  /* Prints the value to the text buffer. */
  Text4(uint32_t value);

  /* Prints the value to the text buffer. */
  Text4(int64_t value);

  /* Prints the value to the text buffer. */
  Text4(uint64_t value);

  /* Prints the value to the text buffer. */
  Text4(float value);

  /* Prints the value to the text buffer. */
  Text4(double value);

  /* Gets the number string. */
  const char32_t* GetString();

 private:
  enum { kSize = 24 };

  char32_t string[kSize * sizeof(char32_t) + 1];  //< String buffer.
};

/* Utility class for printing hex with operator<<. */
class Center4 {
 public:
  /* Prints the value to the text buffer. */
  Center4(const char32_t* string, int column_count);

  /* Prints the value to the text buffer. */
  Center4(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center4(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center4(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center4(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Center4(float value, int column_count);

  /* Prints the value to the text buffer. */
  Center4(double value, int column_count);

  /* Gets the number string. */
  const char32_t* GetString();

  /* Gets the column_count. */
  int GetColumnCount();

 private:
  const char32_t* string;  //< Pointer to the string.
  Text4 number;            //< Pointer to a pointer to print.
  int column_count;        //< Number of columns to center.
};

/* Utility class for printing hex with operator<<. */
class Right4 {
 public:
  /* Prints the value to the text buffer. */
  Right4(const char32_t* string, int column_count);

  /* Prints the value to the text buffer. */
  Right4(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right4(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right4(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right4(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Right4(float value, int column_count);

  /* Prints the value to the text buffer. */
  Right4(double value, int column_count);

  /* Gets the number string. */
  const char32_t* GetString();

  /* Gets the column_count. */
  int GetColumnCount();

 private:
  const char32_t* string;  //< Pointer to the string.
  Text4 number;            //< Pointer to a pointer to print.
  int column_count;        //< Number of columns to center.
};

}  // namespace _

/* Writes a nil-terminated UTF-8 or ASCII string to the print.
@param  printer The printer.
@param  value   The value to print.
@return The printer. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, const char32_t* string);

/* Writes the given value to the print.
@param  printer The printer.
@param  value   The value to print.
@return The printer. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, char32_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, uint8_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, int16_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, uint16_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, int32_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, uint32_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, int64_t value);

/* Writes the given value to the print.
@return The printer.
@desc
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, uint64_t value);

/* Writes the given value to the print.
@return The printer.
@desc
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, float value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, double value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print justified center. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, _::Center4 item);

/* Writes the given value to the print jusified right.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Printer4& operator<<(_::Printer4& printer, _::Right4 item);

#endif  //< #if USING_UTF32
#endif  //< #if HEADER_FOR_CRABS_PRINT_UTF32
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
