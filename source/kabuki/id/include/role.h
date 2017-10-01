/** Kabuki Theater
    @file    /.../Source/_id/Role.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

class ID_API Role
/** A role that an entinty plays in an organization.
    Examples of a role are:
    
    1. volunteer
    2. employee
    3. manager
    4. owner
*/
{
    public:

    Role (const string& aDescription);
    /*< Constructs a user with the given username and password. */

    string& getDescription ();
    /*< Gets a reference to the descritpion string. */

    void setDescription (const string& s);
    /*< Sets the description to the given string. */
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    string description;       //< A description of the role.
};

}   //  _id   
