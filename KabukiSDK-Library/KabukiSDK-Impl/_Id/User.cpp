/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/User.cpp
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

#include <_Id/User.hpp>

namespace _Id {

User::User (const string& aName, const string& aPassoword)
:   name (aName),
    password (aPassword)
{
    //activeAccounts = new Kabuki Software Development Kit.Game.Account.List ();
}

Handle& User::getUsername () { return name; }

Password& User::getPassword ()
{
    return password;
}

bool User::verify (const string& aUsername, const string& aUser)
{
    //return isValidUsername (aUsername);
    return false;
}

bool User::equals (const User& u)
{
    return (name.equals (u.name) && password.equals (u.password));
}

void User::print (Terminal& slot)
{
    slot.print ("User Name: ");
    name.print (slot);
    slot.print (" Password: ");
    password.print (slot);
}

}   //< namespace _Id
