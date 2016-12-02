/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/UserList.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once

#include "User.hpp"

namespace _Id {

class _KabukiSDK UserList
/**  */
{
    public:
    
	UserList ();
	/*< Constructs an empty user list. */
    
	int getNumUsers ();
	/*< Gets the number of users in the list. */
    
	bool add (const User& newUser);
	/*< Adds the new user to the list. */
    
	bool contains (const string& s);
	/*< Returns true if this list contains the given user query result. */
    
	bool contains (const User& u);
    /*< Returns true if this list contains the given user. */
    
	User* find (string username);
	/*< Attemps to find a user by username. 
        @return Returns a nullptr if this list does not contain the search query. */
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */
	
    private:
	
	vector<User> users;
};

}   //< namespace _Id
