/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_print_ui8.cc
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
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
// Dependencies:
#include "print_to_ascii.h"
// End dependencies.
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
#define PRINT_BINARY
#define PRINT_BINARY_TABLE
#define PRINT_HEADER
#define PRINT_HEADING
#endif

namespace _ {

/** @todo Update this algorithm to Script itoa. */

char* Print (char* cursor, char* end, uint64_t value) {
    ASSERT (cursor)
    ASSERT (end) 
    int chars_printed = sprintf_s (cursor, end  - cursor, "%llu", value);
    return cursor + chars_printed;
    /*
    // Lookup table for powers of 10.
    static const uint64_t k10ToThe[20]{
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
        1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000,
        100000000000000, 1000000000000000, 10000000000000000,
        100000000000000000, 1000000000000000000, 10000000000000000000, };

    if (!cursor) {
        return nullptr;
    }
    if (cursor > end) {
        return nullptr;
    }

    if (value >> 31) {
        return Print ((uint32_t)value, cursor, end);
    }

    // 1. Divide 64-bit into  

    uint16_t  * text16;
    char        index;
    uint16_t    digits;
    uint64_t    comparator,
                offset;
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    // Write a bunches of xxxxxx to the buffer for debug purposes.
    for (int i = 0; i <= 21; ++i) {
        *(cursor + i) = 'x';
    }
    *(cursor + 21) = 0;
    char*       begin = cursor;
    char        buffer[24];
    #endif
    // 0000000000000000000000000000000000000000000000100100100111110000
    // bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
    // 6666555555555544444444443333333333222222222211111111110000000000
    // 3210987654321098765432109876543210987654321098765432109876543210
    // |  |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |
    // 2  1  1  1   1  1  1   1  1  1   1  0  0   0  0  0   0  0  0   0
    // 0  9  8  7   6  5  4   3  2  1   0  9  8   7  6  5   4  3  2   1
    // |----------------------------|-------------------|-------------|
    // |        Upper Chunk         |   Lower-Middle    | Lower Chunk |

    if (value >> 34) {
        if (value >> 60) {
            PRINTF ("\n    Path 19 or 20: Range [9.22E+18, 18.4E+18] and ")
            comparator = k10ToThe[19];
            if (value < comparator) {
                if (cursor + 19 > end) {
                    return nullptr;
                }
                PRINTF ("length:19 |")
                end = cursor + 18;
                goto PrintMsdLookDownOdd;
            }
            if (cursor + 20 > end) {
                return nullptr;
            }
            PRINTF ("length:20 |")
            *cursor++ = '1';
            value -= comparator;
            // length:19
            end = cursor + 18;
            goto PrintMsdLookUpOdd;
        }
        else if (value >> 57) {
            PRINTF ("\n    Path 18 or 19   [5.76E+17, 11.5E+17] and ")
            comparator = k10ToThe[18];
            if (value >= comparator) { // length:19
                PRINTF ("length:19")
                if (cursor + 20 > end) {
                    return nullptr;
                }
                *cursor++ = '1';
                value -= comparator;
                // length:18
                end = cursor + 17;
                goto PrintMsdLookUpEven;
            }
            if (cursor + 18 > end) {
                return nullptr;
            }
            PRINTF ("length:18")
            end = cursor + 17;
            goto PrintMsdLookDownEven;
        }
        else if (value >> 54) {
            PRINTF ("\n    Path 17 or 18 : Range [7.21E+16, 14.4E+16] and ")
            comparator = k10ToThe[17];
            if (value >= comparator) { // length:18
                PRINTF ("length:18")
                if (cursor + 19 > end) {
                    return nullptr;
                }
                *cursor++ = '1';
                value -= comparator;
                // length:17
                end = cursor + 16;
                goto PrintMsdLookUpOdd;
            }
            PRINTF ("length:17")
            end = cursor + 16;
            goto PrintMsdLookDownOdd;
        }
        else if (value >> 50) {
            PRINTF ("\n    Path 16 or 17   [9.01E+15, 18.0E+15] and ")
            comparator = k10ToThe[16];
            if (value < comparator) {
                PRINTF ("length:16")
                end = cursor + 15;
                goto PrintMsdLookDownEven;
            }
            // @todo Optimize this path.
            PRINTF ("length:17")
            if (cursor + 17 > end) {
                return nullptr;
            }
            *cursor++ = '1';
            value -= comparator;
            // length: 16
            end = cursor + 15;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 47) {
            PRINTF ("\n    Path 15 or 16    [5.63E+14, 11.3E+14] and ")
            comparator = k10ToThe[15];
            if (value >= comparator) {
                if (cursor + 17 > end) {
                    return nullptr;
                }
                PRINTF (" length:16")
                *cursor++ = '1';
                value -= comparator;
                // length:15
                end = cursor + 14;
                goto PrintMsdLookUpOdd;
            }
            if (cursor + 16 > end) {
                return nullptr;
            }
            PRINTF ("length:15")
            end = cursor + 14;
            goto PrintMsdLookDownOdd;
        }
        else if (value >> 44) {
            PRINTF ("\n    Path 14 or 15   [7.04E+13, 14.1E+13] and ")
            comparator = k10ToThe[14];
            if (value < comparator) {
                if (cursor + 14 > end) {
                    return nullptr;
                }
                PRINTF ("length:14")
                end = cursor + 13;
                goto PrintMsdLookDownEven;
            }
            if (cursor + 15 > end) {
                return nullptr;
            }
            PRINTF ("length:15")
            *cursor++ = '1';
            value -= comparator;
            // length:14
            end = cursor + 13;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 40) {
            PRINTF ("\n    Path 13 or 14 : Range 8.80E+12 | 17.6E+12] and ")
            comparator = k10ToThe[13];
            if (value < comparator) {
                if (cursor + 13 > end) {
                    return nullptr;
                }
                PRINTF ("length:13")
                end = cursor + 12;
                goto PrintMsdLookDownOdd;
            }
            PRINTF ("length:14")
            if (cursor + 14 > end) {
                return nullptr;
            }
            *cursor++ = '1';
            value -= comparator;
            // length: 13
            end = cursor + 12;
            goto PrintMsdLookUpOdd;
        }
        else if (value >> 37) {
            PRINTF ("\n    Path 12 or 13 : Range [5.50E+11, 11.0E+11] and ")
            comparator = k10ToThe[12];
            if (value >= comparator) {
                PRINTF ("length:13")
                if (cursor + 14 > end) {
                    return nullptr;
                }
                *cursor++ = '1';
                value -= comparator;
                // length:12
                end = cursor + 11;
                goto PrintMsdLookUpEven;
            }
            if (cursor + 13 > end) {
                return nullptr;
            }
            PRINTF ("length:12")
            end = cursor + 11;
            goto PrintMsdLookDownEven;
        }
        else { // if (value >> 36) {
            PRINTF ("\n    Path 11 or 12: Range [6.87E+10, 13.7E+10]")
            comparator = k10ToThe[11];
            if (value < comparator) {
                PRINTF ("length:11")
                end = cursor + 10;
                goto PrintMsdLookDownOdd;
            }
            if (cursor + 13 > end) {
                return nullptr;
            }
            PRINTF ("length:12")
            *cursor++ = '1';
            value -= comparator;
            // length:11
            end = cursor + 10;
            goto PrintMsdLookUpOdd;
        }
    }
    else if (value >> 14) {
        if (value >> 30) {
            PRINTF ("\n    Path 10 or 11: Range [8.59E+09, 17.2E+09] and ")
            comparator = k10ToThe[10];
            offset     = k10ToThe[9];
            if (value < comparator) {
                PRINTF ("length:10 |")
                if (cursor + 10 > end) {
                    return nullptr;
                }
                end = cursor + 9;
                goto PrintMsdLookDownEven;
            }
            if (cursor + 11 > end) {
                return nullptr;
            }
            PRINTF ("length:11 |")
            *cursor++ = '1';
            value -= comparator;
            // length:10
            end = cursor + 9;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 27) {
            PRINTF ("\n    Length 09 or 10: Range [5.37E+08, 10.7E+08] and ")
            comparator = k10ToThe[9];
            offset     = k10ToThe[8];
            if (value < comparator) {
                PRINTF ("length:9")
                if (cursor + 8 > end) {
                    return nullptr;
                }
                end = cursor + 8;
                goto PrintMsdLookDownOdd;
            }
            PRINTF ("length:10 |")
            if (cursor + 11 > end) {
                return nullptr;
            }
            *cursor++ = '1';
            value -= comparator;
            // length:9
            end = cursor + 8;
            goto PrintMsdLookUpOdd;
        } 
        else if (value >> 24) {
            PRINTF ("\n    Length 08 or 09: Range [6.71E+07, 13.4E+07 and ")
            comparator = k10ToThe[8];
            offset     = k10ToThe[7];
            if (value < comparator) {
                if (cursor + 8 > end) {
                    return nullptr;
                }
                PRINTF ("length:8 | ")
                end = cursor + 7;
                goto PrintMsdLookDownEven;
            }
            PRINTF ("length:9 | ")
            if (cursor + 9 > end) {
                return nullptr;
            }
            *cursor++ = '1';
            value -= comparator;
            // length:8
            end = cursor + 7;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 20) {
            PRINTF ("\n    Length 07 or 08: Range [8.39E+06 | 16.8E+06]   and ")
            comparator = k10ToThe[7];
            offset     = k10ToThe[6];
            if (value < comparator) {
                if (cursor + 8 > end) {
                    return nullptr;
                }
                PRINTF ("length:7 | ")
                end = cursor + 6;
                goto PrintMsdLookDownOdd;
            }
            PRINTF ("length:8 | ")
            if (cursor + 8 > end) {
                return nullptr;
            }
            *cursor++ = '1';
            value -= comparator;
            // length:7
            end = cursor + 6;
            goto PrintMsdLookUpOdd;
        }
        else if (value >> 17) {
            PRINTF ("\n    Length 06 or 07: Range [5.24E+05, 10.5E+05] and ")
            comparator = k10ToThe[6];
            offset     = k10ToThe[5];
            if (value >= comparator) {
                PRINTF ("length:7 |")
                if (cursor + 9 > end) {
                    return nullptr;
                }
                *cursor++ = '1';
                // The next digit is '0' but it messes up the algorithm and this 
                // branch is rarely executed so that optimization is thrown 
                // out.
                value -= comparator;
                // length:6
                end = cursor + 5;
                PrintMsdLookUpEven:
                PRINTF ("\n    PrintMsdLookUpEven:%I64u", value)
                text16 = reinterpret_cast<uint16_t*> (end - 1);
                offset = (end - cursor) >> 1;
                while (--offset > 0) {
                    index = value % 100;
                    *text16-- = kDigits00To99[index];
                    value /= 100;
                    DEBUG_PRINTED (value, begin, end);
                }
                *text16 = kDigits00To99[value];
                DEBUG_PRINTED (value, begin, end);
                return end + 1;
            }
            if (cursor + 6 > end) {
                return nullptr;
            }
            PRINTF ("length:6 |")
            end = cursor + 5;
            PrintMsdLookDownEven:
            PRINTF ("\n    PrintMsdLookDownEven:%I64u", value)
            text16 = reinterpret_cast<uint16_t*> (end - 1);
            offset = (end - cursor) >> 1;
            PRINTF ("\n    number of times to loop:%I64u", offset)
            while (--offset > 0) {
                index = value % 100;
                value /= 100;
                *text16-- = kDigits00To99[index];
                DEBUG_PRINTED (value, begin, end);
            }
            DEBUG_PRINTED (value, begin, end);
            PRINTF ("\n    value:%I64u", value)
            *text16 = kDigits00To99[value];
            return end + 1;
        }
        else { // if (value >> 14) {
            PRINTF ("\n    Length 05 or 06: Range [6.55E+04, 13.1E+04] | ")
            comparator = k10ToThe[5];
            offset     = k10ToThe[4];
            if (value >= comparator) {
                if (cursor + 6 > end) {
                    return nullptr;
                }
                PRINTF ("length:6 ")
                value -= comparator;
                *cursor++ = '1';
                // length:5
                end = cursor + 4;

                PrintMsdLookUpOdd:
                PRINTF ("\n    PrintMsdLookUpOdd:%I64u", value)
                index = value % 10;
                *end = '0' + index;
                value /= 10;
                DEBUG_PRINTED (value, begin, end);
                text16 = reinterpret_cast<uint16_t*> (end - 2);
                offset = ((end - cursor) >> 1);
                PRINTF ("\n    number of times to loop:%I64u", offset)
                while (--offset > 0) {
                    index = value % 100;
                    *text16-- = kDigits00To99[index];
                    value /= 100;
                    DEBUG_PRINTED (value, begin, end);
                }
                *text16 = kDigits00To99[value];
                DEBUG_PRINTED (value, begin, end);
                return end + 1;
            }
            if (cursor + 5 > end) {
                return nullptr;
            }
            PRINTF ("length:5 ")
            end = cursor + 4;

            PrintMsdLookDownOdd:
            PRINTF ("\n    PrintMsdLookDownOdd:%I64u", value)
            index = value % 10;
            value /= 10;
            *end = '0' + index;
            // length:6
            text16 = reinterpret_cast<uint16_t*> (end - 2);
            offset = (end - cursor) >> 1;
            PRINTF ("\n    number of times to loop:%I64u", offset)
            while (offset-- > 0) {
                PRINTF ("\n    value:%I64u", value)
                index = value % 100;
                *text16-- = kDigits00To99[index];
                value /= 100;
                DEBUG_PRINTED (value, begin, end);
            }
            *text16 = kDigits00To99[value];
            PRINTF ("\n    value:%I64u", value)
            DEBUG_PRINTED (value, begin, end);
            return end + 1;
        }
    }
    else {
        if (value >> 10) {
            PRINTF ("\n    Length 04 or 05: Range [8.19E+03, 16.4E+03] | ")
            comparator = k10ToThe[4];
            if (value >= comparator) {
                PRINTF ("length:5 | ")
                if (cursor + 4 > end) {
                    return nullptr;
                }
                *cursor++ = '1';
                value -= comparator;
                // length:4
            }
            else {
                if (cursor + 4 > end) {
                    return nullptr;
                }
                PRINTF ("length:4 | ")
            }
            end = cursor + 3;
            index = value % 100;
            value /= 100;
            if (cursor && 0x1) {
                PRINTF ("\n    Unaligned value:%I64u", value)
                digits = kDigits00To99[value];
                *(cursor    ) = (char)digits;
                *(cursor + 1) = (char)(digits >> 8);
                digits = kDigits00To99[index];
                *(cursor + 2) = (char)digits;
                *(cursor + 3) = (char)(digits >> 8);
                return end + 1;
            }
            PRINTF ("\n    Aligned value:%I64u", value)
            text16 = reinterpret_cast<uint16_t*> (end - 1);
            *(text16    ) = kDigits00To99[index];
            *(text16 - 1) = kDigits00To99[value];
            return end + 1;
        }
        else if (value >> 7) {
            PRINTF ("\n    Length 03 or 04: Range [5.12E+02, 10.2E+02]")
            comparator = k10ToThe[3];
            if (value >= comparator) {
                PRINTF ("length:4 |")
                if (cursor + 5 > end) {
                    return nullptr;
                }
                *cursor++ = '1';
                value -= comparator;
            } else {
                if (cursor + 5 > end) {
                    return nullptr;
                }
                PRINTF ("length:3 |")
            }
            index = value % 10;
            value /= 10;
            value = kDigits00To99[value];
            *(cursor    ) = (char)value;
            *(cursor + 1) = (char)(value >> 8);
            *(cursor + 2) = '0' + index;
            return cursor + 3;
        }
        else if (value >> 4) {
            PRINTF ("\n    Length 02 or 03: Range [6.40E+01, 12.7E+01]")
            if (value >= 100) {
                PRINTF ("Length 3 |")
                if (cursor + 4 > end) {
                    return nullptr;
                }
                *cursor = '1';
                value = kDigits00To99[value - 100];
                *(cursor + 1) = (char)value;
                *(cursor + 2) = (char)(value >> 8);
                return cursor + 3;
            }
            PRINTF ("Length 2 |")
            if (cursor + 2 > end) {
                return nullptr;
            }
            value = kDigits00To99[value];
            *(cursor    ) = (char)value;
            *(cursor + 1) = (char)(value >> 8);
            return cursor + 2;
        }
        else { //if (value >> 4) {
            PRINTF ("\n    Length 01 or 02: Range [0.00E+00, 15.0E+00]")
            if (value >= 10) {
                PRINTF ("Length 2 |")
                if (cursor + 3 > end) {
                    return nullptr;
                }
                end  = cursor;
                *cursor       = '1';
                *(cursor + 1) = '0' + (char)(value - 10);
                return cursor + 2;
            }
            PRINTF ("Length 1 |")
            if (cursor + 2 > end) {
                return nullptr;
            }
            *cursor = '0' + (char)value;
            return cursor + 1;
        }
    }
    return nullptr; */
}

char* Print (char* cursor, char* end, int64_t value) {
    if (value >= 0)
        return Print (cursor, end, (uint64_t)value);
    *cursor++ = '-';
    return Print (cursor, end, (uint64_t)(value * -1));
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#undef PRINT_BINARY
#undef PRINT_BINARY_TABLE
#undef PRINT_PRINTED
#undef PRINT_HEADING
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1