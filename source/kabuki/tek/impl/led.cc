/** Kabuki Tek
    @version 0.9
    @file    /.../Source/kabuki-tek-Impl/LED.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <LED.h>

namespace kabuki-tek {

inline void TurnLEDOn  ( byte* spiOutBytes, uint bitNumber)
{
    uint bit = bitNumber; 
    spiOutBytes[bit >> BitToByteShift] |=  ( 1 <<  ( bit & BitNumberMask)); 
}

inline void TurnLEDOff  ( byte* spiOutBytes, uint bitNumber)
{
    uint bit = bitNumber;
    spiOutBytes[bit >> BitToByteShift] &= ~  ( 1 <<  ( bit & BitNumberMask));
}

inline void ToggleLED  ( byte* spiOutBytes, uint bitNumber)
{ 
    uint bit = bitNumber;
    byte* pointerToByte = spiOutBytes +  ( bit >> BitToByteShift);
    byte byte = *pointerToByte,
        mask = 1 <<  ( bit &  ( bit & BitNumberMask));
    if ( byte == 0) { *pointerToByte = byte | mask; return; }
    *pointerToByte = byte & ~mask;
}

inline bool GetLEDState  ( byte* spiOutBytes, uint bitNumber)
{
    uint bit = bitNumber;
    return spiOutBytes[bit >> BitToByteShift] &  ( 1 <<  ( bit & BitNumberMask));
}

inline void SetLEDState  ( byte* spiOutBytes, uint bitNumber, bool state)
{
    uint temp = bitNumber;
    byte mask =  ( byte)  ( 1 <<  ( temp & BitNumberMask));
    if ( state) { spiOutBytes[temp >> BitToByteShift] |=  mask; return; }
    spiOutBytes[temp >> 3] &= ~mask;
}

LED::LED  ( uint bit, uint row):
    bitNumber  ( bit),
    rowNumber  ( row)
{
    /// ???
}

byte LED::getState ()
{
    return 0;
}

const char* LED::setState (byte Value)
{
    return 0;
}

const char* LED::Op (byte index, Uniprinter* io)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("LED", "tek::midi"): InvalidIndex ();
}

}   //< namespace leds
}   //< namespace tek
