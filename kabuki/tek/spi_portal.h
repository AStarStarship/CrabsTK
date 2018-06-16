/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/include/spi_portal.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef KABUKI_TEK_SPI_PORTAL_H
#define KABUKI_TEK_SPI_PORTAL_H

#include "config.h"

namespace kabuki { namespace tek {

/** A type of Portal that allows for reading and writing to the local system. */
class SpiPortal : public _::Portal {
    public:

    /** Constructs a SpiPortal. */
    SpiPortal (_::Expr* expr, PinName mosi_pin, PinName miso_pin,
               PinName clock_pin, PinName strobe_pin);

    /** Feeds B-Output bytes through the slot. */
    virtual void Feed ();

    /** Pulls B-Input bytes through the slot. */
    virtual void Pull ();

    private:

    _::Expr* expr_;        //< Expr for this Portal.
    int            start_index_, //< Start index of the buffer.
                   stop_index_,  //< Stop index of the buffer.
                   buffer_size_; //< Buffer size in bytes.
    SPI            spi_;         //< SPI port.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_SPI_PORTAL_H
