/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_target_device.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>

namespace kabuki { namespace toolkit { namespace midi {

#include "target_device.h"

class TargetDevice
{
   public:

    virtual ~TargetDevice () {}
    
    /** Receives short messages. */
    virtual void ReceiveMessage  (uint32_t Msg, uint32_t TimeStamp) = 0;
    
    /** Receives uint32_t messages. */
    virtual void ReceiveMessage  (LPSTR Msg, uint32_t BytesRecorded, uint32_t TimeStamp)=0;
    
    /** Called when an invalid short message is received. */
    virtual void OnError  (uint32_t Msg, uint32_t TimeStamp)=0;
    
    /** Called when an invalid uint32_t message is received. */
    virtual void OnError  (LPSTR Msg, uint32_t BytesRecorded, uint32_t TimeStamp)=0;
};

}   //< namespace midi
}       //< namespace toolkit
}       //< namespace kabuki
