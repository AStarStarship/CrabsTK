/** kabuki::pro
    @file    ~/source/kabuki/id/imp/account.cc
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

char& Account::GetName () 
{ return _name.GetHandle (); }

bool Account::SetName (const char* string)
{
    return name_.SetHandle (string);
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

/*
bool Account::requestNewAccount (Network.Address request_source, char username, char password,
    char first_name, char Last_name, char Adress1, char Adress2, char Zip_Code)
{
    if (informationIsValid (userName, Password, Confirm_Password,
        first_name, Last_name, Adress1, Adress2, Zip_Code))
        return true;
    return false;
}

bool Account::login (User& user)
{
    User aUser;
    aUser = users->find (user.getName ());
    if (!user.equals (aUser))
        return false;
    return true;
}*/

void Account::Print (_::Log& log) {
    expr.PrintLine ("Account Name: ");
    expr.Print (name);
    expr.Print (" Password: ");
    password.Print (expr);
}

}       //< id
}       //< kabuki
        /** kabuki::pro
        @file    ~/source/kabuki/id/imp/account.cc
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
bool AccountRequestNewAccount (Accout* accout, Network.Address request_source, char username, char password,
char first_name, char Last_name, char Adress1, char Adress2, char Zip_Code)
{
    if (informationIsValid (userName, Password, Confirm_Password,
    first_name, Last_name, Adress1, Adress2, Zip_Code))
    return true;
    return false;
}

bool AccountLogin (Accout* accout, User& user)
{
    User aUser;
    aUser = users->find (user.getName ());
    if (!user.equals (aUser))
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
