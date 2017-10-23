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

#include "id_config.h"

namespace kabuki { namespace id {

typedef int PrivilageLevel;     //< typedef for an account privilege level.

/** The default number of privilege levels is 5, because CPUs are fastest at 
    working with the number -1, 0, and 1. */

enum {
    DefaultMinPrivilageLevel = -2,  //< Default min privilege level optimized for -1, 0, 1 numbers.
    DefaultMaxPrivilageLevel = 2    //< Default mix privilege level optimized for -1, 0, 1 numbers.
};

/** Gets the static min privilege level. */
PrivilageLevel GetMinPrivilageLevel ();

/** Gets the static min privilege level. */
PrivilageLevel SetMinPrivilageLevel ();

/** Attempts to set the privilege level range to the given values.
    @warning Algorithm will set min and max to default values if they are not in 
             order min to max!. */
bool SetPrvilageLevelRange (PrivilageLevel min, PrivilageLevel max);

/** An account privilege level. */
class KABUKI Privilage
{
    public:

    /** Default constructor. */
    Privilage ();

    /** Gets the privilege level. */
    PrivilageLevel GetPrivilageLevel ();

    /** Attempts to set the privilege level to the new level. */
    PrivilageLevel SetPrivilageLevel (PrivilageLevel level);

    /** Prints this object to a expression. */
    void Print (_::Log& log);

    private:

    PrivilageLevel privilageLevel;      //< The privilege level.
};
}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_PRIVILAGE_H
