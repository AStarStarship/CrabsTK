/** Kabuki Tek
    @file    /.../KabukiTek/tek/SpiIoExpander.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
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

#include <KabukiFirmwareToolkit-Config.h>

namespace tek {

class Unicontroller;

/** A controller for multiple SPI ports that have varying number of bytes.
    @param NumInBytes  The number of input shift registers.
    @param NumOutBytes The number of output shift registers.
    @param NumRows     The number of rows.
    
    Different controller configurations will have a different number of input 
    and output shift registers, but they all share the same strobe_. At the 
    Beginning of each cycle, the strobe_ clocks in the data into the shift
    registers, then shifts in the input signals in, which outputting the LED data
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
class SpiIoExpander
{
      public:
    
    /** Simple constructor. */
    SpiIoExpander (PinName MOSIPin, PinName MISOPin, PinName ClockPin, PinName StrobePin, byte NumBytesIn,
        byte NumBytesOut) :
        in_bytes     (0),
        out_bytes    (0),
        num_bytes_in_  (NumBytesIn),
        num_bytes_out_ (NumBytesOut),
        port_        (MOSIPin, MISOPin, ClockPin),
        strobe_      (StrobePin)
    {
    }
    
    /** Gets the number of input registers. */
    byte GetNumInBytes ()
    {
        return num_bytes_in_;
    }

    /** Gets the number of output registers. */
    byte GetNumOutBytes ()
    {
        return num_bytes_out_;
    }
        
    /** Attaches the controller to to this port_. */
    void Attach (Controller* controller)
    {
        if (controller == 0) return;


    }
    
    /** Switches to the given row number. */
    void SetDevice (byte deviceNumber)
    {

    }
    
    /** Gets the digital input byte at the given index. */
    inline void Update (byte index)
    {
        if (index < num_bytes_in_ && index < num_bytes_out_)
            in_bytes[index] = port_.write (out_bytes[index]);
        else if (index < num_bytes_in_)
            in_bytes[index] = port_.write (0);
        else
            port_.write (out_bytes[index]);
    }
    
    private:
    
    byte num_bytes_in_, //< The number of input register bytes.
        num_bytes_out_; //< The number of output register bytes.

    byte* in_bytes,     //< The input buffer.
        * out_bytes;    //< The output buffer.
    
    SPI port_;          //< The SPI port_.
    DigitalOut strobe_; //< The strobe_ pin.
};

}   //< namespace tek

