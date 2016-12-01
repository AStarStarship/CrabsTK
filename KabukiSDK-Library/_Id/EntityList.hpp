/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/_Id/EntityList.hpp
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

#include "Entity.hpp"

namespace _Id {

class _KabukiSDK EntityList
/*< A List of Entity(s) and EntityGroup(s). */
{   
    public:
    
    EntityList ();
    /** A list of Entity (s). */

    int getCount ();
    /** Gets the number of Accounts in the List. */
    
    void add (const Entity& e);
    /*< Adds an Entity to the list. */
    
    void add (std::vector<Entity> e);
    /*< Adds a list of Entity (s) to the list. */
    
    Entity find (const char* s);
    /*< Finds an entity in the list by the given search string. */
    
    bool contains (const char* s);
    /*< Returns true if this list of entities contains the given string. */
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    std::vector<Entity> entities;      //< The list of entities.
};

}   //< namespace _Id
