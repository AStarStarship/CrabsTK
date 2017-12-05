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

UserList::UserList () {
}

int UserList::GetSize () { return users_.GetSize (); }

void UserList::Add (User* a) {
    users_.Push (a);
}

void UserList::Add (UserList& user_list) {
    users_.Push (user_list.users_);
}

User* UserList::Find (const char* query) {
    size_t length = strlen (query);
    if (length == 0)
        return nullptr;

    // Currently using sequential search because UserList is not sorted.

    User* ptr;

    for (int i = 0; i < users_.GetSize (); i++) {
        ptr = users_.Element (i);
        if (ptr->Equals (query))
            return ptr;
    }

    return nullptr;//static website guest entities
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
