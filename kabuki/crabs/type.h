/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/type.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_TYPES
#define HEADER_FOR_CRABS_TYPES
// Dependencies:
#include "config.h"
// End dependencies.
/* @tag Comment may be wrapped around
like this.
  @tag Or comment may be
       wrapped around like this.
*/
typedef uint8_t type_t;

namespace _ {

enum {
  kMaxDigitsFloat = 3 + FLT_MANT_DIG - FLT_MIN_EXP,
  kMaxDigitsDouble = 3 + DBL_MANT_DIG - DBL_MIN_EXP,
};

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
  TMU,      //< 11. 64-bit microsecond since epoch timestamp.
  SI8,      //< 12. 64-bit signed integer.
  UI8,      //< 13. 64-bit unsigned integer.
  DBL,      //< 14. 64-bit floating-point number.
  SIH,      //< 15. 128-bit (Hexadeca-bit) signed integer.
  UIH,      //< 16. 128-bit (Hexadeca-bit) unsigned integer.
  DEC,      //< 17. 128-bit (Hexadeca-bit) floating-point number.
  UIX,      //< 18. 2^(6+X)-bit unsigned integer, where 0 <= X <= 7.
  ADR,      //< 19. UTF-8 Operand stack address.
  STR,      //< 20. A UTF-8 string.
  TKN,      //< 21. A UTF-8 string token without whitespace.
  BSQ,      //< 22. B-Sequence.
  OBJ,      //< 23. N-byte object.
  STA,      //< 24. String array.
  SET,      //< 25. A unique set of Type-Value records.
  TBL,      //< 26. A sorted sequencential hash table.
  EXP,      //< 27. Script Expression.
  LST,      //< 28. Set or multiset of Type-Value tuples.
  MAP,      //< 29. One-to-one map of Integer-{Type-Value} records.
  BOK,      //< 30. Many-to-one multimap of Key-{Type-Value} records.
  DIC,      //< 31. One-to-one map of Key-{Type-Value} records.
} AsciiType;

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

/* Returns a pointer to an array of pointers to the type names.*/
KABUKI const char** TypeStrings();

/* Returns the name of the given type. */
inline const char* TypeString(type_t type) {
  return TypeStrings()[type & 0x1f];
}

/* Returns the name of the given type. */
inline const char* TypeString(uint_t type) { return TypeString((uint8_t)type); }

/* Masks off the lower 5-LSb to get the type. */
inline byte TypeMask(byte value) { return value & 0x1f; }

/* Returns true if the given type is an Array type. */
inline bool TypeIsArray(uint_t type) { return type >= kTypeCount; }

/* Returns true if the given type is a BOK type. */
inline bool TypeIsSet(uint_t type) { return type >= kTypeCount; }

/* Converts from a 2's complement integer to a signed varint.
    A signed varint is an complemented signed integer with the sign in the
    LSb. To convert a negative 2's complement value to positive invert the bits
    and add one.
    @param  value A 2's complement integer.
    @return An signed varint.
*/
template <typename T>
inline T TypePackVarint(T value) {
  T sign_bit = value >> ((sizeof(T) * 8 - 1));  //< Extract the sign bit.
  if (sign_bit != 0) {
    T uncomplemented = (~value) + 1;
    return sign_bit | (uncomplemented << 1);
  }
  // Else don't complemented.
  return value << 1;
}

/* Converts from a signed varint to a 2's complement signed integer.
    A varint is an complemented signed integer with the sign in the LSb.
    @param  A signed integer casted as an unsigned integer.
    @return Returns a standard signed integer cased as unsigned. */
template <typename T>
inline T TypeUnpackVarint(T value) {
  T sign_bit = value << (sizeof(T) * 8 - 1);
  value = value >> 1;
  if (sign_bit) {
    value = ~(value - 1);
    return value;
  }
  return value;
}

/* Returns the size or max size of the given type. */
KABUKI uint_t TypeFixedSize(uint_t type);

/* Gets the next address that a data type may be stored at. */
KABUKI void* TypeAlign(type_t type, void* value);

/* Writes the given value to the socket. */
KABUKI char* Write(char* begin, char* end, type_t type, const void* source);

/* Returns true if the given type is an ASCII Object. */
inline bool TypeIsObject(type_t type) {
  if (type < OBJ) return false;
  return true;
}

/* Returns true if the given type is a string type. */
inline bool TypeIsString(type_t type) {
  type &= 0x1f;
  if (type >= ADR && type <= TKN) return true;
  return false;
}

/* Checks if the given type is UTF-16.
    @param  type The type to check.
    @return True if the given type is UTF-16. */
inline bool TypeIsUtf16(type_t type) { return (bool)(type & 0x20); }

/* Prints th given type or type-value.
    @param printer The printer to print to.
    @param type    The type to print.
    @param value   The value to print or nil. */
KABUKI Printer1& PrintType(Printer1& printer, type_t type, const void* value);

inline int TypeSizeWidthCode(type_t type) { return type >> 6; }

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

}  // namespace _

inline _::Printer1& operator<<(_::Printer1& printer,
                               const _::TypeValue& type_value) {
  return _::PrintType(printer, type_value.type, type_value.value);
}

#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#endif  //< HEADER_FOR_CRABS_TYPES
