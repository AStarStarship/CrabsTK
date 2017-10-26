/** kabuki::id
    @file    ~/source/kabuki/id/include/handle.h
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

#ifndef KABUKI_ID_HANDLE_H
#define KABUKI_ID_HANDLE_H

#include "module_config.h"
#include "../../script/include/log.h"

namespace kabuki { namespace id {

/** A handle like a char or UID.
*/
class KABUKI Handle
{
    public:

    enum {
        kMinLength = 3,    //< The min length of a Handle range.
        kMaxLength  = 256   //< The max length of a Handle range.
    };
    
    /** Constructor creates a standard char. */
    Handle (const char* name,
            int min_length = kMinLength, 
            int max_length = kMaxLength);

    /** Gets a reference to the handle string. */
    const char* GetKey ();

    /** Gets true if this password is value. */
    bool SetKey (const char* key);

    /** Returns true if the handle is valid. */
    bool IsValid (const char* key);

    /** Returns true if this Handle is identical to the given Handle. */
    bool Equals (const Handle& h);

    /** Prints this object to the log. */
    void Print (_::Log& log);
    
    private:

    const char* key_;  //< The name of the object.

};      //< class Handle
}       //< id
}       //< kabuki
#endif  //< KABUKI_ID_HANDLE_H
