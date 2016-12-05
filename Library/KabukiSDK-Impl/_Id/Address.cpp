/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Address.cpp
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

#include <_Id/Address.hpp>

namespace _Id {

const char** addressTypeLabels ()
{
    static const char* labels[] = {
        "Home",
        "Work",
        "Other"
    };
    return labels;
}

const char* addressTypeLabel (AddressType type)
{
    if (type < 0 || type >= 3)
    return addressTypeLabels ()[type];
}

Address::Address ()
{
    /// Nothing to do here! :-)
}

Address::Address (const string& aStreet, const string& aCity, const string& aState, const string& aZip, const string& aCountry, AddressType aType)
:   street (aStreet),
    city   (aCity),
    state  (aState),
    zip    (aZip),
    country (aCountry),
    type    (aType)
{
}

string& Address::getStreet () { return street; }

void Address::setStreet (const string& s)
{
    street = s;
}

string& Address::getCity () { return city; }

void Address::setCity (const string& s)
{ 
    city = s;
}

string& Address::getState () { return state; }

void Address::setState (const string& s)
{ 
    state = s;
}

string& Address::getZip () { return zip; }

void Address::setZip (const string& s)
{ 
    zip = s;
}

string& Address::getCountry () { return country; }

void Address::setCountry (const string& s)
{ 
    country = s;
}

AddressType Address::getType () { return type; }

void Address::setType (AddressType t)
{ 
    type = t;
}

void Address::print (Terminal& slot)
{
    slot.prints ( "Street: ", street.c_str (), "\n", " City: ", city.c_str (), "\n State/Province: ", state.c_str (), 
        "\n Zip/Postal Code: ", zip.c_str (), "\n Country: ", country.c_str (), "\n");
}

}   //  _Id
