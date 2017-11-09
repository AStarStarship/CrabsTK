/** kabuki::tek
    @file    ~/source/kabuki/tek/include/unicontroller.h
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
 
#ifndef KABUKI_TEK_UNICONTROLLER_H
#define KABUKI_TEK_UNICONTROLLER_H

#include "debouncer.h"
#include "led_matrix.h"
#include "rgb_led.h"
#include "button.h"
#include "rotary_encoder.h"
#include "rotary_encoder_bank.h"
#include "pot.h"
#include "io_expander.h"

namespace kabuki { namespace tek {

/** Hardware control surface for midi and DMX.
    A hardware control surface has knobs and Buttons and LEDs. There are
    typically a lot of LEDs, so they must be multiplexed. A control surface can 
    be only hooked up to a microcontroller, or it can be a combination of GPIO 
    and shift registers on SPI ports.
    
    ## Components
    
    * Mixer
    * Potentiometers    } Knobs
    * Rotary Encoders   }
    * Buttons
    * Monochrome LEDs
    * Monochrome LED Rings
    * RGB LEDs
    * Touchscreen 
    
    ## Mixer
    Every knob has it's own mixer channel, which uses 16-bit unsigned integer
    values. Buttons do not inherently have a mixer channel associated with them,
    but additional mixer channels can be created by mapping Buttons to virtual
    mixer channels (eventually). The mixer values are static, and the 
    
    ## Controller and SPIController Mixer Organization
    
    The Controller and SPIController both have the same format, allowing them
    to share code.
    
    [Pots][Rotary Encoders][Buttons][Misc]
    
    ## LED Multiplexing
    
    @code
    Button Buttons[] = {

    }
    @endcode
*/
template<uint kNumChannels, uint NumOutBytes, uint NumInBytes, byte NumSPIPorts,
    uint NumLEDs, uint NumButtons, uint NumEncoders, uint NumLEDRingSets,
    uint NumPots, uint NumPortIns, byte NumGPIPorts, SpioExpander* SPIPorts,
    LED* LEDs, Button* Buttons, RotaryEncoder* Encoders, typename T, 
    Pot<T>* Pots, RotaryEncoderBank* EncoderRings, PortIn* PortsIn>
    class Unicontroller {
        public:

        enum {
            DefaultLEDBrightness = 128,
            MaxMIDIValue = 1024 * 14,
            NumAccellorationCurves = 3
        };

        //static SPIController SPIControllers[_NumSPIControllers];

        /** Default constructor. */
        Unicontroller () :
            //numScreens   (NumScreens),
            brightness (DefaultLEDBrightness),
            redMix (DefaultLEDBrightness),
            greenMix (DefaultLEDBrightness),
            blueMix (DefaultLEDBrightness),
            spiIndex (0),
            pulseCount (0),
            numPulses (0) {
            uint longestChainLength = 0,
                temp;

            for (int i = 0; i < NumSPIPorts; ++i) {
                temp = SPIPorts[i].getNumInBytes ();
                if (temp > longestChainLength)
                    longestChainLength = temp;

                temp = SPIPorts[i].getNumOutBytes ();
                if (temp > longestChainLength)
                    longestChainLength = temp;
            }

            longestSPIChain = longestChainLength;
        }

        /** Links up the points in the SPIPorts to the unified buffer. */
        void ConfigureSPIPorts () {
            byte* inPointer = spiInBytes,
                *outPointer = spiOutBytes;

            for (int i = 0; i < NumSPIPorts; ++i) {

            }
        }

        /** Sets the RGB color mix for the LED. */
        void SetRGBColorBalance (byte r, byte g, byte b) {
            redMix = r;
            greenMix = g;
            blueMix = b;
        }

        /** update interrupt for the SPI port.
            FastDebouncer returns an XOR containing the state of which switches have
            changed. This is useful for triggering function pointers and detecting
            mechanical Encoders. What we will do is store the debounced switch
            results on the stack, and pass a pointer to this array into the Button
            and RotaryEncoder classes. */
        void PollInputs () {
            int i;

            /// poll potentiometers.
            for (i = 0; i < NumPots; ++i)
                Pots[i].poll ();

            int time_us = timer.read_us ();

            /// CPU ALU divide rounds down so add 1 to aligned memory if needed.
            uint numWords = numInBytes / sizeof (int) + ((numInBytes & 3)?1:0);

            int debouncedXOR[numWords];    //<

            for (i = 0; i < numWords; ++i)
                debouncedXOR[i] = debouncers[i].debounce (*((int)DigitalIns));

            /// Debounce General Purpose Input (GPI) Ports.
            for (; i < numWords + NumGPIPorts; ++i)
                debouncedXOR[i] = debouncers[i - numWords].debounce (PortsIn[i - numWords]);

            /// Debounce buttons.
            for (i = 0; i < numButtons; ++i)
                Buttons[i].poll (this);

            /// Debounce rotary Encoders.
            int currentTime = timer.read_us ();
            for (i = 0; i < numEncoders; ++i)
                Encoders[i].poll (this, i, debouncedXOR, currentTime);
        }

        /** Frame update handler for the LEDs. */
        inline void UpdateLEDs () {
            int16_t count = pulseCount;  //< local copy.
            pulseCount = (--count < 0)?numPulses - 1:count; //< Increment counter or loop around.

                                                            //for (int i = numLEDs - 1; i >= 0; --i) Lights[i].update (

            for (int i = 0; i < NumLEDRingSets; ++i)
                EncoderRings[i].Update (channels, minValues, maxValues, count >> 8, count);

        }

        /** Reads and writes bytes to all the SPI ports. */
        inline void UpdateSPIByte () {
            for (int i = NumSPIPorts - 1; i >= 0; --i)
                SPIPorts[i].update (spiIndex);

            if (++spiIndex < longestSPIChain) return;

            spiIndex = 0;
            pollInputs ();
            updateLEDs ();
        }

        /*< Gets the microsecond timer. */
        inline int GetTime_us () { return timer.read_us (); }

        private:

        byte brightness,                //< The monochrome LED brightness.
            redMix,                     //< Red LED mix.
            greenMix,                   //< Green LED mix.
            blueMix;                    //< Blue LED mix.
        uint numInBytes,                //< The number of input bytes.
            numInWords,                 //< The number of debounced words.
            numOutBytes,                //< The number of output bytes.
            numOutWords,                //< The number of output words.
            longestSPIChain,            //< The longest SPI chain length.
            spiIndex,                   //< The index of the current SPI byte.
            numButtons,                 //< The number of Button(s).
            numEncoders,                //< The number of RotaryEncoder(s).
            numFlyingFaderBanks,        //< The number of FlyingFadderBank(s).
            numChannels,                //< The number of channels.
            numLEDRows,                 //< The number of LED rows.
            numLEDs,                    //< The number of LEDs.
            num_rotary_banks_;          //< The number of RotaryEncoderBank(s).
        int16_t pulseCount,              //< The PWM counter.
            numPulses;                   //< The number of PWM pulses in a cycle; rows * 256.
        byte spiOutBytes[NumInBytes],    //< Pointer to the output buffer.
            spiInBytes[NumInBytes];      //< Pointer to the debounced digital inputs.
        int16_t channels[kNumChannels],  //< Pointer to the mixer channel values.
            minValues[kNumChannels], //< Pointer to the min mixer values.
            maxValues[kNumChannels]; //< Pointer to the max mixer values.
        Debouncer<int>* debouncers;     //< General purpose input and shift register debouncers.
        Ticker ticker;                  //< Ticker for updating the controls.
        Timer timer;                    //< Microsecond timer.
};

}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_UNICONTROLLER_H
