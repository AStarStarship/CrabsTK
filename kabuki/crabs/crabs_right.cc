/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/crabs_text.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <stdafx.h>
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
// Dependencies:
#include "printer.h"
#include "type.h"
// End dependencies.
#if USING_PRINTER
#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_HEADING \
  std::cout << '\n';  \
  for (int i = 80; i > 0; --i) std::cout << '-';
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#endif

namespace _ {

char* PrintRight(char* buffer, char* buffer_end, const char* token,
                 int num_columns) {
  char* start = buffer;
  ASSERT(token)
  ASSERT(buffer)

  if (buffer >= buffer_end) return nullptr;
  if (buffer + num_columns > buffer_end) return nullptr;

  // PRINTF ("\n\nPrinting \"%s\" aligned right %i columns", token, num_columns)

  intptr_t length = 0;            //< Length of the token.
  const char* token_end = token;  //< Address of the last char.
  char* cursor;                   //< Print cursor pointer.
  char c;                         //< Temp variable.

  // Find string length.
  while (*(++token_end))
    ;
  length = token_end - token;

  // PRINTF ("\n    Found length:%i", (int)length)

  if (token == token_end) {
    return buffer;
  }

  *(buffer + length) = 0;
  // If the length is less than the num_columns we need to print ".", "..",
  // "..." or nothing and chop off some of the token.
  if (length > num_columns) {
    // PRINTF ("\n Wrote with dots:\"")
    intptr_t num_dots = length - num_columns;
    if (num_dots > 3) {
      num_dots = 3;
    }
    buffer_end = buffer + num_columns;
    cursor = buffer_end - num_dots;
    while (buffer < cursor) {
      c = *token++;
      *buffer++ = c;
      // PUTCHAR (c)
    }
    while (buffer < buffer_end) {
      *buffer++ = '.';
      // PUTCHAR ('.')
    }
    // PUTCHAR ('\"')
    *buffer = 0;
    return buffer;
  }
  // PRINTF ("\n Wrote:\"")
  cursor = buffer + num_columns;
  buffer_end = cursor - length;
  while (cursor >= buffer_end) {
    c = *token_end--;
    *cursor-- = c;
    // PUTCHAR (c)
  }
  // PUTCHAR ('\"')
  buffer_end = cursor - num_columns;
  // PRINTF ("\ncursor:0x%p text:0x%x delta:%i", cursor, text, text, cursor)
  while (cursor >= buffer) {
    *cursor-- = ' ';
  }
  buffer_end = buffer + num_columns;
  return buffer_end;
}

}       //< namespace _ {
#undef PRINTF
#undef PUTCHAR
#undef PRINT_HEADING
#endif  //< USING_PRINTER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
