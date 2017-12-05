/** kabuki::pro
    @file    ~/source/kabuki/id/handle.cc
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
#include "handle.h"

using namespace _;
using namespace std;

namespace kabuki { namespace id {

Handle::Handle (const char* key, int min_length, int max_length) {
    if (min_length >= max_length) {
        int temp = min_length;
        min_length = max_length;
        max_length = temp;
    }
    key = key;
    if (min_length < kMinLength) min_length = kMinLength;
    else if (min_length > kMaxLength) min_length = kMinLength;
    StringCopy (key_, key);
}

const char* Handle::GetKey () { return key_; }

bool Handle::SetKey (const char* key) {
    size_t length = StringLength (key);
    if (length < kMinLength || length > kMaxLength)
        return false;

    if (!IsValid (key))
        return false;
    StringCopy (key_, key);
    return true;
}

bool Handle::IsValid (const char* key) {
    size_t length = StringLength (key);
    if (length < kMinLength || length > kMaxLength)
        return false;
    return true;
}

bool Handle::Equals (const Handle& h) {
    return !StringEquals (key_, h.key_);
}

void Handle::Print () {
    cout << "\n| Handle: " << key_;
}

}       //< id
}       //< kabuki
