/** Kabuki Theater
    @file    /.../Source-Impl/_Id/EntityGroup.cpp
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

#include <_Id/EntityGroup.h>

namespace _Id {

EntityGroup::EntityGroup (const string& aName)
:   name (aName)
{
}

string& EntityGroup::getName () { return name; }

void EntityGroup::setName (const string& s)
{
    name = s;
}

void EntityGroup::applyPrivilage (const Privilage& p)
{
    //for (int i = 0; i < base.getNumAccounts (); i++)
    //    accounts[i].Role ().applyPrivilages (new_privilages);
}

int EntityGroup::search (const string& s)
{
    for_each (entities.begin (), entities.end (), [](Entity& e)
    {

    });
}

void EntityGroup::print (Terminal& io)
{
    slot.prints ("Group: ", name, " ");
}

}   //< _Id
