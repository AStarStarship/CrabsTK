
#include <stdafx.h>

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1

#include "script_itoa.h"

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

char* Print (uint64_t value, char* text, char* text_end) {
    return nullptr; /*
    // Lookup table for powers of 10.
    static const uint64_t k10ToThe[20]{
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
        1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000,
        100000000000000, 1000000000000000, 10000000000000000,
        100000000000000000, 1000000000000000000, 10000000000000000000, };

    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }

    if (value >> 31) {
        return Print ((uint32_t)value, text, text_end);
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
        *(text + i) = 'x';
    }
    *(text + 21) = 0;
    char*       begin = text;
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
                if (text + 19 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:19 |")
                text_end = text + 18;
                goto PrintMsdLookDownOdd;
            }
            if (text + 20 > text_end) {
                return nullptr;
            }
            PRINTF ("length:20 |")
            *text++ = '1';
            value -= comparator;
            // length:19
            text_end = text + 18;
            goto PrintMsdLookUpOdd;
        }
        else if (value >> 57) {
            PRINTF ("\n    Path 18 or 19   [5.76E+17, 11.5E+17] and ")
            comparator = k10ToThe[18];
            if (value >= comparator) { // length:19
                PRINTF ("length:19")
                if (text + 20 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // length:18
                text_end = text + 17;
                goto PrintMsdLookUpEven;
            }
            if (text + 18 > text_end) {
                return nullptr;
            }
            PRINTF ("length:18")
            text_end = text + 17;
            goto PrintMsdLookDownEven;
        }
        else if (value >> 54) {
            PRINTF ("\n    Path 17 or 18 : Range [7.21E+16, 14.4E+16] and ")
            comparator = k10ToThe[17];
            if (value >= comparator) { // length:18
                PRINTF ("length:18")
                if (text + 19 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // length:17
                text_end = text + 16;
                goto PrintMsdLookUpOdd;
            }
            PRINTF ("length:17")
            text_end = text + 16;
            goto PrintMsdLookDownOdd;
        }
        else if (value >> 50) {
            PRINTF ("\n    Path 16 or 17   [9.01E+15, 18.0E+15] and ")
            comparator = k10ToThe[16];
            if (value < comparator) {
                PRINTF ("length:16")
                text_end = text + 15;
                goto PrintMsdLookDownEven;
            }
            // @todo Optimize this path.
            PRINTF ("length:17")
            if (text + 17 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // length: 16
            text_end = text + 15;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 47) {
            PRINTF ("\n    Path 15 or 16    [5.63E+14, 11.3E+14] and ")
            comparator = k10ToThe[15];
            if (value >= comparator) {
                if (text + 17 > text_end) {
                    return nullptr;
                }
                PRINTF (" length:16")
                *text++ = '1';
                value -= comparator;
                // length:15
                text_end = text + 14;
                goto PrintMsdLookUpOdd;
            }
            if (text + 16 > text_end) {
                return nullptr;
            }
            PRINTF ("length:15")
            text_end = text + 14;
            goto PrintMsdLookDownOdd;
        }
        else if (value >> 44) {
            PRINTF ("\n    Path 14 or 15   [7.04E+13, 14.1E+13] and ")
            comparator = k10ToThe[14];
            if (value < comparator) {
                if (text + 14 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:14")
                text_end = text + 13;
                goto PrintMsdLookDownEven;
            }
            if (text + 15 > text_end) {
                return nullptr;
            }
            PRINTF ("length:15")
            *text++ = '1';
            value -= comparator;
            // length:14
            text_end = text + 13;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 40) {
            PRINTF ("\n    Path 13 or 14 : Range 8.80E+12 | 17.6E+12] and ")
            comparator = k10ToThe[13];
            if (value < comparator) {
                if (text + 13 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:13")
                text_end = text + 12;
                goto PrintMsdLookDownOdd;
            }
            PRINTF ("length:14")
            if (text + 14 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // length: 13
            text_end = text + 12;
            goto PrintMsdLookUpOdd;
        }
        else if (value >> 37) {
            PRINTF ("\n    Path 12 or 13 : Range [5.50E+11, 11.0E+11] and ")
            comparator = k10ToThe[12];
            if (value >= comparator) {
                PRINTF ("length:13")
                if (text + 14 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // length:12
                text_end = text + 11;
                goto PrintMsdLookUpEven;
            }
            if (text + 13 > text_end) {
                return nullptr;
            }
            PRINTF ("length:12")
            text_end = text + 11;
            goto PrintMsdLookDownEven;
        }
        else { // if (value >> 36) {
            PRINTF ("\n    Path 11 or 12: Range [6.87E+10, 13.7E+10]")
            comparator = k10ToThe[11];
            if (value < comparator) {
                PRINTF ("length:11")
                text_end = text + 10;
                goto PrintMsdLookDownOdd;
            }
            if (text + 13 > text_end) {
                return nullptr;
            }
            PRINTF ("length:12")
            *text++ = '1';
            value -= comparator;
            // length:11
            text_end = text + 10;
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
                if (text + 10 > text_end) {
                    return nullptr;
                }
                text_end = text + 9;
                goto PrintMsdLookDownEven;
            }
            if (text + 11 > text_end) {
                return nullptr;
            }
            PRINTF ("length:11 |")
            *text++ = '1';
            value -= comparator;
            // length:10
            text_end = text + 9;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 27) {
            PRINTF ("\n    Length 09 or 10: Range [5.37E+08, 10.7E+08] and ")
            comparator = k10ToThe[9];
            offset     = k10ToThe[8];
            if (value < comparator) {
                PRINTF ("length:9")
                if (text + 8 > text_end) {
                    return nullptr;
                }
                text_end = text + 8;
                goto PrintMsdLookDownOdd;
            }
            PRINTF ("length:10 |")
            if (text + 11 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // length:9
            text_end = text + 8;
            goto PrintMsdLookUpOdd;
        } 
        else if (value >> 24) {
            PRINTF ("\n    Length 08 or 09: Range [6.71E+07, 13.4E+07 and ")
            comparator = k10ToThe[8];
            offset     = k10ToThe[7];
            if (value < comparator) {
                if (text + 8 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:8 | ")
                text_end = text + 7;
                goto PrintMsdLookDownEven;
            }
            PRINTF ("length:9 | ")
            if (text + 9 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // length:8
            text_end = text + 7;
            goto PrintMsdLookUpEven;
        }
        else if (value >> 20) {
            PRINTF ("\n    Length 07 or 08: Range [8.39E+06 | 16.8E+06]   and ")
            comparator = k10ToThe[7];
            offset     = k10ToThe[6];
            if (value < comparator) {
                if (text + 8 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:7 | ")
                text_end = text + 6;
                goto PrintMsdLookDownOdd;
            }
            PRINTF ("length:8 | ")
            if (text + 8 > text_end) {
                return nullptr;
            }
            *text++ = '1';
            value -= comparator;
            // length:7
            text_end = text + 6;
            goto PrintMsdLookUpOdd;
        }
        else if (value >> 17) {
            PRINTF ("\n    Length 06 or 07: Range [5.24E+05, 10.5E+05] and ")
            comparator = k10ToThe[6];
            offset     = k10ToThe[5];
            if (value >= comparator) {
                PRINTF ("length:7 |")
                if (text + 9 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                // The next digit is '0' but it messes up the algorithm and this 
                // branch is rarely executed so that optimization is thrown 
                // out.
                value -= comparator;
                // length:6
                text_end = text + 5;
                PrintMsdLookUpEven:
                PRINTF ("\n    PrintMsdLookUpEven:%I64u", value)
                text16 = reinterpret_cast<uint16_t*> (text_end - 1);
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
            PRINTF ("length:6 |")
            text_end = text + 5;
            PrintMsdLookDownEven:
            PRINTF ("\n    PrintMsdLookDownEven:%I64u", value)
            text16 = reinterpret_cast<uint16_t*> (text_end - 1);
            offset = (text_end - text) >> 1;
            PRINTF ("\n    number of times to loop:%I64u", offset)
            while (--offset > 0) {
                index = value % 100;
                value /= 100;
                *text16-- = kDigits00To99[index];
                DEBUG_PRINTED (value, begin, text_end);
            }
            DEBUG_PRINTED (value, begin, text_end);
            PRINTF ("\n    value:%I64u", value)
            *text16 = kDigits00To99[value];
            return text_end + 1;
        }
        else { // if (value >> 14) {
            PRINTF ("\n    Length 05 or 06: Range [6.55E+04, 13.1E+04] | ")
            comparator = k10ToThe[5];
            offset     = k10ToThe[4];
            if (value >= comparator) {
                if (text + 6 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:6 ")
                value -= comparator;
                *text++ = '1';
                // length:5
                text_end = text + 4;

                PrintMsdLookUpOdd:
                PRINTF ("\n    PrintMsdLookUpOdd:%I64u", value)
                index = value % 10;
                *text_end = '0' + index;
                value /= 10;
                DEBUG_PRINTED (value, begin, text_end);
                text16 = reinterpret_cast<uint16_t*> (text_end - 2);
                offset = ((text_end - text) >> 1);
                PRINTF ("\n    number of times to loop:%I64u", offset)
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
            PRINTF ("length:5 ")
            text_end = text + 4;

            PrintMsdLookDownOdd:
            PRINTF ("\n    PrintMsdLookDownOdd:%I64u", value)
            index = value % 10;
            value /= 10;
            *text_end = '0' + index;
            // length:6
            text16 = reinterpret_cast<uint16_t*> (text_end - 2);
            offset = (text_end - text) >> 1;
            PRINTF ("\n    number of times to loop:%I64u", offset)
            while (offset-- > 0) {
                PRINTF ("\n    value:%I64u", value)
                index = value % 100;
                *text16-- = kDigits00To99[index];
                value /= 100;
                DEBUG_PRINTED (value, begin, text_end);
            }
            *text16 = kDigits00To99[value];
            PRINTF ("\n    value:%I64u", value)
            DEBUG_PRINTED (value, begin, text_end);
            return text_end + 1;
        }
    }
    else {
        if (value >> 10) {
            PRINTF ("\n    Length 04 or 05: Range [8.19E+03, 16.4E+03] | ")
            comparator = k10ToThe[4];
            if (value >= comparator) {
                PRINTF ("length:5 | ")
                if (text + 4 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
                // length:4
            }
            else {
                if (text + 4 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:4 | ")
            }
            text_end = text + 3;
            index = value % 100;
            value /= 100;
            if (text && 0x1) {
                PRINTF ("\n    Unaligned value:%I64u", value)
                digits = kDigits00To99[value];
                *(text    ) = (char)digits;
                *(text + 1) = (char)(digits >> 8);
                digits = kDigits00To99[index];
                *(text + 2) = (char)digits;
                *(text + 3) = (char)(digits >> 8);
                return text_end + 1;
            }
            PRINTF ("\n    Aligned value:%I64u", value)
            text16 = reinterpret_cast<uint16_t*> (text_end - 1);
            *(text16    ) = kDigits00To99[index];
            *(text16 - 1) = kDigits00To99[value];
            return text_end + 1;
        }
        else if (value >> 7) {
            PRINTF ("\n    Length 03 or 04: Range [5.12E+02, 10.2E+02]")
            comparator = k10ToThe[3];
            if (value >= comparator) {
                PRINTF ("length:4 |")
                if (text + 5 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= comparator;
            } else {
                if (text + 5 > text_end) {
                    return nullptr;
                }
                PRINTF ("length:3 |")
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
            PRINTF ("\n    Length 02 or 03: Range [6.40E+01, 12.7E+01]")
            if (value >= 100) {
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
            PRINTF ("\n    Length 01 or 02: Range [0.00E+00, 15.0E+00]")
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
    return nullptr; */
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1