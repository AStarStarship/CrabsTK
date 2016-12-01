/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Username.hpp
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

#pragma once

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

namespace _Id {

/**  */
class _KabukiSDK Username
{
    public:

    enum {
        MinMinLength = 1,       //< The minimum min length of a Username
        MaxMaxLength = 256      //< The maximum max length of a Username
    };
    
    Username (const char* aName, int AMinLength = MinMinLength, int AMaxLength = MaxMaxLength);
    /*< Constructor creates a standard username. */

    bool Set (const char* S);
    /*< Gets true if this password is value. */

    bool IsValid (const char* S);
    /*<  */

    void print (I2P::Terminal& slot);
    /*< Prints the state of this object. */
    
    private:

    int minLength,      //< The min length.
        maxLength;      //< The max length.

    const char* name;   //< The name of the object.
};
}   //< namespace _Id
