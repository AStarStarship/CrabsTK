/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/set.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef CHINESE_ROOM_SUPERSET_H
#define CHINESE_ROOM_SUPERSET_H

#include "error.h"

namespace _ {

/** Key/name string, params, result, and optional description of a data set.
    @code
    static const Set this_member = { "Key", Rx<1, 2>::Header, Tx<1, 2>::Header,
                                        "Description" };
    static const Set member_device = { "Key", setNumMembers (0), 0,
                                       "Description" };
    @endcode */
struct Superset {
    const char   * name;        //< The set name.
    const uint_t * params,      //< The set parameters. 
                 * result;      //< The set operation result.
    const char   * metadata;    //< Metadata.
};

/** Converts the value to a pointer. */
KABUKI uint_t* NumMembers (std::uintptr_t value);


/** Converts the given value to a pointer. */
KABUKI const uint_t* FirstMember (uint_t value);

/** Returns the number of members an Star has. */
KABUKI uintptr_t Index (const void* ptr);

/** Returns the number of members a Star has. */
KABUKI uintptr_t GetNumMembers (const Superset* m);

/** Error flag member pointers for throwing Terminal io errors. */
KABUKI const Superset* ReadError ();

/** Error flag member pointers for throwing Terminal io errors. */
KABUKI const Superset* WriteError ();

/** Error flag member pointers for throwing Terminal io errors. */
KABUKI const Superset* DeviceStackOverflowError ();

/** An error flag for an invalid member index. */
KABUKI const Superset* InvalidMember ();

/** Prints the given Set to the std::out */
KABUKI void SupersetPrint (const Superset* m);

}   //< namespace _

#endif  //< CHINESE_ROOM_SUPERSET_H


/*
// Creates a Read-only-memory Set for a member without a description.
template<const char * key>
KABUKI const C* DeviceMember () {
    static const C m = { key, 0, 0, 0 };
    return &m;
}

// Creates a Read-only-memory Set for a member without a description.
template<const char * key, byte num_members, const uint_t* params,
    const uint_t* result>
    KABUKI const C* DeviceMember () {
    static const C m = { key, params, result, 0 };
    return &m;
}

// Creates a static const Set in ROM. @code const Set* m = createMember<"memberName", 1, TxHeader<FLT, TMU>, "Description."> @endcode 
template<const char * kKey, const uint_t* kHeaderIn, const char * kHeaderOut,
    const char * kDescription>
    KABUKI const C* DeviceMember () {
    static C m = { kKey, kHeaderIn, kHeaderOut, kDescription };
    return &m;
}

// Creates a Read-only-memory Set for a Star.
template<const char * key, byte num_members, const uint_t* result,
    const char * description>
    KABUKI const C* DeviceMember () {
    static const C m = { key, ConvertNumMembers (num_members), result,
        description };
    return &m;
}

// Creates a Read-only-memory Set for a Star.
template<const char * key, byte num_members, const char * description>
KABUKI const C* MemberDevice () {
    static const C m = { key, ConvertNumMembers (num_members), 0,
        description };
    return &m;
}
*/
