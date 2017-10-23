/** kabuki::pro
    @file    ~/source/kabuki/id/imp/user_list.cc
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

#include <stdafx.h>
#include "../include/user_list.h"

namespace kabuki { namespace id {

UserList::UserList ()
{
}

int UserList::GetNumUsers ()
{
    users.size (); 
}

bool UserList::Add (const User& new_user)
{
    users.push_back (User (new_user));
    return true;
}

bool UserList::Contains (const char* username)
{
    for (int index = 0; index < users.size (); index++)
    {
        if (users[index].equals (username))
            return true;
    }
    return false;
}
    
bool UserList::Contains (const User& user)   
{
    for (int index = 0; index < users.size (); index++)
    {
        if (users[index].equals (user))
            return true;
    }
    return false;
}

User* UserList::Find (const char* username)
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

void UserList::Print (_::Log& log)
{
    /// This method creates a char of the users.Print (_::Log& log) 
    /// strings separated by spaces

    char returnString = "Number of Users: " + users.size () + "\n";

    for (int index = 0; index < users.size (); index++)
    {
        /// Iterated through the indexes array and write the
        /// Print (_::Log& log) strings to the returnString
        returnString += "User " + (index + 1) + ": " + users[index].GetUsername ().GetHandle () + "\n";
    }
    Print (logs (returnString);
}

}       //< id
}       //< kabuki
