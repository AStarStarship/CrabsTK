/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/_tek/DMX/DMXAnimation.cpp
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

#include <_tek/DMX/DMXAnimation.h>

namespace _tek { namespace DMX {

DMXAnimation::DMXAnimation  (uint NumChannels, uint NumChases, uint NumVariants)
:   param1  (0),
    param2  (0),
    param3  (0),
    param4  (0),
    currentChase  (0),
    currentChaseLength  (0),
    currentVariant  (0),
    numChannels  (NumChannels),
    numChases    (NumChases),
    numVariants  (NumVariants),
    data  (0)
{
    // Nothing to do here.
}

void DMXAnimation::ResetAnimation ()
{

}

void DMXAnimation::SetChaseNumber  (uint Index)
{
    if  (Index > numChases)
        return;

    currentChase = Index;
}

void DMXAnimation::SetChannelData  (uint16_t channel, byte data)
{

}
    

void DMXAnimation::SendScene ()
{
    //put  (scene, );
}

void DMXAnimation::SetChase  (byte chaseNumber)
{

}

void DMXAnimation::RandomizeSceneVariant ()
{

}

void DMXAnimation::SetRGBColor  (uint16_t channel, color_t color)
{
    const uint16_t rGBChannelCutoff = 512 - 9;  //< We need 9 bytes in our cue for this operation.
    
    if  (channel > rGBChannelCutoff)             //< There are not 9 channels to shoot this data out too!
        return;
    
    //if  (sceneIndex >= sceneLength - 9)          //< This should never happen in your animation code.
     //   Emptyscene ();                          //< This pauses the animation to flush out the buffer.
    
    // The DMX prototol consist of a 9-bit channel, and an 8-bit data value.
    
    // Decode the channel into LSB and MSB.
    byte channelLSB =  (byte) channel,
        channelMSb =  (byte)  (channel & 0x100) >> 8;
    
    // The cue is a FIFO, so cue up the data, followed by the channel LSB, then MSB.
    
    // The following is an unrolled loop for greater efficiency at runtime.
    
    byte* tempByte = 0;//scene[sceneIndex++];
    // Cue red value
    *tempByte =  (byte) color & 0xFF;
    * (++tempByte) = channelLSB;
    * (++tempByte) = channelMSb;
    
    if  (channelLSB == 0xFF) // If the LSB is about to max out to 255, then we need to increament the MSB.
    {
        ++channelMSb;
        channelLSB = 0;
    }
    else
        ++channelLSB;
    
    // Cue green value
    * (++tempByte) =  (byte)  (color & 0xFF00) >> 8;
    * (++tempByte) = channelLSB;
    * (++tempByte) = channelMSb;
    
    if  (channelLSB == 0xFF) // If the LSB is about to max out to 255, then we need to increament the MSB.
    {
        ++channelMSb;
        channelLSB = 0;
    }
    else
        ++channelLSB;
    
    // Cue blue value
    * (++tempByte) =  (byte)  (color & 0xFF0000) >> 16;
    * (++tempByte) = channelLSB;
    * (++tempByte) = channelMSb;
}

void DMXAnimation::SetColor  (uint16_t channel, byte Red, byte Green, byte Blue)
{
    if  (channel > 511 - 3)             //< We need 3 channels for an RGB color.
    {
        printf ("Error in SetColor  (uint16_t, color_t): channel out of range!\r\n");
        return;
    }
    
    data[channel    ] = Red;
    data[channel + 1] = Green;
    data[channel + 2] = Blue;
}

void DMXAnimation::SetColor  (uint16_t channel, color_t color)
{
    if  (channel > 511 - 3)             //< We need 3 channels for an RGB color.
    {
        printf ("Error in SetColor  (uint16_t, color_t): channel out of range!\r\n");
        return;
    }
    
    data[channel    ] =  (byte)  (color & 0xFF0000) >> 16;   //< Red
    data[channel + 1] =  (byte)  (color & 0xFF00) >> 8;      //< Green
    data[channel + 2] =  (byte)  (color & 0xFF);             //< Blue
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

void DMXAnimation::SetAllColors  (color_t color)
{
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

color_t DMXAnimation::IncreaseBrightness  (color_t color, byte brightnessChange)
{
    uint32_t brightness =  (color & 0xFF000000) >> 24;   //< Mask off MSB and shift into LSB.
    color =  (color & 0x00FFFFFF);                       //< Clear out the alpha Channel
    brightness =  (brightness + brightnessChange) << 24; //< Add the brightnessChange and shift back  (overflow ignored).
    //printf ("Increasing brightness to %hhu.\n", brightness);
    printf ("Increasing brightness to %u.\n", brightness);
    return color & brightness;
}

color_t DMXAnimation::DecreaseBrightness  (color_t color, byte brightnessChange)
{
    uint32_t brightness =  (color & 0xFF000000) >> 24;         //< Mask off MSB and shift into LSB.
    color =  (color & 0x00FFFFFF);                           //< Clear out the alpha Channel
    brightness =  (brightness - brightnessChange) >> 24;     //< Subtract the brightnessChange and shift back  (underflow ignored).
    //printf ("Decreasing brightness to %hhu.\n", brightness);
    printf ("Decreasing brightness to %u.\n", brightness);
    return color & brightness;
}

void DMXAnimation::RandomizeVariant ()
/*< Randomizes the currentVariant */
{
    srand  (time  (0));
    currentVariant =  (byte) rand () % 256;
}

byte DMXAnimation::getState ()
{
    return 0;
}

const char* DMXAnimation::setState (byte Value)
{
    return 0;
}

const char* DMXAnimation::Op (byte index, Uniprinter* io)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("DMXAnimation", "_tek::DMX"): InvalidIndex ();
}

}   //< namespace DMX
}   //< namespace _tek
