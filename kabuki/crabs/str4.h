/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/str4.h
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
#if SEAM >= SEAM_0_0_1
#ifndef INCLUDED_CRABS_PRINT_UTF32
#define INCLUDED_CRABS_PRINT_UTF32
// Dependencies:
#include "clock.h"
// End dependencies.
#if USING_UTF32

namespace _ {

/* UTF-8 printing utilities
@ingroup Utf
*/

/* Checks if the given character is whitespace.
 */
KABUKI bool IsWhitespace(char32_t character);

/* Converts the given value to a printable char32_t if it's non-printable. */
KABUKI char32_t PrintableChar(char32_t value);

/* Scrolls over to the next double quote mark.
@warning This function is only safe to use on ROM strings with a nil-term
char32_t. */
KABUKI const char32_t* TextEnd(const char32_t* begin);

/* Gets the length of the given char32_t.
@return  Returns -1 if the text char32_t is nil.
@warning This function is only safe to use on ROM strings with a nil-term
char32_t. */
KABUKI int TextLength(const char32_t* begin);

/* Clones the given string.
@param  A nil-terminated string in ROM.
@return Returns a new copy you must delete. */
KABUKI char32_t* TextClone(const char32_t* begin);

/* Returns a pointer to the char32_t at the end of the line. */
KABUKI const char32_t* TextLineEnd(const char32_t* begin, int column_count);

/* Returns the pointer to the next char32_t in the char32_t that is not an ASCII
number.
@return A pointer to the next non-number in the text char32_t. */
KABUKI const char32_t* TextNumberStop(const char32_t* begin);

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
KABUKI const char32_t* StringEquals(const char32_t* text_a,
                                  const char32_t* text_b);

/* Compares the source and query char32_t as nil-terminated strings. */
KABUKI const char32_t* StringEquals(const char32_t* begin, const char32_t* end,
                                  const char32_t* query);

/* Searches the given char32_t for the given char32_t.
@param  text  The char32_t to search.
@param  query The char32_t to search for.
@return Returns nil if the parsing failed and a pointer to the first char32_t
after the end of the text upon success. */
KABUKI const char32_t* StringFind(const char32_t* begin, const char32_t* query);

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

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, Clock t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, Tms t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, Tme t);

/* Writes the given time to the text buffer.
@return Null upon failure or a pointer to the byte after the last
byte written.
@param begin The beginning of the write buffer.
@param time  The time to print.
@param end   The end of the write buffer. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, Tss t);

/* Prints th given type or type-value.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure.
@param printer The printer to print to.
@param type    The type to print.
@param value   The value to print or nil. */
KABUKI char32_t* Print(char32_t* begin, char32_t* end, type_t type,
                       const void* value);

/* Prints a line of the given column_count.
@return Returns a pointer to the next char32_t after the end of the read number
or nil upon failure.
@param begin The beginning of the write buffer.
@param end   The end of the write buffer.
@param token The token to print.
@param column_count The number of tokens to print. */
KABUKI char32_t* PrintLine(char32_t* cursor, char32_t* end, char32_t token,
                           int column_count);

/* Prints a line of the given column_count.
@return Returns a pointer to the next char32_t after the end of the read number
or nil upon failure.
@param begin  The beginning of the write buffer.
@param end    The end of the write buffer.
@param string The string to print.
@param column_count The number of columns. */
KABUKI char32_t* PrintLineString(char32_t* cursor, char32_t* end,
                                 const char32_t* string, int column_count);

/* Prints the buffer to the consoele as a UTF-8 string. */
void COutUtf32(uintptr_t* buffer);

/* Prints the buffer to the consoele as a UTF-8 string. */
void COutAutoUtf32(uintptr_t* buffer);

