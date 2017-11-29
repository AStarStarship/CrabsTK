/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/impl/tek_rgb_led.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (r). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include "rgb_led.h"
#include "../script/color.h"

namespace kabuki { namespace tek {
    
/** Creates an RgbLed in the off position. */
RgbLed::RgbLed  (color_t color, offset_t red_offset, offset_t green_offset,
		         offset_t blue_offset):
    color_        (color       ),
	red_offset_   (red_offset  ),
	green_offset_ (green_offset),
	blue_offset_  (blue_offset )
{
}

color_t RgbLed::GetColor () {
    return color_;
}

void RgbLed::SetColor (color_t value) {
	color_ = value;
}

byte RgbLed::GetRed () {
	return (byte)color_;
}

void RgbLed::SetRed (color_t value) {
    color_t color = 0xffff00ff; //< Mask for removing red channel.
    color &= color_;            //< Mask off the red value.
    color |= value << 8;        //< Add the new red channel.
}

byte RgbLed::GetGreen () {
	return (byte)(color_ >> 8);
}

void RgbLed::SetGreen (color_t value) {
    color_t color = 0xffff00ff; //< Mask for removing green channel.
    color &= color_;            //< Mask off the green value.
    color |= value << 8;        //< Add the new green channel.
}

byte RgbLed::GetBlue () {
	return (byte)(color_ >> 16);
}

void RgbLed::SetBlue (color_t value) {
    color_t color = 0xff00ffff; //< Mask for removing blue channel.
    color &= color_;            //< Mask off the blue value.
    color |= value << 8;        //< Add the new blue channel.
}

byte RgbLed::GetBrightness () {
	return (byte)(color_ >> 24);
}

void RgbLed::SetBrightness (color_t value) {
    color_t color = 0x00ffffff; //< Mask for removing brightness channel.
    color &= color_;            //< Mask off the brightness value.
    color |= value << 8;        //< Add the new brightness channel.
}

offset_t RgbLed::GetRedOffset () {
	return red_offset_;
}

offset_t RgbLed::GetGreenOffset () {
	return green_offset_;
}

offset_t RgbLed::GetBlueOffset () {
	return blue_offset_;
}


}       //< namespace tek
}       //< namespace kabuki
