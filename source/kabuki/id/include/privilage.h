/** Kabuki Starship
    @file    /.../Source/_id/Privilage.h
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
 
#pragma once

#include <KabukiTheater-Config.h>

namespace _id {

typedef int PrivilageLevel;     //< typedef for an account privilage level.

/** The default number of privilage levels is 5, because CPUs are fastest at 
    working with the number -1, 0, and 1. */

enum {
    DefaultMinPrivilageLevel = -2,  //< Default min privilage level optimized for -1, 0, 1 numbers.
    DefaultMaxPrivilageLevel = 2    //< Default mix privilage level optimized for -1, 0, 1 numbers.
};

PrivilageLevel getMinPrivilageLevel ();
/*< Gets the static min privilage level. */

PrivilageLevel setMinPrivilageLevel ();
/*< Gets the static min privilage level. */

bool setPrvilageLevelRange (PrivilageLevel min, PrivilageLevel max);
/*< Attmpts to set the privilage level range to the given values.
    @warning Algorihm will set min and max to default values if they are not in 
        order min to max!. */

class ID_API Privilage
/*< An account privilage level. */
{
    public:

    Privilage ();
    /*< Default constructor. */

    PrivilageLevel getPrivilageLevel ();
    /*< Gest the priliage level. */

    PrivilageLevel setPrivilageLevel (PrivilageLevel l);
    /*< Attempts to set the privilage level to the new level. */
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    PrivilageLevel privilageLevel;      //< The privilage level.
};
}   //  _id

