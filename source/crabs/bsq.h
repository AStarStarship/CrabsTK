/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/bsq.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_CRABS_BSQ
#define HEADER_FOR_CRABS_BSQ

#include "type.h"

#if CRABS_SEAM >= 1
#include "slot.h"

namespace _ {

constexpr uint_t BsqSize (const uint_t* params) {
    if (!params) {
        return 0;
    }
    uint_t size  = sizeof (uint_t),
           count = *params++;
    if (count > kParamsMax) {
        return 0;
    }

    for (; count > 0; --count) {
        uint_t param = *params++;

        if (param == NIL) { // This is illegal.
            return 0;
        }

        if (param <= TKN) {
            size += sizeof (uint_t);
            ++params;
        }
        if (param == SIN) {
            size += sizeof (uint_t);
            ++params;
        }
        if (param == UIN) {
            size += sizeof (uint_t);
            ++params;
        }
        if (param >= LST && param <= MAP) { // This is illegal.
            return 0;
        }
        if (param > MAP) {
            if (param >> 8) {        // This is an error.
                return 0;
            }
            if (param & 0x20) {      // It's a multi-dimensional array so
                param = *params;     // add the number of dimensions + 1;
                params += param + 1; // for the dimension count.
            }
        }
        size += sizeof (uint_t);
    }
    return size;
}

/** Creates a immutable Script B-Sequence.
    C++11 variadic template to ensure only one copy in ROM 
    and to eliminate some redundant typing. */
template<const uint_t... N>
inline const uint_t* Bsq () {
    static const uint_t kSize = 0,//BsqSize ({ N... })
                        kList[sizeof... (N)] = { N... };
    return &kSize;
}

/*< Returns the requested parameter number. */
KABUKI uint_t BsqParamNumber (const uint_t* params, int param_number);

KABUKI Slot& BsqPrint (const uint_t* params, Slot& slot);

#if USING_CRABS_TEXT
/**  Prints out the parameters to the debug console. */

/* Reads a b-sequences from the given slot.
   @depreciated I don't think I need this code anymore because I use dictionaries to parse types.

template<typename T>
const char* BsqRead (const char* input, const char* input_end,
                     size_t buffer_count, uint_t* bsq = nullptr) {
    if (!input) {
        return 0;
    }
    if (input > input_end) {
        return 0;
    }
    if (!bsq) {
        bsq = new 
    }
    const char* cursor = input;
    cursor = SlotSkipSpaces (input, input_end);

    char d = *(input + 3);
    if ((input_end - input) < 5) {
        // We will always have at least 2 bytes in the buffer after any ESC.
        // <4
        return 0;
    }
    uint_t params_count;
    cursor = SlotRead (cursor, input_end, params_count);
    if (!cursor) {
        return nullptr;
    }
    for (uint_t index = 0; index < params_count; ++index) {
        char b = *(input + 1),
             c = *(input + 2),
             d = *(input + 3);

        switch (*input++) {
            case 'A': return TypeCharCompare<'D', 'R', ADR> (b, c, d);
            case 'B': switch (b) {
                case 'O': return TypeCharCompare<'L', BOL> (c, d);
                case 'S': return TypeCharCompare<'C', BSQ> (c, d);
                default: return 0;
            }
            case 'C': return 0;
            case 'D': switch (b) {
                case 'B': return TypeCharCompare<'L', DBL> (c, d);
                case 'I': switch (c) {
                    if (c != 'C') {
                        return 0;
                    }
                    if (!IsSpace (*(input + 4))) {
                        return 0;
                    }
                    if (c == '2') {
                        return DIC;
                    }
                    if (c == '4') {
                        return DIC;
                    }
                    if (c == '8') {
                        return DIC;
                    }
                }
            }
            case 'E': {
                if (b != 'S')
                    return 0;
                if (b != 'C')
                    return 0;
                return TypeCharCompareObject<'S', 'C', ESC> (b, c, d,
                                                                 *(input + 4));
            }
            case 'F': return TypeCharCompare<'L', 'T', FLT> (b, c, d);
            case 'G': return 0;
            case 'H': return TypeCharCompare<'L', 'F', HLF> (b, c, d);
            case 'I':
            case 'J':
            case 'K': return 0;
            case 'L': return TypeCharCompareObject<'S', 'T', LST> (b, c, d,
                                                                   *(input + 4));
            case 'M': return TypeCharCompareObject<'A', 'P', MAP> (b, c, d,
                                                                   *(input + 4));
            case 'N': return TypeCharCompare<'I', 'L', NIL> (b, c, d);
            case 'O': switch (b) {
                case 'B': return TypeCharCompare<'J', OBJ> (c, d);
            }
            case 'P':
            case 'Q':
            case 'R': return 0;
            case 'S': switch (b) {
                case 'I': {
                    if (!IsSpace (*(input + 4))) {
                        return 0;
                    }
                    switch (c) {
                        case '1': return SI1;
                        case '2': return SI2;
                        case '4': return SI4;
                        case '8': return SI8;
                        case 'N': return SIN;
                        default: return 0;
                    }
                }
                case 'T': return TypeCharCompare<'R', STR> (d, *(input + 4));
                case 'V': {
                    if (!IsSpace (*(input + 4))) {
                        return 0;
                    }
                    switch (c) {

                        case 'N': return SIN;
                        case '8': return SV8;
                        case 'I': return SVI;
                        default: return 0;
                    }
                }
                default: break;
            }
            case 'T': {
                if (b != 'M') return 0;
                switch (c) {
                    case 'S': return TMS;
                    case 'U': return TMU;
                    default: return 0;
                }
            }
            case 'U': switch (b) {
                case 'I': {
                    if (!IsSpace (d)) {
                        return 0;
                    }
                    switch (c) {
                        case '1': return UI1;
                        case '2': return UI2;
                        case '4': return UI4;
                        case '8': return UI8;
                        case 'N': return UIN;
                        default: return 0;
                    }
                }
                case 'V': {
                    if (!IsSpace (d)) {
                        return 0;
                    }
                    switch (c) {
                        case '8': return UV8;
                        case 'I': return UVI;
                        default: return 0;
                    }
                }
                default: return 0;
            }
        }
    }
    return cursor;
}*/

}       //< namespace _

/**  Prints out the parameters to the debug console. */
inline _::Slot& operator<< (_::Slot& slot, const uint_t* bsq) {
    return BsqPrint (bsq, slot);
}
#else
}       //< namespace _
#endif

#endif  //< CRABS_SEAM >= 1
#endif  //< HEADER_FOR_CRABS_BSQ
