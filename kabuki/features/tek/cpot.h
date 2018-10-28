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

#include "config.h"

namespace kabuki { namespace tek {

/** A potentiometer.
    
    @code
    Pot<int16_t> pot
    @endcode
*/
class KABUKI Pot {
    public:

	/** Constructs a pot from the given ADC pin and assigned channel number. */
    Pot (ch_t channel, PinName adc_pin);

    /** Polls the pot. */
    inline void Poll (uint16_t new_value, _::Expr* expr, uint16_t value,
    		          uint16_t min_value, uint16_t max_channel);

    /** Script operations. */
    const _::Operation* Star (char_t index, _::Expr* expr);

    /** Prints this object to a terminal. */
    void Print (_::Log& log);

    private:

    ch_t     channel_; //< Mixer channel number.
    AnalogIn ain_;     //< Pot ADC input.
};       //< class Pot

class PotOp : public _::Operand {
    public:

    /** Constructs a Pot Operation. */
	PotOp (Pot* object);

    /** Script operations. */
    virtual const _::Operation* Star (char_t index, _::Expr* expr);

    private:

    Pot* object_;  //< The Pot.
};      //< class PotOp
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_POT_H
