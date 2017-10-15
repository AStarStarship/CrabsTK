/** KabukiTheater
    @file    /.../Source/KabukiTheater-Impl/_Entity/Entity.h
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

#include "_Entity/Entity.h"

namespace _Entity {

Entity::Entity (const string& anEmailAdress = "", const string& aFirstName = "", const string& aLastName = "", 
    const string& aPrimaryPhoneNum = "", const string& aStreetAdress1 = "", const string& aZipCode1 = "", 
    const string& aStreetAdress2 = "", const string& aZipCode2 = "")
{
    firstName = aFirstName;
    lastName = aLastName;
    phoneNumber = aPrimaryPhoneNum;
    emailAdress = anEmailAdress;
    streetAdress1 = aStreetAdress1;
    zipCode1 = aZipCode1;
}

string& Entity::getName () { return name; }

int Entity::setName (const string& S) { name = S; }

bool Entity::Contains (string query)
{
    for_each (tags.begin(), tags.end (), [](string &S) 
    { 
        if (S == query) return true; 
    });
    
    for_each (addresses.begin (), addresses.end(), [](Address& A)
    { 
        if (S == query) return true; 
    });
    for_each (emailAddresses.begin (), emailAddresses.end(), [](EmailAddress& A) 
    {
        if (a == query) return true;
    });
    for_each (profiles.begin (), profiles.end(), Profile& P)
    {
        if (a == query) return true; 
    });
    for_each (tags.begin (), tags.end(), [](String& S)
    {
        if (a == query) return true; 
    });
    
    return false;
}
}   //< _G
