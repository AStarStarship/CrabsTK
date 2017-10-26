/** kabuki::id
    @file    ~/source/kabuki/id/include/address.h
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
 
#ifndef KABUKI_ID_ADDRESS_H
#define KABUKI_ID_ADDRESS_H

#include "module_config.h"

namespace kabuki { namespace id {

typedef enum {
    HomeAddress = 0,
    WordAddress,
    OtherAddress
} AddressType;

/** Gets one of the addressTypeLables. */
inline const char* AddressTypeLabel (AddressType type);

/** A physical address. 
    @todo Integrate with Google Maps API.
*/
class KABUKI Address
{
    public:

    /** Default constructor. */
    Address ();
    
    /** Creates an address with the given parameters. */
    Address (const char* street, const char* aCity, const char* aState, const char* aZip, const char* aCcountry, 
        AddressType aType);

    /** Gets the street address. */
    char& GetStreet ();

    /** Sets the street address. */
    void SetStreet (const char* newValue);

    /** Gets the city. */
    char& GetCity ();

    /** Sets the city. */
    void SetCity (const char* newValue);

    /** Gets the State. */
    char& GetState ();

    /** Sets the state. */
    void SetState (const char* newValue);

    /** Gets the zip code. */
    char& GetZip ();

    /** Sets the zip code. */
    void SetZip (const char* newValue);

    /** Gets the country. */
    char& GetCountry ();

    /** Sets the country. */
    void SetCountry (const char* newValue);

    /** Gets the address type.
        The address type means street, PO box, etc.
    */
    AddressType GetType ();

    /** Sets the address type. */
    void SetType (AddressType t);

    /** Prints this object to a Expression. */
    void Print (_::Log& log);

    private:

    char        street_,  //< The street address.
                city_,    //< The city of address
                state_,   //< The state of address
                zip_,     //< The zip code of address
                country_; //< The country of address
    AddressType type_;    //< The type of address
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_ADDRESS_H
