/** Kabuki Starship
    @file    ~/Source/kabuki/_id/account.h
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

#pragma once

#include "user_list.h"
#include "handle.h"
#include "password.h"
#include "user.h"

namespace _id {

/** An account with a username and password. 
    @todo Integrate with OS accounts.
*/
class ID_API Account
{
	public:
	
    /** Constructs an Account with the given username. */
    Account (const char* username);

    /** Constructs an account from the given username and password. */
    Account (const char* username, const char* password);
    
    /** Returns true if this is a valid account. */
    bool IsValid ();
    
    /** Gets a reference to the name string.  */
    const char* GetName ();
    
    /** Sets the name to the string. */
    bool SetName (const char* s);

    /** Retusn true if this account requires a password. */
    bool RequiresPassword ();
    
    /** Sets the password to the new string. */
    void SetPassword (const char* s);

    /** Gets a reverence to the uesr list. */
    UserList& GetUsers ();

    /** Allows another user to be attached to this account. */
    void AddUser (User& u);

    /*
    bool requestNewAccount (Network.Address request_source, string User_Name, string Password,
        string First_Name, string Last_Name, string Adress1, string Adress2, string Zip_Code);

    bool login (User& u);
    Logs the given user into the account. */

    /** Prints this object to the stdout. */
    void Print ();

    private:
    
    Handle name_;           //< The account name.
    Password password_;     //< The admin password for this account.
    UserList users_;        //< The list of users that may login to this account.
};

}   //< _id
