/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/EntityList.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

namespace _Id {

/** A List of Entity  (s) and EntityGroup  (s). */
class _KabukiSDK EntityList
{   
    public:
    
    EntityList ();
    /** A list of Entity  (s). */

    int GetCount ();
    /** Gets the number of Accounts in the List. */
    
    void Add  (Entity a);
    /** Adds an Entity to the list. */
    
    void Add  (List<Entity> a);
    /** Adds a list of Entity  (s) to the list. */
    
    Entity Find  (const char* S);
    /*<  */
    
    bool Contains  (const char* S);
    /*<  */
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    std::vector<Entity> entities;      //< The list of entities.
};
}   //< namespace _Id
}   //< namespace _Search

