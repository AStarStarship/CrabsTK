/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_HMI/TESTS/Module.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

namespace _HMI {

#if DEBUG
/** Unit test for the DMXControl class _KabukiTheater_ */
class _KabukiTheater_ MIDIControlTests : public UnitTest
{
    public:

    MIDIControlTests ()
    :   UnitTest ("Testing Controls::MIDIControl\n" + ConsoleLine ('~')) 
    {
        
    }

    void runTest ()
    {
        MIDIControl controlA ("MIDI Control A");
        logMessage  (controlA.print (I2P::Terminal& slot));

        beginTest ("Testing setCC ()");
        controlA.setCC  (127);
        expect  (controlA.cc () = =  127);
        controlA.setCC  (128);
        expect  (controlA.cc () = =  127);
        controlA.setCC (-1);
        logMessage ("controlA:\n" + controlA.print (I2P::Terminal& slot));
        expect  (controlA.cc () = =  0);
        logMessage ("Done testing Controls::MIDIControl class _KabukiTheater_. :-)\n");
    }
};
static MIDIControlTests testUnit_MIDIControl;

#endif // DEBUG
    
}   //< namespace _HMI
