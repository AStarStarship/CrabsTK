/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_itos.h
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

#include <stdafx.h>
#include "print.h"

#if CRABS_SEAM >= 1 || CRABS_SEAM == 0

#if CRABS_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define DEBUG_PRINTED(value, begin, end)\
    sprintf_s (buffer, 24, "%I64u", value); *(end + 1) = 0;\
    printf ("\n    Printed \"%s\", value:\"%s\"\" with expected length of ",\
            begin, buffer, strlen (buffer));
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define DEBUG_PRINTED(value, begin, end)
#endif
namespace _ {

char* Print (uint64_t value, char* text, char* text_end) {
    // Lookup table for powers of 10.
    static const uint64_t k10ToThe[20]{
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
        1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000,
        100000000000000, 1000000000000000, 10000000000000000,
        100000000000000000, 1000000000000000000, 10000000000000000000, };

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

    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }

    uint16_t  * text16;
    char        index;
    uint16_t    digits;
    uint64_t    comparator,
                offset;
    #if CRABS_SEAM == 1
    char*       begin = text;
    char        buffer[24];
    #endif
    
    /** 0000000000000000000000000000000000000000000000100100100111110000
        bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
        6666555555555544444444443333333333222222222211111111110000000000
        3210987654321098765432109876543210987654321098765432109876543210
        |  |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |
        2  1  1  1   1  1  1   1  1  1   1  0  0   0  0  0   0  0  0   0
        0  9  8  7   6  5  4   3  2  1   0  9  8   7  6  5   4  3  2   1
        |----------------------|-------------------|-------------------|
        |     Upper Chunk      |   Middle Chunk    |    Lower Chunk    |
    */
    if (value >> 34) {
        if (value >> 60) {
            PRINTF ("\n    Path 19 or 20: Range [9.22E+18, 18.4E+18] and ")
            comparator = k10ToThe[19];
            if (value < comparator) {
                if (text + 19 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:19 |")
                text_end = text + 18;
                goto PrintOddDown;
            }
            if (text + 20 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:20 |")
            *text++ = '1';
            value -= comparator;
            // Length:19
            text_end = text + 18;
            goto PrintOddUp;
        }
        else if (value >> 57) {
            PRINTF ("\n    Path 18 or 19   [5.76E+17, 11.5E+17] and ")
            comparator = k10ToThe[18];
            if (value >= comparator) { // Length:19
                PRINTF ("Length:19")
                if (text + 20 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // Length:18
                text_end = text + 17;
                goto PrintEvenUp;
            }
            if (text + 18 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:18")
            text_end = text + 17;
            goto PrintEvenDown;
        }
        else if (value >> 54) {
            PRINTF ("\n    Path 17 or 18 : Range [7.21E+16, 14.4E+16] and ")
            comparator = k10ToThe[17];
            if (value >= comparator) { // Length:18
                PRINTF ("Length:18")
                if (text + 19 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // Length:17
                text_end = text + 16;
                goto PrintOddUp;
            }
            PRINTF ("Length:17")
            text_end = text + 16;
            goto PrintOddDown;
        }
        else if (value >> 50) {
            PRINTF ("\n    Path 16 or 17   [9.01E+15, 18.0E+15] and ")
            comparator = k10ToThe[16];
            if (value < comparator) {
                PRINTF ("Length:16")
                text_end = text + 15;
                goto PrintEvenDown;
            }
            // @todo Optimize this path.
            PRINTF ("Length:17")
            if (text + 17 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // Length: 16
            text_end = text + 15;
            goto PrintEvenUp;
        }
        else if (value >> 47) {
            PRINTF ("\n    Path 15 or 16    [5.63E+14, 11.3E+14] and ")
            comparator = k10ToThe[15];
            if (value >= comparator) {
                if (text + 17 > text_end) {
                    return nullptr;
                }
                PRINTF (" Length:16")
                *text++ = '1';
                value -= comparator;
                // Length:15
                text_end = text + 14;
                goto PrintOddUp;
            }
            if (text + 16 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:15")
            text_end = text + 14;
            goto PrintOddDown;
        }
        else if (value >> 44) {
            PRINTF ("\n    Path 14 or 15   [7.04E+13, 14.1E+13] and ")
            comparator = k10ToThe[14];
            if (value < comparator) {
                if (text + 14 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:14")
                text_end = text + 13;
                goto PrintEvenDown;
            }
            if (text + 15 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:15")
            *text++ = '1';
            value -= comparator;
            // Length:14
            text_end = text + 13;
            goto PrintEvenUp;
        }
        else if (value >> 40) {
            PRINTF ("\n    Path 13 or 14 : Range 8.80E+12 | 17.6E+12] and ")
            comparator = k10ToThe[13];
            if (value < comparator) {
                if (text + 13 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:13")
                text_end = text + 12;
                goto PrintOddDown;
            }
            PRINTF ("Length:14")
            if (text + 14 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // Length: 13
            text_end = text + 12;
            goto PrintOddUp;
        }
        else if (value >> 37) {
            PRINTF ("\n    Path 12 or 13 : Range [5.50E+11, 11.0E+11] and ")
            comparator = k10ToThe[12];
            if (value >= comparator) {
                PRINTF ("Length:13")
                if (text + 14 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // Length:12
                text_end = text + 11;
                goto PrintEvenUp;
            }
            if (text + 13 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:12")
            text_end = text + 11;
            goto PrintEvenDown;
        }
        else { // if (value >> 36) {
            PRINTF ("\n    Path 11 or 12: Range [6.87E+10, 13.7E+10]")
            comparator = k10ToThe[11];
            if (value < comparator) {
                PRINTF ("Length:11")
                text_end = text + 10;
                goto PrintOddDown;
            }
            if (text + 13 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:12")
            *text++ = '1';
            value -= comparator;
            // Length:11
            text_end = text + 10;
            goto PrintOddUp;
        }
    }
    if (value >> 17) {
        if (value >> 30) {
            PRINTF ("\n    Path 10 or 11: Range [8.59E+09, 17.2E+09] and ")
            comparator = k10ToThe[10];
            if (value < comparator) {
                PRINTF ("Length:10 |")
                if (text + 10 > text_end) {
                    return nullptr;
                }
                text_end = text + 9;
                goto PrintEvenDown;
            }
            if (text + 11 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:11 |")
            *text++ = '1';
            value -= comparator;
            // Length:10
            text_end = text + 9;
            goto PrintEvenUp;
        }
        else if (value >> 27) {
            PRINTF ("\n    Path 09 or 10: Range [5.37E+08, 10.7E+08] and ")
            comparator = k10ToThe[9];
            if (value < comparator) {
                PRINTF ("Length:9")
                if (text + 8 > text_end) {
                    return nullptr;
                }
                text_end = text + 8;
                goto PrintOddDown;
            }
            PRINTF ("Length:10 |")
            if (text + 11 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // Length:9
            text_end = text + 8;
            goto PrintOddUp;
        } 
        else if (value >> 24) {
            PRINTF ("\n    Path 08 or 09: Range [6.71E+07, 13.4E+07 and ")
            comparator = k10ToThe[8];
            if (value < comparator) {
                if (text + 8 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:8 | ")
                text_end = text + 7;
                goto PrintEvenDown;
            }
            PRINTF ("Length:9 | ")
            if (text + 9 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // Length:8
            text_end = text + 7;
            goto PrintEvenUp;
        }
        else if (value >> 20) {
            PRINTF ("\n    Path 07 or 08: Range [8.39E+06 | 16.8E+06]   and ")
            comparator = k10ToThe[7];
            if (value < comparator) {
                if (text + 8 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:7 | ")
                text_end = text + 6;
                goto PrintOddDown;
            }
            PRINTF ("Length:8 | ")
            if (text + 8 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // Length:7
            text_end = text + 6;
            goto PrintOddUp;
        }
        else { //if (value >> 14) {
            PRINTF ("\n    Path 06 or 07: Range [5.24E+05, 10.5E+05] and ")
            comparator = k10ToThe[6];
            if (value >= comparator) {
                PRINTF ("Length:7 |")
                if (text + 9 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                // The next digit is '0' but it messes up the algorithm and this 
                // branch is rarely executed so that optimization is thrown 
                // out.
                value -= comparator;
                // Length:6
                text_end = text + 5;
                PrintEvenUp:
                PRINTF ("\n    PrintEvenUp ")
                if (text && 0x1) {
                    PRINTF ("unaligned value:%I64u", value)
                    offset = text_end - text;
                    comparator = k10ToThe[offset];
                    if (value < comparator) {
                        *text = '0';
                    }
                    else {
                        index = '1';
                        offset = comparator;
                        comparator = comparator << 1;
                        PRINTF ("\n    index:%c comparator:%I64u"
                                " offset:%I64u", index, comparator, offset)

                        while (comparator < value) {
                            comparator += offset;
                            ++index;
                            PRINTF ("\n    index:%c comparator = ", index,
                                    comparator)
                        }
                        if (value == comparator) {
                            *text = index + 1;
                        }
                        else {
                            *text = index;
                            comparator -= offset;
                        }
                        PRINTF ("\n    index:%c.) value - comparator = "
                                "%I64u - %I64u", index, value, comparator)
                        value -= comparator;
                        PRINTF (" = %I64u", value)
                    }

                    index = value % 10;
                    *text_end = '0' + index;
                    value /= 10;
                    text16 = reinterpret_cast<uint16_t*> (text_end - 2);
                }
                else {
                    PRINTF ("aligned value:%I64u", value)
                    text16 = reinterpret_cast<uint16_t*> (text_end - 1);
                }
                offset = (text_end - text) >> 1;
                while (--offset > 0) {
                    index = value % 100;
                    *text16-- = kDigits00To99[index];
                    value /= 100;
                    DEBUG_PRINTED (value, begin, text_end);
                }
                *text16 = kDigits00To99[value];
                DEBUG_PRINTED (value, begin, text_end);
                return text_end + 1;
            }
            if (text + 6 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:6 |")
            text_end = text + 5;
            PrintEvenDown:
            PRINTF ("\n    PrintEvenDown ")
            if (text && 0x1) {
                PRINTF ("unaligned value:", value)
                // Convert Most Significant Decimal (MSD).
                index = '9';
                offset = text_end - text;
                comparator = k10ToThe[offset + 1];
                offset = k10ToThe[offset];
                comparator -= offset;

                while (value < comparator) {
                    comparator -= offset;
                    --index;
                    PRINTF ("\n    index:%c.) value - comparator = "
                            "%I64u - %I64u", value, comparator)
                }
                value  -= comparator;
                *text++ = index;
                PRINTF (" = ", value)

                // Convert Least Significant Decimal (LSD).
                index = value % 10;
                *text_end = '0' + index;
                DEBUG_PRINTED (value, begin, text_end);
                value /= 10;

                text16 = reinterpret_cast<uint16_t*> (text_end - 2);
            }
            else {
                PRINTF ("aligned value:", value)
                text16 = reinterpret_cast<uint16_t*> (text_end - 1);
            }
            offset = (text_end - text) >> 1;
            PRINTF ("\n    number of times to loop:", offset)
            while (--offset > 0) {
                index = value % 100;
                value /= 100;
                *text16-- = kDigits00To99[index];
                DEBUG_PRINTED (value, begin, text_end);
            }
            DEBUG_PRINTED (value, begin, text_end);
            PRINTF ("\n    value:", value)
            *text16 = kDigits00To99[value];
            return text_end + 1;
        }
    }
    else {
        if (value >> 14) {
            PRINTF ("\n    Path 05 or 06: Range [6.55E+04, 13.1E+04] | ")
            comparator = k10ToThe[5];
            if (value >= comparator) {
                if (text + 6 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:6 ")
                value -= comparator;
                *text++ = '1';
                // Length:5
                text_end = text + 4;

                PrintOddUp:
                PRINTF ("\n    PrintOddUp ")
                if (text && 0x1) {
                    PRINTF ("unaligned value:%I64u", value)
                    DEBUG_PRINTED (value, begin, text_end);
                    offset = text_end - text;
                    comparator = k10ToThe[offset];
                    if (value < comparator) {
                        *text = '0';
                    } else {
                        offset = comparator;
                        comparator += offset;
                        index = '1';
                        PRINTF ("\n    index:\'%I64u\' comparator = ", index,
                                comparator)
                        while (comparator < value) {
                            comparator += offset;
                            ++index;
                            PRINTF ("\n    index:\'%c\' comparator = %I64u", index,
                                    comparator)
                        }
                        if (value == comparator) {
                            *text = index + 1;
                        }
                        else {
                            comparator -= offset;
                            *text = index;
                        }
                        PRINTF ("\n    index:\'%c\' value - comparator = %I64u "
                                "- %I64u = %I64u", index, comparator, value)
                        value -= comparator;
                        PRINTF (" = ", value)
                        DEBUG_PRINTED (value, begin, text_end);
                    }
                    text16 = reinterpret_cast<uint16_t*> (text_end - 1);
                }
                else {
                    PRINTF ("aligned value:", value)
                    index = value % 10;
                    *text_end = '0' + index;
                    value /= 10;
                    DEBUG_PRINTED (value, begin, text_end);
                    text16 = reinterpret_cast<uint16_t*> (text_end - 2);
                }
                offset = ((text_end - text) >> 1);
                PRINTF ("\n    number of times to loop:", offset)
                while (--offset > 0) {
                    index = value % 100;
                    *text16-- = kDigits00To99[index];
                    value /= 100;
                    DEBUG_PRINTED (value, begin, text_end);
                }
                *text16 = kDigits00To99[value];
                DEBUG_PRINTED (value, begin, text_end);
                return text_end + 1;
            }
            if (text + 5 > text_end) {
                return nullptr;
            }
            PRINTF ("Length:5 ")
            text_end = text + 4;

            PrintOddDown:
            PRINTF ("\n    PrintOddDown ")
            if (text && 0x1) {
                PRINTF ("unaligned value:", value)
                offset = text_end - text;
                comparator = k10ToThe[offset + 1];
                offset = k10ToThe[offset];
                comparator -= offset;
                PRINTF ("\n    comparator:", comparator)
                index = '9';
                while (value < comparator) {
                    PRINTF ("\n    index:", index, " value:", value,
                            " comparator:", comparator)
                    comparator -= offset;
                    --index;
                }
                *text++ = index;
                value -= comparator;
                PRINTF ("\n    index:\'%c\' value - comparator = %I64u "
                        "- %I64u = %I64u", index, comparator, value)
                // Length:6
                text16 = reinterpret_cast<uint16_t*> (text_end - 1);
            }
            else {
                PRINTF ("aligned value:", value)
                index = value % 10;
                value /= 10;
                *text_end = '0' + index;
                // Length:6
                text16 = reinterpret_cast<uint16_t*> (text_end - 2);
            }
            offset = (text_end - text) >> 1;
            PRINTF ("\n    number of times to loop:", offset)
            while (offset-- > 0) {
                PRINTF ("\n    value:", value)
                index = value % 100;
                *text16-- = kDigits00To99[index];
                value /= 100;
                DEBUG_PRINTED (value, begin, text_end);
            }
            *text16 = kDigits00To99[value];
            PRINTF ("\n    value:", value)
            DEBUG_PRINTED (value, begin, text_end);
            return text_end + 1;
        }
        else if (value >> 10) {
            PRINTF ("\n    Path 04 or 05: Range [8.19E+03, 16.4E+03] | ")
            comparator = k10ToThe[4];
            if (value >= comparator) {
                PRINTF ("Length:5 | ")
                if (text + 4 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // Length:4
            }
            else {
                if (text + 4 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:4 | ")
            }
            text_end = text + 3;
            index = value % 100;
            value /= 100;
            if (text && 0x1) {
                PRINTF ("\n    Unaligned value:", value)
                digits = kDigits00To99[value];
                *(text    ) = (char)digits;
                *(text + 1) = (char)(digits >> 8);
                digits = kDigits00To99[index];
                *(text + 2) = (char)digits;
                *(text + 3) = (char)(digits >> 8);
                return text_end + 1;
            }
            PRINTF ("\n    Aligned value:", value)
            text16 = reinterpret_cast<uint16_t*> (text_end - 1);
            *(text16    ) = kDigits00To99[index];
            *(text16 - 1) = kDigits00To99[value];
            return text_end + 1;
        }
        else if (value >> 7) {
            PRINTF ("\n    Path 03 or 04: Range [5.12E+02, 10.2E+02]")
            comparator = k10ToThe[3];
            if (value >= comparator) {
                PRINTF ("Length:4 |")
                if (text + 5 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
            } else {
                if (text + 5 > text_end) {
                    return nullptr;
                }
                PRINTF ("Length:3 |")
            }
            index = value % 10;
            value /= 10;
            value = kDigits00To99[value];
            *(text    ) = (char)value;
            *(text + 1) = (char)(value >> 8);
            *(text + 2) = '0' + index;
            return text + 3;
        }
        else if (value >> 4) {
            PRINTF ("\n    Path 02 or 03: Range [6.40E+01, 12.7E+01]")
            if (value >= 100) { //Length:3
                PRINTF ("Length 3 |")
                if (text + 4 > text_end) {
                    return nullptr;
                }
                *text = '1';
                value = kDigits00To99[value - 100];
                *(text + 1) = (char)value;
                *(text + 2) = (char)(value >> 8);
                return text + 3;
            }
            PRINTF ("Length 2 |")
            if (text + 2 > text_end) {
                return nullptr;
            }
            value = kDigits00To99[value];
            *(text    ) = (char)value;
            *(text + 1) = (char)(value >> 8);
            return text + 2;
        }
        else { //if (value >> 4) {
            PRINTF ("\n    Path 01 or 02: Range [0.00E+00, 15.0E+00]")
            if (value >= 10) {
                PRINTF ("Length 2 |")
                if (text + 3 > text_end) {
                    return nullptr;
                }
                text_end  = text;
                *text       = '1';
                *(text + 1) = '0' + (char)(value - 10);
                return text + 2;
            }
            PRINTF ("Length 1 |")
            if (text + 2 > text_end) {
                return nullptr;
            }
            *text = '0' + (char)value;
            return text + 1;
        }
    }
    return nullptr;
}

}       //< namespace _
#endif  //< CRABS_SEAM >= 1 || CRABS_SEAM == 0
#undef DEBUG_PRINTED
