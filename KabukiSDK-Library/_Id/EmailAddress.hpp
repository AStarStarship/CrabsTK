/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/EmailAddress.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <KabukiSDK-Config.hpp>

namespace _Id {

bool isValidEmailAddress (const string& s);
/*< Returns true if this is a valid email address. */

class _KabukiSDK EmailAddress
    /** An email address. */
{
    public:

    EmailAddress (const string& address);
    /*< Default constructor. */

    string& getAddress ();
    /*< Gets the address string. */
    
    void setAddress (const string& s);
    /*< Attempts to set the address to the new string. */

    //string mapToDomain (Match match);
    /*< Maps a domain to the given map??? */
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    string address;     //< The email address.
};

}   //< namespace _Id
