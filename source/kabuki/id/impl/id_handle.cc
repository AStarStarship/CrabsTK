/** kabuki::pro
    @file    ~/source/kabuki/id/imp/handle.cc
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

static int MinHandleLength = Handle::MinLengthRange,
           MaxHandleLength = Handle::MaxLengthRange;

int GetMinHandleLength ()
{
    return min_handle_length_;
}

int GetMaxHandleLength ()
{
    return max_handle_length_;
}

void SetHandleLengthRange (int min, int max)
{
    if (min >= max)
    {
        min_handle_length_ = min_handle_length_;
        max_handle_length_ = max_handle_length_;
        return;
    }
    min_handle_length_ = min;
    max_handle_length_ = max;
}

Handle::Handle (const char* name, int min_length, int max_length)
{
    if (min_length >= max_length)
    {
        int temp   = min_length;
        min_length = max_length;
        max_length = temp;
    }
    name = name;
    if (min_length < MinLengthRange) min_length = MinLengthRange;
    else if (min_length > MaxLengthRange) min_length = MinLengthRange;
}

char& Handle::GetHandle () { return name; }

bool Handle::SetHandle (const char* string)
{
    if (string.length () < min_handle_length_ || string.length () > max_handle_length_)
        return false;

    return isValid (name);
}

bool Handle::isValid (const char* string)
{
    if (string.length () < min_handle_length_ || string.length () > max_handle_length_)
        return false;

    return true;
}

bool Handle::equals (const Handle& h)
{
    return name == h.name;
}

void Handle::Print (_::Log& log)
{
    Print (logs ("Handle: ", name);
}

}       //< id
}       //< kabuki
