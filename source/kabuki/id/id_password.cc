/** Kabuki Toolkit
    @file    ~/source/kabuki/id/password.cc
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
#include "password.h"

using namespace _;
using namespace std;

namespace kabuki { namespace id {

const char Password::kDefault[] = "";

Password::Password (Authenticator* authenticator, const char* string) :
    authenticator_ (authenticator),
    type_ (kValidation) {
    if (!SetKey (string)) {
        key_ = StrandClone ("");
    }
}

Password::~Password () {
    delete key_;
}

const char* Password::GetKey () {
    return key_;
}

bool Password::SetKey (const char* password) {
    if (password == nullptr)
        return false;

    if (authenticator_->PasswordIsInvalid (password)) {
        return false;
    }
    delete key_;
    key_ = StrandClone (password);
    return true;
}

bool Password::IsValid (const char* string) {
    return true;
}

bool Password::Equals (const char* key) {
    return StrandEquals (key_, key);
}

bool Password::Equals (const Password& other) {
    return StrandEquals (key_, other.key_);
}

Text& Password::Print (Text& txt) {
    return txt << key_;
}

}       //< id
}       //< kabuki
