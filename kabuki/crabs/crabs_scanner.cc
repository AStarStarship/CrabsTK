/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/crabs_text.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
// Dependencies:
#include "scan.h"
#include "text.h"
// End dependencies.
#if MAJOR_SEAM == 1 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

// Some of the Text functions will be required but not all of them.

#if USING_PRINTER

namespace _ {

/*
const char* Scan (char* string, char* string_end) {
    return Print (string, string_end);
}

const char* Scan (char* string, char* string_end, char delimiter) {
    return Print (string, string_end);
}*/

const char* Scan(int8_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  int extra_copy;
  if (!sscanf_s(string, "%i", &extra_copy)) {
    return nullptr;
  }
  result = (int8_t)extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(uint8_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  uint extra_copy;
  if (!sscanf_s(string, "%u", &extra_copy)) {
    return nullptr;
  }
  result = (uint8_t)extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(int16_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  int extra_copy;
  if (!sscanf_s(string, "%i", &extra_copy)) {
    return nullptr;
  }
  result = (int16_t)extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(uint16_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  uint extra_copy;
  if (!sscanf_s(string, "%u", &extra_copy)) {
    return nullptr;
  }
  result = (uint16_t)extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(int32_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  int extra_copy;
  if (!sscanf_s(string, "%i", &extra_copy)) {
    return nullptr;
  }
  result = (int32_t)extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(uint32_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  uint extra_copy;
  if (!sscanf_s(string, "%u", &extra_copy)) {
    return nullptr;
  }
  result = (uint32_t)extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(int64_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  __int64 extra_copy;
  if (!sscanf_s(string, "%lli", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(uint64_t& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-integer function.
  unsigned __int64 extra_copy;
  if (!sscanf_s(string, "%llu", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(float& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-float function.
  float extra_copy;
  if (!sscanf_s(string, "%f", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(double& result, const char* string, const char* string_end) {
  // @todo Rewrite with custom string-to-float function.
  double extra_copy;
  if (!sscanf_s(string, "%lf", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextNextNonNumber(string, string_end);
}

const char* Scan(int8_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  int extra_copy;
  if (!sscanf_s(string, "%i", &extra_copy)) {
    return nullptr;
  }
  result = (int8_t)extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(uint8_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  uint extra_copy;
  if (!sscanf_s(string, "%u", &extra_copy)) {
    return nullptr;
  }
  result = (uint8_t)extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(int16_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  int extra_copy;
  if (!sscanf_s(string, "%i", &extra_copy)) {
    return nullptr;
  }
  result = (int16_t)extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(uint16_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  uint extra_copy;
  if (!sscanf_s(string, "%u", &extra_copy)) {
    return nullptr;
  }
  result = (uint16_t)extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(int32_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  int extra_copy;
  if (!sscanf_s(string, "%i", &extra_copy)) {
    return nullptr;
  }
  result = (int32_t)extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(uint32_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  uint extra_copy;
  if (!sscanf_s(string, "%u", &extra_copy)) {
    return nullptr;
  }
  result = (uint32_t)extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(int64_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  int64_t extra_copy;
  if (!sscanf_s(string, "%lli", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(uint64_t& result, const char* string) {
  // @todo Rewrite with custom string-to-integer function.
  uint64_t extra_copy;
  if (!sscanf_s(string, "%llu", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(float& result, const char* string) {
  float extra_copy;
  if (!sscanf_s(string, "%f", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextSkipNumbers(string);
}

const char* Scan(double& result, const char* string) {
  float extra_copy;
  if (!sscanf_s(string, "%f", &extra_copy)) {
    return nullptr;
  }
  result = extra_copy;
  return TextSkipNumbers(string);
}

/*
const char* Scan (
                        char* target, char* target_end) {
    string = TextSkipSpaces (string, string_end);
    if (!string) {
        return nullptr;
    }
    if (!target) {
        return nullptr;
    }
    if (target > target_end) {
        return nullptr;
    }

    char c = *string;
    while (c && !IsWhitespace<> (c)) {
        #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
        std::cout << c;
        #endif
        if (++string > string_end) {
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
            std::cout << " but string overflowed!";
            #endif
            return nullptr;
        }
        if (++target > target_end) {
            #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
            std::cout << " but target string overflowed!";
            #endif
            return nullptr;
        }
        *target = c;
        c = *string;
    }
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    std::cout << '\"';
    #endif
    *target = 0;
    return string;

}

const char* Scan (const char* target, char* string,
                         char* string_end, char delimiter) {
    #if MAJOR_SEAM == 1 && MINOR_SEAM == 1
    std::cout << "\nparse_string buffer_size: " << string_end - string
              << " delimiter " << delimiter << "\n";
    #endif
    if (!target) {
        std::cout << "\n!target";
        return nullptr;
    }
    if (!string) {
        std::cout << "\n!string";
        return nullptr;
    }
    if (string > string_end) {
        std::cout << "\ndestination > string_end";
        return nullptr;
    }
    //std::cout << delimiter;
    char c = *target; //< Current char from target.
    while (c > delimiter) {
        if (c <= delimiter) {
            //std::cout << delimiter << " parsed.\n";
            *string = 0;
            if (delimiter == 0) {
                return target;
            }
            return target + 1;
        }
        //std::cout << c;
        if (target > string_end) {
            std::cout << "\nBuffer overflow!";
            return nullptr;
        }
        *string++ = c;
        c = *(++target);
    }
    //std::cout << delimiter << " done.\n";
    *string = 0;
    if (delimiter == 0) {
        return target;
    }
    return target + 1;
}*/

/* Reads a b-sequences from the given slot.
   @depreciated I don't think I need this code anymore because I use
                dictionaries to parse types.
template<typename T>
const char* ScanBsq (const char* input, const char* input_end,
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
        // We will always have at least 2 bytes in the string after any ESC.
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
                                                                   *(input +
4)); case 'M': return TypeCharCompareObject<'A', 'P', MAP> (b, c, d,
                                                                   *(input +
4)); case 'N': return TypeCharCompare<'I', 'L', NIL> (b, c, d); case 'O': switch
(b) { case 'B': return TypeCharCompare<'J', OBJ> (c, d);
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
#endif  //< USING_PRINTER
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 2
