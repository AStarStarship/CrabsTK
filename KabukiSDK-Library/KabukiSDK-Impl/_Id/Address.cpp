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

Address::Address ()
{
    /// Nothing to do here! :-)
}

Address::Address (const char* aStreet, const char* aCity, const char* aState, const char* aZip, const char* aCountry, const char* aType)
:   street (aStreet),
    city   (aCity),
    state  (aState),
    zip    (aZip),
    country (aCountry),
    type   (aType)
{
}

const char* Address::getType () { return type; }

void Address::getType (const char* s)
{ 
    type = s;
}

const char* Address::getStreet () { return street; }

void Address::getStreet (const char* s)
{
    street = s;
}

const char* Address::getCity () { return city; }

void Address::getCity (const char* s)
{ 
    city = s;
}

const char* Address::getState () { return state; }

void Address::setState (const char* s)
{ 
    state = s;
}

const char* Address::getZip () { return zip; }

void Address::setZip (const char* s)
{ 
    zip = s;
}

void Address::print (I2P::Terminal& slot)
{
    slot.prints ( "Street: ", Street, "\n", " Cit: ", City, "\n", " State/Province: ", State, "\n", 
        "Zip/Postal Code: ", Zip, "\n", "Country: ", Country, "\n");
}

}   //< namespace _Id
