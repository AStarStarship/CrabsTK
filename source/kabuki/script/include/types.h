/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/types.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef KABUKI_SCRIPT_TYPES_H
#define KABUKI_SCRIPT_TYPES_H

#include "utils.h"

namespace _ {

/** List of the 32 data types.
    To make arrays of the 32 types, use the 3 MSb as follows:

    | Value | Description                                 |
    |:-----:|:--------------------------------------------|
    |   0   | Single element.                             |
    |   1   | 1-dimensional array with up to 2^7 values.  |
    |   2   | 1-dimensional array with up to 2^15 values. |
    |   3   | 1-dimensional array with up to 2^31 values. |
    |   4   | 1-dimensional array with up to 2^63 values. |
    |   5   | N-dimensional array with up to 2^7 values.  |
    |   6   | N-dimensional array with up to 2^15 values. |
    |   7   | N-dimensional array with up to 2^31 values. |

    @note This has been mimicked in order to match the ASCII C0 codes as
    closely as sanely possible. */
typedef enum {
    NIL = 0,    //< 0.  NIL/null/void type.
    SOA,        //< 1.  A Stack Operation Address.
    STR,        //< 2.  A UTF-8 string.
    ST2,        //< 3.  A UTF-16 string.
    ST4,        //< 4.  A UTF-32 string.
    ST8,        //< 5.  A UTF-64 string.
    BOL,        //< 6.  An 8-bit non-zero asserted boolean variable.
    SI1,        //< 7.  An 8-bit signed integer.
    UI1,        //< 8.  An 8-bit unsigned integer.
    SI2,        //< 9.  A 16-bit signed integer.
    UI2,        //< 10. A 16-bit unsigned integer.
    HLF,        //< 11.  A 16-bit floating-point number.
    SI4,        //< 12. A 32-bit signed integer.
    UI4,        //< 13. A 32-bit unsigned integer.
    FLT,        //< 14. A 32-bit floating-point number.
    TMS,        //< 15. A 32-bit second since epoch timestamp.
    TMU,        //< 16. A 64-bit microsecond since epoch timestamp.
    SI8,        //< 17. A 64-bit signed integer.
    UI8,        //< 18. A 64-bit unsigned integer.
    DBL,        //< 19. A 64-bit floating-point number.
    SV2,        //< 20. A 16-bit signed varint.
    UV2,        //< 21. A 16-bit unsigned varint.
    SV4,        //< 22. A 32-bit signed varint.
    UV4,        //< 23. A 32-bit unsigned varint.
    SV8,        //< 24. A 64-bit signed varint.
    UV8,        //< 25. A 64-bit unsigned varint.
    BSQ,        //< 26. A B-Sequence without a hash.
    BSH,        //< 27. A B-Sequence with a hash.
    KHT,        //< 28. A Key-Type-Value unique set with hash-table (map).
    KTV,        //< 29. A Key-Type-Value multiset (unordered_map).
    TVA,        //< 30. A Type-Value array.
    PAK,        //< 31. A Packet for splitting up large sets.
} TType;

enum {
    kInvalidType = 32,  //< The starting index of invalid types.
};

/** Gets the width in bytes (1-8) of the given type. */
KABUKI uint_t SizeOf (uint_t type);

/** Aligns the given pointer to a word-aligned boundary. */
KABUKI const byte* TypeAlignments ();

/** Aligns the given pointer to a word-aligned boundary. */
KABUKI uintptr_t TypeAlign2 (byte* ptr);

/** Aligns the given pointer to a word-aligned boundary. */
KABUKI uintptr_t TypeAlign4 (byte* ptr);

/** Aligns the given pointer to a word-aligned boundary. */
KABUKI uintptr_t TypeAlign8 (byte* ptr);

/** Aligns the given pointer to a word-aligned boundary. */
KABUKI uintptr_t TypeAlign (byte* ptr, byte type);

/** Aligns the given pointer to a word-aligned boundary. */
KABUKI uintptr_t TypeAlign (byte* ptr, uint_t type);

/** Returns true if the type is a valid list type. */
KABUKI bool TypeIsValid (uint_t type);

/** Returns a pointer to an array of pointers to the type names.*/
KABUKI const char** TypeStrings ();

/** Returns true if the TypeName is one of the ErrorStrings. */
KABUKI bool TypeIsValid (const char* type_name);

/** Returns the name of the given type. */
KABUKI const char* TypeString (uint_t type);

/** Checks to see if the given byte is a delimiter. */
KABUKI bool CheckDelimiter (char const c);

/** Checks the last char of the token to check if it is a specified char. */
template<char c>
KABUKI bool CheckLastLetter (uint16_t const token);

/** Checks the last two char(string) of the token to check if it is a specified char. */
template<char LetterTwo, char LetterThree>
KABUKI bool CheckLastLetters (uint32_t const Token);

/** Returns the type from the given index.
    @warning Untested. */
KABUKI byte ReadType (const char* string);

/** Masks off the lower 5-LSb to get the type. */
KABUKI byte MaskType (byte value);

/** Returns true if this type has a buffer. */
KABUKI bool TypeHasLength (uint_t type);

/** Returns true if the given type is an Array type. */
KABUKI bool TypeIsArray (uint_t type);

/** Returns true if the given type is a Bag type. */
KABUKI bool TypeIsSet (uint_t type);

/** Returns true if this type has a buffer. */
KABUKI bool TypeIsHierarchical (uint_t type);

/** Converts from a 2's complement integer to a signed varint.
    @param value A 2's complement integer.
    @return An signed varint.
    A signed varint is an complemented signed integer with the sign in the 
    LSb. To convert a negative 2's complement value to positive invert the bits 
    and add one.
*/
template<typename T>
KABUKI T PackSignedVarint (T value) {
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
KABUKI T UnpackSignedVarint (T value) {
    T sign_bit = value << (sizeof (T) * 8 - 1);
    value = value >> 1;
    if (sign_bit != 0) {
        value = ~(value - 1);
        return value;
    }
    return value;
}

}       //< namespace _
#endif  //< KABUKI_SCRIPT_TYPES_H
