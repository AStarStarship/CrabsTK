/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/Address.hpp
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

#include <string.hpp>

namespace _Id {

class _KabukiSDK Address
/*< A physical address. */
{
    const char* Home = "Home",
        Work = "Work",
        Other = "Other";

    /** Default constructor. **/
    Address ();

    /** Constructor creates an address with the given parameters. */
    Address (const char* street, const char* city, const char* state, const char* zip, const char* country, const char* type);

    const char* getType ();
    /*< Gets the address type.
        The address type means street, PO box, etc.
    */

    void setType (const char* newType);
    /*< Sets the addrss type. */

    const char* getStreet ();
    void setStreet (const char* newValue);
    /*< Sets the street addrss. */
    
    const char* getCity ();
    /*< Gets the city. */

    void setCity (const char* newValue);
    /*< Sets the city. */

    const char* getState ();
    /*< Gest the State. */

    void setState (const char* newValue);
    /*< Sets the state. */
    
    const char* getZip ();
    /*< Gets the zip code. */

    void setZip (const char* newValue);
    /*< Sets the zip code. */
    
    const char* getCountry ();
    /*< Gets the country. */

    void setCountry (const char* newValue);
    /*< Sets the country. */
    
    virtual const char* op (I2P::Terminal* slot, int index);
    /*< Inter-process operations. */

    inline const char* print (I2P::Terminal& slot);
};
}   //< namespace _Id
