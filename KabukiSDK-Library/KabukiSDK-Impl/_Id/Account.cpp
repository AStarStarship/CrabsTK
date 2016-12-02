/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Account.hpp
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

#include "_Id/Account.hpp"

namespace _Id {

Account::Account (const string& aUsername)
:   name (aUsername)
{
}

Account::Account (const string& aUsername, const string& aPassword)
:   name     (aUsername),
    password (aPassword)
{
    //role = new Role ();
}

bool Account::isValid ()
{
    /*
    if (password == nullptr)
    {
        // The the account does not require a password
        if (!name.isValid ())
            return false;

        return true;
    }
    else // we have to test the password
    if (!name.isValid () || !password.isValid ())
        return false;
    */
    return true;
}

string& Account::getName () { return name.getHandle (); }

bool Account::setName (const string& s) { return name.setHandle (s); }

bool Account::requiresPassword ()
{
    return password.equals ("password");
}

void Account::setPassword (const string& s)
{
    password.change (s);
}

UserList& Account::getUsers ()
{
    return users;
}

void Account::addUser (User& u)
{
    users.add (u);
}

/*
bool Account::requestNewAccount (Network.Address request_source, string User_Name, string Password,
    string First_Name, string Last_Name, string Adress1, string Adress2, string Zip_Code)
{
    if (informationIsValid (user_Name, Password, Confirm_Password,
        First_Name, Last_Name, Adress1, Adress2, Zip_Code))
        return true;
    return false;
}

bool Account::login (User& u)
{
    User aUser;
    aUser = users->find (u.getName ());
    if (!u.equals (aUser))
        return false;
    return true;
}*/

void Account::print (Terminal& slot)
{
    slot.print ("Account Name: ");
    name.print (slot);
    slot.print (" Password: ");
    password.print (slot);
}

}   //< namespace _Id
