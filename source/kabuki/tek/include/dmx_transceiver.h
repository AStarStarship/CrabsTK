/** Kabuki Tek
    @file    /.../Source/_tek/_tek/DMX/DMXTransceiver.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)
                            All right reserved  (R).
        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef KABUKI_TEK_DMX_DMXTRANSCEIVER_H
#define KABUKI_TEK_DMX_DMXTRANSCEIVER_H

#include <_tek/config.h>

#include "DMXReceiver.h"
#include "DMXTransmitter.h"

namespace kabuki { namespace theater { namespace dmx {

template <byte NumPorts>
class DMXTransceiver
{
      public:
    
    /** Default constructor. */
    DMXTransceiver ();
    
    /** Returns a reference to the DMXTransmitter. */
    DMXTransmitter& GetTransmitter ();
    
    /** Returns a reference to the DMXReceiver. */
    DMXReceiver& GetReceiver;
    
    private:
    
    DMXTransmitter transmitter;
    DMXReceiver receiver;
};
}   //< namespace dmx
}   //< namespace theater
}   //< namespace kabuki
#endif KABUKI_TEK_DMX_DMXTRANSCEIVER_H
