/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/impl/tek_serial_portal.cc
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

#include "serial_portal.h"

using namespace _ {;

namespace kabuki { namespace tek {

SerialPortal::SerialPortal (Expr* expr, PinName tx_pin, PinName rx_pin) :
    expr_   (expr),
    serial_ (tx_pin, rx_pin) {
    /// Nothing to do here!
}

void SerialPortal::Feed () {
    //while (serial_.readable ()) expr_.StreamTxByte ();
}

void SerialPortal::Pull () {
    //while (serial_.readable ()) expr_.StreamRxByte (serial_.getc ());
}

}       //< namespace tek
}       //< namespace kabuki
