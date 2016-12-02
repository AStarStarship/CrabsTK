/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Handle.hpp
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

#include <KabukiSDK-Config.hpp>

#include <string>
using namespace std;

namespace _Id {

int getMinHandleLength ();
/*< Gets the min handle/username/ length. */

int getMaxHandleLength ();
/*< Gets the max handle/username/ length. */

void setHandleLengthRange (int min, int max);
/*< Sets the min and max hanlde legnth to the given values. */

/**  */
class _KabukiSDK Handle
{
    public:

    enum {
        MinLengthRange = 3,       //< The min length of a Handle range.
        MaxLengthRange = 256      //< The max length of a Handle range.
    };
    
    Handle (const string& aName, int AMinLength = MinLengthRange, int AMaxLength = MaxLengthRange);
    /*< Constructor creates a standard username. */

    string& getHandle ();
    /*< Gets a reference to the handle string. */

    bool setHandle (const string& s);
    /*< Gets true if this password is value. */

    bool isValid (const string& s);
    /*< Returns true if the handle is valid. */

    bool equals (const Handle& h);
    /*< Returns true if this Handle is idential to the given Handle. */

    void print (Terminal& slot);
    /*< Prints the state of this object. */
    
    private:

    string name;   //< The name of the object.
};

}   //< namespace _Id
