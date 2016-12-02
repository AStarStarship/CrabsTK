/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/Address.hpp
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

#include <KabukiSDK-Config.hpp>

namespace _Id {

typedef enum {
    HomeAddress = 0,
    WordAddress,
    OtherAddress
} AddressType;

inline const char** addressTypeLabels ();
/*< A read-only array of the AddressType string labels. */

inline const char* addressTypeLabel (AddressType type);
/*< Gets one of the addressTypeLables. */

class _KabukiSDK Address
/*< A physical address. 
    @todo Integrate with Google Maps API.
*/
{
    public:

    Address ();
    /*< Default constructor. */

    Address (const string& aStreet, const string& aCity, const string& aState, const string& aZip, const string& aCcountry, 
        AddressType aType);
    /*< Constructs an address with the given parameters. */

    string& getStreet ();
    /*< Gets the street addrss. */

    void setStreet (const string& newValue);
    /*< Sets the street addrss. */
    
    string& getCity ();
    /*< Gets the city. */

    void setCity (const string& newValue);
    /*< Sets the city. */

    string& getState ();
    /*< Gest the State. */

    void setState (const string& newValue);
    /*< Sets the state. */
    
    string& getZip ();
    /*< Gets the zip code. */

    void setZip (const string& newValue);
    /*< Sets the zip code. */
    
    string& getCountry ();
    /*< Gets the country. */

    void setCountry (const string& newValue);
    /*< Sets the country. */

    AddressType getType ();
    /*< Gets the address type.
        The address type means street, PO box, etc.
    */

    void setType (AddressType t);
    /*< Sets the addrss type. */

    inline void print (Terminal& slot);

    private:

    string street,          //< The street address.
        city,               //< The city of address
        state,              //< The state of address
        zip,                //< The zip code of address
        country;            //< The country of address
    AddressType type;       //< The type of address
};

}   //< namespace _Id
