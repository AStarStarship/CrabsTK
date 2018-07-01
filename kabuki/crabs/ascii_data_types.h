/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/ascii_data_types.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 3
#ifndef HEADER_FOR_CRABS_TYPES
#define HEADER_FOR_CRABS_TYPES
// Dependencies:
#include "align.h"
#include "config.h"
#include "utfn.h"
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
  ADR,      //< 20. UTF-8 Operand stack address.
  STR,      //< 21. A UTF-8 string.
  TKN,      //< 22. A UTF-8 string token without whitespace.
  BSQ,      //< 23. B-Sequence.
  OBJ,      //< 24. N-byte object.
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
inline bool TypeIsValid(type_t type) {
  if (type >= LST && type <= DIC || (type >= (ADR + 32) && type <= (TKN + 32)))
    return false;
  return true;
}

/* Aligns the given pointer to the correct word boundary for the type. */
KABUKI void* TypeAlign(type_t type, void* value);

/* An ROM string for one of the 32 types.
    C++11 variadic templates ensure there is only one copy in of the given
    string in ROM. */
template <char kCharA_, char kCharB_, char kCharC_>
inline uint32_t T() {
  return ((uint32_t)kCharA_) & (((uint32_t)kCharB_) << 8) &
         (((uint32_t)kCharC_) << 16);
}

enum {
  kTypeCount = 32,  //< The starting index of invalid types.
};

template <typename Char>
const Char** TypeStrings() {
  static const Char* kNames[] = {
      "NIL",  //<  0
      "SI1",  //<  1
      "UI1",  //<  2
      "SI2",  //<  3
      "UI2",  //<  4
      "HLF",  //<  5
      "BOL",  //<  6
      "SVI",  //<  7
      "UVI",  //<  8
      "SI4",  //<  9
      "UI4",  //< 10
      "FLT",  //< 11
      "TMS",  //< 12
      "TMU",  //< 13
      "SI8",  //< 14
      "UI8",  //< 15
      "DBL",  //< 16
      "SV8",  //< 17
      "UV8",  //< 18
      "DEC",  //< 19
      "UIX",  //< 20
      "ADR",  //< 23
      "STR",  //< 24
      "TKN",  //< 25
      "OBJ",  //< 21
      "LOM",  //< 25
      "BSQ",  //< 26
      "ESC",  //< 27
      "LST",  //< 28
      "BOK",  //< 29
      "DIC",  //< 30
      "MAP",  //< 31
  };
  return kNames;
}

/* Returns a pointer to an array of pointers to the type names.*/
KABUKI const char** TypeStrings();

/* Returns the name of the given type. */
inline KABUKI const char* TypeString(type_t type);

/* Returns the name of the given type. */
inline KABUKI const char* TypeString(uint_t type);

/* Masks off the lower 5-LSb to get the type. */
inline KABUKI byte TypeMask(byte value);

/* Returns true if the given type is an Array type. */
inline KABUKI bool TypeIsArray(uint_t type);

/* Returns true if the given type is a BOK type. */
inline KABUKI bool TypeIsSet(uint_t type);

/* Returns the size or max size of the given type. */
KABUKI uint_t TypeFixedSize(uint_t type);

/* Gets the next address that a data type may be stored at. */
KABUKI void* TypeAlign(type_t type, void* value);

/* Writes the given value to the socket. */
KABUKI char* Write(char* begin, char* end, type_t type, const void* source);

/* Returns true if the given type is an ASCII Object. */
inline KABUKI bool TypeIsObject(type_t type);

/* Returns true if the given type is a string type. */
inline KABUKI bool TypeIsString(type_t type);

/* Checks if the given type is UTF-16.
    @param  type The type to check.
    @return True if the given type is UTF-16. */
inline KABUKI bool TypeIsUtf16(type_t type);

inline KABUKI int TypeSizeWidthCode(type_t type);

