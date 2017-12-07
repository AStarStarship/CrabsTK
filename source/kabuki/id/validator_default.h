/** kabuki::id
    @file    ~/source/kabuki/id/validator.h
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

#include "validator.h"

namespace kabuki { namespace id {

class ValidatorDefault : public Validator {
    public:

    ValidatorDefault ();

    int GetMinDislpayNameLength ();
    int GetMaxDisplayNameLength ();
    int GetMinHandleLength ();
    int GetMaxHandleLength ();
    int GetMinPasswordLength ();
    int GetMaxPasswordLength ();

    virtual const char* IsValid (const char* input, int type);
    
    private:

    int min_dislpay_name_length_,
        max_display_name_length_,
        min_handle_length_,
        max_handle_length_,
        min_password_length_,
        max_password_length;
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_VALIDATORDEFAULT
