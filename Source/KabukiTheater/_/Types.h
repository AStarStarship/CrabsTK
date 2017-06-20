/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoom\Type.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

             All right reserved(R).

        Licensed under the Ap License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_TYPES_H
#define CHINESEROOM_TYPES_H

#include "Config.h"

namespace _ {

typedef enum
/** List of the 32 data types.
    This has been mimiced in order to match the ASCII C0 codes as closely 
    as sanely possible. Many of the codes just don't make sense as types, but
    the codes were picked for the specific purpose that they are not human 
    readable, with the exception of HT, VT, CR, and LF. */
{   
    NIL   = 0,  //< 0.  NIL/null/void type.
    STR   = 1,  //< 1.  A UTF-8/ASCII string.
    STX   = 2,  //< 2.  A UTF-16 string.
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
    ESC   = 27, //< 27. An atomic escape sequence of I2P procedure calls.
    BK8   = 28, //< 28. A book with up to 2^31-1 members and 2^64-1 bytes data.
    BK4   = 29, //< 29. A book with up to 2^15-1 members and 2^32-1 bytes data.
    BK2   = 30, //< 30. A book with up to 2^15-1 members and 2^16-1 bytes data.
    US    = 31, //< 31. A unit separator for breaking files and data into transmission blocks.
} TData;

inline uint_t getSizeOfType(uint_t Type)
/*< Gets the width in bytes(1-8) of the given type. */
{
    static const int8_t widths[] =
    {
        0,      //< NIL: 0 
        0,      //< STR: 1 
        0,      //< STX: 2 
        1,      //< SI1: 3 
        1,      //< UI1: 4 
        1,      //< BOL: 5 
        2,      //< SI2: 6 
        2,      //< UI2: 7 
        2,      //< HLF: 8 
        4,      //< SI4: 9 
        4,      //< UI4: 10
        4,      //< FLT: 11
        4,      //< TMS: 12
        8,      //< SI8: 13
        8,      //< UI8: 14
        8,      //< DBL: 15
        8,      //< TMU: 16
        3,      //< SV2: 17
        9,      //< SV4: 18
        9,      //< SV8: 19
        3,      //< UV2: 20
        5,      //< UV4: 21
        9,      //< UV8: 22
        0,      //< GS1: 23
        0,      //< GR2: 24
        0,      //< GR4: 25
        0,      //< GR8: 26
        0,      //< ESC: 27
        0,      //< FS:  28
        0,      //< BK2: 29
        0,      //< BK4: 30
        0,      //< BK8: 31
    };
    return widths[Type & 0x1f];
}

inline uint_t getAlignment(uint_t Type)
/*< Gets aliment required for the specified type. 
    The types that get aligned are 2, 4, and 8-byte-wide types. */
{
    static const int8_t widths[] =
    {
        0,      //< NIL: 0 
        0,      //< STR: 1 
        2,      //< STX: 2 
        0,      //< SI1: 3 
        0,      //< UI1: 4 
        0,      //< BOL: 5 
        2,      //< SI2: 6 
        2,      //< UI2: 7 
        2,      //< HLF: 8 
        4,      //< SI4: 9 
        4,      //< UI4: 10
        4,      //< FLT: 11
        4,      //< TMS: 12
        8,      //< SI8: 13
        8,      //< UI8: 14
        8,      //< DBL: 15
        4,      //< TMU: 16
        0,      //< SV2: 17
        0,      //< UV2: 18
        0,      //< SV4: 19
        0,      //< UV4: 20
        0,      //< SV8: 21
        0,      //< UV8: 22
        4,      //< AR1: 23
        8,      //< AR2: 24
        8,      //< AR4: 25
        8,      //< AR8: 26
        8,      //< ESC: 27
        8,      //< BK8: 28
        8,      //< BK4: 29
        8,      //< BK2: 30
        8,      //< US:  31
    };
    return widths[Type & 0x1f];
}

inline bool typeIsValid(uint_t type)
/*< Returns true if the type is a valid list type. */
{

    return type > 31 ? false : true;
}

inline bool typeIsValid(const char* TypeName)
/*< Returns true if the TypeName is one of the ErrorStrings. */
{

    if (TypeName < typeStrings ()[0] || TypeName > typeStrings ()[BK8]) return false;
    return true;
}

inline const char** typeStrings()
/*< Returns a pointer to an array of pointers to the type names.*/
{
    static const char* names[] = {
        "NIL",
        "STR",
        "STX",
        "SI1",
        "UI1",
        "BOL",
        "SI2",
        "UI2",
        "HLF",
        "SI4",
        "UI4",
        "FLT",
        "TMS",
        "TMU",
        "SI8",
        "UI8",
        "DBL",
        "SV2",
        "UV2",
        "SV4",
        "UV4",
        "SV8",
        "UV8",
        "AR1",
        "AR2",
        "AR4",
        "AR8",
        "ESC",
        "BK8",
        "BK4",
        "BK2",
        "US",
    };
    return names;
}

inline const char* typeString(uint_t type)
/*< Returns the name of the given type. */
{
    return typeStrings ()[type];
}


inline bool checkDelimiter (char const Char)
{
    if (Char == 0) return false;
    if (Char == ' ') return false;
    if (Char == '\n') return false;
    if (Char == '\t') return false;
    return true;
}

template<char Char>
bool checkLastLetter (uint16_t const Token)
{
    if (Char != (char)Token) return true;
    return checkDelimiter (Token >> 8);
}

template<char LetterTwo, char LetterThree>
bool checkLastLetters (uint32_t const Token)
{
    char check = Token >> 8;
    if (LetterTwo != check) return true;

    check = Token >> 16;
    if (LetterThree != check) return true;

    check = Token >> 24;
    return checkDelimiter (Token >> 8);
}

inline byte getType(const char* s)
/*< Returns the type from the given index. */
{
    uint32_t token = *((uint32_t*)s);

    char letter = (char)token;

    switch (letter)
    {
        case 'A': if ((char)(token >> 8) != 'R') return 0xff;
            switch (letter = (char)(token >> 16))
            {
                case '1': return checkDelimiter (token >> 24) ? 0xff : AR1;
                case '2': return checkDelimiter (token >> 24) ? 0xff : AR2;
                case '4': return checkDelimiter (token >> 24) ? 0xff : AR4;
                case '8': return checkDelimiter (token >> 24) ? 0xff : AR8;
                default: return 0xff;
            }
        case 'B': switch (letter = (char)(token >> 8))
        {
            case 'K': switch (letter = (char)(token >> 16))
            {
                case '2': return checkDelimiter (token >> 24) ? 0xff : BK2;
                case '4': return checkDelimiter (token >> 24) ? 0xff : BK4;
                case '8': return checkDelimiter (token >> 24) ? 0xff : BK8;
                default: return 0xff;
            }
            case 'O': return checkLastLetter<'L'> (token >> 16) ? 0xff : BOL;
            default: return 0xff;
        }
        case 'D': return checkLastLetters<'B', 'L'> (token >> 16) ? 0xff : DBL;
        case 'E': return checkLastLetters<'S', 'C'> (token >> 16) ? 0xff : ESC;
        case 'F': return checkLastLetters<'L', 'T'> (token >> 16) ? 0xff : FLT;
        case 'G': return 0xff;
        case 'H': return checkLastLetters<'L', 'F'> (token >> 16) ? 0xff : HLF;
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M': return 0xff;
        case 'N': return checkLastLetters<'I', 'L'> (token >> 16) ? 0xff : NIL;
        case 'O':
        case 'P':
        case 'Q':
        case 'R': return 0xff;
        case 'S': switch (letter = (char)(token >> 8))
        {
            case 'I': switch (letter = (char)(token >> 16))
            {
                case '1': return checkDelimiter (token >> 24) ? 0xff : SI1;
                case '2': return checkDelimiter (token >> 24) ? 0xff : SI2;
                case '4': return checkDelimiter (token >> 24) ? 0xff : SI4;
                case '8': return checkDelimiter (token >> 24) ? 0xff : SI8;
                default: return 0xff;
            }
            case 'T': switch (letter = (char)(token >> 16))
            {
                case 'R': return checkDelimiter (token >> 24) ? 0xff : STR;
                default: return 0xff;
            }
            case 'V': switch (letter = (char)(token >> 16))
            {
                case '4': return checkDelimiter (token >> 24) ? 0xff : SV4;
                case '8': return checkDelimiter (token >> 24) ? 0xff : SV8;
                default: return 0xff;
            }
            default: break;
        }
        case 'T': switch (letter = (token >> 8))
        {
            case 'M': switch (letter = (token >> 16))
            {
                case 'E': return checkDelimiter (token >> 24) ? 0xff : TMS;
                case 'U': return checkDelimiter (token >> 24) ? 0xff : TMU;
                default: return 0xff;
            }
            default: return 0xff;
        }
        case 'U': switch (letter = (token >> 8))
        {
            case 'S': return checkDelimiter (token >> 16) ? 0xff : US;
            case 'I': switch (letter = (char)(token >> 16))
            {
                case '1': return checkDelimiter (token >> 24) ? 0xff : UI1;
                case '2': return checkDelimiter (token >> 24) ? 0xff : UI2;
                case '4': return checkDelimiter (token >> 24) ? 0xff : UI4;
                case '8': return checkDelimiter (token >> 24) ? 0xff : UI8;
                default: return 0xff;
            }
            case 'V': switch (letter = (char)(token >> 16))
            {
                case '4': return checkDelimiter (token >> 24) ? 0xff : UV4;
                case '8': return checkDelimiter (token >> 24) ? 0xff : UV8;
                default: return 0xff;
            }
            default: return 0xff;
        }
    }
    return 0xff;
}

inline byte maskType(byte value)
/*< Masks off the lower 5-LSb to get the type. */
{
    return value & 0x1f;
}

inline bool typeHasBuffer(uint_t type)
/*< Returns true if this type has a buffer. */
{
    if (type == STR || type == STX || type < AR1)
        return false;
    return true;
}

inline bool typeIsHierarchical(uint_t type)
/*< Returns true if this type has a buffer. */
{
    return type > 20;
}

}       //< namespace _

#endif  //< CHINESEROOM_TYPES_H