template <typename T = char>
T* TypeAlignUpPointer(void* pointer, type_t type) {
  if (type <= UI1)
    return reinterpret_cast<T*>(pointer);
  else if (type <= HLF)
    return AlignUpPointer2<T>(pointer);
  else if (type <= TMS)
    return AlignUpPointer4<T>(pointer);
  else if (type <= DEC)
    return AlignUpPointer8<T>(pointer);
  // else it's an ASCII Object.
  // | Code | Binary | Mask needed |
  // |:----:|:------:|:-----------:|
  // |  0   | 0b'00  |   0b'000    |
  // |  1   | 0b'01  |   0b'001    |
  // |  2   | 0b'10  |   0b'011    |
  // |  3   | 0b'11  |   0b'111    |
  uintptr_t ptr = reinterpret_cast<uintptr_t>(pointer), mask = (type >> 6);
  if (mask == 2) return AlignUpPointer4<T>(pointer);
  if (mask == 3) return AlignUpPointer8<T>(pointer);
  ptr += ((~ptr) + 1) & mask;
  return reinterpret_cast<T*>(ptr);
}

template <typename Char>
Char* PrintTypePod(Char* cursor, Char* end, type_t type, const void* value) {
  if (!value) return printer << "Nil";
  switch (type & 0x1f) {
    case NIL:
      return Print<Char>("Error");
    case SI1:
      return Print<Char>(cursor, end, *reinterpret_cast<const int8_t*>(value));
    case UI1:
      return Print<Char>(cursor, end, *reinterpret_cast<const uint8_t*>(value));
    case SI2:
      return Print<Char>(cursor, end, *reinterpret_cast<const int16_t*>(value));
    case UI2:
      return Print<Char>(cursor, end,
                         *reinterpret_cast<const uint16_t*>(value));
    case HLF:
      return Print<Char>(cursor, end, "not_implemented");
    case BOL:
      return Print<Char>(cursor, end, *reinterpret_cast<const bool*>(value));
    case SI4:
      return Print<Char>(cursor, end, *reinterpret_cast<const int32_t*>(value));
    case UI4:
      return Print<Char>(cursor, end,
                         *reinterpret_cast<const uint32_t*>(value));
    case FLT:
      return Print<Char>(cursor, end, *reinterpret_cast<const float*>(value));
    case TMS:
      return Print<Char>(cursor, end, *reinterpret_cast<const int*>(value));
    case TME:
    case TMU:
      return Print<Char>(cursor, end, *reinterpret_cast<const int64_t*>(value));
    case SI8:
      return Print<Char>(cursor, end, *reinterpret_cast<const int64_t*>(value));
    case UI8:
      return Print<Char>(cursor, end,
                         *reinterpret_cast<const uint64_t*>(value));
    case DBL:
      return Print<Char>(cursor, end, *reinterpret_cast<const double*>(value));
      // case SV8:
      //  return Print<Char> (cursor, end, *reinterpret_cast<const
      //  int64_t*>(value));
      // case UV8:
      //  return Print<Char> (cursor, end, *reinterpret_cast<const
      //  uint64_t*>(value));
    case DEC:
      return Print<Char>(cursor, end, "not_implemented");
  }
  return nullptr;
}

template <typename Char>
Char* Print(Char* cursor, Char* end, type_t type, const void* value) {
  if (cursor == nullptr) return nullptr;
  ASSERT(cursor < end);

  if (type <= DEC) {
    cursor = PrintTypePod<Char>(cursor, end, type, value);
    if (!cursor) return nullptr;
    cursor = Print<Char>(cursor, end, ':');
    if (!cursor) return nullptr;
    return Print<Char>(cursor, end, TypeString<Char>(type));
  }

  if (!TypeIsValid(type)) return Print<Char>(cursor, end, "illegal_type");

  if (TypeIsString(type)) {
    cursor = Print<Char>(cursor, end, '\"');
    if (!cursor) return nullptr;
    cursor = Print<Char>(cursor, end, *reinterpret_cast<const char*>(value));
    if (!cursor) return nullptr;
    cursor = Print<Char>(cursor, end, reinterpret_cast<const char*>(value));
    if (!cursor) return nullptr;
    cursor = Print<Char>(cursor, end, "\":");
    if (!cursor) return nullptr;
    return cursor = Print<Char>(cursor, end, TypeString(type));
  }

  return PrintTypePod(utf, type & 0x1f, value) << "b:" << TypeString(type);
}

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

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 3
#endif  //< HEADER_FOR_CRABS_TYPES
