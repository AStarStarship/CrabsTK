/** kabuki::pro
    @file    ~/source/kabuki/id/imp/user.cc
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

#include "../include/user.h"

namespace kabuki { namespace id {

User::User (const char* name, const char* password) :
    name (name),
    password (password)
{
    //activeAccounts = new kabuki::pro.Game.Account.List ();
}

Handle& User::GetUsername () { return name; }

Password& User::GetPassword ()
{
    return password;
}

bool User::Verify (const char* username, const char* aUser)
{
    //return IsValidUsername (username);
    return false;
}

bool User::Equals (const User& user)
{
    return (name.equals (user.name) && password.Equals (user.password));
}

bool User::equals (const char* name)
{
    return name.Equals (name);
}

void User::Print (_::Log& log)
{
    slot.Prints ("User Name: ");
    name.Print (slot);
    slot.Prints (" Password: ");
    password.Print (slot);
}

}       //< id
}       //< kabuki
