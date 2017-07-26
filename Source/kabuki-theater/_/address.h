/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoomConsole/Address.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_ADDRESS_H
#define CHINESEROOM_ADDRESS_H

namespace _ {
    
/** C++11 constexpr struct for creating parameter headers with less typing.
template<const byte... N>
struct Address {
    static const byte path[sizeof... (N)] = { N... };
    static const byte term_char = 0;
    //static constexpr size_t length = sizeof... (N);
}; */

template<const byte... N>
const byte* Address () {
    static const byte path[sizeof... (N)] = { N... };
    static const byte term_char = 0;
    return path;
}

/** Returns a pointer to static variable for writing the address { a, \0 } to.
*/
inline uint16_t PackAddress (byte a) {
    return ((1 << 8) | a);
}

/** Returns a pointer to static variable for writing the address { a, b, \0, \0
    } to. */
inline uint32_t PackAddress (uint32_t a, uint32_t b) {
    return ((1 << 16) | a | (b << 8));
}

/** Returns a pointer to static variable for writing the address { a, b, c, \0 }
to. */
inline uint32_t PackAddress (uint32_t a, uint32_t b, uint32_t c) {
    return ((1 << 24) | a | (b << 8) | (c << 16));
}

/** Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
inline uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c,
    uint64_t d) {
    uint64_t one = 1;
    return (one << 32 | a | (b << 8) | (c << 16) | (d << 24));
}

/** Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
inline uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t e) {
    uint64_t one = 1;
    return (one << 40 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32));
}

/* Returns a pointer to static variable for writing the address { a, b, c, d, e,
   f, g, \0 } to. */
inline uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t e, uint64_t f) {
    uint64_t one = 1;
    return (one << 48 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
           (f << 40));
}

/** Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
inline uint64_t PackAddress (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t e, uint64_t f, uint64_t g) {
    uint64_t one = 1;
    return (one << 56 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
            (f << 40) | (g << 48));
}

}       //< namespace _
#endif  //< CHINESEROOM_ADDRESS_H
