/** kabuki::id
    @file    /.../source/kabuki/id/entity.h
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

#ifndef KABUKI_ID_ENTITY_H
#define KABUKI_ID_ENTITY_H

#include "address.h"
#include "email_address.h"
#include "profile.h"

namespace kabuki { namespace id {

/** A entity with contact information. 
    @todo Attach a compressed image link.
*/
class KABUKI Entity
{
    public:

    /** A real-world entity with . */
    Entity (const char* first_name      = "", const char* last_name     = "", 
            const char* email_address   = "", const char* primary_phone = "", 
            const char* street_address1 = "", const char* zip_ode1      = "",
            const char* street_address2 = "", const char* zip_code2     = "");

    /** Gets the entity first/only name. */
    char& GetName ();

    /** Attempts to set the name char to the new char. */
    void SetName (const char* new_name);

    /** Returns true if this entity contains the search query. */
    virtual int Search (const char* query);

    /** Prints this object to a expression. */
    virtual void Print (_::Log& log);
    
    private:

    char*                name,           //< The legal name of the entity.
                         phoneNumber_;    //< The phone number of the entity.
    Address              address_;        //< The address of the entity.
    std::vector<Address>      addresses_;      //< The address of the entity.
    std::vector<EmailAddress> emailAddresses_; //< The email address of the entity.
    std::vector<Profile>      profiles_;       //< The online profiles this entity is attached to.
    _::Array<char, int>         tags_,           //< Metadata tags.
                         notes_;          //< An array of notes about the entity.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_ENTITY_H
