/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/type.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#ifndef HEADER_FOR_CRABS_TYPES
#define HEADER_FOR_CRABS_TYPES
// Dependencies:
#include "memory.h"
// End dependencies.

typedef uint8_t type_t;

namespace _ {

/** A type-value tuple. */
struct KABUKI TypeValue {
    type_t      type;  //< ASCII Type.
    const void* value; //< Pointer to the value data.

    /** Stores the type and value. */
    TypeValue (type_t type, const void* value = nullptr);
};

/** List of the 32 ASCII Data Types.
    Types are organized with types that are allowed in text B-Sequences first,
    followed by objects which get created in text using Script Operations. */
typedef enum AsciiTypes {
    NIL = 0, //< 0.  NIL/nil/void type.
    SI1,     //< 1.  8-bit signed integer.
    UI1,     //< 2.  8-bit unsigned integer.
    SI2,     //< 3.  16-bit signed integer.
    UI2,     //< 4.  16-bit unsigned integer.
    HLF,     //< 5.  16-bit floating-point number.
    BOL,     //< 6.  16-bit or 32-bit boolean signed varint.
    SVI,     //< 7.  16-bit or 32-bit signed varint.
    UVI,     //< 8.  16-bit or 32-bit unsigned varint.
    SI4,     //< 9.  32-bit signed integer.
    UI4,     //< 10. 32-bit unsigned integer.
    FLT,     //< 11. 32-bit floating-point number.
    TMS,     //< 12. 32-bit second since epoch timestamp.
    TMU,     //< 13. 64-bit microsecond since epoch timestamp.
    SI8,     //< 14. 64-bit signed integer.
    UI8,     //< 15. 64-bit unsigned integer.
    DBL,     //< 16. 64-bit floating-point number.
    SV8,     //< 17. 64-bit signed varint.
    UV8,     //< 18. 64-bit unsigned varint.
    U16,     //< 19. 128-bit unsigned integer.
    DEC,     //< 20. 128-bit floating-point number.
    U32,     //< 21. 256-bit unsigned integer.
    OBJ,     //< 22. N-byte object.
    ADR,     //< 23. UTF-8 Operand stack address.
    STR,     //< 24. A UTF-8 string.
    TKN,     //< 25. A UTF-8 string token without whitespace.
    BSQ,     //< 26. B-Sequence.
    EXP,     //< 27. Script Expression.
    LST,     //< 28. Stack of Type-Value tuples.
    MAP,     //< 29. One-to-one map of Id-{Type-Value} tuples.
    BOK,     //< 30. Multiset of unordered Key-{Type-Value} tuples.
    DIC,     //< 31. One-to-one map of Key-{Type-Value} tuples.
} AsciiType;

enum {
    kTypeLast2Byte = (sizeof (int) == 4) ? BOL : HLF,
};

/** Checks if the given type is valid.
    @return False if the given type is an 8-bit LST, MAP, BOK, or DIC. */
inline bool TypeIsValid (type_t type) {
    if (type >= LST && type <= DIC || 
        (type >= (ADR + 32) && type <= (TKN + 32)))
        return false;
    return true;
}

/** Aligns the given pointer to the correct word boundary for the type. */
KABUKI void* TypeAlign (type_t type, void* value);

/** An ROM string for one of the 32 types.
    C++11 variadic templates ensure there is only one copy in of the given
    string in ROM. */
template<char kCharA_, char kCharB_, char kCharC_>
inline uint32_t T () {
    return ((uint32_t)kCharA_) & (((uint32_t)kCharB_) << 8) &
           (((uint32_t)kCharC_) << 16);
}

enum {
    kTypeCount = 32,  //< The starting index of invalid types.
};

/** Returns a pointer to an array of pointers to the type names.*/
KABUKI const char** TypeStrings ();

/** Returns the name of the given type. */
inline const char* TypeString (type_t type) {
    return TypeStrings ()[type & 0x1f];
}

/** Returns the name of the given type. */
inline const char* TypeString (uint_t type) {
    return TypeString ((uint8_t)type);
}

/** Checks the last char of the text to check if it is a specified char. */
template<AsciiType kType_>
inline bool TypeCharCompare (char const text) {
    if (text != kChar_) {
        return 0xff;
    }
    return kType_;
}

/** Checks the last char of the text to check if it is a specified char. */
template<char kChar_, AsciiType kType_>
inline bool TypeCharCompare (char const text, char const d) {
    if (text != kChar_) {
        return 0xff;
    }
    return kType_;
}

/** Checks the last two char(string) of the text to check if it is a specified
    char. */
template<char kLetterTwo_, char kLetterThree_, AsciiType kType_>
inline byte TypeCharCompare (char const b, char const c, char const d) {
    if (kLetterTwo_ != b) {
        return 0xff;
    }
    if (kLetterThree_ != c) {
        return 0xff;
    }
    if (!IsSpace (d)) {
        return 0xff;
    }
    if (IsDelimiter)
        return kType_;
}

/** Checks the last two char(string) of the text to check if it is a specified
    char. */
template<char kLetterTwo_, char kLetterThree_, AsciiType kType_>
inline byte TypeCharCompareObject (char const b, char const c, char const d,
                                   char const e) {
    if (kLetterTwo_ != b) {
        return 0xff;
    }
    if (kLetterThree_ != c) {
        return 0xff;
    }
    if (!IsSpace (e)) {
        return 0xff;
    }
    if (d == '2') {
        return kType_;
    }
    if (d == '4') {
        return kType_;
    }
    if (d == '8') {
        return kType_;
    }
    return 0xff;
}

/** Masks off the lower 5-LSb to get the type. */
inline byte TypeMask (byte value) {
    return value & 0x1f;
}

/** Returns true if the given type is an Array type. */
inline bool TypeIsArray (uint_t type) {
    return type >= kTypeCount;
}

/** Returns true if the given type is a BOK type. */
inline bool TypeIsSet (uint_t type) {
    return type >= kTypeCount;
}

/** Converts from a 2's complement integer to a signed varint.
    A signed varint is an complemented signed integer with the sign in the
    LSb. To convert a negative 2's complement value to positive invert the bits
    and add one.
    @param  value A 2's complement integer.
    @return An signed varint.
*/
template<typename T>
inline T TypePackVarint (T value) {
    T sign_bit = value >> ((sizeof (T) * 8 - 1));   //< Extract the sign bit.
    if (sign_bit != 0) {
        T uncomplemented = (~value) + 1;
        return sign_bit | (uncomplemented << 1);
    }
    // Else don't complemented.
    return value << 1;
}

/** Converts from a signed varint to a 2's complement signed integer.
    A varint is an complemented signed integer with the sign in the LSb.
    @param  A signed integer casted as an unsigned integer.
    @return Returns a standard signed integer cased as unsigned. */
template<typename T>
inline T TypeUnpackVarint (T value) {
    T sign_bit = value << (sizeof (T) * 8 - 1);
    value = value >> 1;
    if (sign_bit) {
        value = ~(value - 1);
        return value;
    }
    return value;
}

/** Returns the size or max size of the given type. */
KABUKI uint_t TypeFixedSize (uint_t type);

/** Gets the next address that a data type may be stored at. */
KABUKI void* TypeAlign (type_t type, void* value);

/** Writes the given value to the socket. */
KABUKI char* Write (char* begin, char* end, type_t type, const void* source);

/** Returns true if the given type is an ASCII Object. */
inline bool TypeIsObject (type_t type) {
    if (type < OBJ) return false;
    return true;
}

/** Returns true if the given type is a string type. */
inline bool TypeIsString (type_t type) {
    type &= 0x1f;
    if (type >= ADR && type <= TKN)
        return true;
    return false;
}

/** Checks if the given type is UTF-16.
    @param  type The type to check.
    @return True if the given type is UTF-16. */
inline bool TypeIsUtf16 (type_t type) {
    return (bool)(type & 0x20);
}

/** Prints th given type or type-value.
    @param printer The printer to print to.
    @param type    The type to print.
    @param value   The value to print or nil. */
KABUKI Printer& PrintType (Printer& printer, type_t type, const void* value);

inline int TypeSizeWidthCode (type_t type) {
    return type >> 6;
}

template<typename T>
T* TypeAlignUpPointer (void* pointer, type_t type) {
    if (type <= UI1)
        return reinterpret_cast<T*> (pointer);
    else if (type <= kTypeLast2Byte)
        return AlignUpPointer2<T> (pointer);
    else if (type <= TMS)
        return AlignUpPointer4<T> (pointer);
    else if (type <= DEC)
        return AlignUpPointer8<T> (pointer);
    // else it's an ASCII Object.
    uintptr_t ptr  = reinterpret_cast<uintptr_t> (pointer),
              mask = (type >> 6);
    if (mask == 0)
        return reinterpret_cast<T*> (ptr);
    ptr += ((~ptr) + 1) & mask;
    printf ("\npointer:0x%p type:%u mask:%u result = %p", pointer, 
            (uint)(type >> 6), (uint)mask, reinterpret_cast<T*> (ptr));
    return reinterpret_cast<T*> (ptr);
}

}       //< namespace _

inline _::Printer& operator<< (_::Printer& printer, 
                               const _::TypeValue& type_value) {
    return _::PrintType (printer, type_value.type, type_value.value);
}

#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
#endif  //< HEADER_FOR_CRABS_TYPES
