/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/Entity.hpp
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

#include <string>
#include <vector>

namespace _Id {

/**  */
class _KabukiSDK Entity
{
    /**  */
    Entity (const char* anEmailAdress = "", const char* aFirstName = "", const char* aLastName = "", 
        const char* aPrimaryPhoneNum = "", const char* aStreetAdress1 = "", const char* aZipCode1 = "", 
        const char* aStreetAdress2 = "", const char* aZipCode2 = "");

    const char* GetName ();
    int setName (const char* S);

    bool contains (string queery);
    
    private:

    const char* firstName,
        lastName,
        emailAdress,
        streetAdress1,
        zipCode1,
        streetAddress2,
        zipCode2,
        phoneNumber;
    
    std::vector<string> tags;
    std::vector<Address> addresses;
    std::vector<EmailAddress> emailAddresses;
    std::vector<Profile> profiles;
    std::vector<Note> notes;
};
}   //< namespace _Id
}   //< namespace _Search

