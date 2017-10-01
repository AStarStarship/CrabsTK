/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/Sensuators/FlyingFader.cpp
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

#include <tek/Sensuators/flying_fader.h>

namespace tek { namespace sensuators {

FlyingFader:: FlyingFader ()
{
}

byte FlyingFader::getState ()
{
    return 0;
}

const char* FlyingFader::setState (byte Value)
{
    return 0;
}

const Member* FlyingFader::Op (byte index, Uniprinter* io)
{
    static const Member this_member = { "FlyingFader", NumMembers (0), 
	                                    FirstMember ('A'), 
										"tek::sensuators") };
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
}

}   //< namespace sensuators
}   //< namespace _
