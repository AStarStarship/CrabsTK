/** Kabuki Theater
    @file    /.../Source-Impl/_id/Handle.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#ifndef KABUKI_THEATER_ID_HANDLE_H
#define KABUKI_THEATER_ID_HANDLE_H

#include <config.h>

namespace _id {

/** Gets the min handle/username/ length. */
int MinHandleLength ();

/** Gets the max handle/username/ length. */
int MaxHandleLength ();

/** Sets the min and max hanlde legnth to the given values. */
void SetHandleLengthRange (int min, int max);

/** A handle like a username or uid.
*/
class ID_API Handle
{
    public:

    enum {
        kMinLengthRange = 3,    //< The min length of a Handle range.
        MaxLengthRange  = 256   //< The max length of a Handle range.
    };
    
    /** Constructor creates a standard username. */
    Handle (const char* name, int min_length = kMinLengthRange, 
            int aMaxLength = kMaxLengthRange);

    /** Gets a reference to the handle string. */
    const char* GetHandle ();

    /** Gets true if this password is value. */
    bool SetHandle (const char* s);

    /** Returns true if the handle is valid. */
    bool IsValid (const char* s);

    /** Returns true if this Handle is idential to the given Handle. */
    bool Equals (const Handle& h);

    /** Prints this object to the stdout. */
    void Print ();
    
    private:

    const char* name_;  //< The name of the object.
};      //< class Handle
}       //< _id
#endif  //< KABUKI_THEATER_ID_HANDLE_H
