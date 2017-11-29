/** kabuki::tek
    @file    ~/source/kabuki/tek/include/electrolytic_mic.h
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
 
#ifndef KABUKI_TEK_ELECTROLYTIC_MIC_H
#define KABUKI_TEK_ELECTROLYTIC_MIC_H

#include "photosensor.h"

namespace kabuki { namespace tek {

/** An electrolytic microphone. */
class ElectrolyticMic {
    public:

    /** Constructs . */
	ElectrolyticMic (PinName pin);

    /** Sets the minimum value to the given value. */
    uint16_t GetMin ();

    /** Sets the minimum value to the given value. */
    void SetMin (uint16_t value);

    /** Gets the max value */
    uint16_t GetMax ();

    /** Attempts to set the max to the new value. */
    void SetMax (uint16_t value);

    /** Bounds the min and max. */
    void BoundMinMax ();

    /** Reads the value of the microphone. */
    uint16_t Read ();

    /** Prints this object to a terminal. */
    void Print ();

    private:

    uint16_t min_,       //< The normal min mic value.
             max_;       //< The normal max mic value.
    AnalogIn input_;     //< The analog input pin.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_ELECTROLYTIC_MIC_H
