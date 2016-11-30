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
using namespace _Id;

Account::Account (const char* aUsername)
{
    // If one calls this method then it is assuming that there is no password for the account.
    username = new Name (aUsername);
    password = nullptr;

    users = new UserList ();
}

Account::Account (const char* aUsername, const char* aPassword)
{
    username = aUsername;
    password = aPassword;

    role = new Role ();

    users = new UserList ();
}

Role Account::getRole ()
{
    return role;
}

bool Account::isValid ()
{
    if (password == nullptr)
    {
        // The the account does not require a password
        if (!username.IsValid ())
            return false;

        return true;
    }
    else // we have to test the password
    if (!username.IsValid () || !password.IsValid ())
        return false;

    return true;
}
const char* Account::getName () { return username; }

int Account::setName (const char* S) { username = S; }

bool Account::requiresPassword ()
{
    if (password == nullptr)
        return false;
    return true;
}

void Account::getPassword (const char* S)
{
    password.change (new_password);
}

vector<User> Account::getUsers ()
{
    return users;
}

void Account::allowUser (User& U)
{
    users->add (U);
}

bool Account::requestNewAccount (Network.Address request_source, const char* User_Name, const char* Password,
    const char* First_Name, const char* Last_Name, const char* Adress1, const char* Adress2, const char* Zip_Code)
{
    if (informationIsValid (user_Name, Password, Confirm_Password,
        First_Name, Last_Name, Adress1, Adress2, Zip_Code))
        return true;
    return false;
}

bool Account::informationIsValid (const char* Username, const char* Password, const char* First_Name, 
    const char* Last_Name, const char* Adress1, const char* Adress2, const char* Zip_Code)
{
    if ()
        return false;
    return true;
}

bool Account::login (User& U)
{
    User aUser;
    aUser = users->find (U.getName ());
    if (!U.equals (aUser))
        return false;
    return true;
}

void Account::print (I2P::Terminal& slot)
{
    print (slot, "Account Name: ", username.getCString () + " Password: " + password.getCString ());
}

}   //< namespace _Id
