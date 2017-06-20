/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoomConsole/Address.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
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

#include "Room.h"

namespace _ {

template<const char... N>
struct Address
/*< C++11 constexpr struct for creating parameter headers with less typing. */
{
    //static constexpr size_t length = sizeof...(N);
    static const char Path[sizeof...(N)] = { N... };
    static const char TermChar = 0;;
};

inline uint16_t address (char a)
/*< Returns a pointer to static variable for writing the address { a, \0 } to.
    */
{
    return ((1 << 8) | a);
}

inline uint32_t address (uint32_t a, uint32_t b)
/*< Returns a pointer to static variable for writing the address { a, b, \0, \0
    } to. */
{
    return ((1 << 16) | a | (b << 8));
}

inline uint32_t address (uint32_t a, uint32_t b, uint32_t c)
/*< Returns a pointer to static variable for writing the address { a, b, c, \0 }
    to. */
{
    return ((1 << 24) | a | (b << 8) | (c << 16));
}

inline uint64_t address (uint64_t a, uint64_t b, uint64_t c,
    uint64_t d)
/*< Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
{
    return ((1 << 32) | a | (b << 8) | (c << 16) | (d << 24));
}

inline uint64_t address (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t e)
/*< Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
{
    return ((1 << 40) | a | (b << 8) | (c << 16) | (d << 24) | (e << 32));
}

inline uint64_t address (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t e, uint64_t f)
/* Returns a pointer to static variable for writing the address { a, b, c, d, e,
 *  f, g, \0 } to. */
{
    return ((1 << 48) | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
        (f << 40));
}

inline uint64_t address (uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t e, uint64_t f, uint64_t g)
/*< Returns a pointer to static variable for writing the address { a, b, c, d,
    e, f, g, \0 } to. */
{
    return ((1 << 56) | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
        (f << 40) | (g << 48));
}

const char* get (uint16_t* address)
/*< Generates a temp byte* pointer to the address. */
{
    return reinterpret_cast<const char*>(address);
}

const char* get (uint32_t* address)
/*< Generates a temp byte* pointer to the address. */
{
    return reinterpret_cast<const char*>(address);
}

const char* get (uint64_t* address)
/*< Generates a temp byte* pointer to the address. */
{
    return reinterpret_cast<const char*>(address);
}

}       //< namespace _

#endif  //< CHINESEROOM_ADDRESS_H
