/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/EntityGroup.cpp
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

#include <_Id/EntityGroup.hpp>

namespace _Id {

EntityGroup::EntityGroup (string groups_name)
{
    name = new Name (groups_name);
    accounts = new _Id.Account.Account[0];
}

bool EntityGroup::IsValid ()
{
    return name.IsValid ();
}

string EntityGroup::getName () { return name.print (I2P::Terminal& slot); }

void EntityGroup::Rename (string newName)
{
    name.Rename (newName);
}

void EntityGroup::ApplyPrivilages (Privilages& P)
{
    if (new_privilages == nullptr)
        return;
    for (int i = 0; i < base.Num_Accounts (); i++)
        accounts[i].Role ().Apply_Privilages (new_privilages);
}

byte EntityGroup::getState ()
{
    return 0;
}

const char* EntityGroup::getState (byte Value)
{
    return 0;
}

const char* EntityGroup::op (I2P::Terminal& slot, int index, int Enq)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("EntityGroup", "_Id"): InvalidIndex ();
}

string EntityGroup::print (I2P::Terminal& slot)
{
    return "Group: " + name.print (I2P::Terminal& slot) + " " + base.print (I2P::Terminal& slot);
}

}   //< namespace _Id
