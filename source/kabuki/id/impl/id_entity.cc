/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/entity.cc
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
#include "../include/entity.h"

namespace kabuki { namespace id {

Entity::Entity (const char* email_address, const char* first_name, const char* last_name, 
    const char* primary_phone, const char* street_adress1, const char* zip_code1, 
    const char* street_address2, const char* zip_code2) :
    first_name_     (first_name),
    last_name_      (last_name),
    phone_number_   (primary_phone),
    email_adress_   (email_adress),
    street_adress1_ (street_adress1),
    zip_code1_      (zip_code1)
{
    
}

char& Entity::GetFirstName () { return first_name_; }

void Entity::SetFirstName (const char* string) { first_name_ = string; }

bool Entity::Search (const char* query)
{
    /*
    for_each (tags.begin (), tags.end (), [](char& string(
    {
        if (string == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](Address& a(
    {
        if (a == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](Entity& e(
    {
        if (e == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](Profile& p(
    {
        if (p == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](char& string(
    {
        if (string == query) return 1;
    });
    */
    return false;
}

void Entity::Print (_::Log& log)
{
    Print (logs ("Entity: ");
}

}       //< id
}       //< kabuki
