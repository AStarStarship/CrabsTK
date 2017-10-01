/** Kabuki Tek
    @file    /.../KabukiTek/sensors/bouncy_switch.h
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
 
#ifndef KABUKI_TEK_BOUNCY_SWITCH_H
#define KABUKI_TEK_BOUNCY_SWITCH_H

#include <KabukiTek\Config.h>

#include <mbed.h>

namespace tek { namespace sensors {
	
/** A software debounced switch. */
class BouncySwitch: public _::Device
{
    public:
	
    /** A software debcounced switch. */
    Switch  (uintptr_t DInPin);

    virtual ~Switch ();
    
    /** I2P operations. */
    const Member* Op (byte index, Uniprinter* io) override;
    
    private:
    
    DigitalIn input;    //< The DIN pin.
};
}   	//< namespace sensors
}   	//< namespace tek
#endif	//< KABUKI_TEK_BOUNCY_SWITCH_H
