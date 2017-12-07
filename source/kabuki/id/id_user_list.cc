/** kabuki::pro
    @file    ~/source/kabuki/id/user_list.cc
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
#include "user_list.h"

using namespace _;
using namespace std;

namespace kabuki { namespace id {

UserList::UserList (int max_users): 
    users_ (max_users) {
    // Nothing to do here ({:->)
}

UserList::~UserList () {
    for (int i = users_.GetCount (); i > 0; --i) {
        delete users_.Pop ();
    }
}

int UserList::GetSize () { return users_.GetSize (); }

int UserList::GetCount () { return users_.GetCount (); }

int UserList::Add (const char* handle, const char* password) {
    if (handle == nullptr) {
        handle = StringClone (Handle::kDefault);
    }
    if (Find (handle)) {
        return -1;
    }
    if (password == nullptr) {
        password = StringClone (Password::kDefault);
    }
    return users_.Push (new User (this, handle, password));
}

int UserList::Add (UserList* user_list) {
    int count;
    for (int i = 0; i < user_list.GetCount (); ++i) {
        User* user = user_list.UserNumber (i);
        // user should never be nil.
        count = Add (user->GetHandle ().GetKey (), user->GetPassword ().GetKey ());
        if (count < 0) {
            return count;
        }
    }
    return count;
}

User* UserList::Find (const char* handle) {
    if (handle == nullptr) {
        return nullptr;
    }
    size_t length = strlen (handle);
    if (length == 0)
        return nullptr;

    // Currently using sequential search because UserList is not sorted.

    User* user;

    for (int i = users_.GetSize () - 1; i >= 0; --i) {
        user = users_.Element (i);
        if (user->GetHandle ().Equals (handle))
            return user;
    }

    return nullptr;//static website guest entities
}

User* UserList::UserNumber (int user_number) {
    if (user_number < 0) {
        return nullptr;
    }
    if (user_number >= users_.GetCount ()) {
        return nullptr;
    }
    return users_[user_number];
}

const char* UserList::IsValid (const char* input, int type) {
    if (input == nullptr) {
        return "nil input";
    }
    int length = StringLength (input);
    switch (type) {
        case kPasswordValidation: {
            if (length < kMinPasswordLength) {
                return "Password too short";
            }
            if (length > kMaxPasswordLength) {
                return "password too long";
            }
            for (int i = 0; i < length; ++length) {
                if (isspace (input[i])) {
                    return "password can't contain whitespace.";
                }
            }
            return nullptr;
        }
        case kHandleValidation: {
            if (length < kMinHandleLength) {
                return "Password too short";
            }
            if (length > kMaxHandleLength) {
                return "password too long";
            }
            for (int i = 0; i < length; ++length) {
                if (isspace (input[i])) {
                    return "password can't contain whitespace.";
                }
            }
            return nullptr;
        }
    }
}

void UserList::Print () {
    cout << "Number of Accounts: " << users_.GetCount () << (char)13;

    for (int i = 0; i < users_.GetSize (); i++) {
        cout << "Account " << (i + 1) << ": "
            << users_.Element (i)->GetHandle ().GetKey () << (char)13;
    }
}

}       //< id
}       //< kabuki
