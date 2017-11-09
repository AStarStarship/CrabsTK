/** kabuki::tek
    @file    ~/source/kabuki/tek/include/pot.h
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
 
#ifndef KABUKI_TEK_POT_H
#define KABUKI_TEK_POT_H

#include "module_config.h"

namespace kabuki { namespace tek {

/** A potentiometer.
    
    @code
    Pot<int16_t> pot
    @endcode
*/
template<typename Sample>
class Pot {
    public:

    Pot (PinName AInPin, channel_t Channel)
        : input (AInPin),
        channel (Channel) {

    }

    /** Polls the pot. */
    inline void Poll (Sample new_value, _::Expression* expr, uint16_t value,
                      Sample min_value, uint6_t max_channel) {
        /// 1. Auto-set the min/max bounds.


        /// 2. Poll,
    }

    /** Prints this object to a terminal. */
    void Print (_::Log& log) {
        log << "Pot";
    }

    private:

    AnalogIn  input;        //< The pot ADC input.
    channel_t channel;      //< The channel in the mixer.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_POT_H
