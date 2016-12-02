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

#pragma once

#include "UserList.hpp"
#include "Handle.hpp"
#include "Password.hpp"
#include "User.hpp"

namespace _Id {

class Account
/** An account with a username and password. */
{
	public:
	
    Account (const string& aUsername = "New user");
    /*< Default constructor. */

    Account (const string& aUsername, const string& aPassword);
    /*< Constructs an account from the given username and password. */
    
    bool isValid ();
    /*< Returns true if this is a valid account. */
    
    string& getName ();
    /*< Gets a reference to the name string.  */
    
    bool setName (const string& s);
    /*< Sets the name to the string. */

    bool requiresPassword ();
    /*< Retusn true if this account requires a password. */
    
    void setPassword (const string& s);
    /*< Sets the password to the new string. */

    UserList& getUsers ();
    /*< Gets a reverence to the uesr list. */

    void addUser (User& u);
    /*< Allows another user to be attached to this account. */

    //bool requestNewAccount (Network.Address request_source, string User_Name, string Password,
    //    string First_Name, string Last_Name, string Adress1, string Adress2, string Zip_Code);

    bool login (User& u);
    /*< Logs the given user into the account. */

    void print (Terminal& slot);
    /*< Prints this object to a terminal. */

    private:
    
    Handle name;            //< The account name.
    Password password;      //< The admin password for this account.

    UserList users;         //< The list of users that may login to this account.
};

}   //< namespace _Id
