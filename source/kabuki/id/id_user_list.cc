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

UserList::UserList (Authenticator* authenticator, int max_users) : 
    point_cost_    (0.0),
    authenticator_ (authenticator),
    users_         (max_users) {
    // Nothing to do here ({:->)
}

UserList::~UserList () {
    int count = users_.GetCount ();
    if (count == 0) {
        return;
    }
    for (int i = count - 1; i > 0; --i) {
        delete users_.Pop ();
    }
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

bool UserList::BuyCoins (int session, uint64_t num_coins, double point_cost) {
    User* user = GetUser (session);
    if (!user) {
        return false;
    }
    return users_[session]->BuyCoins (num_coins, point_cost);
}

bool UserList::IncreaseBalance (int session, double amount) {
    User* user = GetUser (session);
    if (!user) {
        return false;
    }
    return users_[session]->IncreaseBalance (amount);
}

Authenticator* UserList::GetAuthenticator () {
    return authenticator_;
}

int UserList::GetSize () { return users_.GetSize (); }

int UserList::GetCount () { return users_.GetCount (); }

int UserList::Register (const char* handle, const char* password) {
    cout << "\n| Attempting to add new User with password:\"" 
         << ((handle == nullptr) ? "null" : handle)
         << "\", password:\"" 
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
        password = StringClone (Password::kDefault);
    }
    if (authenticator_->PasswordIsInvalid (password)) {
        cout << "\n| Invalid password!";
        return -1;
    }
    User* user = new User (authenticator_, users_.GetCount (),
                           handle, password);
    cout << "\n| User added successfully. :-)";
    return users_.Push (user);
}

int UserList::Register (UserList& users) {
    int user_count = users.GetCount (),
        new_count;
    if (user_count == 0) {
        return users_.GetCount ();
    }
    for (int i = 0; i < user_count; ++i) {
        User* user = users.GetUser (i);
        // user should never be nil.
        new_count = Register (user->GetHandle ().GetKey (), 
                     user->GetPassword ().GetKey ());
        if (new_count < 0) {
            return new_count;
        }
    }
    return new_count;
}

int UserList::Find (const char* handle) {
    if (handle == nullptr) {
        return -1;
    }
    cout << "\n| Searching for handle:\"" << handle << '\"';
    //if (*handle == 0) { //< It's an empty string.
    //    return -1;      //  Not sure if I care about this or not.
    //}
    // Currently using sequential search because UserList is not sorted.
    int count = users_.GetCount ();
    if (count == 0) {
        return -1;
    }
    for (int i = count - 1; i >= 0; --i) {
        if (users_[i]->GetHandle ().Equals (handle)) {
            cout << "\n| Found handle at index:" << i;
            return i;
        }
    }

    cout << "\n| Did not find handle.";
    return -1;
}

User* UserList::GetUser (uid_t user_uid) {
    if (user_uid < 0) {
        return nullptr;
    }
    if (user_uid >= users_.GetCount ()) {
        return nullptr;
    }
    return users_[(int)user_uid];
}

int32_t UserList::LogIn (const char* handle, const char* password) {
    return LogIn (Find (handle), password);
}

int32_t UserList::LogIn (int index, const char* password) {
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
        return users_.GetCount () - 1; //< Session id is 
    }
    cout << "\n| Login unsuccessful.";
    return UidServer<>::kInvalidUid;
}

int UserList::Remove (int user_id) {
    User* user = users_.Pop ();
    delete user;
    return users_.GetCount ();
}

void UserList::Print () {
    cout << "\n| UserList: Count:" << users_.GetCount () << " Size:" 
         << users_.GetSize ();

    for (int i = 0; i < users_.GetCount (); i++) {
        cout << "\n| Account " << (i + 1) << ": "
            << users_.Element (i)->GetHandle ().GetKey ();
    }
}

}       //< id
}       //< kabuki
