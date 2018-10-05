/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/f2_types.cc
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
#if SEAM >= SEAM_0_0_3

#include "ascii_data.h"
#include "socket.h"
#include "talign.h"
#include "test.h"

namespace _ {

bool TypeIsValid(type_t type) {
  if (type >= LST && type <= DIC || (type >= (ADR + 32) && type <= (TKN + 32)))
    return false;
  return true;
}

TypeValue::TypeValue(type_t type, const void* value)
    : type(type), value(value) {
  // Nothing to do here! ({:-)-+=<
}

uint_t TypeFixedSize(uint_t type) {
  static const int8_t kWidths[] = {
      0,   //< NIL: 0
      1,   //< SI1: 1
      1,   //< UI1: 2
      2,   //< SI2: 3
      2,   //< UI2: 4
      2,   //< HLF: 5
      4,   //< BOL: 6
      4,   //< SI4: 7
      4,   //< UI4: 8
      4,   //< FLT: 9
      4,   //< TMS: 10
      8,   //< TME: 11
      8,   //< SI8: 12
      8,   //< UI8: 13
      8,   //< DBL: 14
      16,  //< SIH: 15
      16,  //< UIH: 16
      16,  //< DEC: 17
  };
  type_t type_upper_bits = type >> 3;
  type &= 0x1f;
  if (type == UIX) return ((uint_t)2) << type_upper_bits;
  if (type > OBJ) return -1;
  return kWidths[type];
}

const char** TypeStrings() { return TypeStrings<char>(); }

const char* TypeString(type_t type) { return TypeStrings()[type & 0x1f]; }

const char* TypeString(uint_t type) { return TypeString((uint8_t)type); }

uint8_t TypeMask(uint8_t value) { return value & 0x1f; }

bool TypeIsArray(uint_t type) { return type >= kTypeCount; }

bool TypeIsSet(uint_t type) { return type >= kTypeCount; }

void* TypeAlign(type_t type, void* value) {
  ASSERT(value);
  if (type == 0) return nullptr;
  if (!TypeIsValid(type)) return nullptr;

  uint_t size = TypeFixedSize(type);
  if (type <= UI1) return value;
  type_t* value_ptr = reinterpret_cast<type_t*>(value);
#if WORD_SIZE == 2
  if (type <= HLF) return AlignUpPointer2<>(value);
#else
  if (type <= BOL) return AlignUp2<>(value);
#endif
  if (type <= TMS) return AlignUp<>(value, 3);
  if (type <= DEC) return AlignUp<>(value, 7);

  switch (type >> 6) {
    case 0:
      return value;
    case 1:
      return AlignUp2<>(value);
    case 2:
      return AlignUp<>(value, 3);
    case 3:
      return AlignUp<>(value, 7);
  }
  return 0;
}
/*
template <typename Char, typename UI>
inline char* WriteString(char* begin, char* end, const void* value) {
  begin = AlignUpPointer<char>(begin);
  if (end - begin < 2 * sizeof(UI)) return nullptr;
  const Char* source = reinterpret_cast<const Char*>(value);
  UI length = TextLength<UI, Char>(source);
  UI* target = reinterpret_cast<UI*>(begin);
  *target++ = length;
  return MemoryCopy(target, end, value, length + sizeof(Char));
}*/

template <typename UI>
inline char* WriteObj(char* begin, char* end, const void* value) {
  UI* target = AlignUpPointer<UI>(begin);
  const UI* source = reinterpret_cast<const UI*>(value);
  UI size = *source++;
  if (size < sizeof(UI) || size >=) return nullptr;
  *target++ = size;
  return MemoryCopy(target, end, value, size - sizeof(UI));
}

char* Write(char* begin, char* end, type_t type, const void* value) {
  // Algorithm:
  // 1.) Determine type.
  // 2.) Align begin pointer to type width.
  // 3.) Check for enough room in begin-end socket.
  // 4.) Use MemoryCopy to copy the data into the given begin-end socket.

  ASSERT(begin);
  ASSERT(end);
  ASSERT(value);
  if (!TypeIsValid(type)) return nullptr;

  if (type <= UI1) {
    char* target_1 = reinterpret_cast<char*>(begin);
    *target_1++ = *reinterpret_cast<const char*>(value);
    return target_1;
  }
  if (type <= BOL) {
    char16_t* target_2 = AlignUp2<char16_t>(begin);
    *target_2++ = *reinterpret_cast<const char16_t*>(value);
    return reinterpret_cast<char*>(target_2);
  }
  if (type <= TMS) {
    char32_t* target_4 = AlignUp<char32_t>(begin, 3);
    *target_4++ = *reinterpret_cast<const char32_t*>(value);
    return reinterpret_cast<char*>(target_4);
  }
  if (type <= DEC) {
    uint64_t* target_8 = AlignUp<uint64_t>(begin, 7);
    const uint64_t* source_8 = reinterpret_cast<const uint64_t*>(value);
    *target_8++ = *source_8++;
    if (type == DEC) {
      *target_8++ = *source_8;
      return reinterpret_cast<char*>(target_8);
    }
    return reinterpret_cast<char*>(target_8);
  }
  if (TypeIsString(type)) {
    switch (type >> 6) {
      case 0:
        return Print<char>(begin, end, reinterpret_cast<const char*>(value));
      case 1:
        return Print<char>(begin, end, reinterpret_cast<const char*>(value));
      case 2:
        return Print<char>(begin, end, reinterpret_cast<const char*>(value));
      case 3:
        return reinterpret_cast<char*>(
            Print<char16_t>(reinterpret_cast<uint16_t*>(begin),
                            reinterpret_cast<uint16_t*>(end),
                            reinterpret_cast<const uint16_t*>(value)));
      case 4:
        return Print<char>(begin, end, reinterpret_cast<const char*>(value));
      case 5:
        return reinterpret_cast<char*>(
            Print<char16_t>(reinterpret_cast<uint16_t*>(begin),
                            reinterpret_cast<uint16_t*>(end),
                            reinterpret_cast<const uint16_t*>(value)));
      case 6:
        return Print<char>(begin, end, reinterpret_cast<const char*>(value));
      case 7:
        return reinterpret_cast<char*>(
            Print<char16_t>(reinterpret_cast<uint16_t*>(begin),
                            reinterpret_cast<uint16_t*>(end),
                            reinterpret_cast<const uint16_t*>(value)));
    }
  }
  char array_type = type >> 6;
  switch (array_type) {
    case 0:
      return WriteObj<uint8_t>(begin, end, value);
    case 1:
      return WriteObj<uint16_t>(begin, end, value);
    case 2:
      return WriteObj<uint32_t>(begin, end, value);
    case 3:
      return WriteObj<uint64_t>(begin, end, value);
  }
  return nullptr;
}

bool TypeIsObj(type_t type) {
  if (type < OBJ) return false;
  return true;
}

bool TypeIsString(type_t type) {
  type &= 0x1f;
  if (type >= ADR && type <= TKN) return true;
  return false;
}

bool TypeIsUtf16(type_t type) { return (bool)(type & 0x20); }

inline int TypeSizeWidthCode(type_t type) { return type >> 6; }

}  // namespace _

#if USING_UTF8
namespace _ {
char* Print(char* begin, char* end, type_t type, const void* value) {
  return Print<char>(begin, end, type, value);
}  // namespace _
_::Utf8& operator<<(_::Utf8& utf, const _::TypeValue& item) {
  return utf.Set(_::Print(utf.begin, utf.end, item.type, item.value));
}
#endif
#if USING_UTF16
namespace _ {
char16_t* Print(char16_t* begin, char16_t* end, type_t type,
                const void* value) {
  return Print<char16_t>(begin, end, type, value);
}
}  // namespace _
_::Utf16& operator<<(_::Utf16& utf, const _::TypeValue& item) {
  return utf.Set(_::Print(utf.begin, utf.end, item.type, item.value));
}
#endif
#if USING_UTF32
namespace _ {
char32_t* Print(char32_t* begin, char32_t* end, type_t type,
                const void* value) {
  return Print<char32_t>(begin, end, type, value);
}
}  // namespace _
_::Utf32& operator<<(_::Utf32& utf, const _::TypeValue& item) {
  return utf.Set(_::Print(utf.begin, utf.end, item.type, item.value));
}
#endif

#endif  //< #if SEAM >= SEAM_0_0_3
