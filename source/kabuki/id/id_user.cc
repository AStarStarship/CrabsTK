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

const double User::kDefaultBalance = 0.0;

User::User (Authenticator* authenticator, uid_t uid, const char* handle,
            const char* password, double balance,
            int64_t value) :
            handle_        (authenticator, handle),
            status_        (StrandClone ("")),
            password_      (authenticator, password),
            authenticator_ (authenticator),
            uid_           (uid),
            session_       (0),
            session_key_   (0),
            balance_       (balance),
            value_         (value),
            slot_          (nullptr) {
}

User::User (const User& other) :
           handle_        (other.handle_),
           status_        (other.status_),
           password_      (other.password_),
           authenticator_ (other.authenticator_),
           session_       (other.session_),
           uid_           (other.uid_),
           session_key_   (other.session_key_),
           response_      (other.response_),
           balance_       (other.balance_),
           value_         (other.value_),
           slot_          (other.slot_) {

}

User& User::operator= (const User& other) {
    handle_        = other.handle_;
    status_        = other.status_;
    password_      = other.password_;
    authenticator_ = other.authenticator_;
    session_       = other.session_;
    uid_           = other.uid_;
    session_key_   = other.session_key_;
    response_      = other.response_;
    balance_       = other.balance_;
    value_         = other.value_;
    slot_          = other.slot_;
    return *this;
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
    status_ = StrandClone (name);
    return nullptr;
}

Handle& User::GetHandle () { return handle_; }

const char* User::GetHandleKey () { return handle_.GetKey (); }

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

bool User::BuyValue (int64_t num_coins, double point_cost) {
    // Right now we're not checking how much money the player has.
    double cost = point_cost * (double)num_coins;

    if (cost > balance_) {
        cout << "\n| Can't buy coins because your ass is broke.";
        return false;
    }
    value_ += num_coins;
    balance_ -= cost;
    return true;
}

bool User::AddBalance (double amount) {
    double balance = balance_;
    balance_ = balance + amount;
    return balance;
}

int64_t User::GetValue () {
    return value_;
}

const char* User::SetValue (int64_t value) {
    value_ = value;
    return nullptr;
}

int64_t User::AddValue (int64_t amount) {
    int64_t value = value_;
    value_ = value + amount;
    return value;
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

const char* User::HandleText (const char* text,
                              const char* text_end) {
    enum {
        kMessageLength = 141,
    };
    const char* next_token;
    double      balance;
    int64_t     value;
    char        input[kMessageLength];

    text = TextSkipSpaces (text, text_end);
    if (!text) {
        return nullptr;
    }
    if (TextTokenCompare (text, text_end, "AddBalance")) {
        next_token = TextRead (text, text_end, balance);
        if (!next_token) {
            return nullptr;
        }
        balance_ += balance;
    } else if (TextTokenCompare (text, text_end, "AddValue")) {
        next_token = TextRead (text, text_end, value);
        if (!next_token) {
            return nullptr;
        }
        value_ += value;
    } else if (TextTokenCompare (text, text_end, "SetStatus")) {
        next_token = TextRead (text, text_end, input, input + kMessageLength);
        if (!next_token) {
            return nullptr;
        }
        SetStatus (input);
    } else if (TextTokenCompare (text, text_end, "SetHandle")) {
        next_token = TextRead (text, text_end, input, input + kMessageLength);
        if (!next_token) {
            return nullptr;
        }
        if (!authenticator_->HandleIsInvalid (input)) {
            return "\n| Error: Password in invalid format!";
        }
        handle_.SetKey (input);
    } else if (TextTokenCompare (text, text_end, "SetPassword")) {
        next_token = TextRead (text, text_end, input, input + 141);
        if (!next_token) {
            return nullptr;
        }
        if (authenticator_->PasswordIsInvalid (input)) {
            return "\n| Error: Password in invalid format!";
        }
        password_.SetKey (input);
    } else if (TextTokenCompare (text, text_end, "Print")) {
        Print ();
        return text;
    }
    
    next_token = TextRead (text, text_end, input, input + kMessageLength);
    if (!next_token) {
        return nullptr;
    }
    return next_token;
}

}       //< id
}       //< kabuki
