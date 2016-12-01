/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Username.c
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

#include "_Id/Username.hpp"

namespace _Id;

Username::Username (string aName, int aMinLength = 8, int aMaxLength = 32)
{
    if (aMinLength > aMaxLength)
    {
        int temp = aMinLength;
        aMinLength = aMaxLength;
        aMaxLength = temp;
    }
    name = aName;
    if (aMinLength < MinMinLength) aMinLength = MinMinLength;
    else if (aMinLength > MaxMaxLength) aMinLength = MinMinLength;
}

/** Returns true if this password is value. */
bool Username::get (const char* a)
{
    if (a.Length < minLength || a.Length > maxLength)
        return false;

    return IsValid (name);
}

/**  */
bool Username::isValid (string a)
{
    if (a.Length < minLength || a.Length > maxLength)
        return false;


    return true;
}

const char* Username::print (I2P::Terminal& slot)
{
    std::string string = "Username: " + name;
    return string;
}