/* Converts the given string to a 8-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, int8_t& result);

/* Converts the given string to a 8-bit unsigned integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, uint8_t& result);

/* Converts the given string to a 16-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, int16_t& result);

/* Converts the given string to a 16-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, uint16_t& result);

/* Converts the given string to a 32-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, int32_t& result);

/* Converts the given string to a 32-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, uint32_t& result);

/* Converts the given string to a 64-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, int64_t& result);

/* Converts the given string to a 64-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, uint64_t& result);

/* Converts the given string to a 32-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, float& result);

/* Converts the given string to a 64-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* begin, double& result);
/* Converts the given string to a 8-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, int8_t& result);

/* Converts the given string to a 8-bit unsigned integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, uint8_t& result);

/* Converts the given string to a 16-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, int16_t& result);

/* Converts the given string to a 16-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, uint16_t& result);

/* Converts the given string to a 32-bit signed integer.
@param  text A nil-terminated string in ROM.
@param  result  The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, int32_t& result);

/* Converts the given string to a 32-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, uint32_t& result);

/* Converts the given string to a 64-bit signed integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, int64_t& result);

/* Converts the given string to a 64-bit unsigned integer.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, uint64_t& result);

/* Converts the given string to a 32-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, float& result);

/* Converts the given string to a 64-bit floating-point number.
@param  text  A nil-terminated string in ROM.
@param  result The result of the conversion.
@return Returns a pointer to the next char32_t after the end
of the read number or nil upon failure. */
KABUKI const char32_t* Scan(const char32_t* text, double& result);

/* Universal Text Formatter (UTF) 8 is a utility class for printing UTF-8
strings.
This class only stores the end of buffer pointer and a pointer to the
write begin. It is up the user to store start of buffer pointer and if they
would like to replace the begin with the beginning of buffer pointer when they
are done printing.
*/
struct KABUKI Utf32 {
  char32_t *begin,  //< Write begin pointer.
      *end;         //< End of buffer pointer.

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Utf32(char32_t* begin, size_t size);

  /* Initializes the Utf& from the given buffer pointers.
  @param begin The beginning of the buffer.
  @param end   The end of the buffer. */
  Utf32(char32_t* begin, char32_t* end);

  /* Clones the other print. */
  Utf32(const Utf32& other);

  /* Sets the begin pointer to the new_pointer. */
  inline Utf32& Set(char32_t* new_pointer);

  /* Prints the given value as hex. */
  inline Utf32& Hex(int8_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(uint8_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(int16_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(uint16_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(int32_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(uint32_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(int64_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(uint64_t value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(float value);

  /* Prints the given value as hex. */
  inline Utf32& Hex(double value);

  /* Prints the given pointer as hex. */
  inline Utf32& Hex(const void* pointer);

  /* Prints the given value as binary. */
  inline Utf32& Binary(int8_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(uint8_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(int16_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(uint16_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(int32_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(uint32_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(int64_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(uint64_t value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(float value);

  /* Prints the given value as binary. */
  inline Utf32& Binary(double value);

  /* Prints the given pointer as binary. */
  inline Utf32& Binary(const void* pointer);
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
class Utf32Center {
 public:
  /* Prints the value to the text buffer. */
  Utf32Center(const char32_t* string, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Center(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Center(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Center(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Center(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Center(float value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Center(double value, int column_count);

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
class Utf32Right {
 public:
  /* Prints the value to the text buffer. */
  Utf32Right(const char32_t* string, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Right(int32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Right(uint32_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Right(int64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Right(uint64_t value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Right(float value, int column_count);

  /* Prints the value to the text buffer. */
  Utf32Right(double value, int column_count);

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
KABUKI _::Utf32& operator<<(_::Utf32& printer, const char32_t* string);

/* Writes the given value to the print.
@param  printer The printer.
@param  value   The value to print.
@return The printer. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, char32_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, uint8_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, int16_t value);

/* Writes the given value to the print.
@param  printer The printer.
@param  value The value to write to the print.
@return The printer. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, uint16_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, int32_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, uint32_t value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, int64_t value);

/* Writes the given value to the print.
@return The printer.
@desc
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, uint64_t value);

/* Writes the given value to the print.
@return The printer.
@desc
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, float value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, double value);

/* Writes the given value to the print.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print justified center. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, _::Utf32Center item);

/* Writes the given value to the print jusified right.
@return The printer.
@param  printer The printer.
@param  value The value to write to the print. */
KABUKI _::Utf32& operator<<(_::Utf32& printer, _::Utf32Right item);

#endif  //< #if USING_UTF32
#endif  //< #if INCLUDED_CRABS_PRINT_UTF32
#endif  //< #if SEAM >= SEAM_0_0_1
