/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_types.cc
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

#include <stdafx.h>
#include "types.h"
#include "text.h"

namespace _ {

uint_t SizeOf (uint_t type) {
    static const int8_t kWidths[] =
    {
        0,      //< NIL: 0 
        0,      //< ADR: 1 
        0,      //< STR: 2 
        0,      //< ST2: 3 
        0,      //< ST4: 4 
        1,      //< BOL: 6
        1,      //< SI1: 7 
        1,      //< UI1: 8 
        2,      //< SI2: 8 
        2,      //< UI2: 9
        2,      //< HLF: 10
        4,      //< SI4: 11
        4,      //< UI4: 12
        4,      //< FLT: 13
        4,      //< TMS: 14
        8,      //< TMU: 15
        8,      //< SI8: 16
        8,      //< UI8: 17
        8,      //< DBL: 18
        3,      //< SV2: 19
        3,      //< UV2: 20
        9,      //< SV4: 21
        5,      //< UV4: 22
        9,      //< SV8: 23
        9,      //< UV8: 24
        0,      //< BSC: 25
        0,      //< OBJ: 26
        0,      //< OBV: 27
        0,      //< LST: 28
        0,      //< BAG: 29
        0,      //< BOK: 30
        0,      //< MAP: 31
    };
    if (type >= 32)
        return 0;
    return kWidths[type];
}

const byte* TypeAlignments () {
    static const byte kWidths[] =
    {
        0,      //< NIL: 0 
        0,      //< ADR: 1 
        0,      //< STR: 2 
        0,      //< ST2: 3
        0,      //< ST4: 4 
        1,      //< BOL: 5
        1,      //< SI1: 6 
        1,      //< UI1: 7 
        2,      //< SI2: 8 
        2,      //< UI2: 9 
        2,      //< HLF: 10 
        4,      //< SI4: 11
        4,      //< UI4: 12
        4,      //< FLT: 13
        4,      //< TMS: 14
        8,      //< TMU: 15
        8,      //< SI8: 16
        8,      //< UI8: 17
        8,      //< DBL: 18
        2,      //< SV2: 19
        2,      //< UV2: 20
        4,      //< SV4: 21
        4,      //< UV4: 22
        8,      //< SV8: 23
        8,      //< UV8: 24
        0,      //< AR1: 25
        2,      //< AR2: 26
        4,      //< AR4: 27
        8,      //< AR8: 
        8,      //< ESC: 27
        8,      //< FS: 28
        8,      //< GS: 29
        8,      //< RS: 30
        8,      //< US:  31
    };
    return kWidths;
}

uintptr_t TypeAlign2 (byte* ptr) {
    // Mask off lower bit and add it to the ptr.
    uintptr_t value = reinterpret_cast<uintptr_t> (ptr);
    return value & 0x1;
}

uintptr_t TypeAlign4 (byte* ptr) {
    return MemoryAlignOffset<int> (ptr);
}

uintptr_t TypeAlign8 (byte* ptr) {
    return MemoryAlignOffset<long> (ptr);
}

uintptr_t TypeAlign (byte* ptr, byte type) {
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

uintptr_t TypeAlign (byte* ptr, uint_t type) {
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

bool TypeIsValid (uint_t type) {
    return type > 31 ? false : true;
}

const char** TypeTexts () {
    static const char* kNames[] = {
        "NIL",
        "ADR",
        "STR",
        "ST2",
        "ST4",
        "BOL",
        "SI1",
        "UI1",
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
        "BSC",
        "OBJ",
        "OBV",
        "LST",
        "BAG",
        "BOK",
        "MAP",
    };
    return kNames;
}

bool TypeIsValid (const char* type_name) {
    // @warning I'm not sure what I was thinking here.
    //if (type_name < TypeTexts ()[0] || type_name > TypeTexts ()[FS])
    //    return false;
    //return true;
    return false;
}

const char* TypeText (uint_t type) {
    if (type >= kInvalidType)
        return "Invalid";
    return TypeTexts ()[type];
}

template<char c>
bool CharCompare (uint16_t const token) {
    if (c != (char)token)
        return true;
    return (token >> 8) == 0;
}

template<char LetterTwo, char LetterThree>
bool CharCompare (char letter_two, char letter_three) {
    char check = letter_two;
    if (LetterTwo != check)
        return true;
    if (LetterThree != check)
        return true;
    return false;
}

byte ReadType (const char* string) {
    char d = *(string + 3);
    if (!isspace (d) && (d != 0))
        return 0xff;
    char a = *string,
         b = *(string + 1),
         c = *(string + 2);

    switch (a) {
        case 'A': return CharCompare<'D', 'R'> (b, c)?0xff:ADR;
        case 'B': switch (b) {
            case 'O': switch (c) {
                case 'K': return BOK;
                case 'L': return BOL;
                default: return 0xff;
            }
            case 'S': return (c != 'C')?0xff:BSC;
            default: return 0xff;
        }
        case 'C': return 0xff;
        case 'D': return CharCompare<'B', 'L'> (b, c)?0xff:DBL;
        case 'E': return 0xff;
        case 'F': return CharCompare<'L', 'T'> (b, c)?0xff:FLT;
        case 'G': return 0xff;
        case 'H': return CharCompare<'L', 'F'> (b, c)?0xff:HLF;
        case 'I':
        case 'J':
        case 'K': return 0xff;
        case 'L': return CharCompare<'S', 'T'> (b, c)?0xff:LST;
        case 'M': return CharCompare<'A', 'P'> (b, c)?0xff:MAP;
        case 'N': return CharCompare<'I', 'L'> (b, c)?0xff:NIL;
        case 'O': switch (b) {
            case 'B': switch (c) {
                case 'J': return OBJ;
                case 'V': return OBV;
                default: return 0xff;
            }
        }
        case 'Q':
        case 'R': return 0xff;
        case 'S': switch (b) {
            case 'I': switch (c) {
                case '1': return  SI1;
                case '2': return  SI2;
                case '4': return  SI4;
                case '8': return  SI8;
                default: return 0xff;
            }
            case 'T': switch (c) {
                case '2': return ST2;
                case '4': return ST4;
                case 'R': return STR;
                default: return 0xff;
            }
            case 'V': switch (c) {
                case '2': return SV2;
                case '4': return SV4;
                case '8': return SV8;
                default: return 0xff;
            }
            default: break;
        }
        case 'T':
        {
            if (b != 'M') return 0xff;
            switch (c) {
                case 'S': return TMS;
                case 'U': return TMU;
                default: return 0xff;
            }
        }
        case 'U': switch (b) {
            case 'I': switch (c) {
                case '1': return UI1;
                case '2': return UI2;
                case '4': return UI4;
                case '8': return UI8;
                default: return 0xff;
            }
            case 'V': switch (c) {
                case '4': return UV4;
                case '8': return UV8;
                default: return 0xff;
            }
            default:      return 0xff;
        }
    }
    return 0xff;
}


byte MaskType (byte value) {
    return value & 0x1f;
}

bool TypeHasLength (uint_t type) {
    switch (type) {
        case STR: return true;
        case ST2: return true;
        case ST4: return true;
        case OBJ: return true;
    }
    return false;
}

bool TypeIsArray (uint_t type) {
    return type >= kNumTypes;
}

bool TypeIsSet (uint_t type) {
    return type >= kNumTypes;
}

bool TypeIsHierarchical (uint_t type) {
    return type >= BSC;
}

}       //< namespace _
