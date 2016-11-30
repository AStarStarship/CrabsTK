/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/EntityList.cpp
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

#include <_Id/EntityList.hpp>

namespace _Id {

EntityList::EntityList ()
{
    entities = new List<Entity> ();
}

int EntityList::getSize () { return entities.size (); }

void EntityList::add (Entity a)         { entities.add (a); }

void EntityList::add (List<Entity> a)   { entities.AddRange (a); }

Entity EntityList::find (string a)
{
    if (a.Length == 0)
    {
        return nullptr;//static website guest entities
    }
    for (int i = 0; i < entities.size (); i++)
        if (strcmp (a, entities[i].getName ()) == 0)
            return entities[i];
        
    return nullptr;//static website guest entities
}

bool EntityList::Contains (string a)
{
    if (a.Length == 0 || a == nullptr) return false;

    for (int i = 0; i < entities.size (); i++)
        if (entities[i].Name == a)
            return true;

    return false;//static website guest entities
}

byte EntityList::getState ()
{
    return 0;
}

const char* EntityList::getState (byte Value)
{
    return 0;
}

const char* EntityList::op (I2P::Terminal& slot, int index, int Enq)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("EntityList", "_Id"): InvalidIndex ();
}

const char* EntityList::print (I2P::Terminal& slot)
{
    string returnstring;
    returnstring = "Number of Accounts: " + entities.size () + (char)13;

    for (int i = 0; i < entities.size (); i++)
    {
        // Iterated throught the users array and write the
        // print (I2P::Terminal& slot) strings to the returnstring
        returnstring = returnstring + ("Account " + (i + 1) + ": " + entities[i].print (I2P::Terminal& slot) + (char)13);
    }

    return returnstring.c_str ();
}

}   //< namespace _Id
