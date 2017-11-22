/** kabuki::tek
    @file    ~/source/kabuki/tek/include/flying_fader.h
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
 
#ifndef KABUKI_TEK_FLYING_FADER_H
#define KABUKI_TEK_FLYING_FADER_H

#include "config.h"

namespace kabuki { namespace tek {

/** A bank of one or more groups of flying faders.
    Flying faders on most mixers have pages full of controls, and work similar 
    to the RotaryEncoder class.
*/
class FlyingFader {
    public:
    
    /** Constructs a blank flying fader. */
    FlyingFader ();
    
    /** Prints this object to a console. */
    void Print ();
    
    const _::Operation* Star (char_t index, _::Expression* expr);

    private:


};

class FlyingFaderOp {
    public:

	FlyingFaderOp (FlyingFader* ff);

    virtual const _::Operation* Star (char_t index, _::Expression* expr);

    private:

    FlyingFader* ff_; //< Pointer to the selected FlyingFader object.
};

}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_FLYING_FADER_H
