/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/portal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_BOUTPORTAL_H
#define SCRIPT_BOUTPORTAL_H

#include "bout.h"
#include "portal.h"

namespace _ {

/** A slot in Door that connects two or more Chinese Rooms.
    Examples of common Portal are UART, Dual-UART, SPI, I2C, Quad SPI, 
    TCP/HTTP/UDP, Bluetooth, CAN. */
class KABUKI Window : public Portal {
    
    public:

    /** Constructs a Window from the given bout. */
    Window (Bin* bin, Bout* bout);

    /** Sets the Portal up for a batch of bytes transfer.
         Implementation of this function is not required to do anything, but 
         regardless it will be called by the Set. */
    virtual void Prime ();

    /** Gets the length of current portal.
        @warning Length might not be the actual length, but rather the length
                 of the data that is read to be pulled. */
    virtual uint_t Length ();

    /** Feeds tx messages through the a without scanning them. */
    virtual void Feed ();

    /** Pulls rx messages through the a and runs them through the scanner. */
    virtual void Pull ();

    private:

    Bout* bout;     //< The B-Output for this Portal.
};
}       //< namespace _
#endif  //< SCRIPT_BOUTPORTAL_H
