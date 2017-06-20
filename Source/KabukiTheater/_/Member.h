/** The Chinese Room
    @version 0.x
    @file    /.../Member.h
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

#ifndef CHINESEROOM_MEMBER_H
#define CHINESEROOM_MEMBER_H

#include "Config.h"
#include "Error.h"

namespace _{ 

template<typename T>
struct Member 
/*< Stores the key/name string, rxHeader, txHeader, and optional description
    string.
    @code
    static const Member member = { "Key", Rx<1, 2>::Header, Tx<1, 2>::Header,
        "Description" };
    static const Member memberObject = { "Key", setNumMembers(0), 0,
        "Description" };
    @endcode
*/
{
    const char* key,        //< The member key.
        description;        //< An optional description of the member.
    const T* rxHeader,      //< The rx format header.
        * txHeader;         //< The tx format header.
};

template<typename T>
inline const Member<T>* readError()
/** Error flag member pointers for throwing Terminal io errors. */
{
    static const Member<T> error = { "readError",
        0,
        0,
        0
    };
    return &error;
}

template<typename T>
const Member<T>* writeError()
/** Error flag member pointers for throwing Terminal io errors. */
{
    static const Member<T> error = { "write error",
        0,
        0,
        0
    };
    return &error;
}

template<typename T>
const Member<T>* deviceStackOverflowError()
/** Error flag member pointers for throwing Terminal io errors. */
{
    static const Member<T> error = { "Device stack overflow error",
        0,
        0,
        0
    };
    return &error;
}

template<typename T>
const Member<T>* invalidMember ()
/*< An error flag for an invalid member index. */
{
    static const Member<T> error = { "Invalid member index",
        0,
        0,
        0
    };
    return &error;
}

template<typename T, const char* key, byte numMember, const char* description>
const Member<T>* deviceHeader ()
/** Creates a static const Member in ROM.
    @code
    const Member<T>* member = createMember<"memberName", RxHeader<STX, UI2>,
        TxHeader<FLT, TMU>, "Description.">
    @endcode
*/
{
    static Member<T> m = { key, getNumMembers, 0, description };
    return &m;
}

template<const char* key, const uint_t* headerIn, const byte* headerOut,
const char* description>
const Member<T>* deviceMember ()
/** Creates a static const Member in ROM.
    @code
    const Member<T>* member = createMember<"memberName", 1, TxHeader<FLT, TMU>,
        "Description.">
    @endcode
*/
{
    static Member<T> m = { key, headerIn, headerOut, description };
    return &m;
}

template<typename T>
inline uint_t* setNumMembers(std::uintptr_t value)
/*< Converts the input value to a uint_t*. */
{
    return reinterpret_cast<T*>(value);
}

template<typename T>
inline std::uintptr_t getNumMembers(const Member<T>* m)
/*< Returns the number of members an IDevice has. */
{
    return m == nullptr ? 0 : reinterpret_cast<uintptr_t>(m->rxHeader);
}

}   //< namespace _

#endif  //< CHINESEROOM_MEMBER_H
