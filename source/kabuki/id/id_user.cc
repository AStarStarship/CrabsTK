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

const char User::kDefaultDislpayName[] = "Steve\0";

User::User (Validator* validator, const char* display_name, const char* handle,
            const char* password) :
    handle_    (validator, handle),
    password_  (validator, password),
    validator_ (validator_) {
    if (SetDisplayName (display_name))
        SetDisplayName ("Jo");
}

const char* User::GetDisplayName () {
    return display_name_;
}

const char* User::SetDisplayName (const char* name) {
    if (name == nullptr) {
        return "name can't be nil.";
    }
    display_name_ = StringClone (name);
    return nullptr;
}

Handle& User::GetHandle () { return handle_; }

Password& User::GetPassword () { return password_; }

uid_t User::GetSession () {
    return session_;
}

void User::SetSession (uid_t uid) {
    session_ = uid;
}

uid_t User::GetSessionKey () {
    return session_key_;
}

void User::SetSessionKey (uid_t key) {
    session_key_ = key;
}

bool User::IsValid (const char* dislpay_name, const char* handle,
                    const char* password) {
    if (dislpay_name == nullptr) {
        // @todo Add rules for dislpay_name.
        return false;
    }
    if (!handle_.IsValid (handle))
        return false;
    return password_.IsValid (password);
}

bool User::Equals (const User& user) {
    if (!handle_.Equals (handle_.GetKey ()))
        return false;
    return password_.Equals (user.password_);
}

void User::Print () {
    cout << "\n| User: Handle:\"" << handle_.GetKey () << "\"  Password: \""
         << password_.GetKey () << '\"';
}

}       //< id
}       //< kabuki
