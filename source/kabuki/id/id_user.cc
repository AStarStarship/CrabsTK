/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/user.cc
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
#include "user.h"

using namespace _;
using namespace std;

namespace kabuki { namespace id {

User::User (const char* name, const char* password) :
    handle_ (name),
    password_ (password) {
    //activeAccounts = new kabuki::pro.Game.Account.List ();
}

const char* User::GetUsername () {
    return handle_.GetKey ();
}

Handle& User::GetHandle () { return handle_; }

Password& User::GetPassword () { return password_; }

bool User::Verify (const char* username, const char* user) {
    //return IsValidUsername (char);
    return false;
}

bool User::Equals (const User& user) {
    if (!handle_.Equals (handle_.GetKey ()))
        return false;
    return password_.Equals (user.password_);
}

bool User::Equals (const char* name) {
    return handle_.Equals (name);
}

void User::Print () {
    cout << "\n| Username: " << handle_.GetKey () << "  Password: "
        << password_.GetKey ();
}

}       //< id
}       //< kabuki
