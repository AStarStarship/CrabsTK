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

User::User (const char* aName, const char* APassoword)
{
    if (IsValid (AUser))
        return;

    username = AUsername;
    password = AUser;

    //activeAccounts = new Kabuki Software Development Kit.Game.Account.List ();
}

string User::getName { return username; }

string User::getUser ()
{
    return password;
}

string User::EncryptUser ()
{
    return "";
}

bool User::IsValid ()
{
    if (username == nullptr || username == "")
        return false;
    return true;
}

bool User::Verify (const char* aUsername, const char* aUser)
{
    return username == aUsername;
}

bool User::Login (Account& thisAccount)
{
    if (!thisAccount.Users ().contains (this))
        return false;
    activeAccounts.add (thisAccount);
    return true;
}

bool User::equals (User& aUser)
{
    if (username != aUser.Name || password != aUser.password) //< The aUser is not this user
        return false;
    return true;
}

byte User::getState ()
{
    return 0;
}

const char* User::getState (byte Value)
{
    return 0;
}

const char* User::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("User", "_Id"): InvalidIndex ();
}

const char* User::print (I2P::Terminal& slot)
{
    std::string returnString = "User Name: " + username + " User: " + password;
    return returnString.c_str ();
}

}   //< namespace _Id
