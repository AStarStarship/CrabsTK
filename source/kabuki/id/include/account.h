/** kabuki::id
    @file    ~/source/kabuki/id/account.h
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

#ifndef KABUKI_ID_ACCOUNT_H
#define KABUKI_ID_ACCOUNT_H

#include "user_list.h"
#include "handle.h"
#include "password.h"
#include "user.h"

namespace kabuki { namespace id {

/** An account with a username and password. 
    @todo Integrate with OS accounts.
*/
class KABUKI Account
{
    public:
    
    /** Constructs an Account with the given username. */
    Account (const char* username);

    /** Constructs an account from the given username and password. */
    Account (const char* username, const char* password);
    
    /** Returns true if this is a valid account. */
    bool IsValid ();
    
    /** Gets a reference to the name char.  */
    const char* GetName ();
    
    /** Sets the name to the char. */
    bool SetName (const char* string);

    /** Returns true if this account requires a password. */
    bool RequiresPassword ();
    
    /** Sets the password to the new char. */
    void SetPassword (const char* string);

    /** Gets a reverence to the user list. */
    UserList& GetUsers ();

    /** Allows another user to be attached to this account. */
    void AddUser (User& user);

    /* Requests a new account with the given information. */
    bool RequestNewAccount (IpAddress request_source, char User_Name, char password,
        char first_name, char last_name, char adress1, char adress2, char zip_code);

    /** Logs the given user into the account. */
    bool Login (User& user);

    /** Prints this object to the stdout. */
    void Print ();

    private:
    
    Handle name_;           //< The account name.
    Password password_;     //< The admin password for this account.
    UserList users_;        //< The list of users that may login to this account.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_ACCOUNT_H
        /** kabuki::id
        @file    ~/source/kabuki/id/account.h
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

#ifndef KABUKI_ID_ACCOUNT_H
#define KABUKI_ID_ACCOUNT_H

#include "user_list.h"
#include "handle.h"
#include "password.h"
#include "user.h"
#include "ip_address.h"

namespace kabuki {
namespace id {

/** An account with a username and password.
@todo Integrate with OS accounts.
*/
struct KABUKI Account
{
    Handle name;           //< The account name.
    Password password;     //< The admin password for this account.
    UserList users;        //< The list of users that may login to this account.
};

/** Creates an Account with the given username. */
Account* AccountInit (Account* accout, const char* username);

/** Creates an account from the given username and password. */
Account* AccountInit (Account* accout, const char* username, const char* password);

/** Returns true if this is a valid account. */
bool AccountIsValid (Account* accout);

/** Sets the name to the char. */
bool AccountSetName (Account* accout, const char* string);

/** Returns true if this account requires a password. */
bool AccountRequiresPassword (Account* accout);

/** Sets the password to the new char. */
void AccountSetPassword (Account* accout, const char* string);

/** Allows another user to be attached to this account. */
void AccountAddUser (Account* accout, User& user);

/* Requests a new account with the given information. */
bool RequestNewAccount (Account* accout, IpAddress request_source, char User_Name, char password,
    char first_name, char last_name, char adress1, char adress2, char zip_code);

/** Logs the given user into the account. */
bool AccountLogin (Account* accout, User& user);

/** Prints this object to the stdout. */
void AccountPrint (Account* accout, _::Log& log);

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_ACCOUNT_H
