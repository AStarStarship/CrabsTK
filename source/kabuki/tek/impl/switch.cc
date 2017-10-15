/** Kabuki Tek
    @file    /.../Source/kabuki-tek-impl/sensors/photosensor.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <sensors/switch.h>

namespace tek { namespace sensors {

Switch::Switch  (word Pin):
    input ((PinName)Pin)
{
    /// Nothing to do here!
}

Switch::~Switch () {}

const Member* Switch::Op (byte index, Uniprinter* io)
{
	static const Member this_member = {
        "Switch", NumMembers (0), FirstMember ('A'), 
		"A software debounced switch."
    };
    if (!index) return this_member;
    switch (index)
    {
		case 'A': {
			static const Member m_A = {
                "Read", 
                Params<1, SI4> (),
                Params<NIL>,
                "Reads the switch state."
            };
			if  (!io) return m_A;

			int temp = input;
			byte switchState =  (byte) temp;

			return Write  (io, txHeader, &switchState);
		}
    }
    return nullptr;
}

}   //< namespace sensors
}   //< namespace tek
