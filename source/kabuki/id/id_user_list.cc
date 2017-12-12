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

namespace kabuki {
namespace id {

UserList::UserList (Authenticator* authenticator, int max_users) :
    point_cost_ (0.0),
    authenticator_ (authenticator),
    users_ () {
    users_.reserve (max_users);
    // Nothing to do here ({:->)
}

UserList::~UserList () {
    int count = GetCount ();
    if (count == 0) {
        return;
    }
    for (int i = count - 1; i > 0; --i) {
        User* user = users_[GetCount () - 1];
        users_.pop_back ();
        delete user;
    }
}

uid_t UserList::PeekNextUid () {
    return uids_.PeekNextUid ();
}

int UserList::GetSize () {
    return users_.max_size ();
}

int UserList::GetCount () {
    return users_.size ();
}

double UserList::GetPointCost () {
    return point_cost_;
}

bool UserList::SetPointCost (double point_cost) {
    if (point_cost < 0.0) {
        return false;
    }
    point_cost_ = point_cost;
    return true;
}

bool UserList::BuyValue (int session, uint64_t num_coins, double point_cost) {
    User* user = GetUser (session);
    if (!user) {
        return false;
    }
    return users_[session]->BuyValue (num_coins, point_cost);
}

bool UserList::AddBalance (int session, double amount) {
    User* user = GetUser (session);
    if (!user) {
        return false;
    }
    return users_[session]->AddBalance (amount);
}

Authenticator* UserList::GetAuthenticator () {
    return authenticator_;
}

int UserList::Register (const char* handle, const char* password,
                        double balance, int64_t value) {
    cout << "\n| Attempting to add @" 
         << ((handle == nullptr) ? "null" : handle)
         << ":\"" 
         << ((password == nullptr) ? "null" : password) << '\"';
    if (handle == nullptr) {
        return -1;
    }
    if (authenticator_->HandleIsInvalid (handle)) {
        cout << "\n| Invalid handle!";
        return -1;
    }
    if (Find (handle) >= 0) {
        cout << "\n| UserList contains handle!";
        return -1;
    }
    if (password == nullptr) {
        password = StrandClone (Password::kDefault);
    }
    if (authenticator_->PasswordIsInvalid (password)) {
        cout << "\n| Invalid password!";
        return -1;
    }
    User* user = new User (authenticator_, uids_.GetNextUid (), 
                           handle, password, balance, value);
    cout << "\n| User added successfully. :-)";
    users_.push_back (user);
    return GetCount ();
}

int UserList::Register (UserList& users) {
    int user_count = users.GetCount (),
        new_count;
    if (user_count == 0) {
        return GetCount ();
    }
    for (int i = 0; i < user_count; ++i) {
        User* user = users.GetUser (i);
        // user should never be nil.
        new_count = Register (user->GetHandle ().GetKey (), 
                     user->GetPassword ().GetKey ());
        if (new_count < 0) {
            std::cout << "\n| Registration failed!";
            return new_count;
        }
    }
    return new_count;
}

int UserList::Add (User* user) {
    if (!user) {
        return -1;
    }
    users_.push_back (user);
    return GetCount () - 1;
}

int UserList::Find (const char* handle) {
    if (handle == nullptr) {
        return -1;
    }
    //if (*handle == 0) { //< It's an empty string.
    //    return -1;      //  Not sure if I care about this or not.
    //}
    // Currently using sequential search because UserList is not sorted.
    size_t count = GetCount ();
    cout << "\n| Searching for handle:\"" << handle << "\" cout:" << count;
    if (count == 0) {
        return -1;
    }
    for (size_t i = 0; i < count; ++i) {
        User* user = users_[i];
        cout << "\n| " << i << ' ';
        user->Print ();
        if (users_[i]->GetHandle ().Equals (handle)) {
            cout << "\n| Found handle at index:" << i;
            return i;
        }
    }

    cout << "\n| Did not find handle.";
    return -1;
}

User* UserList::GetUser (int session) {
    if (session < 0) {
        return nullptr;
    }
    if (session >= GetCount ()) {
        return nullptr;
    }
    return users_[session];
}

int UserList::Unregister (const char* handle, const char* password) {
    int session = Find (handle);
    if (session < 0) {
        return -1;
    }
    if (!GetUser (session)->GetPassword ().Equals (handle)) {
        return -1;
    }
    User* user = users_[session];
    delete user;
    users_.erase (users_.begin () + session);
    return GetCount ();
}

uid_t UserList::LogIn (const char* handle, const char* password) {
    return LogIn (Find (handle), password);
}

uid_t UserList::LogIn (int index, const char* password) {
    cout << "\n| Attempting to log in as " << index << "\"" << password << "\"";
    User* user = GetUser (index);
    if (!user) {
        cout << "\n| nil user!";
        return UidServer<>::kInvalidUid;
    }
    if (password == nullptr) {
        cout << "\n| nil password!";
        return UidServer<>::kInvalidUid;
    } 
    cout << "\n| Comparing credentials \"" << user->GetHandle ().GetKey ()
         << "\":\"" << user->GetPassword ().GetKey () << "\" to \""
         << password << "\"";

    if (user->GetPassword ().Equals (password)) {
        cout << "\n| Login successful :-)";
        //uid_t uid = uids_.GetNextUid ();

        return Random<uid_t> ();
    }
    cout << "\n| Login unsuccessful.";
    return UidServer<>::kInvalidUid;
}

int UserList::Remove (int user_id) {
    User* user = users_.back ();
    users_.pop_back ();
    delete user;
    return GetCount ();
}

void UserList::Print () {
    cout << "\n| UserList: Count:" << GetCount ()
         << " Size:" << users_.capacity ();

    for (int i = 0; i < GetCount (); i++) {
        cout << "\n| Account " << (i + 1) << ":\""
             << users_[i]->GetHandle ().GetKey () << '\"';
    }
}

const char* UserList::HandleText (const char* text,
                                  const char* text_end) {
    // This algorithm's job is to figure out white user to route the message
    // too. Users are not sorted right so we're doing it the slow way.
    // @todo Update to hash table.
    const char* next_token;
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    for (int i = 0; i < GetCount (); ++i) {
        User* user = GetUser (i);
        next_token = TextTokenEquals (text, text_end,
                                      user->GetHandle ().GetKey ());
        if (next_token) {
            return user->HandleText (next_token + 1, text_end);
        }
    }
    return nullptr;
}

const _::Operation* UserList::Star (uint index, _::Expression* expr) {
    static const _::Operation This { "UserList",
        OperationCount (0), OperationFirst ('A'),
        "A session handling user list.", 0 };
    //void* args[1];
    switch (index) {
        case '?': return ExpressionQuery (expr, &This);
    }
    return nullptr;
}

}       //< id
}       //< kabuki
