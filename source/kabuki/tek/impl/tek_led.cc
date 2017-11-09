/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/impl/tek_led.cc
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

#include "../include/led.h"

namespace kabuki { namespace tek {

inline void TurnLEDOn  ( byte* spiOutBytes, uint bitNumber) {
    uint bit = bitNumber; 
    spiOutBytes[bit >> BitToByteShift] |=  ( 1 <<  ( bit & BitNumberMask)); 
}

inline void TurnLEDOff  ( byte* spiOutBytes, uint bitNumber) {
    uint bit = bitNumber;
    spiOutBytes[bit >> BitToByteShift] &= ~  ( 1 <<  ( bit & BitNumberMask));
}

inline void ToggleLED  ( byte* spiOutBytes, uint bitNumber) { 
    uint bit = bitNumber;
    byte* pointerToByte = spiOutBytes +  ( bit >> BitToByteShift);
    byte byte = *pointerToByte,
        mask = 1 <<  ( bit &  ( bit & BitNumberMask));
    if ( byte == 0) { *pointerToByte = byte | mask; return; }
    *pointerToByte = byte & ~mask;
}

inline bool GetLEDState  ( byte* spiOutBytes, uint bitNumber) {
    uint bit = bitNumber;
    return spiOutBytes[bit >> BitToByteShift] &  ( 1 <<  ( bit & BitNumberMask));
}

inline void SetLEDState  ( byte* spiOutBytes, uint bitNumber, bool state) {
    uint temp = bitNumber;
    byte mask =  ( byte)  ( 1 <<  ( temp & BitNumberMask));
    if ( state) { spiOutBytes[temp >> BitToByteShift] |=  mask; return; }
    spiOutBytes[temp >> 3] &= ~mask;
}

LED::LED  ( uint bit, uint row) :
    bitNumber  ( bit),
    rowNumber  ( row)
{
    /// ???
}

const _::Operation* LED::Star (char_t index, _::Expression* expr)
{
    static const _::Operation this_op = { "LED",
        _::NumOperations (0), _::FirstOperation ('a'),
        "tek::midi", 0 };

    switch (index){
        case '?': return &this_op
    }
    
    return nullptr;
}

}       //< namespace tek
}       //< namespace kabuki
