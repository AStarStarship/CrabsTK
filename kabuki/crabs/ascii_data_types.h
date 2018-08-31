/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/crabs_console.cc
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
#if SEAM >= SEAM_0_0_2
#ifndef INCLUDED_CRABS_TYPES
#define INCLUDED_CRABS_TYPES
// Dependencies:
#include "align.h"
#include "config.h"
#include "tstr.h"
// End dependencies.
/* @tag Comment may be wrapped around
like this.
  @tag Or comment may be
       wrapped around like this.
*/

namespace _ {

/* A type-value tuple. */
struct KABUKI TypeValue {
  type_t type;        //< ASCII Type.
  const void* value;  //< Pointer to the value data.

  /* Stores the type and value. */
  TypeValue(type_t type, const void* value = nullptr);
};

/* List of the 32 ASCII Data Types.
    Types are organized with types that are allowed in text B-Sequences first,
    followed by objects which get created in text using Script Operations. */
typedef enum AsciiTypes {
  NIL = 0,  //< 0.  NIL/nil/void type.
  SI1,      //< 1.  8-bit signed integer.
  UI1,      //< 2.  8-bit unsigned integer.
  SI2,      //< 3.  16-bit signed integer.
  UI2,      //< 4.  16-bit unsigned integer.
  HLF,      //< 5.  16-bit floating-point number.
  BOL,      //< 6.  32-bit non-zero true boolean as signed integer.
  SI4,      //< 7.  32-bit signed integer.
  UI4,      //< 8.  32-bit unsigned integer.
  FLT,      //< 9.  32-bit floating-point number.
  TMS,      //< 10. 32-bit second since epoch timestamp.
  TSS,      //< 11. 64-bit sub-second timestamp with TMS and an UI4 tick.
  TME,      //< 12. 64-bit second since epoch timestamp.
  SI8,      //< 13. 64-bit signed integer.
  UI8,      //< 14. 64-bit unsigned integer.
  DBL,      //< 15. 64-bit floating-point number.
  SIH,      //< 16. 128-bit (Hexadeca-bit) signed integer.
  UIH,      //< 17. 128-bit (Hexadeca-bit) unsigned integer.
  DEC,      //< 18. 128-bit (Hexadeca-bit) floating-point number.
  UIX,      //< 19. 2^(6+X)-bit unsigned integer, where 0 <= X <= 7.
  OBJ,      //< 20. N-byte object.
  ADR,      //< 21. UTF-8 Operand stack address.
  STR,      //< 22. A UTF-8 string.
  TKN,      //< 23. A UTF-8 string token without whitespace.
  BSQ,      //< 24. B-Sequence.
  LOM,      //< 25. A loom of UTF-8, UTF-16, or UTF-32 strings.
  TBL,      //< 26. A sorted sequencential hash table.
  EXP,      //< 27. Script Expression.
  LST,      //< 28. Set or multiset of Type-Value tuples.
  MAP,      //< 29. One-to-one map of Integer-{Type-Value} records.
  BOK,      //< 30. Many-to-one multimap of Key-{Type-Value} records.
  DIC,      //< 31. One-to-one map of Key-{Type-Value} records.
} StdType;

/* Checks if the given type is valid.
    @return False if the given type is an 8-bit LST, MAP, BOK, or DIC. */
inline bool TypeIsValid(type_t type);

/* Aligns the given pointer to the correct word boundary for the type. */
KABUKI void* TypeAlign(type_t type, void* value);

enum {
  kTypeCount = 32,  //< The starting index of invalid types.
};

/* Returns a pointer to an array of pointers to the type names.*/
KABUKI const char** TypeStrings();

/* Returns the name of the given type. */
KABUKI inline const char* TypeString(type_t type);

/* Returns the name of the given type. */
KABUKI inline const char* TypeString(uint_t type);

/* Masks off the lower 5-LSb to get the type. */
KABUKI inline byte TypeMask(byte value);

/* Returns true if the given type is an Array type. */
KABUKI inline bool TypeIsArray(uint_t type);

/* Returns true if the given type is a BOK type. */
KABUKI inline bool TypeIsSet(uint_t type);

/* Returns the size or max size of the given type. */
KABUKI uint_t TypeFixedSize(uint_t type);

/* Gets the next address that a data type may be stored at. */
KABUKI void* TypeAlign(type_t type, void* value);

/* Writes the given value to the socket. */
KABUKI char* Write(char* begin, char* end, type_t type, const void* source);

/* Returns true if the given type is an ASCII OBJ. */
KABUKI inline bool TypeIsObj(type_t type);

/* Returns true if the given type is a string type. */
KABUKI inline bool TypeIsString(type_t type);

/* Checks if the given type is UTF-16.
    @param  type The type to check.
    @return True if the given type is UTF-16. */
KABUKI inline bool TypeIsUtf16(type_t type);

KABUKI inline int TypeSizeWidthCode(type_t type);

#if USING_UTF8
namespace _ {
/* Prints th given type or type-value.
@return Returns a pointer to the next char after the end of the read number or
nil upon failure.
@param utf The utf to print to.
@param type    The type to print.
@param value   The value to print or nil. */
KABUKI char* Print(char* begin, char* end, type_t type, const void* value);
}  // namespace _
/* Writes the given value to the print jusified right.
@return The utf.
@param  utf The utf.
@param  item The item to print. */
KABUKI _::Utf8& operator<<(_::Utf8& utf, const _::TypeValue& type_value);
#endif
#if USING_UTF16
namespace _ {
/* Prints th given type or type-value.
@return Returns a pointer to the next char after the end of the read number or
nil upon failure.
@param utf The utf to print to.
@param type    The type to print.
@param value   The value to print or nil. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, type_t type,
                       const void* value);
}  // namespace _
/* Writes the given value to the print jusified right.
@return The utf.
@param  utf The utf.
@param  item The item to print. */
KABUKI _::Utf16& operator<<(_::Utf16& utf, const _::TypeValue& type_value);
#endif
#if USING_UTF32

namespace _ {
/* Prints th given type or type-value.
@return Returns a pointer to the next char after the end
of the read number or nil upon failure.
@param printer The printer to print to.
@param type    The type to print.
@param value   The value to print or nil. */
KABUKI char16_t* Print(char16_t* begin, char16_t* end, type_t type,
                       const void* value);
}  // namespace _
/* Writes the given value to the print jusified right.
@return The utf.
@param  utf The utf.
@param  item The item to print. */
KABUKI _::Utf32& operator<<(_::Utf32& utf, const _::TypeValue& type_value);
#endif

}  // namespace _

#endif  //< #if SEAM >= SEAM_0_0_2
#endif  //< INCLUDED_CRABS_TYPES
