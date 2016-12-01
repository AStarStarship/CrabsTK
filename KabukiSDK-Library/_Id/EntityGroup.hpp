/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/EntityGroup.hpp
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

namespace _Id {

class _KabukiSDK EntityGroup
/*< A group of entities.
*/
{
    public:

    EntityGroup (const char* Name);
    /*< A group of entities such as people or businesses. */

    bool isValid ();
    /*< Returns true if this is a valid group of entities. */

    const char* getName ();
    /*< Gets the name of the entity group. */

    void setName (const char* newName);
    /*< Sets the name of the entity group. */

    void applyPrivilages (Privilages& P);
    /*< Applies privilages to the entity group. */
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
};

}   //< namespace _Id
