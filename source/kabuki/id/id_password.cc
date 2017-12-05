/** kabuki::pro
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

Password::Password (const char* string) {
    SetKey (string);
}

const char* Password::GetKey () {
    return password_;
}

const char* Password::SetKey (const char* password) {
    if (password == nullptr)
        return nullptr;
    
    if (!IsValid (password)) return false;
    password_ = _::StringClone (password);
    return nullptr;
}

bool Password::IsValid (const char* string)
{
    return true;
}

bool Password::Equals (const char* string)
{
    return (password_ == string);
}

bool Password::Equals (const Password& other)
{
    return !strcmp (password_, other.password_);
}

void Password::Print () {
    cout << password_;
}

}       //< id
}       //< kabuki
