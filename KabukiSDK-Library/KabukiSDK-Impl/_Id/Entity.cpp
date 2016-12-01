/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Entity.cpp
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

#include <_Id/Entity.h>

namespace _Id {

Entity::Entity (const char* anEmailAdress = "", const char* aFirstName = "", const char* aLastName = "", 
    const char* aPrimaryPhoneNum = "", const char* aStreetAdress1 = "", const char* aZipCode1 = "", 
    const char* aStreetAdress2 = "", const char* aZipCode2 = "")
{
    firstName = aFirstName;
    lastName = aLastName;
    phoneNumber = aPrimaryPhoneNum;
    emailAdress = anEmailAdress;
    streetAdress1 = aStreetAdress1;
    zipCode1 = aZipCode1;
}

const char* Entity::getName () { return name; }

void Entity::getName (const char* S) { name = S }

bool Entity::contains (const char* queery)
{
    foreach (const char* a in tags)
        if (a == queery) return true;
    foreach (Address a in addresses)
        if (a == queery) return true;
    foreach (Entity a in emailAddresses)
        if (a == queery) return true;
    foreach (Profile a in profiles)
        if (a == queery) return true;
    foreach (const char* a in tags)
        if (a == queery) return true;
    
    return false;
}

const char* Entity::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("Entity", "_Id"): InvalidIndex ();
}

}   //< namespace _Id
