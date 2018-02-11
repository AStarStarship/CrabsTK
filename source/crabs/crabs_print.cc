/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_text.cc
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

#include "print.h"
#include "text.h"

#if CRABS_SEAM >= 1
#if USING_CRABS_TEXT

namespace _ {

static const uint16_t kDigits0To99[100] = {
    0x3030, 0x3031, 0x3032, 0x3033, 0x3034, 0x3035,
    0x3036, 0x3037, 0x3038, 0x3039, 0x3130, 0x3131,
    0x3132, 0x3133, 0x3134, 0x3135, 0x3136, 0x3137,
    0x3138, 0x3139, 0x3230, 0x3231, 0x3232, 0x3233,
    0x3234, 0x3235, 0x3236, 0x3237, 0x3238, 0x3239,
    0x3330, 0x3331, 0x3332, 0x3333, 0x3334, 0x3335,
    0x3336, 0x3337, 0x3338, 0x3339, 0x3430, 0x3431,
    0x3432, 0x3433, 0x3434, 0x3435, 0x3436, 0x3437,
    0x3438, 0x3439, 0x3530, 0x3531, 0x3532, 0x3533,
    0x3534, 0x3535, 0x3536, 0x3537, 0x3538, 0x3539,
    0x3630, 0x3631, 0x3632, 0x3633, 0x3634, 0x3635,
    0x3636, 0x3637, 0x3638, 0x3639, 0x3730, 0x3731,
    0x3732, 0x3733, 0x3734, 0x3735, 0x3736, 0x3737,
    0x3738, 0x3739, 0x3830, 0x3831, 0x3832, 0x3833,
    0x3834, 0x3835, 0x3836, 0x3837, 0x3838, 0x3839,
    0x3930, 0x3931, 0x3932, 0x3933, 0x3934, 0x3935,
    0x3936, 0x3937, 0x3938, 0x3939 };

char* Print64 (uint64_t value, char* text) {
    if ()
    return nullptr;
}

char* Print20Decimals (uintptr_t value, char* text) {
    return text;
}

/*
char* Print (uint16_t value, char* text, char* text_end, char delimiter) {

    // Below is a the number of min and max length of integer strings. Please 
    // note there is two groups of 4 at min 1, min 4, and max 4. with two edge cases per 
    // group.

    // +----------------------------------------------------------------------+
    // |  bit  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  | 
    // |  min  1   1   1   1   2   2   2   3   3   3   4   4   4   4   5   5  | 
    // |  max  1   1   1   2   2   2   3   3   3   4   4   4   4   5   5   5  | 
    // +----------------------------------------------------------------------+

    // To generate string length in Excel, use the equations:
    // =LEN (TEXT (POWER(2,B$2-1), "0"))
    // =LEN (TEXT (POWER(2,B$2)-1, "0"))



    uint16_t* digit; //< Pointer to the current digit
    uint16_t  size;  //< 

    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (!value) {
        if (text_end - text < 3) {
            return nullptr;
        }
        *text = '0';
        *(text + 1) = delimiter;
        return text + 2;
    }
    if (!(value >> 13)) { // Has 4-5 digits.
        // 2^13 = 8KB 2^14 = 16KB
        if (value << 16 > 0) { // There are 5 digits
            if (value < 10000) {
                goto Print3Chars16;
            }
        } else {
            size = 4;
        }
    } else if (value >> 10) { // Has 3-4 digits.
        value =  //< value = 1000
        if (value < (1 << 10) - 24) {
            size = 3;
        }
        Print3Chars16:
        ///size = 4;
        }
    } else if (value >> 7) { // Has 3-4 digits.
        if (value < ) {

        }
    } else if (value >> 4) { // Has 2-3 digits.
    }  else { // Has 1-2 digits.
        if (value - 100 > 0) {
            goto Print3Chars16;
        }
            value -= 500;
            digit = kDigits0To99[value]; // << 1 to * 2
            *text = '5';
            *(text + 1) = *digit;
            *(text + 2) = *(digit + 1);
            *(text + 3) = delimiter;
            return text + 4;
        }
        hundreds -= one_hundred; // hundreds should be 400
        value -= hundreds;
        digit = kDigits0To99[value]; // << 1 to * 2
        *text = '4';
        *(text + 1) = *digit;
        *(text + 2) = *(digit + 1);
        *(text + 3) = delimiter;
        return text + 4;
    }
    if (value >> 4) {
        if (value >= 127) {
            goto Print2Chars16;
        }
        Print3Chars16:
        value -= 300;
        digit = kDigits0To99[value]; // << 1 to * 2
        *text = '3';
        *(text + 1) = *digit;
        *(text + 2) = *(digit + 1);
        *(text + 3) = delimiter;
        return text + 4;
    }
    if (value > (one_hundred << 1)) {
        Print1Chars16:
        value -= (one_hundred << 1);
        digit = kDigits0To99[value]; // << 1 to * 2
        *text = '2';
        *(text + 1) = *digit;
        *(text + 2) = *(digit + 1);
        *(text + 3) = delimiter;
        return text + 4;
    }
    value -= one_hundred;
    digit = kDigits0To99[value]; // << 1 to * 2
    *text = '1';
    *(text + 1) = *digit;
    *(text + 2) = *(digit + 1);
    *(text + 3) = delimiter;
    return text + 4;
    if (value > one_hundred) { // Range 100 through 511
                               // 0.63% random statistical chance of this branch but in many
                               // situations numbers will typically be lower in value because 
                               // the full range of the type is not being used.

    }
    digit = kDigits0To99[value]; // << 1 to * 2
    *text = *digit;
    *(text + 1) = *(digit + 1);
    *(text + 2) = delimiter;
    return text + 3;
    if (!(value >> 14)) {
        size = 4;
    } else if (value < 1000) {
        // (1000-512)/2^16 = 7.45% chance of this branch.
        size = 3;
    } else {
        // 5/16=33.33% chance of this branch.
        size = 5;
    }
    if (text + size + 1 > text_end) {
        return nullptr;
    }
    text_end = text + size;
    *(text_end - 1) = delimiter;

    while (value >= 100) {
        int hundreds = value % 100;
        value /= 100;
        digit = kDigits0To99[hundreds]; // << 1 to * 2
        *text-- = *(digit + 1);
        *text-- = *digit;
    }
    if (value < 10) {
        *text-- = '0' + value;
        return ;
    }
    *text       = *digit;
    *(text + 1) = *(digit + 1);
    return text_end;
    
}

char* Print (int16_t value, char* text, char* text_end, char delimiter) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (value < 0) {
        uint32_t abs_value = (~value) + 1;
        *text = '-';
        return Print (abs_value, text + 1, text_end, delimiter);
    }
    return Print (value, text + 1, text_end, delimiter);
}

char* Print (uint32_t value, char* text, char* text_end, char delimiter) {
    // The design trade-off going on here is cost of if statement branches
    // vs the size of the inline code vs statistical models. Random 
    // statistical numbers make some of the branches possible add extra time 
    // to the gestalt system from reduce i-cache performance but the reality is 
    // that most numbers will be smaller range due to the full range of the 
    // word not be used. The later assumption has been made but not benchmarked.

    // +------------------------------------------------------------------------+
    // |  bit    1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  | 
    // |  min    1   1   1   1   2   2   2   3   3   3   4   4   4   4   5   5  | 
    // |  max    1   1   1   2   2   2   3   3   3   4   4   4   4   5   5   5  | 
    // +------------------------------------------------------------------------+
    // |  bit   17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  | 
    // |  min    5   6   6   6   7   7   7   7   8   8   8   9   9   9  10  10  | 
    // |  max    6   6   6   7   7   7   7   8   8   8   9   9   9  10  10  10  | 
    // +------------------------------------------------------------------------+

    char* digit;
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (!value) {
        *text = '0';
        return text;
    }

    int size;
    if (value >> 16) {
        if (value >> 29) {
            size = 10;
        }
        else if (value >> 26) {
            size = 9;
        }
        else if (value >> 23) {
            size = 8;
        }
        else if (value >> 19) {
            size = 7;
        }
        else {
            size = 6;
        }
    } else {
        if (value >> 13) {
            size = 5;
        }
        if (value >> 9) {
            size = 4;
        }
        else if (value >> 6) {
            size = 3;
        }
        else if (value >> 3) {
            size = 2;
        }
        else {
            size = 1;
        }
    }

    if (text + size > text_end) {
        return nullptr;
    }
    text_end = text + size - 1;
    *(text_end + 1) = delimiter;

    while (value >= 100) {
        int pos = value % 100;
        value /= 100;
        *(int16_t*)(text_end - 1) = *(int16_t*)(kDigits0To99 + 2 * pos);
        text_end -= 2;
    }
    while (value>0)
    {
        *text_end-- = '0' + (value % 10);
        value /= 10;
    }
    return text;
}*/

char* PrintUnaligned (int64_t value, char* text) {

}

char* Print (int64_t value, char* text, char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (value < 0) {
        *text++ = '-';
        value = ~value + 1; // Uncompliment the negative value.
    }
    return PrintFast (value, text, text_end);
}

char* Print (uint64_t value, char* text, char* text_end) {
    if (!text) {
        return nullptr;
    }
    return PrintFast (value, text, text_end);
}
char* PrintFast (uint64_t value, char* text, char* text_end) {

    static const uint64_t k10ToThe[20]{
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 
        10000000000, 100000000000, 1000000000000, 10000000000000, 
        100000000000000, 1000000000, 1000000000000000, 10000000000000000, 
        100000000000000000, 1000000000000000000, 10000000000000000000
    };

    const char* digits;
    uint16_t  * text16;
    uint64_t    digit,
                comparison,
                offset;
    uint16_t    size,
                count;
     
    if (value >> 46) {
        if (value >> 63) { // min:9.22E+18 | max:18.4E+18
            digit = k10ToThe[19];
            if (value >= digit) {
                if (text + 20 > text_end) {
                    return nullptr;
                }
                if (text && 0x1) {
                    *text++ = '1';
                    value -= digit;
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print18Or19;
                }
                goto Print18Or19;
            }
            if (text + 19 > text_end) {
                return nullptr;
            }
            if (text && 0x1) {
                *text++ = '9';
                text16 = reinterpret_cast<uint16_t*> (text);
                goto Print18Or19;
            }
            text16 = reinterpret_cast<uint16_t*> (text);
            goto Print18Or19;
        }
        else if (value >> 59) { // min:5.76E+17 | max:11.5E+17
            digit = k10ToThe[18];
            if (value >= digit) { // Length:19
                if (text + 19 > text_end) {
                    return nullptr;
                }
                *text++ = '1';
                value -= digit;
                text16 = reinterpret_cast<uint16_t*> (text);
                goto Print18Or19;
            }
                // Length:18
                if (text && 0x1) {
                    if (text + 18 > text_end) {
                        return nullptr;
                    }
                    if (text && 0x1) {
                        offset = k10ToThe[16];
                        comparison = digit - offset;
                        if (value >= comparison) {
                            *text++ = '9';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        if (value >= (comparison -= offset)) {
                            *text++ = '8';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        if (value >= (comparison -= offset)) {
                            *text++ = '7';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        if (value >= (comparison -= offset)) {
                            *text++ = '6';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        *text++ = '7';
                        digit -= comparison;
                        text16 = reinterpret_cast<uint16_t*> (text);
                        goto Print18Or19;
                    }
                    goto Print18Or19;
                }
                text16 = reinterpret_cast<uint16_t*> (text);
                goto Print16Or17;
            }
            else if (value >> 56) { // 7.21E+16 | 14.4E+16
                digit = k10ToThe[16];
                if (value >= digit) { // Length:18
                    if (text + 20 > text_end) {
                        return nullptr;
                    }
                    if (text && 0x1) {
                        *text++ = '1';
                        value -= digit;
                        text16 = reinterpret_cast<uint16_t*> (text);
                        goto Print18Or19;
                    }
                    goto Print18Or19;
                }
                else { // Length:17
                    if (text + 19 > text_end) {
                        return nullptr;
                    }
                    if (text && 0x1) {
                        *text++ = '9';
                        text16 = reinterpret_cast<uint16_t*> (text);
                        goto Print16Or17;
                    }
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                }
            }
            else if (value >> 59) { // min:5.76E+17 | max:11.5E+17
                digit = k10ToThe[18];
                if (value >= digit) { // Length:17
                    if (text + 19 > text_end) {
                        return nullptr;
                    }
                    *text++ = '1';
                    value -= digit;
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                    // Length:16
                    if (text && 0x1) {
                        if (text + 18 > text_end) {
                            return nullptr;
                        }
                        // @todo insert module 10 here
                    }
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                }
                else { // Length:18
                    digit = k10ToThe[18];
                    if (value >= digit) { // Length:17
                        if (text + 20 > text_end) {
                            return nullptr;
                        }
                        if (text && 0x1) {
                            *text++ = '1';
                            value -= digit;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        goto Print18Or19;
                    }
                    else { // Length:16
                        if (text + 19 > text_end) {
                            return nullptr;
                        }
                        if (text && 0x1) {
                            if (text && 0x1) {
                                offset = k10ToThe[13];
                                comparison = digit - offset;
                                if (value >= comparison) {
                                    *text++ = '9';
                                    digit -= comparison;
                                    text16 = reinterpret_cast<uint16_t*> (text);
                                    goto Print16Or17;
                                }
                                if (value >= (comparison -= offset)) {
                                    *text++ = '8';
                                    digit -= comparison;
                                    text16 = reinterpret_cast<uint16_t*> (text);
                                    goto Print16Or17;
                                }
                                if (value >= (comparison -= offset)) {
                                    *text++ = '7';
                                    digit -= comparison;
                                    text16 = reinterpret_cast<uint16_t*> (text);
                                    goto Print16Or17;
                                }
                                if (value >= (comparison -= offset)) {
                                    *text++ = '6';
                                    digit -= comparison;
                                    text16 = reinterpret_cast<uint16_t*> (text);
                                    goto Print16Or17;
                                }
                                *text++ = '5';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print16Or17;
                            }
                            goto Print18Or19;
                        }
                        text16 = reinterpret_cast<uint16_t*> (text);
                        goto Print18Or19;
                    }
                }
            }
            else if (value >> 59) { // min:5.76E+17 | max:11.5E+17
                digit = k10ToThe[17];
                if (value >= digit) { // Length:16
                    if (text + 19 > text_end) {
                        return nullptr;
                    }
                    *text++ = '1';
                    value -= digit;
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print18Or19;
                    // Length:15
                    if (text && 0x1) {
                        if (text + 18 > text_end) {
                            return nullptr;
                        }
                        if (text && 0x1) {
                            offset = k10ToThe[16];
                            comparison = digit - offset;
                            if (value >= comparison) {
                                *text++ = '9';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '8';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '7';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '6';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            *text++ = '5';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        goto Print18Or19;
                    }
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                }
            }
            else if (value >> 53) { // 9.01E+15 | 1.8.0E+15
                digit = k10ToThe[16];
                if (value >= digit) { // Length:16
                    if (text + 19 > text_end) {
                        return nullptr;
                    }
                    *text++ = '1';
                    value -= digit;
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print18Or19;
                    // Length:15
                    if (text && 0x1) {
                        if (text + 18 > text_end) {
                            return nullptr;
                        }
                        if (text && 0x1) {
                            offset = k10ToThe[16];
                            comparison = digit - offset;
                            if (value >= comparison) {
                                *text++ = '9';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '8';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '7';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '6';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            *text++ = '5';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        goto Print18Or19;
                    }
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                }
            }
            else if (value >> 49) { // 5.63E+14 | 11.3E+14
                digit = k10ToThe[15];
                if (value >= digit) { // Length:16
                    if (text + 19 > text_end) {
                        return nullptr;
                    }
                    *text++ = '1';
                    value -= digit;
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print18Or19;
                    // Length:15
                    if (text && 0x1) {
                        if (text + 18 > text_end) {
                            return nullptr;
                        }
                        if (text && 0x1) {
                            offset = k10ToThe[16];
                            comparison = digit - offset;
                            if (value >= comparison) {
                                *text++ = '9';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '8';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '7';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '6';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            *text++ = '5';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        goto Print18Or19;
                    }
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                }
            }
            else { // if (value >> 46) // 7.04E+13 | 14.1E+13
                digit = k10ToThe[14];
                if (value >= digit) { // Length:16
                    if (text + 19 > text_end) {
                        return nullptr;
                    }
                    *text++ = '1';
                    value -= digit;
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print18Or19;
                    // Length:15
                    if (text && 0x1) {
                        if (text + 18 > text_end) {
                            return nullptr;
                        }
                        if (text && 0x1) {
                            offset = k10ToThe[16];
                            comparison = digit - offset;
                            if (value >= comparison) {
                                *text++ = '9';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '8';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '7';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            if (value >= (comparison -= offset)) {
                                *text++ = '6';
                                digit -= comparison;
                                text16 = reinterpret_cast<uint16_t*> (text);
                                goto Print18Or19;
                            }
                            *text++ = '5';
                            digit -= comparison;
                            text16 = reinterpret_cast<uint16_t*> (text);
                            goto Print18Or19;
                        }
                        goto Print18Or19;
                    }
                    text16 = reinterpret_cast<uint16_t*> (text);
                    goto Print16Or17;
                }
            }
        }
        else if (value >> 29) {
            if (value >> 46) { // 5.63E+14 | 11.3E+14
                goto Print16Or17;
            }
            else if (value >> 43) { // 8.80E+12 | 17.6E+12
                goto Print16Or17;
            }
            else if (value >> 56) { // 5.50E+11 | 11.0E+11
                goto Print16Or17;
            }
            else if (value >> 36) { // 6.87E+10 | 13.7E+10
                goto Print16Or17;
            }
            else { // value >> 33    8.59E+09 | 17.2E+09
                goto Print16Or17;
            }
        }
        else if (!(value >> 48)) {
            if (value >> 29) { // 5.37E+08 | 1.07E+09
                goto Print16Or17;
            }
            else if (value >> 25) { // 6.71E+07 | 13.4E+07
                goto Print16Or17;
            }
            else if (value >> 23) { // 8.39E+06 | 16.8E+06
                goto Print16Or17;
            }
            else if (value >> 19) { // 5.24E+05 | 10.5E+
                goto Print16Or17;
            }
            else { // value >> 16    6.55E+04 | 13.1E+04
                goto Print16Or17;
            }
            if (value >> 13) { // 8.19E+03 | 16.4E+03
                goto Print16Or17;
            }
            else if (value >> 9) { // 5.12E+02 | 10.2E+02
                goto Print16Or17;
            }
            else if (value >> 6) { // 6.40E+01 | 12.7E+01
                goto Print16Or17;
            }
            else if (value >> 3) { // 8.00E+00 | 15.0E+00
                goto Print16Or17;
            }
            else { // value <= 127

                if (value >= 100) {
                    *text++ = '1';
                }
            }
        }
    }
    Print18Or19:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print16Or17:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print14Or15:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print12Or13:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print10Or11:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print8Or9:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print6Or7:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    Print4Or5:
    digit = value % 100;
    value /= 100;
    *text16++ = kDigits0To99[digit];

    // Print3
    if (text_end && 0x1) {
        digit = value % 100;
        value /= 100;
        *text16++ = kDigits0To99[digit];
        *text_end = value;
        return text_end + 1;
    }
    // Print2
    *text16++ = kDigits0To99[digit];
    return text_end + 1;
}

char* Print (int32_t value, char* text, char* text_end, char delimiter) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    if (!value) {
        if (text_end - text < 3) {
            return nullptr;
        }
        *text = '0';
        *(text + 1) = 0;
        return text + 1;
    }
    if (value < 0) {
        // Convert from 2's compliment to uncomplimented form.
        uint32_t abs_value = (~value) + 1;
        *text = '-';
        return Print (abs_value, text + 1, text_end, delimiter);
    }
    return Print (value, text + 1, text_end, delimiter);
}

char* Print (const char* string, char* target, char* target_end, char delimiter) {
    if (!string) {
        return nullptr;
    }
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    char* cursor = target;
    char s = *string;
    while (s > delimiter) {
        *cursor++ = s;
        if (cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return nullptr;
        }
        s = *string++;
    }
    *cursor = s;
    return cursor;
}

Slot& Print (const char* text, Slot& slot, char delimiter) {
    if (!text) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        * end = slot.end;
    char c = *text;
    ++text;
    while (c > delimiter) {
        if (cursor > end) {
            cursor = slot.begin;
            end    = slot.start;
            if (cursor == end) {
                return slot;
            }
        }
        *cursor++ = c;
        c = *text++;
    }
    *cursor = 0;
    return slot;
}

char* Print (const char* text, const char* text_end, char* target,
             char* target_end, char delimiter) {
    if (target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    char* cursor = target;
    char t = *text;
    while (t > delimiter) {
        if (!t) {
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return nullptr;
        }
        if (++text > text_end) {
            *target = 0;
            return nullptr;
        }
        t = *text;
    }
    *target = t;
    return target;
}

Slot& Print (const char* text, const char* text_end, Slot& slot, char delimiter) {
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = slot.stop + 1,
        * end = slot.end;
    char c = *text;
    ++text;
    while (c > delimiter) {
        if (cursor > end) {
            cursor = slot.begin;
            end    = slot.start;
            if (cursor == end) {
                return slot;
            }
        }
        *cursor++ = c;
        c = *text++;
    }
    *cursor = 0;
    return slot;
}

char* Print (int8_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        return nullptr;
    }
    return target + result;
}

Slot& Print (int8_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 5,
    };

    char * begin,
         * start  = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end   = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end   = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    } else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (uint8_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (uint8_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 4,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (int16_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (int16_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 7,
    };

    char * begin,
         * start  = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (uint16_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (uint16_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 6,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (int32_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%i", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (int32_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 12,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
        // 4. Update the ring buffer stop.
        slot.stop = cursor;
        return slot;
    }
    // 2b. Check if there enough room in the buffer.
    size = start - cursor;
    if (size < kSizeMax) {
        return slot;
    }
    // 3b. There is no buffer overflow for this branch
    //     so just write the string like any old string.
    cursor = Print (value, cursor, end + kSizeMax, delimiter);
    if (!cursor) {
        return slot;
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (uint32_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%u", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (uint32_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 11,
    };

    char  * begin,
          * start = slot.start,
          * cursor = slot.stop,
          * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (int64_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result      = sprintf_s (target, buffer_size, "%lli", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (int64_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 22,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (uint64_t value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t result = sprintf_s (target, target_end - target, "%llu", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    std::cout << "\n!!! " << target << " result:" << result;
    return target + result;
}

Slot& Print (uint64_t value, Slot& slot, char delimiter) {
    enum {
        kSizeMax = 21,
    };

    char * begin,
         * start = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (float value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (float value, Slot& slot, char delimiter) {
    char * begin,
        *start = slot.start,
        *cursor = slot.stop,
        *end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // 1a. Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kStringFloatSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kStringFloatSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kStringFloatSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kStringFloatSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kStringFloatSizeMax, delimiter);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* Print (double value, char* target, char* target_end, char delimiter) {
    // Right now we're going to enforce there be enough room to write any
    // int32_t.
    intptr_t buffer_size = target_end - target,
             result = sprintf_s (target, buffer_size, "%f", value);
    if (result < 0) {
        *target = 0;
        return nullptr;
    }
    return target + result;
}

Slot& Print (double value, Slot& slot, char delimiter) {
    char * begin,
         * start  = slot.start,
         * cursor = slot.stop,
         * end;
    size_t size;
    int    num_chars;

    // First we need to check if the ring buffer is wrapped around.
    if (start > cursor) {
        // Check if there is enough room in the upper chunk first.
        end = slot.end;
        size = end - cursor;
        if (size < kStringDoubleSizeMax) {
            // Add the lower chunk size to the size
            begin = slot.begin;
            end = slot.end;
            size += (start - begin);
            if (size < kStringDoubleSizeMax) {
                // @todo Do I need to clear the buffer here in case of attack?
                return slot;
            }
            // Check if it was an Overflow Wraparound and shift the bytes into
            // the lower chunk if it was.
            cursor = SlotOverflowShift (begin, end, cursor);
        }
        // 1b. Write to the ring buffer.
        cursor = Print (value, cursor, end + kStringDoubleSizeMax);
        if (!cursor) {
            return slot;
        }
    }
    else {
        // 2b. Check if there enough room in the buffer.
        size = start - cursor;
        if (size < kStringDoubleSizeMax) {
            return slot;
        }
        // 3b. There is no buffer overflow for this branch
        //     so just write the string like any old string.
        cursor = Print (value, cursor, end + kStringDoubleSizeMax);
        if (!cursor) {
            return slot;
        }
    }
    // 4. Update the ring buffer stop.
    slot.stop = cursor;
    return slot;
}

char* PrintRight (const char* token, int num_columns, char* text, 
                  char* text_end, char delimiter) {
    if (!token) {
        return nullptr;
    }
    if (!text) {
        return nullptr;
    }
    if (text >= text_end) {
        return nullptr;
    }
    if (text + num_columns > text_end) {
        // Can't fit it in the buffer so this is an error.
        return nullptr;
    }
    intptr_t    length    = 0;     //< Length of the token.
    const char* token_end = token; //< Address of the last char.
    char*       cursor;            //< Print cursor pointer.

    // Find string length.
    while (*(++token_end) > delimiter);
    length = token_end - token;

    // If the length is less than the num_columns we need to print ".", "..", 
    // "..." or nothing and chop off some of the token.
    if (length > num_columns) {
        if (!length) {
            return text;
        }
        // Print some dots.
        if (!(length - 1)) {
            *text = '.';
            return text + 1;
        } else if (!(length - 2)) {
            *text = '.';
            *(text + 1) = '.';
            return text + 2;
        } else {
            cursor    = text + num_columns;
            text_end  = cursor;
            *cursor-- = '.';
            *cursor-- = '.';
            *cursor-- = '.';
        }
        while (cursor >= text) {
            *cursor-- = *token_end--;
        }
        return text_end;
    }
    cursor = text + num_columns;
    text = cursor;
    text_end = cursor - length;
    while (cursor > text_end) {
        *cursor-- = *token_end--;
    }
    while (cursor > text) {
        *cursor-- = ' ';
    }
    return text_end + 1;
}

Slot& PrintRight (const char* token, int num_columns, Slot& slot,
                  char delimiter) {
    char* start = slot.start,
        * stop  = slot.stop;
    if (start < stop) {
        stop = PrintRight (token, num_columns, start, stop);
        if (!stop) {
            return slot;
        }
    }
}

char* PrintCentered (const char* string, int width, char* text, char* text_end,
                     char delimiter) {
    if (width < 2) {
        //? Not sure if this is an error.
        return nullptr;
    }
    // We need to leave at least one space to the left and right of
    int length = TextLength (string, delimiter);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                *text++ = '\n';
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i) {
        *text++ = '\n';
    }
    printf (string);
    for (offset = width - length - offset; offset <= width; ++offset) {
        *text++ = '\n';
    }
    return text;
}

Slot& PrintCentered (const char* string, int width, Slot& slot,
                     char delimiter) {
    return slot;
}

/*
Slot& SlotWrite (Slot& slot, const char* text,
                   char delimiter) {
    if (!text) {
        return slot;
    }
    char* cursor = target;
    char s = *text;
    while (s != delimiter) {
        if (!s) {
            *target = 0; //< Replace the nil-term char.
            return slot;
        }
        *cursor = s;
        if (++cursor > target_end) {
            *target = 0; //< Replace the nil-term char.
            return slot;
        }
        ++text;
        s = *text;
    }
    *cursor = s;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* text,
                   const char* text_end) {
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = target;
    char t = *text;
    while (t) {
        *cursor = t;
        if (++cursor > target_end) {
            *target = 0;
            return slot;
        }
        if (++text > text_end) {
            *target = 0;
            return slot;
        }
        t = *text;
    }
    *cursor = t;
    return slot;
}

Slot& SlotWrite (Slot& slot, const char* text,
                   const char* text_end, char delimiter) {
    if (target > target_end) {
        return slot;
    }
    if (!text) {
        return slot;
    }
    if (text > text_end) {
        return slot;
    }
    char* cursor = target;
    char t = *text;
    while (t != delimiter) {
        if (!t) {
            //std::cout << "\n There was a !t error in SlotWrite.";
            return target;
        }
        *target = t;
        if (++target > target_end) {
            *target = 0;
            return slot;
        }
        if (++text > text_end) {
            *target = 0;
            return slot;
        }
        t = *text;
    }
    *target = t;
    return target;
}
*/
/*
Slot& PrintLine (const char* text, const char* header, int length, Slot& slot) {
    #if CRABS_SEAM == 1
    std::cout << header << "Writing Line with length " << length;
    #endif

    char* begin,
        * start  = slot.start,
        * cursor = slot.stop,
        * end;
    intptr_t size;

    if (cursor < start) {
        size = cursor - start;
        if (size < length + 1) {
            return slot;
        }
        TextLine (cursor, stop, header);
    } else {

    }

    SlotWrite (slot, cursor, end, header);

    if (!cursor) {
        #if CRABS_SEAM == 1
        std::cout << "\n Error writing header!";
        #endif
        return slot;
    }
    #if CRABS_SEAM == 1
    std::cout << "... wrote " << TextLength (cursor) << " chars.";
    #endif
    
    stop = cursor + length;
    if ((cursor + length) > end) {
        stop = end;
    }
    #if CRABS_SEAM == 1
    std::cout << "\n new_stop_length:" << stop - cursor;
    #endif

    while (cursor < stop) {
        *cursor++ = text;
    }
    *cursor = 0;
    #if CRABS_SEAM == 1
    std::cout << "\n - Wrote " << SlotLength (cursor) << " chars: " 
              << cursor;
    #endif
    slot.stop = cursor;
    return slot;
}*/

Slot& PrintLine (const char* string, int num_columns, Slot& slot,
                 char delimiter) {
    //Line ();
    char* begin    = slot.begin,
        *cursor    = slot.start,
        *last_char = cursor + num_columns + 1, //< +1 for nil-term char.
        *end       = slot.end;

    const char* read = string;
    if (num_columns < 1) {
        return slot;
    }
    if (!string) {
        return slot;
    }
    if (cursor == end) {
        return slot;
    }
    if (last_char > end) { // Chop of some of the columns.
        last_char = end;
    }

    while (cursor < last_char) {
        char c = *read++;
        if (!c) {
            *cursor++ = '_';
            read = string;
        }
    }
    *cursor = delimiter;
    slot.stop = cursor;
    return slot;
}

Slot& PrintLineBreak (const char* message, int top_bottom_margin,
                      char c, int num_columns, Slot& slot, char delimiter) {
    PrintLines (top_bottom_margin, slot);
    slot << "\n " << message;
    return PrintLine (c, "\n", num_columns, slot);
}

Slot& PrintLines (int num_rows, Slot& slot, char delimiter) {
    char* cursor = slot.stop + 1,
        *end = slot.end,
        *stop = cursor + num_rows + 1;
    uintptr_t upper_size = end - cursor + 1;
    if (upper_size < num_rows) {
        while (cursor < stop) {

        }
    }
    while (num_rows > 0) {
        *cursor++ = '\n';
    }
    *cursor = delimiter;
    slot.stop = cursor + 1;
    return slot;
}

Slot& PrintHex (byte c, Slot& slot, char delimiter) {
    uint16_t chars = TextByteToUpperCaseHex (c);
    return slot << (char)chars << ((char)(chars >> 8)) << ' ';
}

Slot& PrintHex (uintptr_t value, Slot& slot, char delimiter) {
    // @todo Replace this with faster algorithm.
    char    * begin,
            * start,
            * cursor = slot.stop + 1,
            * end    = slot.end,
            * value_ptr;                //< Pointer to a byte in value.
    char      c;                        //< Temp variable.
    uintptr_t size;                     // Current size we care about.
    uint16_t  hex_value;                //< Hex byte-to-uint16_t temp variable.

    enum { kHexStringLengthSizeMax = sizeof (void*) * 2 + 3 };

    if (cursor < end) {
        size = end - cursor;
        if (size < kHexStringLengthSizeMax) {
            return slot;
        }
        value_ptr = reinterpret_cast<char*> (&value);
        for (int i = sizeof (uintptr_t); i > 0; --i) {
            hex_value = TextByteToUpperCaseHex (*value_ptr++);

        }
        slot.stop = cursor + sprintf_s (cursor, end - cursor, "0x%p", value);
    }

    for (int i = sizeof (void*); i > 0; --i) {

    }
    int bytes_written = sprintf_s (cursor, end - cursor, "0x%p", value);
    cursor += bytes_written;
    if (cursor > end) {
    }
    return slot;
}

Slot& PrintMemory (const void* address, const void* address_end, Slot& slot,
                   char delimiter) {
    slot << "\n " << 0;
    //  columns
    char* start = slot.start,
        * cursor = slot.stop,
        * end = slot.end;

    for (int i = 8; i <= 66; i += 8) {
        PrintRight (i, 8, slot);
    }
    slot << "\n|";
    for (int i = 0; i < 65; ++i) {
        *cursor++ = '_';
        if (cursor > end) {
            cursor = slot.begin;
        }
    }

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < cursor) {
        slot << "\n|";
        for (int i = 0; i < 64; ++i) {
            temp = *chars++;
            if (chars >= cursor)
                temp = 'x';
            *cursor++ = temp;
        }
        slot << "| " << PrintHex (chars + (address_end - address), slot);
    }
    slot << "\n|";
    for (int i = 0; i < 64; ++i) {
        slot << '_';
    }
    return slot << "| " << PrintHex (chars + (address_end - address),
                                     slot);
}

Slot& PrintMemory (const void* address, int size, Slot& slot, char delimiter) {
    return PrintMemory (address, reinterpret_cast<const char*> (address) + size,
                        slot);
}

Slot& PrintError (const char* message, const char* end_string, Slot& slot) {
    return slot << "\nError: " << message << end_string;
}

}       //< namespace _
#endif  //< USING_CRABS_TEXT
#endif  //< CRABS_SEAM >= 1
