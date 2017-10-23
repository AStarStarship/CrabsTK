/** kabuki::id
    @file    ~/source/kabuki/id/include/entity_group.h
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
 
#ifndef KABUKI_ID_ENTITYGROUP_H
#define KABUKI_ID_ENTITYGROUP_H

#include "entity.h"
#include "privilage.h"

namespace kabuki { namespace id {

/** A group of entities.
    @todo 
*/
class KABUKI EntityGroup : public Entity
{
    public:

    /** A group of entities such as people or businesses. */
    EntityGroup (const char* name);

    /** Gets the name of the entity group. */
    char& GetName ();

    /** Sets the name of the entity group. */
    void SetName (const char* string);

    /** Applies privileges to the entity group. */
    void ApplyPrivilage (const Privilage& p);

    /** Returns true if this list of entities contains the given char. */
    virtual int Search (const char* string) override;

    /** Prints this object to a expression. */
    virtual void Print (_::Log& log) override;

    private:

    char name;                      //< The name of the entity group.
    std::vector<Entity*> entities_; //< A vector if Entity pointers.
};

}       //< namespace id
}       //< namespace kabuki
#define //< KABUKI_ID_ENTITYGROUP_H
