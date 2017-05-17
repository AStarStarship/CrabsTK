/** Kabuki Theater
    @file    /.../KabukiTheater/_Id/User.hpp
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

#include "Handle.hpp"
#include "Password.hpp"

namespace _Id {

class _KabukiTheater User
/*< A user account on a computer.
    A user does not necessarily have an account. Some accounts are and any user can use. i.e. the guest account.
*/
{
    public:

    User (const string& aUsername, const string& aPassword);
    /*< Constructs a user with the given username and password. */

    Handle& getUsername ();
    /*< Gets a reference to the username handle. */

    Password& getPassword ();
    /*< Gets a reference to the password. */

    bool verify (const string& aUsername, const string& aPassword);
    /*< Checks to see if the given username and password are in the corrent format. */

    bool equals (const User& u);
    /*< Returns true if this user is the same as the given one.  */

    bool equals (const string& aName);
    /*< Returns true if this username is the same as the given one.  */
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    Handle name;            //< The username.
    Password password;      //< The user's password.
};
}   //  _Id


