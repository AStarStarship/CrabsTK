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

#include "Address.hpp"
#include "EmailAddress.hpp"
#include "Profile.hpp"

#include <string>
#include <vector>
using namespace std;

namespace _Id {

class _KabukiSDK Entity
/** A entinty with contact information. */
{
    /**  */
    Entity (string anEmailAdress = "", string aFirstName = "", string aLastName = "", 
        string aPrimaryPhoneNum = "", string aStreetAdress1 = "", string aZipCode1 = "", 
        string aStreetAdress2 = "", string aZipCode2 = "");

    string getName ();
    /*< Gets the entity name string. */

    int setName (string s);
    /*< Attempts to set the name string to the new string. */

    int contains (string query);
    /*< Returns true if this entity contains the search query. */
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:

    string firstName,
        lastName,
        emailAdress,
        streetAdress1,
        zipCode1,
        streetAddress2,
        zipCode2,
        phoneNumber;
    
    vector<string> tags;
    vector<Address> addresses;
    vector<EmailAddress> emailAddresses;
    vector<Profile> profiles;
    vector<string> notes;
};

}   //< namespace _Id
