/** kabuki::id
    @file    ~/source/kabuki/id/user_hist.h
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
 
#ifndef HEADER_FOR_KABUKI_ID_USERLIST
#define HEADER_FOR_KABUKI_ID_USERLIST

#include "user.h"
#include "../data/array.h"

namespace kabuki { namespace id {

/** A List of entities.
    This is essentially a contact list. */
class KABUKI UserList {   
    public:

    /** Creates an empty list. */
    UserList ();

    /** Gets the number of Accounts in the List. */
    int GetSize ();

    /** Adds an User to the list. */
    void Add (User* entity);

    /** Adds a list of User (string) to the list. */
    void Add (UserList& enities);

    /** Finds an entity in the list by the given search char. */
    User* Find (const char* string);

    /** Prints this object to the log. */
    void Print ();
    
    private:
    
    data::Array<User*> users_; //< The list of users.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_USERLIST
