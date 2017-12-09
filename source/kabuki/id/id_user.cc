/** kabuki::pro
    @file    ~/source/kabuki/id/user.cc
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

const double User::kDefaultBalance = 0.0;

User::User (Authenticator* authenticator, uid_t uid, const char* handle,
            const char* password, const char* status, double balance,
            uint64_t value) :
            handle_        (authenticator, handle),
            status_        (StringClone ("")),
            password_      (authenticator, password),
            authenticator_ (authenticator),
            uid_           (uid),
            session_       (0),
            session_key_   (0),
            balance_       (balance),
            value_         (value),
            slot_          (nullptr) {
    if (SetStatus (status)) {
        SetStatus ("");
    }
}

User::~User () {

}

const char* User::GetStatus () {
    return status_;
}

const char* User::SetStatus (const char* name) {
    if (name == nullptr) {
        return "name can't be nil.";
    }
    status_ = StringClone (name);
    return nullptr;
}

Handle& User::GetHandle () { return handle_; }

Password& User::GetPassword () { return password_; }

uid_t User::GetUid () {
    return uid_;
}

int32_t User::GetSession () {
    return session_;
}

const char* User::SetSession (int32_t session) {
    if (session == 0)
        return "Invalid session key";
    session_ = session;
    return nullptr;
}

uid_t User::GetSessionKey () {
    return session_key_;
}

uid_t User::GetResponse () {
    return response_;
}

const char* User::SetResponse (uid_t response) {
    response_ = response;
    return nullptr;
}

double User::GetBalance () {
    return balance_;
}

const char* User::SetBalance (double balance) {
    balance_ = balance;
    return nullptr;
}

bool User::BuyCoins (uint64_t num_coins, double point_cost) {
    // Right now we're not checking how much money the player has.
    double cost = point_cost * (double)num_coins;

    if (cost > balance_) {
        cout << "\n| Can't buy coins because your ass is broke.";
        return false;
    }
    value_ += num_coins;
    balance_ -= cost;
}

bool User::IncreaseBalance (double amount) {
    if (amount < 0.0) {
        return false;
    }
    balance_ += amount;
    return true;
}

uint64_t User::GetValue () {
    return value_;
}

const char* User::SetValue (uint64_t value) {
    value_ = value;
    return nullptr;
}

bool User::Equals (User* user) {
    if (user == nullptr) {
        return false;
    }
    if (!handle_.Equals (handle_.GetKey ())) {
        return false;
    }
    return password_.Equals (user->password_);
}

Expression* User::GetSlot () {
    return slot_;
}

Window* User::GetWindow () {
    return nullptr;
}

bool User::IsAuthentic (int32_t session, uid_t session_key) {
    if (session_ != session) {
        return false;
    }
    if (session_key_ != session_key) {
        return false;
    }
    return true;
}

void User::Print () {
    cout << "\n| User: Handle:\"" << handle_.GetKey () << "\"  Password: \""
         << password_.GetKey () << '\"';
}

}       //< id
}       //< kabuki
