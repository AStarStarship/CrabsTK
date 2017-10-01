/** Kabuki Tek
    @version 0.9
    @file    /.../Source-Impl/Portals/SerialPortal.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <portals/serial_portal.h>

namespace tek { namespace portals {

SerialPortal::SerialPortal (word TxPin, word RxPin, void* BaseAddress, uint_t BufferSize)
:   slot (0, 0)   //< What happens wh,RxPin)
    
{
    /// Nothing to do here!
}

const char* SerialPortal::getError () { return error; }

void SerialPortal::Feed ()
{
    while (slot.isReadable()) slot.StreamTxByte ();
}

void SerialPortal::Pull ()
{
    while (serial.readable ()) slot.StreamRxByte (serial.getc ());
}

}   //< Portals
}   //< tek
