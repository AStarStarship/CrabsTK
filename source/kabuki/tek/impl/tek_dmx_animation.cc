/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/dmx_animation.cc
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

#include "../include/dmx_animation.h"

namespace kabuki { namespace tek {

DMXAnimation::DMXAnimation (uint kNumChannels, uint NumChases, uint NumVariants)
    : param1_ (0),
    param2_ (0),
    param3_ (0),
    param4_ (0),
    current_chase_ (0),
    current_chase_length_ (0),
    current_variant_ (0),
    num_channels_ (kNumChannels),
    num_chases_ (NumChases),
    num_variants_ (NumVariants),
    data_ (0) {
    // Nothing to do here.
}

void DMXAnimation::ResetAnimation () {

}

void DMXAnimation::SetChaseNumber (uint Index) {
    if (Index > num_chases_)
        return;

    current_chase_ = Index;
}

void DMXAnimation::SetChannelData (uint16_t channel, byte data) {

}


void DMXAnimation::SendScene () {
    //put  (scene, );
}

void DMXAnimation::SetChase (byte chaseNumber) {

}

void DMXAnimation::RandomizeSceneVariant () {

}

void DMXAnimation::SetRGBColor (uint16_t channel, color_t color) {
    const uint16_t rGBChannelCutoff = 512 - 9;
    //< We need 9 bytes in our cue for this operation.

    if (channel > rGBChannelCutoff) {
        // There are not 9 channels to shoot this data out too!
        return;
    }

    //if  (sceneIndex >= sceneLength - 9) {
          // This should never happen in your animation code.
          //Emptyscene (); //< This pauses the animation to flush out the buffer.

    // The DMX protocol consist of a 9-bit channel, and an 8-bit data value.

    // Decode the channel into LSB and MSB.
    byte channelLSB = (byte)channel,
        channelMSb = (byte)(channel & 0x100) >> 8;

    // The cue is a FIFO, so cue up the data, followed by the channel LSB, then MSB.

    // The following is an unrolled loop for greater efficiency at runtime.

    byte* tempByte = 0;//scene[sceneIndex++];
    // Cue red value
    *tempByte = (byte)color & 0xFF;
    *(++tempByte) = channelLSB;
    *(++tempByte) = channelMSb;

    if (channelLSB == 0xFF) { 
        // If the LSB is about to max out to 255, then we need to increment the MSB.
        ++channelMSb;
        channelLSB = 0;
    } else
        ++channelLSB;

    // Cue green value
    *(++tempByte) = (byte)(color & 0xFF00) >> 8;
    *(++tempByte) = channelLSB;
    *(++tempByte) = channelMSb;

    if (channelLSB == 0xFF) {
        // If the LSB is about to max out to 255, then we need to increment the MSB.
        ++channelMSb;
        channelLSB = 0;
    } else
        ++channelLSB;

    // Cue blue value
    *(++tempByte) = (byte)(color & 0xFF0000) >> 16;
    *(++tempByte) = channelLSB;
    *(++tempByte) = channelMSb;
}

void DMXAnimation::SetColor (uint16_t channel, byte Red, byte Green, byte Blue) {
    if (channel > 511 - 3) {
        // We need 3 channels for an RGB color.
        printf ("Error in SetColor  (uint16_t, color_t): channel out of range!\r\n");
        return;
    }

    data_[channel] = Red;
    data_[channel + 1] = Green;
    data_[channel + 2] = Blue;
}

void DMXAnimation::SetColor (uint16_t channel, color_t color) {
    if (channel > 511 - 3) {
        // We need 3 channels for an RGB color.
        printf ("Error in SetColor  (uint16_t, color_t): channel out of range!\r\n");
        return;
    }

    data_[channel    ] = (byte)(color & 0xFF0000) >> 16; //< Red
    data_[channel + 1] = (byte)(color & 0xFF00) >> 8;    //< Green
    data_[channel + 2] = (byte)(color & 0xFF);           //< Blue
}

/*
void DMXAnimation::SetColor  (uint16_t channel, color_t color)
{
    if  (channel > 511 - 3)             //< We need 3 channels for an RGB color.
    {
        printf ("Error in SetColor  (uint16_t, color_t): channel out of range!\r\n");
        return;
    }
    float alphaValue =  (float) ((color & 0xFF000000) >> 24),
    Red     =  (float) ((color & 0xFF0000) >> 16),
    Green   =  (float) ((color & 0xFF00) >> 8),
    Blue    =  (float)  (color & 0xFF);

    data[channel    ] =  (byte)  (Red   * alphaValue);    //< Red
    data[channel + 1] =  (byte)  (Green * alphaValue);    //< Green
    data[channel + 2] =  (byte)  (Blue  * alphaValue);    //< Blue
}

void DMXAnimation::SetAllColors  (color_t color)
{
    for  (int i = 0; i < numBasePairs; ++i)
    {
        SetColor  (nucleobaseB[i], color);
        SetColor  (nucleobaseT[i], color);
    }

    for  (int i = 0; i < numBasePairs; ++i)
    {
        SetColor  (backboneB[i], color);
        SetColor  (backboneT[i], color);
    }
}
*/

void DMXAnimation::SetAllColors (color_t color) {
    /*
    for  (int i = 0; i < numChannels; ++i)
    {
    SetColor  (nucleobaseB[i], color);
    SetColor  (nucleobaseT[i], color);
    }

    for  (int i = 0; i < numBasePairs; ++i)
    {
    SetColor  (backboneB[i], color);
    SetColor  (backboneT[i], color);
    }
    */
}

color_t DMXAnimation::IncreaseBrightness (color_t color, byte brightness_change) {
    uint32_t brightness = (color & 0xFF000000) >> 24;
    //< Mask off MSB and shift into LSB.
    color = (color & 0x00FFFFFF);
    //< Clear out the alpha Channel
    brightness = (brightness + brightness_change) << 24;
    //< Add the brightness_change and shift back  (overflow ignored).
    //printf ("Increasing brightness to %hhu.\n", brightness);
    printf ("Increasing brightness to %u.\n", brightness);
    return color & brightness;
}

color_t DMXAnimation::DecreaseBrightness (color_t color, byte brightness_change) {
    uint32_t brightness = (color & 0xFF000000) >> 24;
    //< Mask off MSB and shift into LSB.
    color = (color & 0x00FFFFFF);
    //< Clear out the alpha Channel
    brightness = (brightness - brightness_change) >> 24;
    //< Subtract the brightness_change and shift back  (underflow ignored).
    //printf ("Decreasing brightness to %hhu.\n", brightness);
    printf ("Decreasing brightness to %u.\n", brightness);
    return color & brightness;
}

void DMXAnimation::RandomizeVariant () {
    srand (time (0));
    current_variant_ = (byte)rand () % 256;
}

const _::Operation* DMXAnimation::Star (char_t index, _::Expression* expr) {
    switch (index) {
        case 0: return _::NumOperations (0);
    }

    return Query?Enquery ("DMXAnimation", "_tek::DMX"):InvalidIndex ();
}

}       //< namespace tek
}       //< namespace kabuki
