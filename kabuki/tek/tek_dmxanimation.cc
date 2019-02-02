/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_dmxanimation.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "c_dmxanimation.h"

namespace _ {

DmxAnimation::DmxAnimation(UI4 kChannelCount, UI4 NumChases, UI4 NumVariants)
    : param1_(0),
      param2_(0),
      param3_(0),
      param4_(0),
      current_chase_(0),
      current_chase_length_(0),
      current_variant_(0),
      channel_count_(kChannelCount),
      num_chases_(NumChases),
      num_variants_(NumVariants),
      data_(0) {
  // Nothing to do here.
}

void DmxAnimation::ResetAnimation() {}

void DmxAnimation::SetChaseNumber(UI4 Index) {
  if (Index > num_chases_) return;

  current_chase_ = Index;
}

void DmxAnimation::SetChannelData(UI2 channel, UI1 data) {}

void DmxAnimation::SendScene() {
  // put  (scene, );
}

void DmxAnimation::SetChase(UI1 chaseNumber) {}

void DmxAnimation::RandomizeSceneVariant() {}

void DmxAnimation::SetRGBColor(UI2 channel, CRGBAUI4 color) {
  const UI2 rGBChannelCutoff = 512 - 9;
  //< We need 9 bytes in our cue for this operation.

  if (channel > rGBChannelCutoff) {
    // There are not 9 channels to shoot this data out too!
    return;
  }

  // if  (sceneIndex >= sceneLength - 9) {
  // This should never happen in your animation code.
  // Emptyscene (); //< This pauses the animation to flush out the buffer.

  // The DMX protocol consist of a 9-bit channel, and an 8-bit data value.

  // Decode the channel into LSB and MSB.
  UI1 channelLSB = (UI1)channel, channelMSb = (UI1)(channel & 0x100) >> 8;

  // The cue is a FIFO, so cue up the data, followed by the channel LSB, then
  // MSB.

  // The following is an unrolled loop for greater efficiency at runtime.

  UI1* tempByte = 0;  // scene[sceneIndex++];
  // Cue red value
  *tempByte = (UI1)color & 0xFF;
  *(++tempByte) = channelLSB;
  *(++tempByte) = channelMSb;

  if (channelLSB == 0xFF) {
    // If the LSB is about to max out to 255, then we need to increment the MSB.
    ++channelMSb;
    channelLSB = 0;
  } else
    ++channelLSB;

  // Cue green value
  *(++tempByte) = (UI1)(color & 0xFF00) >> 8;
  *(++tempByte) = channelLSB;
  *(++tempByte) = channelMSb;

  if (channelLSB == 0xFF) {
    // If the LSB is about to max out to 255, then we need to increment the MSB.
    ++channelMSb;
    channelLSB = 0;
  } else
    ++channelLSB;

  // Cue blue value
  *(++tempByte) = (UI1)(color & 0xFF0000) >> 16;
  *(++tempByte) = channelLSB;
  *(++tempByte) = channelMSb;
}

void DmxAnimation::SetColor(UI2 channel, UI1 Red, UI1 Green, UI1 Blue) {
  if (channel > 511 - 3) {
    // We need 3 channels for an RGB color.
    printf("Error in SetColor  (UI2, CRGBAUI4): channel out of range!\r\n");
    return;
  }

  data_[channel] = Red;
  data_[channel + 1] = Green;
  data_[channel + 2] = Blue;
}

void DmxAnimation::SetColor(UI2 channel, CRGBAUI4 color) {
  if (channel > 511 - 3) {
    // We need 3 channels for an RGB color.
    printf("Error in SetColor  (UI2, CRGBAUI4): channel out of range!\r\n");
    return;
  }

  data_[channel] = (UI1)(color & 0xFF0000) >> 16;   //< Red
  data_[channel + 1] = (UI1)(color & 0xFF00) >> 8;  //< Green
  data_[channel + 2] = (UI1)(color & 0xFF);         //< Blue
}

/*
void DmxAnimation::SetColor  (UI2 channel, CRGBAUI4 color)
{
    if  (channel > 511 - 3)             //< We need 3 channels for an RGB color.
    {
        printf ("Error in SetColor  (UI2, CRGBAUI4): channel out of range!\r\n");
        return;
    }
    FP4 alphaValue =  (FP4) ((color & 0xFF000000) >> 24),
    Red     =  (FP4) ((color & 0xFF0000) >> 16),
    Green   =  (FP4) ((color & 0xFF00) >> 8),
    Blue    =  (FP4)  (color & 0xFF);

    data[channel    ] =  (UI1)  (Red   * alphaValue);    //< Red
    data[channel + 1] =  (UI1)  (Green * alphaValue);    //< Green
    data[channel + 2] =  (UI1)  (Blue  * alphaValue);    //< Blue
}

void DmxAnimation::SetAllColors  (CRGBAUI4 color)
{
    for  (SI4 i = 0; i < numBasePairs; ++i)
    {
        SetColor  (nucleobaseB[i], color);
        SetColor  (nucleobaseT[i], color);
    }

    for  (SI4 i = 0; i < numBasePairs; ++i)
    {
        SetColor  (backboneB[i], color);
        SetColor  (backboneT[i], color);
    }
}
*/

void DmxAnimation::SetAllColors(CRGBAUI4 color) {
  /*
  for  (SI4 i = 0; i < numChannels; ++i)
  {
  SetColor  (nucleobaseB[i], color);
  SetColor  (nucleobaseT[i], color);
  }

  for  (SI4 i = 0; i < numBasePairs; ++i)
  {
  SetColor  (backboneB[i], color);
  SetColor  (backboneT[i], color);
  }
  */
}

CRGBAUI4 DmxAnimation::IncreaseBrightness(CRGBAUI4 color, UI1 brightness_change) {
  UI4 brightness = (color & 0xFF000000) >> 24;
  //< Mask off MSB and shift into LSB.
  color = (color & 0x00FFFFFF);
  //< Clear out the alpha Channel
  brightness = (brightness + brightness_change) << 24;
  //< Add the brightness_change and shift back  (overflow ignored).
  // printf ("Increasing brightness to %hhu.\n", brightness);
  cout << "\n| Increasing brightness to " << brightness << '.';
  return color & brightness;
}

CRGBAUI4 DmxAnimation::DecreaseBrightness(CRGBAUI4 color, UI1 brightness_change) {
  UI4 brightness = (color & 0xFF000000) >> 24;
  //< Mask off MSB and shift into LSB.
  color = (color & 0x00FFFFFF);
  //< Clear out the alpha Channel
  brightness = (brightness - brightness_change) >> 24;
  //< Subtract the brightness_change and shift back  (underflow ignored).
  cout << "\n| Decreasing brightness to " << brightness << '.';
  return color & brightness;
}

void DmxAnimation::RandomizeVariant() {
  srand(time(0));
  current_variant_ = (UI1)rand() % 256;
}

}  // namespace _
