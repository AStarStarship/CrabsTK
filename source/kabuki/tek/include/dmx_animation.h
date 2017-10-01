/** Kabuki Tek
    @file    /.../Source/_tek/_tek/dmx/dmx_animation.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <config.h>
#include <color.h>

namespace kabuki { namespace tek { namespace dmx {

typedef void (*UpdateHandler)  (uint Chase, uint Frame, byte Scene);
/*< @function UpdateHandler  (uint, uint, byte);
    @brief C function Update handler for DMXAnimation.
    @code
    uint ExampleAnimation1  (uint
    @endcode
*/

//inline IsInvalidFrame  (uint Index);

/** A simple DMX animation with variant.

*/
class DMXAnimation
{
      public:
    
    enum {
        FPS         = 30    //< The target frames per second.
    };
    
    /** Simple default constructor. */
    DMXAnimation  (uint NumChannels, uint NumChases, uint NumVariants);

    /** Resets the animation to the first frame */
    void ResetAnimation ();
    
    /** Sets the currentChase to the given Index. */
    void SetChaseNumber  (uint Index);
    
    /** Function sends out DMX data for the given channel. */
    void SetChannelData  (uint16_t Channel, byte Value);
    
    /** Function sets an RGB color starting at the given channel. */
    void SetRGBColor  (uint16_t Channel, color_t Value);
    
    /** Randomizes the currentVariant */
    void RandomizeVariant ();

    void SendScene ();
    void SetChase  (byte chaseNumber);
    void RandomizeSceneVariant ();
    void SetColor  (uint16_t channel, byte Red, byte Green, byte Blue);
    void SetColor  (uint16_t channel, color_t color);
    void SetAllColors  (color_t color);
    color_t IncreaseBrightness  (color_t color, byte brightnessChange);
    color_t DecreaseBrightness  (color_t color, byte brightnessChange);
    
    protected:

    byte param1,            //< Animation parameter 1/Red.
        param2,             //< Animation parameter 2/Green.
        param3,             //< Animation parameter 3/Blue.
        param4;             //< Animation parameter 4/White.
    uint currentChase,      //< The current chase number.
        currentChaseLength, //< The length of the current chase.
        currentVariant,     //< Reserved for memory alignment.
        numChannels,        //< The number of DMX channels.
        numChases,          //< The number of DMX chases.
        numVariants;        //< The number of variations of the animation.
    byte* data;             //< Pointer to the DMX data for the current scene.
};
}   //< namespace dmx
}   //< namespace tek
}   //< namespace kabuki

