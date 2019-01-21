/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cdmxreceiver.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_ELECTROLYTICMIC
#define INCLUDED_KABUKI_FEATURE_TEK_ELECTROLYTICMIC 1

#include "cphotosensor.h"

namespace _ {

/* An electrolytic microphone. */
class ElectrolyticMic {
    public:

    /* Constructs . */
	ElectrolyticMic (PinName pin);

    /* Sets the minimum value to the given value. */
    UI2 GetMin ();

    /* Sets the minimum value to the given value. */
    void SetMin (UI2 value);

    /* Gets the max value */
    UI2 GetMax ();

    /* Attempts to set the max to the new value. */
    void SetMax (UI2 value);

    /* Bounds the min and max. */
    void BoundMinMax ();

    /* Reads the value of the microphone. */
    UI2 Read ();

    /* Prints this object to a terminal. */
    void Print ();

    private:

    UI2 min_,       //< The normal min mic value.
             max_;       //< The normal max mic value.
    AnalogIn input_;     //< The analog input pin.
};
}       //< namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_ELECTROLYTICMIC
