/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_print_ui4.cc
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1

#include "print_int.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 1

#define DEBUG 1

#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_PRINTED\
    sprintf_s (buffer, 24, "%u", value); *text_end = 0;\
    printf ("\n    Printed \"%s\" leaving value:\"%s\":%u",\
            begin, buffer, (uint)strlen (buffer));
#define PRINT_BINARY PrintBinary (value);
#define PRINT_BINARY_TABLE PrintBinaryTable (value);
#define PRINT_HEADER\
    for (int i = 0; i < 10; ++i) {\
        *(text + i) = 'x';\
    }\
    *(text + 21) = 0;\
    char* begin = text;\
    char buffer[256];\
    sprintf_s (buffer, 256, "%u", value);\
    printf ("Expecting %s:%u", buffer, (uint)strlen (buffer));
#define PRINT_HEADING\ 
    std::cout << '\n';\
    for (int i = 80; i > 0; --i)\
        std::cout << '-';
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_PRINTED
#define PRINT_HEADER
#define PRINT_HEADING
#endif
 
namespace _ {

char* Print (char* cursor, char* end, uint32_t value) {

    PRINT_HEADING

    // Lookup table for powers of 10.
    static const uint32_t k10ToThe[]{
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
        1000000000, ~(uint32_t)0 };

    /** Lookup table of ASCII char pairs for 00, 01, ..., 99.
        To convert this algorithm to big-endian, flip the digit pair bytes. */
    static const uint16_t kDigits00To99[100] = {
        0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830,
        0x3930, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
        0x3831, 0x3931, 0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632,
        0x3732, 0x3832, 0x3932, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533,
        0x3633, 0x3733, 0x3833, 0x3933, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434,
        0x3534, 0x3634, 0x3734, 0x3834, 0x3934, 0x3035, 0x3135, 0x3235, 0x3335,
        0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x3036, 0x3136, 0x3236,
        0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x3037, 0x3137,
        0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x3038,
        0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
        0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839,
        0x3939, };

    static const char kMsbShift[] = { 4, 7, 11, 14, 17, 21, 24, 27, 30, };

    if (!cursor) {
        return nullptr;
    }
    if (cursor >= end) {
        return nullptr;
    }

    uint16_t* text16;
    char      digit;
    uint32_t  scalar;
    uint16_t  digits1and2,
              digits3and4,
              digits5and6,
              digits7and8;
    uint32_t  comparator;

    PRINT_HEADER

    if (value < 10) {
        PRINTF ("\n    Range:[0, 9] length:1 ")
        if (cursor + 1 >= end) {
            return nullptr;
        }
        *cursor++ = '0' + (char)value;
        PRINT_PRINTED
        *cursor = 0;
        return cursor;
    }
    if (value < 100) {
        PRINTF ("\n    Range:[10, 99] length:2 ")
        if (cursor + 2 >= end) {
            return nullptr;
        }
        *reinterpret_cast<uint16_t*> (cursor) = kDigits00To99[value];
        PRINT_PRINTED
        *(cursor + 2) = 0;
        return cursor + 2;
    }
    if (value >> 14) {
        if (value >> 27) {
            if (value >> 30) {
                PRINTF ("\n    Range:[1073741824, 4294967295] length:10")
                Print10:
                if (cursor + 10 >= end) {
                    return nullptr;
                }
                comparator = 100000000;
                digits1and2 = (uint16_t)(value / comparator);
                PRINTF ("\n    digits1and2:%u", digits1and2)
                value -= digits1and2 * comparator;
                *reinterpret_cast<uint16_t*> (cursor) = kDigits00To99[digits1and2];
                PRINT_PRINTED
                cursor += 2;
                goto Print8;
            }
            else {
                comparator = 1000000000;
                if (value >= comparator) {
                    PRINTF ("\n    Range:[100000000, 1073741823] length:10")
                    goto Print10;
                }
                PRINTF ("\n    Range:[134217727, 999999999] length:9")
                if (cursor + 9 >= end) {
                    return nullptr;
                }
                comparator = 100000000;
                digit = (char)(value / comparator);
                *cursor++ = digit + '0';
                PRINT_PRINTED
                value -= comparator * digit;
                goto Print8;
            }
        }
        else if (value >> 24) {
            comparator = k10ToThe[8];
            if (value >= comparator) {
                PRINTF ("\n    Range:[100000000, 134217728] length:9")
                if (cursor + 9 >= end) {
                    return nullptr;
                }
                *cursor++ = '1';
                PRINT_PRINTED
                value -= comparator;
            }
            PRINTF ("\n    Range:[16777216, 9999999] length:8")
            if (cursor + 8 >= end) {
                return nullptr;
            }
            Print8:
            PRINTF ("\n    Print8:")
            scalar = 10000;
            digits5and6 = (uint16_t)(value / scalar);
            digits1and2 = value - scalar * digits5and6;
            digits7and8 = digits5and6 / 100;
            digits3and4 = digits1and2 / 100;
            digits5and6 -= 100 * digits7and8;
            digits1and2 -= 100 * digits3and4;
            *reinterpret_cast<uint16_t*> (cursor + 6) = 
                kDigits00To99[digits1and2];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor + 4) = 
                kDigits00To99[digits3and4];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor + 2) = 
                kDigits00To99[digits5and6];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor) = 
                kDigits00To99[digits7and8];
            PRINT_PRINTED
            *(cursor + 8) = 0;
            return cursor + 8;
        }
        else if (value >> 20) {
            comparator = 10000000;
            if (value >= comparator) {
                PRINTF ("\n    Range:[10000000, 16777215] length:8")
                if (cursor + 8 >= end) {
                    return nullptr;
                }
                *cursor++ = '1';
                PRINT_PRINTED
                value -= comparator;
            }
            else {
                PRINTF ("\n    Range:[1048576, 9999999] length:7")
                if (cursor + 7 >= end) {
                    return nullptr;
                }
            }
            scalar = 10000;
            digits5and6 = (uint16_t)(value / scalar);
            digits1and2 = value - scalar * digits5and6;
            digits7and8 = digits5and6 / 100;
            digits3and4 = digits1and2 / 100;
            digits5and6 -= 100 * digits7and8;
            digits1and2 -= 100 * digits3and4;;
            *reinterpret_cast<uint16_t*> (cursor + 5) = 
                kDigits00To99[digits1and2];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor + 3) = 
                kDigits00To99[digits3and4];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor + 1) = 
                kDigits00To99[digits5and6];
            PRINT_PRINTED
            *cursor = (char)digits7and8 + '0';
            *(cursor + 7) = 0;
            return cursor + 7;
        }
        else if (value >> 17) {
            comparator = 1000000;
            if (value >= comparator) {
                PRINTF ("\n    Range:[100000, 1048575] length:7")
                if (cursor + 7 >= end) {
                    return nullptr;
                }
                *cursor++ = '1';
                PRINT_PRINTED
                value -= comparator;
            }
            else {
                PRINTF ("\n    Range:[131072, 999999] length:6")
                if (cursor + 6 >= end) {
                    return nullptr;
                }
            }
            Print6:
            scalar = 10000;
            digits5and6 = (uint16_t)(value / scalar);
            digits1and2 = value - scalar * digits5and6;
            digits7and8 = digits5and6 / 100;
            digits3and4 = digits1and2 / 100;
            digits5and6 -= 100 * digits7and8;
            digits1and2 -= 100 * digits3and4;
            text16 = reinterpret_cast<uint16_t*> (cursor + 6);
            *reinterpret_cast<uint16_t*> (cursor + 4) = kDigits00To99[digits1and2];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor + 2) = kDigits00To99[digits3and4];
            PRINT_PRINTED
            *reinterpret_cast<uint16_t*> (cursor    ) = kDigits00To99[digits5and6];
            PRINT_PRINTED
            *(cursor + 6) = 0;
            return cursor + 6;
        }
        else { // (value >> 14)
            if (value >= 100000) {
                PRINTF ("\n    Range:[65536, 131071] length:6")
                goto Print6;
            }
            PRINTF ("\n    Range:[10000, 65535] length:5")
            if (cursor + 5 >= end) {
                return nullptr;
            }
            digits5and6 = 10000;
            digit = (uint8_t)(value / digits5and6);
            value -= digits5and6 * digit;
            *cursor = digit + '0';
            PRINT_PRINTED
            digits1and2 = (uint16_t)value;
            digits5and6 = 100;
            digits3and4 = digits1and2 / digits5and6;
            digits1and2 -= digits3and4 * digits5and6;
            *reinterpret_cast<uint16_t*> (cursor + 1) = 
                kDigits00To99[digits3and4];
            PRINT_PRINTED
                PRINTF ("\n    digits1and2:%u", digits1and2)
            *reinterpret_cast<uint16_t*> (cursor + 3) = 
                kDigits00To99[digits1and2];
            PRINT_PRINTED
            *(cursor + 5) = 0;
            return cursor + 5;
        }
    }
    digits1and2 = (uint16_t)value;
    if (value >> 10) {
        digits5and6 = 10000;
        if (digits1and2 >= digits5and6) {
            if (cursor + 5 >= end) {
                return nullptr;
            }
            PRINTF ("\n    Range:[10000, 16383] length:5")
            *cursor++ = '1';
            PRINT_PRINTED
            digits1and2 -= digits5and6;

        }
        else {
            PRINTF ("\n    Range:[1024, 9999] length:4")
            if (cursor + 4 >= end) {
                return nullptr;
            }
        }
        digits5and6 = 100;
        digits3and4 = digits1and2 / digits5and6;
        digits1and2 -= digits3and4 * digits5and6;
        *reinterpret_cast<uint16_t*> (cursor    ) = kDigits00To99[digits3and4];
        PRINT_PRINTED
        *reinterpret_cast<uint16_t*> (cursor + 2) = kDigits00To99[digits1and2];
        PRINT_PRINTED
        *(cursor + 4) = 0;
        return cursor + 4;
    }
    else {
        if (cursor + 4 >= end) {
            return nullptr;
        }
        digits3and4 = 1000;
        if (digits1and2 >= digits3and4) {
            PRINTF ("\n    Range:[1000, 1023] length:4")
            digits1and2 -= digits3and4;
            text16 = reinterpret_cast<uint16_t*> (cursor + 2);
            *text16-- = kDigits00To99[digits1and2];
            PRINT_PRINTED
            *text16 = (((uint16_t)'1') | (((uint16_t)'0') << 8));
            PRINT_PRINTED
            *(cursor + 4) = 0;
            return cursor + 4;
        }
        PRINTF ("\n    Range:[100, 999] length:3")
        digits1and2 = (uint16_t)value;
        digits3and4 = 100;
        digit = (char)(digits1and2 / digits3and4);
        digits1and2 -= digit * digits3and4;
        *cursor = digit + '0';
        PRINT_PRINTED
        *reinterpret_cast<uint16_t*> (cursor + 1) = kDigits00To99[digits1and2];
        PRINT_PRINTED
        *(cursor + 3) = 0;
        return cursor + 3;
    }
}

char* Print (char* cursor, char* end, int32_t value)  {
    if (value >= 0)
        return Print (cursor, end, (uint32_t)value);
    *cursor++ = '-';
    return Print (cursor, end, (uint32_t)(value * -1));
}

}       //< namespace _
#undef  PRINTF
#undef  PRINT_PRINTED
#undef  PRINT_HEADER
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
