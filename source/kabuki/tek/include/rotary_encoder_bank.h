/** Kabuki Tek
    @file    /.../KabukiTek/sensors/RotaryEncoderBank.h
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


#include <KabukiTek\Config.h>

#include "../Displays/LED.h"
#include "Button.h"

namespace tek { namespace sensors {

typedef enum 
{
    NoButtons = 0,
    ButtonsTop,
    ButtonsBottom 
} LEDRingConfig;

class RotaryEncoderBank
/** A bank of rotary encoders with LED rings and optional bank switch button  (s). 
    Rotary encoder groups can have one or more banks of controls such as Aux 
    sends, etc. Banks are sequentially in the mixer starting at the 
    startChannel.
    
    Switching Banks
    ---------------
    Switching banks can be done using a single button that cycles through the 
    banks, or there can be a dedicated button per bank, or there can be no 
    buttons at all. The user inputs the number of buttons, and if it is one and 
    then numButtons is more than one, then that mode is chosen. If there is no 
    
    Wiring Configuration
    --------------------
    All Blue-Storm LED Encoder 
    Rings use 15 segments. All LED encoder rings use the same wiriing patterns 
    that makes multiplexing code simple. ColA has 8 rows, and ColB has 7 rows. 
    The design can also include a 16th LED either on top or on bottom.
    
    LEDRingConfig.NoButtons
    Row8------------D8--------------SS------------D8
    Row7---------D7----D9-----------SS---------D7----D9
    Row6-------D6--------D10--------SS-------D6--------D10
    Row5------D5-----------D11------SS------D5-----------D11
    Row4-----D4-------------D12-----SS-----D4-------------D12
    Row3------D3-----------D13------SS------D3-----------D13
    Row2--------D2-------D14--------SS--------D2-------D14
    Row1----------D1---D15----------SS----------D1---D15
    Col1__________/___/_________________________/____/
    Col2__________/___/_________________________/
    Col 2K________/___/
    Col  (2k + 1)__/
    
    TODO:   There are three configurations of the LED encoder ring, the 
        ButtonsOnTop, and ButtonsOnBottom configurations.
    
    The power for the LED comes from the shift register, so only one LED per 
    column may be on at one time. The rows will always be the same on every 
    control surface, but the columns will vary significantly. The rows will 
    always be on a single shift register but the columns will be stored as a 
    
    There will be both monochrome and RGB LEDs on the same buss, so the byte 
    array will be shared by multiple.
***/
{
      public:
        
    RotaryEncoderBank  (byte NumKnobs, uint NumBanks, uint StartChannel, byte* Rows, byte* Columns);
    /*< Constructor.
        @param NumKnobs     The number of rotatry encoders.
     	@param NumBanks		The number of ??? banks.
		@param StartChannel	The starting channel on the mixer.
		@param Rows			The number of
		@param Columns */
    
    inline void SwitchBank  (byte value);
    /*< Switchs the bank to new value. */
    
    inline void CycleBank ();
    /*< Cycles the bank. */
    
    inline void Update  (int16_t* channel, int16_t* minValues, int16_t* maxValues, byte* spiOutBytes, int16_t row,
        int8_t count);
    /*< Frame update handler for the LEDs.
        This handler has a counter that counts from 0-255 in an infinite loop. 
        Each time around the loop, all of the LEDs get turned on. */
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    byte numKnobs,          //< The number of knobs  (rotary encoders).
        numBanks,           //< The number of banks.
        numButtons,         //< The number of buttons.
        wiringConfig,       //< The RotaryEncoderBank wiring configuration.
        currentBank;
    
    uint startChannel,      //< The start channel in the mixer.
        currentChannel;     //< The current bank start channel in the mixer.

    byte* rows,             //< The array of columns bits.
        * columns;   	    //< The array of columns bits.
    
    Button* buttons;        //< The array of buttons.
    
    LED* lights;            //< The
    
    inline void UpdateChannel  (uint16_t index, int16_t* channels, int16_t* minValues, int16_t* maxValues, 
        byte* spiOutBytes, uint16_t row, byte count);
    /*< Updates the mixer channel number LED ring. */
};
}   //< namespace sensors
}   //< namespace tek

