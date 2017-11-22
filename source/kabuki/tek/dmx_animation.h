/** kabuki::tek
    @file    ~/source/kabuki/tek/include/dmx_animation.h
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

#ifndef KABUKI_TEK_DMX_ANIMATION_H
#define KABUKI_TEK_DMX_ANIMATION_H

#include "config.h"

namespace kabuki { namespace tek {

/*< @fn    UpdateHandler  (uint, uint, byte);
    @brief C function Update handler for DMXAnimation.
    @code
    uint ExampleAnimation1  (num_channels, num_chases, num_variants);
    @endcode */
typedef void (*UpdateHandler)  (uint Chase, uint Frame, byte Scene);

//inline bool IsInvalidFrame  (uint Index);

/** A simple DMX animation with variant.

*/
class DmxAnimation {
      public:
    
    enum {
        FPS         = 30    //< The target frames per second.
    };
    
    /** Simple default constructor. */
    DmxAnimation  (uint num_channels, uint num_chases, uint num_variants);

    /** Resets the animation to the first frame */
    void ResetAnimation ();
    
    /** Sets the currentChase to the given Index. */
    void SetChaseNumber  (uint Index);
    
    /** Function sends out DMX data for the given channel. */
    void SetChannelData  (uint16_t Channel, byte value);
    
    /** Function sets an RGB color starting at the given channel. */
    void SetRGBColor  (uint16_t Channel, color_t value);
    
    /** Randomizes the currentVariant */
    void RandomizeVariant ();

    void SendScene ();
    void SetChase  (byte chaseNumber);
    void RandomizeSceneVariant ();
    void SetColor  (uint16_t channel, byte red, byte green, byte blue);
    void SetColor  (uint16_t channel, color_t color);
    void SetAllColors  (color_t color);
    color_t IncreaseBrightness  (color_t color, byte brightness_change);
    color_t DecreaseBrightness  (color_t color, byte brightness_change);
    
    protected:

    byte  param1_,              //< Animation parameter 1/Red.
          param2_,              //< Animation parameter 2/Green.
          param3_,              //< Animation parameter 3/Blue.
          param4_;              //< Animation parameter 4/White.
    uint  current_chase_,       //< Current chase number.
          current_chase_length_,//< Length of the current chase.
          current_variant_,     //< Current variant.
          num_channels_,        //< The number of DMX channels.
          num_chases_,          //< The number of DMX chases.
          num_variants_;        //< The number of variations of the animation.
    byte* data_;                //< Pointer to the DMX data for the current scene.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_DMX_ANIMATION_H
