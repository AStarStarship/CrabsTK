/** kabuki::tek
    @file    ~/source/kabuki/tek/include/io_expander.h
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
 
#ifndef KABUKI_TEK_IO_EXPANDER_H
#define KABUKI_TEK_IO_EXPANDER_H

#include "config.h"

namespace kabuki { namespace tek {

/** A controller for multiple SPI ports that have varying number of bytes.
    
    Different controller configurations will have a different number of input 
    and output shift registers, but they all share the same strobe_. At the 
    beginning of each cycle the strobe_ clocks in the data into the shift
    registers then shifts in the input signals in, which outputs the LED data
    for that frame. This is accomplished by finding the longest shift register 
    chain of all the SPI ports and a counter that increments in a loop. This 
    index is checked to see if it is greater than the number of input and output
    shift registers, and data is only sent and received on the correct byte
    indexes.
    
    @code
    char OutputData[5];
    SPIPort controller8 (D11, D12, D13, D10, 1, &OutputData[5], 4, &OutputData[0]);
    @endcode
*/
class IoExpander {
    public:

    /** Simple constructor.

        @param num_in_bytes  The number of input shift registers.
        @param num_out_bytes The number of output shift registers.
        @param num_rows      The number of rows. */
    IoExpander (byte* buffer, byte num_in_bytes, byte num_out_bytes,
                PinName mosi_pin, PinName miso_pin, PinName clock_pin,
                PinName strobe_pin, uint32_t frequency);

    /** Gets the number of input registers. */
    byte GetNumInBytes ();

    /** Gets the number of output registers. */
    byte GetNumOutBytes ();

    /** Attaches the controller to to this spi_. */
    //void Attach (Controller* controller);

    /** Switches to the given row number. */
    //void SetDevice (byte device_number);

    /** Gets the digital input byte at the given index. */
    void Update (byte index);

    private:

    SPI   spi_;           //< SPI spi_.
    byte  num_in_bytes_,  //< Number of in bytes.
	      num_out_bytes_; //< Number of out bytes.
    byte* in_bytes_,      //< Input buffer.
        * out_bytes_;     //< Output buffer.
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_IO_EXPANDER_H
