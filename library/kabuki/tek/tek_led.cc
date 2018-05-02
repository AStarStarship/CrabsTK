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

#include "led.h"

using namespace _;

namespace kabuki { namespace tek {

inline void TurnLedOn  (byte* spiOutBytes, offset_t bit_number) {
    offset_t bit = bit_number;
    spiOutBytes[bit >> kBitToByteShift] |= ( 1 << ( bit & kBitNumberMask));
}

inline void TurnLedOff  (byte* spiOutBytes, offset_t bit_number) {
    offset_t bit = bit_number;
    spiOutBytes[bit >> kBitToByteShift] &= ~( 1 << ( bit & kBitNumberMask));
}

inline void ToggleLed  (byte* spiOutBytes, offset_t bit_number_) {
    offset_t bit = bit_number_;
    byte* pointerToByte = spiOutBytes + ( bit >> kBitToByteShift);
    byte byte = *pointerToByte,
        mask = 1 <<  ( bit &  ( bit & kBitNumberMask));
    if ( byte == 0) { *pointerToByte = byte | mask; return; }
    *pointerToByte = byte & ~mask;
}

inline bool GetLedState  (byte* spiOutBytes, offset_t offset) {
    return spiOutBytes[offset >> kBitToByteShift] &
           ( 1 << ( offset & kBitNumberMask));
}

inline void SetLedState  (byte* spiOutBytes, offset_t bit_number_, bool state) {
    offset_t temp = bit_number_;
    byte mask =  (byte)  ( 1 << ( temp & kBitNumberMask));
    if ( state) { spiOutBytes[temp >> kBitToByteShift] |=  mask; return; }
    spiOutBytes[temp >> 3] &= ~mask;
}

Led::Led  (offset_t bit, offset_t row) :
    bit_number_  (bit),
    row_number_  (row) {
    /// ???
}

const Operation* Led::Star (char_t index, Expression* expr) {
    static const Operation This = { "LED",
        NumOperations (0), FirstOperation ('A'),
        "tek", 0 };

    switch (index){
        case '?': return &This;
    }
    
    return nullptr;
}

LedOp::LedOp (Led* object) :
	object_ (object) {
	// Nothing to do here!
}

const Operation* LedOp::Star (char_t index, Expression* expr) {
	object_->Star (index, expr);
}

}       //< namespace tek
}       //< namespace kabuki
