/** Kabuki Tek
    @file    /.../KabukiTek/tek/sensors/Pot.h
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
 
#pragma once


#include <KabukiTek\Config.h>

namespace tek {
namespace sensors {

class Unicontroller;
/** A potentiometer.
    
    @code
    Pot<int16_t> pot
    @endcode
*/
template<typename uint6_t>
class Pot
      public:
    
    Pot (PinName AInPin, channel_t Channel)
    :   input    (AInPin),
        channel  (Channel)
    {

    }
    
    inline void Poll  (uint_t NewValue, _::Expression* io, uint16_t value, uint6_t minValue, uint6_t maxChannel)
    {
        /// 1. Autoset the min-max bounds.


        /// 2. Poll,
        /*
        if  (NewValue < minValue)
        {
            I2P::Roombot* Botvalue = minValue;
        }
        else if  (NewValue > maxValue)
        {
            I2P::Roombot* Botvalue = maxValue;
        }
        else
            I2P::Roombot* Botvalue =  (maxValue - minValue) / NewValue;
        */
    }
    
    void Print (I2P::Expression& Slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    AnalogIn input;

    channel_t channel;
};
}   //< namespace sensors
}   //< namespace tek
