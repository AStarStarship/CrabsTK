/** kabuki::tek
    @file    ~/source/kabuki/tek/tek/switch.h
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
 
#ifndef KABUKI_TEK_BOUNCYSWITCH_H
#define KABUKI_TEK_BOUNCYSWITCH_H

#include "config.h"
#include <mbed.h>

namespace kabuki { namespace tek {
    
/** A software debounced switch. */
class Switch: public _::Operation {
    public:
    
    /** A software debcounced switch. */
    Switch (PinName din_pin);
    
    /** Script operations. */
    const _::Operation* Star (char_t index, _::Expr* expr);
    
    private:
    
    DigitalIn input_;    //< The DIN pin.
};

class SwitchOp: public _::Operation {
    public:

	SwitchOp (Switch* sw);

	virtual const _::Operation* Star (char_t index, _::Expr* expr);

    private:

	Switch* object_; //< The Switch.
};		//< SwitchOp
}       //< namespace tek
}       //< namespace kabuki
#endif	//< KABUKI_TEK_BOUNCYSWITCH_H
