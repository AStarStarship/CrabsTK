/** kabuki::id
    @file    ~/source/kabuki/id/include/user.h
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
 
#ifndef KABUKI_ID_USER_H
#define KABUKI_ID_USER_H

#include "handle.h"
#include "password.h"

namespace kabuki { namespace id {

/** A user account on a computer.
    A user does not necessarily have an account. Some accounts are and any user can use. i.e. the guest account.
*/
class KABUKI User
{
    public:

    /** Creates a user with the given username and password. */
    User (const char* username, const char* password);

    /** Gets a reference to the username handle. */
    Handle& GetUsername ();

    /** Gets a reference to the password. */
    Password& GetPassword ();

    /** Checks to see if the given username and password are in the correct format. */
    bool Verify (const char* username, const char* password);

    /** Returns true if this user is the same as the given one.  */
    bool Equals (const User& user);

    /** Returns true if this username is the same as the given one.  */
    bool equals (const char* name);

    /** Prints this object to a expression. */
    void Print (_::Log& log);

    private:

    Handle username;      	//< The username.
    Password password;      //< The user's password.
};
}       //< namespace id
}       //< kabuki
#endif  //< KABUKI_ID_USER_H
