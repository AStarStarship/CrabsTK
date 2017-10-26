/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/user_list.cc
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
    users_.GetCount (); 
}

bool UserList::Add (const User& new_user)
{
    users_->Push (User (new_user));
    return true;
}

bool UserList::Contains (const char* username)
{
    for (int index = 0; index < users_.GetCount (); index++)
    {
        if (users_[index]->Equals (username))
            return true;
    }
    return false;
}
    
bool UserList::Contains (const User& user)   
{
    for (int index = 0; index < users_.GetCount (); index++)
    {
        if (users_[index]->Equals (user))
            return true;
    }
    return false;
}

User* UserList::Find (const char* username)
{
    size_t length = strlen (username);
    if (length == 0)
    {
        return nullptr;//static website guest account
    }

    for (int index = 0; index < users_.GetCount (); index++)
        if (users_[index]->Equals (username))
            return &users_[index];
    return nullptr;//static website guest account
}

int UserList::GetCount () {
    return users_->GetCount ();
}

void UserList::Print (_::Log& log)
{
    /// This method creates a char of the users.Print (_::Log& log) 
    /// strings separated by spaces

    log += "Number of Users: " + users_.GetCount () + "\n";

    for (int index = 0; index < users_.GetCount (); index++)
    {
        /// Iterated through the indexes array and write the
        /// Print (_::Log& log) strings to the returnString
        log += "User " + (index + 1) + ": " + users_[index]->GetUsername ().GetKey () + "\n";
    }
}

}       //< id
}       //< kabuki
