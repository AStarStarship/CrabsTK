/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Dev/_Dev/Components/ADSR.hpp
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

namespace _Dev { namespace Components {

class _KabukiSDK ADSR: public Device {
    
class _KabukiSDK ADSR
/*< An ADSR filter. */
{
    ADSR ();
    /*< Constructs an ADSR with all zeroed out controls. */

    virtual byte GetState ();
    /*< Gets the Inter-process state. */
    
    virtual string SetState (byte Value);
    /*< Sets the Inter-process state.
        @return returns 0 upon success, and an Errror upon failure. */
    
    virtual string op (Terminal* slot, int index);
    /*< Inter-process operations. */
}

};

}   //< namespace Components
}   //< namespace _Dev

