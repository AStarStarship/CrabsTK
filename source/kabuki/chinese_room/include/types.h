/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/types.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_TYPES_H
#define CHINESE_ROOM_TYPES_H

#include "utils.h"

namespace _ {

/** List of the 32 data types.
    This has been mimicked in order to match the ASCII C0 codes as closely 
    as sanely possible. Many of the codes just don't make sense as types, but 
    the codes were picked for the specific purpose that they are not human 
    readable, with the exception of HT, VT, CR, and LF. */
typedef enum {
    NIL   = 0,  //< 0.  NIL/null/void type.
    SOH   = 1,  //< 1.  An address to an I2P device.
    STX   = 2,  //< 2.  A UTF-8/ASCII string.
    SI1   = 3,  //< 3.  An 8-bit signed integer.
    UI1   = 4,  //< 4.  An 8-bit unsigned integer.
    BOL   = 5,  //< 5.  An 8-bit non-zero asserted boolean variable.
    SI2   = 6,  //< 6.  A 16-bit signed integer.
    UI2   = 7,  //< 7.  A 16-bit unsigned integer.
    HLF   = 8,  //< 8.  A 16-bit floating-point number.
    SI4   = 9,  //< 9.  A 32-bit signed integer.
    UI4   = 10, //< 10. A 32-bit unsigned integer.
    FLT   = 11, //< 11. A 32-bit floating-point number.
    TMS   = 12, //< 12. A 32-bit second since epoch timestamp.
    TMU   = 13, //< 13. A 64-bit microsecond since epoch timestamp.
    SI8   = 14, //< 14. A 64-bit signed integer.
    UI8   = 15, //< 15. A 64-bit unsigned integer.
    DBL   = 16, //< 16. A 64-bit floating-point number.
    SV2   = 17, //< 17. A 16-bit signed varint.
    UV2   = 18, //< 20. A 16-bit unsigned varint.
    SV4   = 19, //< 18. A 32-bit signed varint.
    UV4   = 20, //< 21. A 32-bit unsigned varint.
    SV8   = 21, //< 19. A 64-bit signed varint.
    UV8   = 22, //< 22. A 64-bit unsigned varint.
    AR1   = 23, //< 23. An array of up to 2^8-1 like primitive types 3-16.
    AR2   = 24, //< 24. An array of up to 2^16-1 like primitive types 3-16.
    AR4   = 25, //< 25. An array of up to 2^32-1 like primitive types 3-16.
    AR8   = 26, //< 26. An array of up to 2^32-1 like primitive types 3-16.
    ESC   = 27, //< 27. An atomic escape sequence of SCRIPT procedure calls.
    BK8   = 28, //< 28. A bag with up to 2^31-2 members and 2^64-1 bytes data.
    BK4   = 29, //< 29. A bag with up to 2^15-2 members and 2^32-1 bytes data.
    BK2   = 30, //< 30. A bag with up to 2^7-2 members and 2^16-1 bytes data.
    US    = 31, //< 31. A unit separator for breaking files and data into 
                //<     transmission blocks.
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

/** Checks the last two char(s) of the token to check if it is a specified char. */
template<char LetterTwo, char LetterThree>
KABUKI bool CheckLastLetters (uint32_t const Token);

/** Returns the type from the given index.
    @warning Untested. */
KABUKI byte ReadType (const char* s);

/** Masks off the lower 5-LSb to get the type. */
KABUKI byte MaskType (byte value);

/** Returns true if this type has a buffer. */
KABUKI bool TypeHasLength (uint_t type);

/** Returns true if the given type is an Array type. */
KABUKI bool TypeIsArray (uint_t type);

/** Returns true if the given type is a Bag type. */
KABUKI bool TypeIsBag (uint_t type);

/** Returns true if this type has a buffer. */
KABUKI bool TypeIsHierarchical (uint_t type);

/** Converts from a 2's complement integer to a signed varint.
    @param value A 2's complement integer.
    @return An signed varint.
    A signed varint is an uncomplemented signed integer with the sign in the 
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
    // Else don't uncomplement.
    return value << 1;
}

/** Converts from a signed varint to a 2's complement signed integer.
    @param  A signed integer casted as an unsigned integer.
    @return Returns a standard signed integer cased as unsigned.
    A varint is an uncomplemented signed integer with the sign in the LSb. */
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
#endif  //< CHINESE_ROOM_TYPES_H
