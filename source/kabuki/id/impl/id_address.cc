/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/address.cc
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
#include "../include/address.h"

namespace kabuki { namespace id {

const char** AddressTypeLabels ()
{
    static const char* labels[] = {
        "Home",
        "Work",
        "Other"
    };
    return labels;
}

const char* AddressTypeLabel (AddressType type)
{
    if (type < 0 || type >= 3)
        return AddressTypeLabels ()[0];
    if (type < 0 || type > 2)
        return AddressTypeLabels ()[2];
    return AddressTypeLabels ()[type];
}

Address::Address ()
{
    /// Nothing to do here! :-)
}

Address::Address (const char* street, const char* city, const char* state,
                  const char* zip, const char* country, AddressType type) :
    street_  (street ),
    city_    (city   ),
    state_   (state  ),
    zip_     (zip    ),
    country_ (country),
    type_    (type   ) {
}

char& Address::GetStreet () { return street_; }

void Address::SetStreet (const char* string) {
    street_ = string;
}

char& Address::GetCity () { return city_; }

void Address::SetCity (const char* string) {
    city_ = string;
}

char& Address::GetState () { return state_; }

void Address::SetState (const char* string) {
    state_ = string;
}

char& Address::GetZip () { return zip_; }

void Address::SetZip (const char* string) {
    zip_ = string;
}

char& Address::GetCountry () { return country_; }

void Address::SetCountry (const char* string) {
    country_ = string;
}

AddressType Address::GetType () { return type_; }

void Address::SetType (AddressType t) {
    type_ = t;
}

void Address::Print (_::Log& log) {
    log + "Address:"
        + "\n Street         : " + street_
        + "\n City           : " + city_ 
        + "\n State/Province : " + state_
        + "\n Zip/Postal Code: " + zip_ 
        + "\n Country        : " + country_ + "\n";
}

}       //< id
}       //< kabuki
