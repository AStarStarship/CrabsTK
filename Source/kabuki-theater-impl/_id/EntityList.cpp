/** Kabuki Theater
    @file    /.../Source-Impl/_Id/EntityList.cpp
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

#include <_Id/EntityList.hpp>

namespace _Id {

EntityList::EntityList ()
{
}

int EntityList::getSize () { return entities.size (); }

void EntityList::add (Entity* a)
{ 
    entities.push_back (a); 
}

void EntityList::add (EntityList& l)   
{ 
    //entities.push_back (l); 
}

Entity* EntityList::find (const string& a)
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

bool EntityList::search (const string& query)
{
    if (query.length () == 0) return false;

    Entity* entity_ptr;

    for (int i = 0; i < entities.size (); i++)
    {
        entity_ptr = entities[i];
        if (entity_ptr->getFirstName () == query)
            return true;
    }
    return false;//static website guest entities
}

void EntityList::print (Terminal& io)
{
    string returnstring;
    returnstring = "Number of Accounts: " + entities.size () + (char)13;

    for (int i = 0; i < entities.size (); i++)
    {
        // Iterated throught the users array and write the
        // print (Terminal& io) strings to the returnstring
        returnstring = returnstring + ("Account " + (i + 1) + ": " + entities[i].getFirstName () + (char)13);
    }

    slot.prints (returnstring.c_str ());
}

}   //  _Id
