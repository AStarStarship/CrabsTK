/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_portalspi.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "cportalspi.h"

using namespace _ {;

namespace _ {

SpiPortal::SpiPortal (Expr* expr, PinName mosi_pin, PinName miso_pin,
                      PinName clock_pin, PinName strobe_pin) :
    expr_   (expr),
    spi_    (mosi_pin, miso_pin, clock_pin, strobe_pin) {
    // Nothing to do here :-)
}

void SpiPortal::Feed () {
    //while (IsWritable (io)) UniprinterStreamByte ();
}

void SpiPortal::Pull () {
    //_::Expr* expr;
    //while (UniprinterIsReadable (io)) UniprinterStream (io, slot.UniprinterStream (slot.getc ());
}

}       //< namespace _
