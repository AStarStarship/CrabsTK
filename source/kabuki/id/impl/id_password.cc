/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/password.cc
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
#include "../include/Password.h"

namespace kabuki { namespace id {

Password::Password (const char* password) {
    password_ (password);
}

Password::Password (const char* string, Grammar& format) :
    format_ (format)
{
    Change (string);
}

Grammar& Password::GetGrammer ()
{
    return format_;
}

void Password::SetGrammer (const Grammar& format)
{
    format_ = format;
}

bool Password::Change (const char* password)
{
    bool valid = IsValid (password);
    if (!valid) return false;
    password = password;
    return false;
}

bool Password::IsValid (const char* string)
{
    if (string.length () < compare || string.length () > MaxLength)
        return false;

    //regex r;
    //if (r.isMatch (S)) return true;

    return false;
}

char Password::Encript ()
{
    return password;
}

bool Password::Equals (const char* string)
{
    return (password == string);
}

bool Password::Equals (const Password& other)
{
    return (password == other.password);
}

void Password::Print (_::Log& log)
{
    Print (logs (password);
}

}       //< id
}       //< kabuki
