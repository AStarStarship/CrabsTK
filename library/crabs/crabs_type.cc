/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_types.cc
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

#include <stdafx.h>
#include "type.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {

TypeValue::TypeValue (type_t type, const void* value) :
    type (type),
    value (value) {
    // Nothing to do here! ({:-)-+=<
}

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
        "SI1",  //<  1
        "UI1",  //<  2
        "SI2",  //<  3
        "UI2",  //<  4
        "HLF",  //<  5
        "BOL",  //<  6
        "SVI",  //<  7
        "UVI",  //<  8
        "SI4",  //<  9
        "UI4",  //< 10
        "FLT",  //< 11
        "TMS",  //< 12
        "TMU",  //< 13
        "SI8",  //< 14
        "UI8",  //< 15
        "DBL",  //< 16
        "SV8",  //< 17
        "UV8",  //< 18
        "DEC",  //< 19
        "OBJ",  //< 20
        "SIN",  //< 21
        "UIN",  //< 22
        "ADR",  //< 23
        "STR",  //< 24
        "TKN",  //< 25
        "BSQ",  //< 26
        "ESC",  //< 27
        "LST",  //< 28
        "BOK",  //< 29
        "DIC",  //< 30
        "MAP",  //< 31
    };
    return kNames;
}

void* TypeAlign (type_t type, void* value) {
    ASSERT (value);
    if (type == 0)
        return nullptr;
    if (!TypeIsValid (type))
        return nullptr;

    uint_t size = TypeFixedSize (type);
    if (type <= UI1)
        return value;
    type_t* value_ptr = reinterpret_cast<type_t*> (value);
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
    return 0;
}

template<typename T>
char* WriteType (char* begin, char* end, type_t type, const void* value) {
    return nullptr;
}

char* Write (char* begin, char* end, type_t type, const void* value) {
    // Algorithm:
    // 1.) Determine type.
    // 2.) Align begin pointer to type width.
    // 3.) Check for enough room in begin-end socket.
    // 4.) Use MemoryCopy to copy the data into the given begin-end socket.

    ASSERT (begin)
    ASSERT (value)
    if (!TypeIsValid (type))
        return nullptr;

    char    * target_1,
            * target_1_end;
    char16_t* target_2,
            * target_2_end;
    char32_t* target_4;
    uint64_t* target_8;

    const char    * source_1;
    const char16_t* source_2;
    const char32_t* source_4;
    const uint64_t* source_8;

    if (type <= UI1) {
        target_1    = reinterpret_cast<char*> (begin);
        *target_1++ = *reinterpret_cast<const char*> (value);
        return target_1;
    }
    if (type <= kTypeLast2Byte) {
        target_1 = AlignUpPointer2<char> (begin);
        target_2 = reinterpret_cast<char16_t*> (target_1);
        *target_2++ = *reinterpret_cast<const char16_t*> (value);
        return reinterpret_cast<char*> (target_2);
    }
    if (type <= TMS) {
        target_1 = AlignUpPointer4<char> (begin);
        target_4 = reinterpret_cast<char32_t*> (target_1);
        *target_4++ = *reinterpret_cast<const char32_t*> (value);
        return reinterpret_cast<char*> (target_4);
    }
    if (type <= DEC) {
        target_1 = AlignUpPointer8<char> (begin);
         target_8 = reinterpret_cast<uint64_t*> (target_1);
         source_8 = reinterpret_cast<const uint64_t*> (value);
        *target_8++ = *source_8++;
        if (type == DEC) {
            *target_8++ = *source_8;
            return reinterpret_cast<char*> (target_8);
        }
        return reinterpret_cast<char*> (target_8);
    }
    if (TypeIsString (type)) {
        switch (type >> 6) {
            case 0: {
                source_1 = reinterpret_cast<const char*> (value);
                uint8_t length = TextLength<uint8_t, char> (source_1);
                target_1 = AlignUpPointer2<char> (begin);
                target_1_end = reinterpret_cast<char*> (end);
                if (target_1_end - target_1 == 0)
                    return nullptr;
                *target_1++ = (char)length;
                target_1 = Print (target_1, target_1_end,
                                  reinterpret_cast<const char*> (value));
                return reinterpret_cast<char*> (target_1);
            }
            case 1: return nullptr;
            case 2: {
                source_1 = reinterpret_cast<const char*> (value);
                uint16_t length = TextLength<uint16_t, char> (source_1);
                target_2 = AlignUpPointer2<char16_t> (begin);
                target_2_end = reinterpret_cast<char16_t*> (end);
                if (target_2_end - target_2 == 0)
                    return nullptr;
                *target_2++ = (char16_t)length;
                target_2 = Print (target_2, target_2_end,
                                  reinterpret_cast<const char16_t*> (value));
                return reinterpret_cast<char*> (target_2);
            }
            case 3: {
                source_2 = reinterpret_cast<const char16_t*> (value);
                uint16_t length = TextLength<uint16_t, char16_t> (source_2);
                target_4 = AlignUpPointer4<char32_t> (begin);
                *target_4++ = (char32_t)length;
                target_2 = Print (reinterpret_cast<char16_t*> (target_4),
                                  reinterpret_cast<char16_t*> (end),
                                  reinterpret_cast<const char16_t*> (value));
                return reinterpret_cast<char*> (target_2);
            }
            case 4: {
                source_1 = reinterpret_cast<const char*> (value);
                uint32_t length = TextLength<uint16_t, char> (source_1);
                target_4 = AlignUpPointer4<char32_t> (begin);
                *target_4++ = (char32_t)length;
                target_1 = Print (reinterpret_cast<char*> (target_4),
                                  reinterpret_cast<char*> (end),
                                  reinterpret_cast<const char*> (value));
                return reinterpret_cast<char*> (target_1);
            }
            case 5: {
                source_2 = reinterpret_cast<const char16_t*> (value);
                uint32_t length = TextLength<uint32_t, char16_t> (source_2);
                target_4 = AlignUpPointer4<char32_t> (begin);
                *target_4++ = (char32_t)length;
                target_2 = Print (reinterpret_cast<char16_t*> (target_4),
                                  reinterpret_cast<char16_t*> (end),
                                  reinterpret_cast<const char16_t*> (value));
                return reinterpret_cast<char*> (target_2);
            }
            case 6: {
                source_1 = reinterpret_cast<const char*> (value);
                uint64_t length = TextLength<uint64_t, char> (source_1);
                target_8 = AlignUpPointer8<uint64_t> (begin);
                *target_8++ = (uint64_t)length;
                target_1 = Print (reinterpret_cast<char*> (target_8),
                                  reinterpret_cast<char*> (end),
                                  reinterpret_cast<const char*> (value));
                return reinterpret_cast<char*> (target_1);
            }
            case 7: {
                source_2 = reinterpret_cast<const char16_t*> (value);
                uint64_t length = TextLength<uint64_t, char16_t> (source_2);
                target_8 = AlignUpPointer8<uint64_t> (begin);
                *target_8++ = (uint64_t)length;
                target_2 = Print (reinterpret_cast<char16_t*> (target_8),
                                  reinterpret_cast<char16_t*> (end),
                                  reinterpret_cast<const char16_t*> (value));
                return reinterpret_cast<char*> (target_2);
            }
        }
    }
    char array_type = type >> 6;
    switch (array_type) {
        case 0: {
            target_1 = reinterpret_cast<char*> (begin);
            source_1 = reinterpret_cast<const char*> (value);
            type_t size_1 = *source_1++;
            *target_1++ = size_1;
            return MemoryCopy (target_1, end, value, size_1 - 1);
        }
        case 1: {
            target_2 = reinterpret_cast<char16_t*> (begin);
            source_2 = reinterpret_cast<const char16_t*> (value);
            char16_t size_2 = *source_2++;
            *target_2++ = size_2;
            return MemoryCopy (target_2, end, value, 
                               size_2 - sizeof (int16_t));
        }
        case 2: {
            target_4 = reinterpret_cast<char32_t*> (begin);
            source_4 = reinterpret_cast<const char32_t*> (value);
            char32_t size_4 = *source_4++;
            *target_4++ = size_4;
            return MemoryCopy (target_4, end, value, 
                               size_4 - sizeof (int32_t));
        }
        case 3: {
            target_8 = reinterpret_cast<uint64_t*> (begin);
            source_8 = reinterpret_cast<const uint64_t*> (value);
            uint64_t size_8 = *source_8++;
            *target_8++ = size_8;
            return MemoryCopy (target_8, end, value, 
                               size_8 - sizeof (int64_t));
        }
    }
    return nullptr;
}

