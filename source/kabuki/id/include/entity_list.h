/** Kabuki Starship
    @file    /.../Source/_id/EntityList.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include "Entity.h"

using namespace std;

namespace _id {

class ID_API EntityList
/*< A List of Entity(s) and EntityGroup(s).
    This is essentially a contact list.
*/
{   
    public:
    
    EntityList ();
    /** A list of Entity (s). */

    int getSize ();
    /** Gets the number of Accounts in the List. */
    
    void add (Entity* e);
    /*< Adds an Entity to the list. */
    
    void add (EntityList& l);
    /*< Adds a list of Entity (s) to the list. */
    
    Entity* find (const string& s);
    /*< Finds an entity in the list by the given search string. */
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */
    
    private:
    
    vector<Entity*> entities;       //< The list of entities.
};

}   //< _id
