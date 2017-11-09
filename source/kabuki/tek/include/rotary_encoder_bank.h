/** kabuki::tek
    @file    ~/source/kabuki/tek/include/rotary_encoder_bank.h
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
 
#ifndef KABUKI_TEK_ROTARY_ENCODER_BANK_H
#define KABUKI_TEK_ROTARY_ENCODER_BANK_H

#include "led.h"
#include "button.h"

namespace kabuki { namespace tek {

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
    Rings use 15 segments. All LED encoder rings use the same wiring patterns 
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
        
    /** Constructor.
        @param NumKnobs     The number of rotary encoders.
        @param NumBanks		The number of ??? banks.
        @param StartChannel	The starting channel on the mixer.
        @param Rows			The number of
        @param Columns */
    RotaryEncoderBank  (byte NumKnobs, uint NumBanks, uint StartChannel, byte* Rows, byte* Columns);
    
    /** Switches the bank to new value. */
    inline void SwitchBank  (byte value);
    
    /** Cycles the bank. */
    inline void CycleBank ();
    
    /** Frame update handler for the LEDs.
        This handler has a counter that counts from 0-255 in an infinite loop. 
        Each time around the loop, all of the LEDs get turned on. */
    inline void Update  (int16_t* channel, int16_t* minValues, int16_t* maxValues, byte* spiOutBytes, int16_t row,
        int8_t count);
    
    /** Prints this object to a terminal. */
    void Print (_::Log& log);
    
    private:
    
    byte    num_knobs_,       //< Number of knobs  (rotary encoders).
            num_banks_,       //< Number of banks.
            num_buttons_,     //< Number of buttons.
            wiring_config_,   //< RotaryEncoderBank wiring configuration.
            current_bank_;    //< Currently selected bank.
    uint    start_channel_,   //< Start channel in the mixer.
            current_channel_; //< Current bank start channel in the mixer.
    byte  * rows_,            //< Pointer to the array of columns bits.
          * columns_;   	  //< Pointer to the array of columns bits.
    Button* buttons_;         //< Pointer to the array of buttons.
    LED   * lights_;          //< Pointer to the array of LEDs.
    
    /** Updates the mixer channel number LED ring. */
    inline void UpdateChannel  (uint16_t index, int16_t* channels, int16_t* minValues, int16_t* maxValues, 
        byte* spiOutBytes, uint16_t row, byte count);
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_ROTARY_ENCODER_BANK_H
