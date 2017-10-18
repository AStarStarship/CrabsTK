/** Kabuki Starship
    @file    /.../Source-Impl/_Id/UserList.cpp
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

#include <_Id/UserList.h>

namespace _Id {

UserList::UserList ()
{
}

int UserList::getNumUsers ()
{
    users.size (); 
}

bool UserList::add (const User& newUser)
{
    users.push_back (User (newUser));
    return true;
}

bool UserList::contains (const string& username)
{
    for (int index = 0; index < users.size (); index++)
    {
        if (users[index].equals (username))
            return true;
    }
    return false;
}
    
bool UserList::contains (const User& u)   
{
    for (int index = 0; index < users.size (); index++)
    {
        if (users[index].equals (u))
            return true;
    }
    return false;
}

User* UserList::find (const string& username)
{
    if (username.length () == 0)
    {
        return nullptr;//static website guest account
    }

    for (int index = 0; index < users.size (); index++)
        if (users[index].equals (username))
            return &users[index];
    return nullptr;//static website guest account
}

void UserList::print (Terminal& io)
{
    /// This method creates a string of the users.print (Terminal& io) 
    /// strings separated by spaces

    string returnString = "Number of Users: " + users.size () + "\n";

    for (int index = 0; index < users.size (); index++)
    {
        /// Iterated through the indexs array and write the
        /// print (Terminal& io) strings to the returnString
        returnString += "User " + (index + 1) + ": " + users[index].getUsername ().getHandle () + "\n";
    }
    slot.prints (returnString);
}

}   //  _Id
