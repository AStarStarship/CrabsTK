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

#include "../include/serial_portal.h"

namespace kabuki { namespace tek {

SerialPortal::SerialPortal (PinName tx_pin, PinName rx_pin, void* base_address,
                            uint_t buffer_size)
:   slot (0, 0)   //< What happens wh,RxPin)
    
{
    /// Nothing to do here!
}

const char* SerialPortal::GetError () { return error; }

void SerialPortal::Feed ()
{
    while (slot.isReadable()) slot.StreamTxByte ();
}

void SerialPortal::Pull ()
{
    while (serial.readable ()) slot.StreamRxByte (serial.getc ());
}

}       //< namespace tek
}       //< namespace kabuki
