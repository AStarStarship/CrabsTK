/** Kabuki Tek
    @version 0.9
    @file    /.../Source/tek/Portals/SerialPortal.h
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

#pragma once

#include <ChineseRoom.h>

#if _Platform == _mbed
#include <mbed.h>
#endif

namespace tek {

/** A portal for passing messages through a slot in a Chinse Door.
*/
class SerialPortal: public Portal, public Serial
{
  public:
    
    /** Constructor creates a loopback port. */
    SerialPortal (PinName TxPin, PinName RxPin, void* BaseAddress, index_t BufferSize);

    /** gets the last error originating from this Portal. */
    virtual const char* GetError ();

    virtual void Feed ();
    /** Feeds (Tx) punch cards through the slot. */

    virtual void Pull ();
    /** Pulls (Rx) punch cards through the slot. */

  private:

    Terminal slot;          //< Terminal for this Portal.
    //Serial serial;          //< mbed Serial port.
};

}
