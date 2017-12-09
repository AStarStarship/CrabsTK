/** kabuki::id
    @file    ~/source/kabuki/id/authenticator.h
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

#ifndef HEADER_FOR_KABUKI_ID_VALIDATORDEFAULT
#define HEADER_FOR_KABUKI_ID_VALIDATORDEFAULT

#include "authenticator.h"

namespace kabuki { namespace id {

class AuthenticatorDefault : public Authenticator {
    public:

    AuthenticatorDefault ();

    int GetMinHandleLength ();

    int GetMaxHandleLength ();

    int GetMinPasswordLength ();

    int GetMaxPasswordLength ();

    virtual const char* HandleIsValid (const char* handle);

    virtual const char* PasswordIsValid (const char* password);
    
    private:

    int min_handle_length_,
        max_handle_length_,
        min_password_length_,
        max_password_length;
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_VALIDATORDEFAULT
