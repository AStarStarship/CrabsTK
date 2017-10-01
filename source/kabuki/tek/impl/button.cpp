/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/tek/sensors/Button.cpp
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

#include <sensors/Button.h>

namespace tek { namespace sensors {

Button::Button  (uint bit)
:   bit_number_  (bit)
{
}

inline byte Button::GetState  (byte* digitalIns) 
{ 
    uint bit = bit_number_; 
    return digitalIns[bit >> BitToByteShift] &  (1 <<  (bit & BitNumberMask)); 
}

inline void Button::Poll  (byte* debouncedXOR)
{
    byte index = bit_number_ >> BitToByteShift,
        mask = 1 <<  (bit_number_ & BitNumberMask),
        state = debouncedXOR[index] & mask;
    
    if  (state == 0) return;
    
    if  (debouncedXOR[index] & mask) { PressFunction.call (); return; }
    
    DepressFunction.call ();
}

const Member* Button::Op (byte index, Uniprinter* io)
{
	const Member this_member = { "Button", NumMembers (0), FirstMember ('A'), 
							     "tek::sensors::Button" };
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return _::InvalidIndex ();
}
}   //< namespace sensors
}   //< namespace tek
