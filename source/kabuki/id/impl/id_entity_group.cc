/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/entity_group.cc
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
#include "../include/entity_group.h"

namespace kabuki { namespace id {

EntityGroup::EntityGroup (const char* name) :
   name (name)
{
}

char& EntityGroup::GetName () { return name; }

void EntityGroup::SetName (const char* string)
{
    name = string;
}

void EntityGroup::ApplyPrivilage (const Privilage& privilage)
{
    //for (int i = 0; i < base.getNumAccounts (); i++)
    //    accounts[i].Role ().applyPrivilages (new_privilages);
}

int EntityGroup::Search (const char* string)
{
    for_each (entities.begin (), entities.end (), [](Entity& e)
    {

    });
}

void EntityGroup::Print (_::Log& log)
{
    Print (logs ("Group: ", name, " ");
}

}       //< id
}       //< kabuki
