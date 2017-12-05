/** kabuki::id
    @file    ~/source/kabuki/id/include/privilage.h
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
 
#ifndef KABUKI_ID_PRIVILAGE_H
#define KABUKI_ID_PRIVILAGE_H

#include "config.h"

namespace kabuki { namespace id {

typedef int Privileges;     //< typedef for an account privilege level.

/** An account privilege level. */
class KABUKI Privilege
{
    public:

    /** Default constructor. */
    Privilege (int num_privileges = 1);

    /** Gets the privilege level. */
    Privileges GetPrivilegeLevel ();

    /** Attempts to set the privilege level to the new level. */
    Privileges SetPrivilegeLevel (Privileges privileges);

    /** Prints this object to a expression. */
    void Print (_::Log& log);

    private:

    int num_privileges;         //< The number of privileges.
    Privileges privileges_;      //< The privilege level.
};
}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_PRIVILAGE_H
