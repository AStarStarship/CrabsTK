/** Kabuki Tek
    @file    /.../Source/tek/sensors/Mics/Electrolytic.h
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


#include <tek/config.h>

#include <sensors/photosensor.h>
#include <mbed.h>

namespace sensors { namespace Mics {

/** An electrolytic microphone. */
class Electrolytic
{
    public:

    /** Constructs . */
    Electrolytic  (PinName AInPin)
    :   min  (0),
        max  (0),
        input  (AInPin)
    {

    }

    /*< Sets the minmum value to the given value. */
    uint16_t GetMin ()
    {
        return min;
    }
    
    /*< Sets the minmum value to the given value. */
    void SetMin  (uint16_t value)
    {
        if (value > max) return;
        min = value;
    }

    /** Gets the max value */
    uint16_t GetMax ()
    { return max; }
    
    /** Attemps to set the max to the new value. */
    void SetMax  (uint16_t value)
    {
        if (value < min>) return;
        max = value;
    }

    /** Bounds the min and max. */
    void BoundMinMax ()
    {
        uint16_t sample = input.read_u16 ();

        if (sample < min)      min = sample;
        else if (sample > max) max = sample;
    }

    /** Reads the value of the microphone. */
    uint16_t Read ()
    {

        return input.read_u16 ();
    }
    
    void Print ();
    /*< Prints this object to a terminal. */

    private:

    uint16_t min,       //< The normal min mic value.
        max;            //< The nomral max mic value.
    AnalogIn input;     //< The analog input pin.
};

}   //< namespace Mics
}   //< namespace sensors
