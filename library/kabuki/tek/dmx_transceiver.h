/** kabuki::tek
    @file    ~/source/kabuki/tek/include/dmx_transceiver.h
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

#ifndef KABUKI_TEK_DMX_DMXTRANSCEIVER_H
#define KABUKI_TEK_DMX_DMXTRANSCEIVER_H

#include "tek/dmx_receiver.h"
#include "tek/dmx_transmitter.h"

namespace kabuki { namespace tek {

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
    
    DMXTransmitter transmitter_;
    DMXReceiver    receiver_;
};
}       //< namespace tek
}       //< namespace kabuki
#endif KABUKI_TEK_DMX_DMXTRANSCEIVER_H
