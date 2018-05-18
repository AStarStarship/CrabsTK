/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/type.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_CRABS_TYPES
#define HEADER_FOR_CRABS_TYPES

#include "memory.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {

/** List of the 32 ASCII Data Types.
    Types are organized with types that are allowed in text B-Sequences first,
    followed by objects which get created in text using Script Operations. */
typedef enum AsciiTypes {
    NIL = 0,    //< 0.  NIL/nil/void type.
    SI1,        //< 1.  8-bit signed integer.
    UI1,        //< 2.  8-bit unsigned integer.
    SI2,        //< 3.  16-bit signed integer.
    UI2,        //< 4.  16-bit unsigned integer.
    HLF,        //< 5.  16-bit floating-point number.
    SVI,        //< 6.  16-bit or 32-bit signed varint.
    UVI,        //< 7.  16-bit or 32-bit unsigned varint.
    BOL,        //< 8.  16-bit or 32-bit boolean signed varint.
    SI4,        //< 9.  32-bit signed integer.
    UI4,        //< 10. 32-bit unsigned integer.
    FLT,        //< 11. 32-bit floating-point number.
    TMS,        //< 12. 32-bit second since epoch timestamp.
    TMU,        //< 13. 64-bit microsecond since epoch timestamp.
    SI8,        //< 14. 64-bit signed integer.
    UI8,        //< 15. 64-bit unsigned integer.
    DBL,        //< 16. 64-bit floating-point number.
    SV8,        //< 17. 64-bit signed varint.
    UV8,        //< 18. 64-bit unsigned varint.
    DEC,        //< 19. 128-bit floating-point number.
    OBJ,        //< 20. N-byte object with size not specified in BSQ.
    SIN,        //< 21. N-bit signed integer.
    UIN,        //< 22. N-bit unsigned integer or data structure.
    ADR,        //< 23. UTF-8 Operand stack address.
    STR,        //< 24. A UTF-8 string.
    TKN,        //< 25. A UTF-8 string token without whitespace.
    BSQ,        //< 26. B-Sequence.
    ESC,        //< 27. Escape sequence Expression.
    LST,        //< 28. Stack of Type-Value tuples.
    MAP,        //< 29. One-to-one map of Id-{Type-Value} tuples.
    BOK,        //< 30. Multiset of unordered Key-{Type-Value} tuples.
    DIC,        //< 31. One-to-one map of Key-{Type-Value} tuples.
} AsciiType;

enum {
    kTypeLast2Byte = (sizeof (int) == 4) ? BOL : HLF,
};

/** Checks if the given type is valid.
    @return False if the given type is an 8-bit LST, MAP, BOK, or DIC. */
inline bool TypeIsValid (char type) {
    if (type >= LST && type <= DIC)
        return false;
    return true;
}

/** Aligns the given pointer to the correct word boundary for the type. */
void* TypeAlign (uint8_t type, void* value);

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
inline const char* TypeString (uint_t type) {
    return TypeStrings ()[type & 0x1f];
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

/** Returns true if this type has a buffer.
inline bool TypeHasLength (uint_t type) {
    switch (type) {
        case STR: return true;
        case ST2: return true;
        case ST4: return true;
        case OBJ: return true;
    }
    return false;
} */

/** Returns true if the given type is an Array type. */
inline bool TypeIsArray (uint_t type) {
    return type >= kTypeCount;
}

/** Returns true if the given type is a BOK type. */
inline bool TypeIsSet (uint_t type) {
    return type >= kTypeCount;
}

/** Converts from a 2's complement integer to a signed varint.
    @param value A 2's complement integer.
    @return An signed varint.
    A signed varint is an complemented signed integer with the sign in the
    LSb. To convert a negative 2's complement value to positive invert the bits
    and add one.
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
    @param  A signed integer casted as an unsigned integer.
    @return Returns a standard signed integer cased as unsigned.
    A varint is an complemented signed integer with the sign in the LSb. */
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
KABUKI void* TypeAlign (uint8_t type, void* value);

KABUKI char* TypeWrite (uint8_t type, char* begin, char* end, const char* source);

inline bool TypeIsObject (uint8_t type) {
    if (type < OBJ) return false;
    return true;
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#endif  //< HEADER_FOR_CRABS_TYPES
