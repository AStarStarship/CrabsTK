/** Kabuki Theater
    @file    /.../KabukiTheater/_Id/EntityGroup.hpp
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
#include "Privilage.hpp"

namespace _Id {

class _KabukiTheater EntityGroup : public Entity
/*< A group of entities.
    @todo 
*/
{
    public:

    EntityGroup (const string& aName);
    /*< A group of entities such as people or businesses. */

    string& getName ();
    /*< Gets the name of the entity group. */

    void setName (const string& s);
    /*< Sets the name of the entity group. */

    void applyPrivilage (const Privilage& p);
    /*< Applies privilages to the entity group. */
    
    virtual int search (const string& s) override;
    /*< Returns true if this list of entities contains the given string. */ 
    
    virtual void print (Terminal& io) override;
    /*< Prints this object to a terminal. */

    private:

    string name;                 //< The name of the entity group.

    vector<Entity*> entities;    //< A vector if Entity pointers.
};

}   //< _Id
