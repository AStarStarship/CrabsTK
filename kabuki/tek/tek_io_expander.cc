/** kabuki::tek
    @file    ~/source/kabuki/tek/io_expander.cc
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

#include "io_expander.h"

using namespace std;

namespace kabuki { namespace tek {

IoExpander::IoExpander (byte* buffer, byte num_in_bytes, byte num_out_bytes,
			            PinName mosi_pin, PinName miso_pin, PinName clock_pin,
			            PinName strobe_pin, uint32_t frequency) :
	spi_           (mosi_pin, miso_pin, clock_pin, strobe_pin),
	num_in_bytes_  (num_in_bytes),
	num_out_bytes_ (num_out_bytes),
	in_bytes_      (reinterpret_cast<byte*> (buffer)),
	out_bytes_     (in_bytes_ + num_in_bytes) {
    spi_.frequency (frequency);
}

byte IoExpander::GetNumInBytes () {
	return num_in_bytes_;
}

byte IoExpander::GetNumOutBytes () {
	return num_out_bytes_;
}

//void IoExpander::Attach (Controller* controller) {
//	if (controller == 0)
//		return;
//}

//void IoExpander::SetDevice (byte device_number) {
//}

/** Gets the digital input byte at the given index. */
inline void IoExpander::Update (byte index) {
	if ((index < num_in_bytes_) && (index < num_out_bytes_))
		in_bytes_[index] = spi_.write (out_bytes_[index]);
	else if (index < num_in_bytes_)
		in_bytes_[index] = spi_.write (0);
	else
		spi_.write (out_bytes_[index]);
}

}       //< namespace tek
}       //< namespace kabuki
