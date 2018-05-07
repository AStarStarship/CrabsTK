/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/crabs_utils.cc
    @author  Cale McCollough <https://calemccollough.github.io>
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
#include "memory.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#endif

namespace _ {

uint32_t AlignPowerOf2 (uint32_t value) {
   // @cite https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    if (value < 0) {
        return 4;
    }
    // 
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;
    return value;
}

int32_t AlignPowerOf2 (int32_t value) {
    if (value < 0) {
        return 4;
    }
    uint32_t v = (uint32_t)value;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return (int32_t)value;
}

}       //< namespace _
#undef PRINTF
#undef PUTCHAR
#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 1
