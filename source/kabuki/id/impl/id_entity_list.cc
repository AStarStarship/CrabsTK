/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/entity_list.cc
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
#include "../include/entity_list.h"

namespace kabuki { namespace id {

EntityList::EntityList ()
{
}

int EntityList::GetSize () { return entities.size (); }

void EntityList::Add (Entity* a)
{ 
    entities.push_back (a); 
}

void EntityList::Add (EntityList& l)   
{ 
    //entities.push_back (l); 
}

Entity* EntityList::Find (const char* a)
{
    if (a.length () == 0)
        return nullptr;
    
    Entity* ptr;

    for (int i = 0; i < entities.size (); i++)
    {
        ptr = entities[i];
        if (ptr->equals (a))
            return ptr;
    }
        
    return nullptr;//static website guest entities
}

bool EntityList::Search (const char* query)
{
    if (query.length () == 0) return false;

    Entity* entity_ptr;

    for (int i = 0; i < entities.size (); i++)
    {
        entity_ptr = entities[i];
        if (entity_ptr->GetFirstName () == query)
            return true;
    }
    return false;//static website guest entities
}

void EntityList::Print (_::Log& log)
{
    char returnstring;
    returnstring = "Number of Accounts: " + entities.size () + (char)13;

    for (int i = 0; i < entities.size (); i++)
    {
        // Iterated through the users array and write the
        // Print (_::Log& log) strings to the return string
        returnstring = returnstring + ("Account " + (i + 1) + ": " + entities[i].GetFirstName () + (char)13);
    }

    log + returnstring_;
}

}       //< id
}       //< kabuki
