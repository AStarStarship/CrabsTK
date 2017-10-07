/** The Chinese Room
    @version 0.x
    @file    ~/chinese_room/impl/types.cpp
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

#include "../include/types.h"
#include "../include/utils.h"

namespace _ {

KABUKI uint_t SizeOf (uint_t type) {
    static const int8_t kWidths[] =
    {
        0,      //< NIL: 0 
        0,      //< SOH: 1 
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
        8,      //< TMU: 13
        8,      //< SI8: 14
        8,      //< UI8: 15
        8,      //< DBL: 16
        3,      //< SV2: 17
        3,      //< UV2: 18
        9,      //< SV4: 19
        5,      //< UV4: 20
        9,      //< SV8: 21
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
    if (type >= 32)
        return 0;
    return kWidths[type];
}

KABUKI const byte* TypeAlignments () {
    static const byte kWidths[] =
    {
        0,      //< NIL: 0 
        0,      //< SOH: 1 
        0,      //< STX: 2 
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
        8,      //< TMU: 13
        8,      //< SI8: 14
        8,      //< UI8: 15
        8,      //< DBL: 16
        2,      //< SV2: 17
        2,      //< UV2: 18
        4,      //< SV4: 19
        4,      //< UV4: 20
        8,      //< SV8: 21
        8,      //< UV8: 22
        0,      //< AR1: 23
        2,      //< AR2: 24
        4,      //< AR4: 25
        8,      //< AR8: 26
        8,      //< ESC: 27
        8,      //< BK8: 28
        8,      //< BK4: 29
        8,      //< BK2: 30
        8,      //< US:  31
    };
    return kWidths;
}

KABUKI uintptr_t TypeAlign2 (byte* ptr) {
    // Mask off lower bit and add it to the ptr.
    uintptr_t value = reinterpret_cast<uintptr_t> (ptr);
    return value & 0x1;
}

KABUKI uintptr_t TypeAlign4 (byte* ptr) {
    return WordAlignOffset<int> (ptr);
}

KABUKI uintptr_t TypeAlign8 (byte* ptr) {
    return WordAlignOffset<long> (ptr);
}

KABUKI uintptr_t TypeAlign (byte* ptr, byte type) {
    if (type >= 32)     return 0;
    byte alignment = TypeAlignments ()[type];
    if (alignment <= 1) return 0;
    uintptr_t offset;
    if (alignment == 4) return TypeAlign4 (ptr);
    if (alignment == 8) return TypeAlign8 (ptr);
    // else alignment == 2
    // Mask off lower bit and add it to the ptr.
    uintptr_t value = reinterpret_cast<uintptr_t> (ptr);
    offset = value & 0x1;
    return offset;
}

KABUKI uintptr_t TypeAlign (byte* ptr, uint_t type) {
    if (type >= 32)     return 0;
    byte alignment = TypeAlignments ()[type];
    if (alignment <= 1) return 0;
    uintptr_t offset;
    if (alignment == 4) return TypeAlign4 (ptr);
    if (alignment == 8) return TypeAlign8 (ptr);
    // else alignment == 2
    // Mask off lower bit and add it to the ptr.
    uintptr_t value = reinterpret_cast<uintptr_t> (ptr);
    offset = value & 0x1;
    return offset;
}

KABUKI bool TypeIsValid (uint_t type) {
    return type > 31 ? false : true;
}

KABUKI const char ** TypeStrings () {
    static const char * kNames[] = {
        "NIL",
        "SOH",
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
        "Invalid"
    };
    return kNames;
}

KABUKI bool TypeIsValid (const char * type_name) {
    if (type_name < TypeStrings ()[0] || type_name > TypeStrings ()[BK8])
        return false;
    return true;
}

KABUKI const char * TypeString (uint_t type) {
    if (type >= kInvalidType)
        return "Invalid";
    return TypeStrings ()[type];
}

KABUKI bool CheckDelimiter (char const c) {
    if (c == 0   ) return false;
    if (c == ' ' ) return false;
    if (c == '\n') return false;
    if (c == '\t') return false;
    return true;
}

template<char c>
KABUKI bool CheckLastLetter (uint16_t const token) {
    if (c != (char)token) return true;
    return CheckDelimiter (token >> 8);
}

template<char LetterTwo, char LetterThree>
KABUKI bool CheckLastLetters (uint32_t const Token) {
    char check = Token >> 8;
    if (LetterTwo != check) return true;

    check = Token >> 16;
    if (LetterThree != check) return true;

    check = Token >> 24;
    return CheckDelimiter (Token >> 8);
}

KABUKI byte ReadType (const char * s) {
    uint32_t token = * ((uint32_t*)s);

    char_t index = (byte)token;

    switch (index)
    {
        case 'A': {
            if ((byte)(token >> 8) != 'R')
                return 0xff;    //< There are only AR1, AR2, AR4, and AR8
            switch (index = (byte)(token >> 16)) {
                case '1': return CheckDelimiter (token >> 24) ? 0xff : AR1;
                case '2': return CheckDelimiter (token >> 24) ? 0xff : AR2;
                case '4': return CheckDelimiter (token >> 24) ? 0xff : AR4;
                case '8': return CheckDelimiter (token >> 24) ? 0xff : AR8;
                default: return 0xff;
            }
        }
        case 'B': switch (index = (byte) (token >> 8))
        {
            case 'K': switch (index = (byte) (token >> 16))
            {
                case '2': return CheckDelimiter (token >> 24) ? 0xff : BK2;
                case '4': return CheckDelimiter (token >> 24) ? 0xff : BK4;
                case '8': return CheckDelimiter (token >> 24) ? 0xff : BK8;
                default: return 0xff;
            }
            case 'O': return CheckLastLetter<'L'> (token >> 16) ? 0xff : BOL;
            default: return 0xff;
        }
        case 'D': return CheckLastLetters<'B', 'L'> (token >> 16) ? 0xff : DBL;
        case 'E': return CheckLastLetters<'S', 'C'> (token >> 16) ? 0xff : ESC;
        case 'F': return CheckLastLetters<'L', 'T'> (token >> 16) ? 0xff : FLT;
        case 'G': return 0xff;
        case 'H': return CheckLastLetters<'L', 'F'> (token >> 16) ? 0xff : HLF;
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M': return 0xff;
        case 'N': return CheckLastLetters<'I', 'L'> (token >> 16) ? 0xff : NIL;
        case 'O':
        case 'P':
        case 'Q':
        case 'R': return 0xff;
        case 'S': switch (index = (byte) (token >> 8))
        {
            case 'I': switch (index = (byte) (token >> 16))
            {
                case '1': return CheckDelimiter (token >> 24) ? 0xff : SI1;
                case '2': return CheckDelimiter (token >> 24) ? 0xff : SI2;
                case '4': return CheckDelimiter (token >> 24) ? 0xff : SI4;
                case '8': return CheckDelimiter (token >> 24) ? 0xff : SI8;
                default: return 0xff;
            }
            case 'O': switch (index = (byte) (token >> 16))
            {
                case 'H': return CheckDelimiter (token >> 24) ? 0xff : SOH;
                default: return 0xff;
            }
            case 'V': switch (index = (byte) (token >> 16))
            {
                case '4': return CheckDelimiter (token >> 24) ? 0xff : SV4;
                case '8': return CheckDelimiter (token >> 24) ? 0xff : SV8;
                default: return 0xff;
            }
            default: break;
        }
        case 'T': switch (index = (token >> 8))
        {
            case 'M': switch (index = (token >> 16))
            {
                case 'E': return CheckDelimiter (token >> 24) ? 0xff : TMS;
                case 'U': return CheckDelimiter (token >> 24) ? 0xff : TMU;
                default: return 0xff;
            }
            default: return 0xff;
        }
        case 'U': switch (index = (token >> 8))
        {
            case 'S': return CheckDelimiter (token >> 16) ? 0xff : US;
            case 'I': switch (index = (byte) (token >> 16))
            {
                case '1': return CheckDelimiter (token >> 24) ? 0xff : UI1;
                case '2': return CheckDelimiter (token >> 24) ? 0xff : UI2;
                case '4': return CheckDelimiter (token >> 24) ? 0xff : UI4;
                case '8': return CheckDelimiter (token >> 24) ? 0xff : UI8;
                default: return 0xff;
            }
            case 'V': switch (index = (byte) (token >> 16))
            {
                case '4': return CheckDelimiter (token >> 24) ? 0xff : UV4;
                case '8': return CheckDelimiter (token >> 24) ? 0xff : UV8;
                default: return 0xff;
            }
            default: return 0xff;
        }
    }
    return 0xff;
}

KABUKI byte MaskType (byte value) {
    return value & 0x1f;
}

KABUKI bool TypeHasLength (uint_t type) {
    if (type == STX)
        return true;
    if (type >= AR1 && type <= AR8)
        return true;
    if (type == ESC)
        return false;
    return false;
}

KABUKI bool TypeIsArray (uint_t type) {
    return (type >= AR1) && (type <= AR8);
}

KABUKI bool TypeIsBag (uint_t type) {
    return (type >= BK8) && (type <= BK2);
}

KABUKI bool TypeIsHierarchical (uint_t type) {
    return type > ESC;
}

}       //< namespace _
