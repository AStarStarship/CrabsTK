/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/impl/tek_spi_portal.cc
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

#include "spi_portal.h"

using namespace _;

namespace kabuki { namespace tek {

SpiPortal::SpiPortal (Expression* expr, PinName mosi_pin, PinName miso_pin,
                      PinName clock_pin, PinName strobe_pin) :
    expr_   (expr),
    spi_    (mosi_pin, miso_pin, clock_pin, strobe_pin) {
    // Nothing to do here :-)
}

void SpiPortal::Feed () {
    //while (IsWritable (io)) UniprinterStreamByte ();
}

void SpiPortal::Pull () {
    //_::Expression* expr;
    //while (UniprinterIsReadable (io)) UniprinterStream (io, slot.UniprinterStream (slot.getc ());
}

}       //< namespace tek
}       //< namespace kabuki
