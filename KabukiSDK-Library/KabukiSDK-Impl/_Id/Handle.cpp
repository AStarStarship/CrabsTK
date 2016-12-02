/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Handle.c
    pp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include "_Id/Handle.hpp"

namespace _Id {

static int minHandleLength = Handle::MinLengthRange,
    maxHandleLength = Handle::MaxLengthRange;

int getMinHandleLength ()
{
    return minHandleLength;
}

int getMaxHandleLength ()
{
    return maxHandleLength;
}

void setHandleLengthRange (int min, int max)
{
    if (min >= max)
    {
        minHandleLength = minHandleLength;
        maxHandleLength = maxHandleLength;
        return;
    }
    minHandleLength = min;
    maxHandleLength = max;
}

Handle::Handle (const string& aName, int aMinLength, int aMaxLength)
{
    if (aMinLength >= aMaxLength)
    {
        int temp = aMinLength;
        aMinLength = aMaxLength;
        aMaxLength = temp;
    }
    name = aName;
    if (aMinLength < MinLengthRange) aMinLength = MinLengthRange;
    else if (aMinLength > MaxLengthRange) aMinLength = MinLengthRange;
}

string& Handle::getHandle () { return name; }

bool Handle::setHandle (const string& s)
{
    if (s.length () < minHandleLength || s.length () > maxHandleLength)
        return false;

    return isValid (name);
}

bool Handle::isValid (const string& s)
{
    if (s.length () < minHandleLength || s.length () > maxHandleLength)
        return false;

    return true;
}

bool Handle::equals (const Handle& h)
{
    return name == h.name;
}

void Handle::print (Terminal& slot)
{
    slot.prints ("Handle: ", name.c_str ());
}

}   //< namespace _Id
