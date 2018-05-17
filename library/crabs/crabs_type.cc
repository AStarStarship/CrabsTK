/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_types.cc
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

#include <stdafx.h>
#include "type.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {

uint_t TypeFixedSize (uint_t type) {
    static const int8_t kWidths[] = {
        0,  //< NIL: 0
        1,  //< SI1: 1 
        1,  //< UI1: 2 
        2,  //< SI2: 3 
        2,  //< UI2: 4 
        2,  //< HLF: 5 
        sizeof (int),  //< SVI: 6
        sizeof (uint), //< UVI: 7
        sizeof (int),  //< BOL: 8
        4,  //< SI4: 9
        4,  //< UI4: 10
        4,  //< FLT: 11
        4,  //< TMS: 12
        8,  //< TMU: 13
        8,  //< SI8: 14
        8,  //< UI8: 15
        4,  //< DBL: 16
        8,  //< SV8: 17
        8,  //< UV8: 18
        16, //< DEC: 19
    };
    if (type > OBJ)
        return -1;
    return kWidths[type];
}

const char** TypeStrings () {
    static const char* kNames[] = {
        "NIL",  //<  0
        "ADR",  //<  1
        "STR",  //<  2
        "TKN",  //<  3
        "BOL",  //<  4
        "SI1",  //<  5
        "UI1",  //<  6
        "SI2",  //<  7
        "UI2",  //<  8
        "HLF",  //<  9
        "SVI",  //< 10
        "UVI",  //< 11
        "SI4",  //< 12
        "UI4",  //< 13
        "FLT",  //< 14
        "TMS",  //< 15
        "SI8",  //< 16
        "UI8",  //< 17
        "DBL",  //< 18
        "TMU",  //< 19
        "SV8",  //< 20
        "UV8",  //< 21
        "DEC",  //< 22
        "OBJ",  //< 23
        "SIN",  //< 24
        "UIN",  //< 25
        "BSQ",  //< 26
        "ESC",  //< 27
        "LST",  //< 28
        "BOK",  //< 29
        "DIC",  //< 30
        "MAP",  //< 31
    };
    return kNames;
}

void* TypeAlign (uint8_t type, void* value) {
    assert (value);
    if (type == 0)
        return nullptr;
    uint_t size = TypeFixedSize (type);
    if (type <= UI1)
        return value;
    uint8_t* value_ptr = reinterpret_cast<uint8_t*> (value);
    #if WORD_SIZE == 2
    if (type <= HLF)
        return AlignUpPointer2 (value);
    #else
    if (type <= BOL)
        return AlignUpPointer2 (value);
    #endif
    if (type <= TMS)
        return AlignUpPointer4 (value);
    if (type <= DEC)
        return AlignUpPointer8 (value);

    switch (type >> 6) {
        case 0: return value;
        case 1: return AlignUpPointer2<> (value);
        case 2: return AlignUpPointer4<> (value);
        case 3: return AlignUpPointer8<> (value);
    }
}

void* TypeWrite (uint8_t type, char* begin, char* end, const char* source) {
    // Algoirhm:
    // 1.) Determine type
    // 2.) Align begin pointer to type width.
    // 3.) Check for enough room in begin-end socket.
    // 4.) Use MemoryCopy to copy the data into the given begin-end socket.

    assert (begin);
    assert (source);

    char * target_1;
    uint16_t* target_2;
    uint32_t* target_4;
    uint64_t* target_8;

    const char * source_1;
    const uint16_t* source_2;
    const uint32_t* source_4;
    const uint64_t* source_8;

    if (type <= UI1) {
        target_1 = reinterpret_cast<char*> (begin);
        *target_1++ = *reinterpret_cast<const char*> (source);
        return target_1;
    }
    if (type <= kTypeLast2Byte) {
        target_2 = reinterpret_cast<uint16_t*> (AlignUpPointer2<> (begin));
        *target_2++ = *reinterpret_cast<const uint16_t*> (source);
        return target_2;
    }
    if (type <= TMS) {
        target_4 = reinterpret_cast<uint32_t*> (AlignUpPointer4 (begin));
        *target_4++ = *reinterpret_cast<const uint32_t*> (source);
        return target_4;
    }
    if (type <= DEC) {
         target_8 = reinterpret_cast<uint64_t*> (AlignUpPointer8 (begin));
         source_8 = reinterpret_cast<const uint64_t*> (source);
        *target_8++ = *source_8++;
        if (type == DEC) {
            *target_8++ = *source_8;
            return target_8;
        }
        return target_8;
    }
    char array_type = type >> 6;
    switch (array_type) {
        case 0: {
            target_1 = reinterpret_cast<char*> (begin);
            source_1 = reinterpret_cast<const char*> (source);
            uint8_t size_1 = *source_1++;
            *target_1++ = size_1;
            MemoryCopy (target_1, end, source, size_1 - 1);
            break;
        }
        case 1: {
            target_2 = reinterpret_cast<uint16_t*> (begin);
            source_2 = reinterpret_cast<const uint16_t*> (source);
            uint16_t size_2 = *source_2++;
            *target_2++ = size_2;
            MemoryCopy (target_2, end, source, size_2 - sizeof (int16_t));
            break;
        }
        case 2: {
            target_4 = reinterpret_cast<uint32_t*> (begin);
            source_4 = reinterpret_cast<const uint32_t*> (source);
            uint32_t size_4 = *source_4++;
            *target_4++ = size_4;
            MemoryCopy (target_4, end, source, size_4 - sizeof (int32_t));
            break;
        }
        case 3: {
            target_8 = reinterpret_cast<uint64_t*> (begin);
            source_8 = reinterpret_cast<const uint64_t*> (source);
            uint64_t size_8 = *source_8++;
            *target_8++ = size_8;
            MemoryCopy (target_8, end, source, size_8 - sizeof (int64_t));
            break;
        }
    }
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
