/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/type.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef SCRIPT_TYPES_H
#define SCRIPT_TYPES_H

#include "memory.h"

namespace _ {

/** List of the 32 ASCII Data Types.
    Types are organized with types that are allowed in text B-Sequences first,
    followed by objects which get created in text using Script Operations. */
typedef enum AsciiTypes {
    NIL = 0,    //< 0.  NIL/nil/void type.
    ADR,        //< 1.  UTF-8 Operand stack address.
    STR,        //< 2.  Nil-terminated ASCII, UTF-8, or UTF-32 string.
    TKN,        //< 3.  Nil-terminated ASCII, UTF-8, or UTF-32 string token.
    SI1,        //< 4.  8-bit signed integer.
    UI1,        //< 5.  8-bit unsigned integer.
    BOL,        //< 6.  8-bit non-zero asserted boolean variable.
    SI2,        //< 7.  16-bit signed integer.
    UI2,        //< 8.  16-bit unsigned integer.
    HLF,        //< 9.  16-bit floating-point number.
    SVI,        //< 10. 16-bit or 32-bit signed varint.
    UVI,        //< 11. 16-bit or 32-bit unsigned varint.
    SI4,        //< 12. 32-bit signed integer.
    UI4,        //< 13. 32-bit unsigned integer.
    FLT,        //< 14. 32-bit floating-point number.
    TMS,        //< 15. 32-bit second since epoch timestamp.
    TMU,        //< 16. 64-bit microsecond since epoch timestamp.
    SI8,        //< 17. 64-bit signed integer.
    UI8,        //< 18. 64-bit unsigned integer.
    DBL,        //< 19. 64-bit floating-point number.
    SV8,        //< 20. 64-bit signed varint.
    UV8,        //< 21. 64-bit unsigned varint.
    DEC,        //< 22. 128-bit floating-point number compatible with C#.
    SIN,        //< 23. N-bit signed integer.
    UIN,        //< 24. N-bit unsigned integer.
    OBJ,        //< 25. N-byte object with size unknown at compile time.
    BSQ,        //< 26. B-Sequence.
    ESC,        //< 27. Escape sequence of B-Sequences.
    LST,        //< 28. Stack of Type-Value tuples.
    BOK,        //< 29. Multiset of unordered Key-{Type-Value} tuples.
    DIC,        //< 30. One-to-one map of Key-{Type-Value} tuples.
    MAP,        //< 31. One-to-one map of Id-{Type-Value} tuples.
} AsciiType;

inline uintptr_t TypeAlign (char* cursor, uint_t type) {
#if WORD_SIZE >= 32
    if (type <= BOL) {
        return MemoryAlign2 (cursor);
    }
    if (type <= UVI) {
        return MemoryAlign4 (cursor);
    }
#else
    if (type <= UVI) {
        return MemoryAlign2 (cursor);
    }
    if (type <= TMS) {
        return MemoryAlign4 (cursor);
    }
#endif
    if (type <= UV8) {
        return MemoryAlign8 (cursor);
    }
    type = type >> 6;
    switch (type) {
        case 1: return MemoryAlign2 (cursor);
        case 2: return MemoryAlign4 (cursor);
        case 3: return MemoryAlign8 (cursor);
    }
    return 0;
}

/** An ROM string for one of the 32 types.
    C++11 variadic templates ensure there is only one copy in of the given
    string in ROM. */
template<char kCharA_, char kCharB_, char kCharC_>
inline uint32_t Type () {
    return ((uint32_t)kCharA_) & (((uint32_t)kCharB_) << 8) &
           (((uint32_t)kCharC_) << 16);
}

enum {
    kTypeCount = 32,  //< The starting index of invalid types.
};

/** Gets the width in bytes (1-8) of the given type.
KABUKI uint_t TypeSize (uint_t type); */

/** Returns true if the type is a valid list type. */
KABUKI bool TypeIsValid (uint_t type) {
    // @warning I'm not sure what I was thinking here.
    //if (type_name < TypeTexts ()[0] || type_name > TypeTexts ()[FS])
    //    return false;
    //return true;
    return false;
}

/** Returns a pointer to an array of pointers to the type names.*/
KABUKI const char** TypeStrings ();

/** Returns the name of the given type. */
KABUKI const char* TypeString (uint_t type) {
    if (type >= kTypeCount) {
        return "Invalid";
    }
    return TypeStrings ()[type];
}

/** Checks the last char of the token to check if it is a specified char. */
template<AsciiType kType_>
inline bool TypeCharCompare (char const token) {
    if (token != kChar_) {
        return 0xff;
    }
    return kType_;
}

/** Checks the last char of the token to check if it is a specified char. */
template<char kChar_, AsciiType kType_>
inline bool TypeCharCompare (char const token, char const d) {
    if (token != kChar_) {
        return 0xff;
    }
    return kType_;
}

/** Checks the last two char(string) of the token to check if it is a specified
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

/** Checks the last two char(string) of the token to check if it is a specified
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

/** Returns the type from the given index.
    @return A type 0-31 upon success and 0xff upon failure. */
KABUKI byte BsqRead (const char* input, const char* input_end);

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

/** Returns true if the given type is a Bag type. */
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
KABUKI uint_t TypeSize (uint_t type);

}       //< namespace _
#endif  //< SCRIPT_TYPES_H
