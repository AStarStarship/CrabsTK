/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/UserList.cpp
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

#include <_Id/UserList.hpp>

namespace _Id {

UserList::UserList ()
{
}

int UserList::NumUsers { get { return users.size (); } }

bool UserList::add (User newUser)
{
    if (!newUser.IsValid ()) return false;
    users.add (newUser);
    return true;
}

bool UserList::contains (const char* username)
{
    for (int index = 0; index < users.size (); index++)
    {
        if (users[index].Name == username)
            return true;
    }
    return false;
}

bool UserList::contains (User& U)
{
    for (int index = 0; index < users.size (); index++)
    {
        if (users[index].equals (U))
            return true;
    }
    return false;
}

User UserList::Find (const char* username)
{
    if (username.Length == 0)
    {
        return nullptr;//static website guest account
    }
    for (int index = 0; index < users.size (); index++)
        if (users[index].Name.equals (username))
            return users[index];
    return nullptr;//static website guest account
}

byte UserList::getState ()
{
    return 0;
}

const char* UserList::getState (byte Value)
{
    return 0;
}

const char* UserList::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("UserList", "_Id"): InvalidIndex ();
}

const char* UserList::print (I2P::Terminal& slot)
{
    /// This method creates a string of the users.print (I2P::Terminal& slot) 
    /// strings separated by spaces

    std::string returnString;
    returnString = "Number of Users: " + users.size () + "\n";

    for (int index = 0; index < users.size (); index++)
    {
        /// Iterated through the indexs array and write the
        /// print (I2P::Terminal& slot) strings to the returnString
        returnString += "User " + (index + 1) + ": " + users[index].print (I2P::Terminal& slot) + "\n";
    }

    return returnString;
}

}   //< namespace _Id
