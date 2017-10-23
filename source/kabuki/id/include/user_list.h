/** kabuki::id
    @file    ~/source/kabuki/id/include/user_list.h
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
 
#ifndef KABUKI_ID_USERLIST_H
#define KABUKI_ID_USERLIST_H

#include "user.h"

namespace kabuki { namespace id {

/** A list of registered users.

*/
class KABUKI UserList
{
    public:

    /** Creates an empty user list. */
    UserList ();

    /** Gets the number of users in the list. */
    int GetNumUsers ();

    /** Adds the new user to the list. */
    bool Add (const User& new_user);

    /** Returns true if this list contains the given user query result. */
    bool Contains (const char* string);

    /** Returns true if this list contains the given user. */
    bool Contains (const User& user);
    
    /** Attempts to find a user with the given query. 
        @return Returns a nullptr if this list does not contain the search query. */
    User* Find (const char* query);

    /** Prints this object to a expression. */
    void Print ();
    
    private:
    
    std::vector<User> users;
};
}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_USERLIST_H
