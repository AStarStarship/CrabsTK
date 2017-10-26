/** kabuki::pro
    @file    ~/source/kabuki/id/include/imp/handle.cc
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
#include "../include/handle.h"

namespace kabuki { namespace id {

Handle::Handle (const char* name, int min_length, int max_length)
{
    if (min_length >= max_length)
    {
        int temp   = min_length;
        min_length = max_length;
        max_length = temp;
    }
    name = name;
    if (min_length < kMinLength) min_length = kMinLength;
    else if (min_length > kMaxLength) min_length = kMinLength;
}

char& Handle::GetKey () { return key_; }

bool Handle::SetKey (const char* key)
{
    if (key.length () < kMinLength || key.length () > kMaxLength)
        return false;

    return IsValid (key_);
}

bool Handle::IsValid (const char* key)
{
    size_t length = strlen (key);
    if (length < kMinLength || length > kMaxLength)
        return false;

    return true;
}

bool Handle::Equals (const Handle& h)
{
    return (strcmp (key_, h.key_) == 0);
}

void Handle::Print (_::Log& log)
{
    log += "Handle: " + key_ + "\n";
}

}       //< id
}       //< kabuki
