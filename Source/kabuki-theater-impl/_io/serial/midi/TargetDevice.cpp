/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Com/MIDI/TargetDevice.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

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

namespace _Com { namespace MIDI {

#include <_Com/MIDI/TargetDevice.h>

class TargetDevice
{
   public:

    virtual ~TargetDevice () {}
    
    virtual void ReceiveMessage  (uint32_t Msg, uint32_t TimeStamp) = 0;
    // Receives short messages
    
    virtual void ReceiveMessage  (LPSTR Msg, uint32_t BytesRecorded, uint32_t TimeStamp)=0;
    // Receives long messages
    
    virtual void OnError  (uint32_t Msg, uint32_t TimeStamp)=0;
    // Called when an invalid short message is received
    
    virtual void OnError  (LPSTR Msg, uint32_t BytesRecorded, uint32_t TimeStamp)=0;
    // Called when an invalid long message is received
};

}   //< namespace MIDI
}   //< namespace _Com
