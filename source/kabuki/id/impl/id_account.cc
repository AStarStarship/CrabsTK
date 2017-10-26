/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/account.cc
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
#include "../include/account.h"

namespace kabuki {
namespace id {

Account::Account (const char* username) : 
    name_ (username) {
}

Account::Account (const char* username, const char* password) :
    name_     (username),
    password_ (password) {
    //role = new Role ();
}

bool Account::IsValid () {
    /*
    if (password == nullptr)
    {
        // The account does not require a password
        if (!name.IsValid ())
            return false;

        return true;
    }
    else // we have to test the password
    if (!name.IsValid () || !password.IsValid ())
        return false;
    */
    return true;
}

const char* Account::GetName () 
{ return name_.GetKey (); }

bool Account::SetName (const char* string)
{
    return name_.SetKey (string);
}

bool Account::RequiresPassword () {
    return password_.Equals ("");
}

void Account::SetPassword (const char* string) {
    password_.Change (string);
}

UserList& Account::GetUsers () {
    return users_;
}

void Account::AddUser (User& user) {
    users_.Add (user);
}

bool Account::RequestNewAccount (const char* source   , const char* username,
                                 const char* password , const char* first_name,
                                 const char* last_name, const char* adress1,
                                 const char* adress2  , const char* zip_Code)
{
    if (InformationIsValid (username, password, first_name, last_name,
                            adress1, adress2, zip_Code))
        return true;
    return false;
}

uint_t Account::Login (User& user)
{
    User* user;
    user = users_->Find (user.GetName ());
    if (user == nullptr)
        return false;
    if (!user.Equals (user))
        return false;
    return true;
}

void Account::Print (_::Log& log) {
    log + "Account Name: " 
        + name_.GetKey ()
        + " Password: "
        + password_.GetString ();
}

}       //< id
}       //< kabuki

        /** kabuki::id
        @file    ~/source/kabuki/id/include/account.h
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

/** An account with a char and password.
@todo Integrate with OS accounts.
*/
struct KABUKI Account
{
    Handle name;           //< The account name.
    Password password;     //< The admin password for this account.
    UserList users;        //< The list of users that may login to this account.
};

/** Creates an Account with the given char. */
Account* AccountInit (Account* accout, const char* username);

/** Creates an account from the given char and password. */
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
bool RequestNewAccount (Account* accout,
    const char*char, const char*password,
    const char*first_name, const char*last_name,
    const char*adress1, const char*adress2,
    const char*zip_code);

/** Logs the given user into the account. */
bool AccountLogin (Account* accout, User& user);

/** Prints this object to the log. */
void AccountPrint (Account* accout, _::Log& log);

}       //< namespace id
}       //< namespace kabuki

        /** kabuki::pro
        @file    ~/source/kabuki/id/include/imp/account.cc
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
#include "../include/account.h"

namespace kabuki { namespace id {

void AccountInit (Accout* accout, const char* username) {
    if (account == nullptr)
        return;
    account->name = username;
}

void AccountInit (Accout* accout, const char* username, const char* password) {
    if (account == nullptr)
        return;
    account->name = username;
    account->password = password;
    //role = new Role ();
}

bool AccountIsValid (Accout* accout) {
    if (accout == nullptr)
        return false;
    if (accout->password == nullptr) {
        // The account does not require a password
        if (!NameIsValid (name))
            return false;

        return true;
    } else // we have to test the password
        if (!NameIsValid (name) || !PasswordIsValid (password))
            return false;
    return true;
}

const char* AccountGetName (Accout* accout) {
    if (account == nullptr)
        return nullptr;
    return NameGetHandle (Accout* name);
}

bool AccountSetName (Accout* accout, const char* string) {
    if (account == nullptr)
        return false;
    return NameSetHandle (name, string);
}

bool AccountRequiresPassword (Accout* accout) {
    if (account == nullptr)
        return false;
    return PasswordEquals (password, "");
}

void AccountSetPassword (Accout* accout, const char* string) {
    if (account == nullptr)
        return;
    PasswordChange (password, string);
}

UserList* AccountGetUsers (Accout* accout) {
    if (account == nullptr)
        return nullptr;
    return users;
}

void AccountAddUser (Accout* accout, User& user) {
    UsersAdd (account, user);
}

/*
bool AccountRequestNewAccount (Account* account, const char*source, const char* username, const char* password,
const char* first_name, const char* Last_name, const char* Adress1, const char* Adress2, const char* Zip_Code)
{
    if (informationIsValid (char, Password, confirm_password,
    first_name, Last_name, Adress1, Adress2, Zip_Code))
    return true;
    return false;
}

bool AccountLogin (Account* account, User& user)
{
    User user;
    user = users->find (user.GetName ());
    if (!user.equals (user))
    return false;
    return true;
}*/

void AccountPrint (Accout* accout, _::Log& log) {
    LogPrintLine (log, "Account Name: ");
    LogPrint (log, name);
    LogPrint (log, " Password: ");
    LogPrint (log, password);
}

}       //< id
}       //< kabuki

#endif  //< KABUKI_ID_ACCOUNT_H