Printer& PrintTypePod (Printer& printer, type_t type, const void* value) {
    ASSERT (value)
    
    switch (type & 0xf) {
        case NIL: return printer << "Error";
        case SI1: return printer << *reinterpret_cast<const int8_t*> (value);
        case UI1: return printer << *reinterpret_cast<const uint8_t*> (value);
        case SI2: return printer << *reinterpret_cast<const int16_t*> (value);
        case UI2: return printer << *reinterpret_cast<const uint16_t*> (value);
        case HLF: return printer << "not_implemented_yet";
        case SVI: return printer << *reinterpret_cast<const int*> (value);
        case UVI: return printer << *reinterpret_cast<const uint*> (value);
        case BOL: return printer << *reinterpret_cast<const bool*> (value);
        case SI4: return printer << *reinterpret_cast<const int32_t*> (value);
        case UI4: return printer << *reinterpret_cast<const uint32_t*> (value);
        case FLT: return printer << *reinterpret_cast<const float*> (value);
        case TMS: return printer << *reinterpret_cast<const int*> (value);
        case TMU: return printer << *reinterpret_cast<const int*> (value);
        case SI8: return printer << *reinterpret_cast<const int64_t*> (value);
        case UI8: return printer << *reinterpret_cast<const uint64_t*> (value);
        case DBL: return printer << *reinterpret_cast<const double*> (value);
        case SV8: return printer << *reinterpret_cast<const int64_t*> (value);
        case UV8: return printer << *reinterpret_cast<const uint64_t*> (value);
        case DEC: return printer << "not_implemented_yet";
    }
    return printer;
}

Printer& PrintType (Printer& printer, type_t type, const void* value) {
    ASSERT (value)

    if (type <= DEC)
        return PrintTypePod (printer, type, value) << ':' << TypeString (type);

    if (!TypeIsValid (type))
        return printer << "illegal_type";

    if (TypeIsString (type))
        return printer << '\"' << reinterpret_cast<const char*> (value) << "\":" 
                       << TypeString (type);

    return PrintTypePod (printer, type & 0x1f, value) << "b:" << 
           TypeString (type);
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
