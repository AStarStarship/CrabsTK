/** kabuki::id
    @file    ~/source/kabuki/id/authenticator_default.cc
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

#include "authenticator_default.h"
#include "user.h"

using namespace _;
using namespace kabuki::id;

namespace kabuki { namespace id {

AuthenticatorDefault::AuthenticatorDefault () :
    min_handle_length_       (Handle::kDefaultMinLength),
    max_handle_length_       (Handle::kDefaultMaxLength),
    min_password_length_     (Password::kDefaultMinLength),
    max_password_length      (Password::kDefaultMaxLength) {
    // Nothing to do here. :-)
}

int AuthenticatorDefault::GetMinHandleLength () {
    return min_handle_length_;
}

int AuthenticatorDefault::GetMaxHandleLength () {
    return max_handle_length_;
}

int AuthenticatorDefault::GetMinPasswordLength () {
    return min_password_length_;
}

int AuthenticatorDefault::GetMaxPasswordLength () {
    return max_password_length;
}

const char* AuthenticatorDefault::HandleIsValid (const char* input) {
    if (input == nullptr) {
        return "nil input";
    }
    int length = StringLength (input);

    if (length < Handle::kDefaultMinLength) {
        return "Password too short";
    }
    if (length > Handle::kDefaultMaxLength) {
        return "password too long";
    }
    for (int i = 0; i < length; ++length) {
        if (isspace (input[i])) {
            return "password can't contain whitespace.";
        }
    }
    return nullptr;
}

const char* AuthenticatorDefault::PasswordIsValid (const char* input) {
    if (input == nullptr) {
        return "nil input";
    }
    int length = StringLength (input);
    if (length < Password::kDefaultMinLength) {
        return "Password too short";
    }
    if (length > Password::kDefaultMaxLength) {
        return "password too long";
    }
    for (int i = 0; i < length; ++length) {
        if (isspace (input[i])) {
            return "password can't contain whitespace.";
        }
    }
    return nullptr;
}

}       //< namespace id
}       //< namespace kabuki
