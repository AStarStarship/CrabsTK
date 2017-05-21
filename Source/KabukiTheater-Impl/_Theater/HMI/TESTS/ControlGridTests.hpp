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

#if _DebugKabukiTheater
class ControlGridTests : public UnitTest
{
    public:
    ControlGridTests () : UnitTest ("Testing Controls::ControlGrid class _KabukiTheater_.") {}

    void runTest ()
    {
        auto controlGrid = ControlGrid  (5, 2);

        beginTest ("Testing const char* print (I2P::Terminal& slot):\n");
        logMessage  (controlGrid.print (I2P::Terminal& slot));

        beginTest ("Testing setControl ()");

        auto testControl0_0 = DMXControl  ("DMX Control 1");
        auto testControl1_0 = MIDIControl ("MIDI Control 1");
        auto testControl2_0 = DMXButton   ("DMX Button 1");
        auto testControl3_0 = MIDIButton  ("MIDI Button 1");
        auto testControl4_0 = MacroButton ("Macro Button 1");

        auto testControl0_1 = DMXControl  ("DMX Control 2");
        auto testControl1_1 = MIDIControl ("MIDI Control 2");
        auto testControl2_1 = DMXButton   ("DMX Button 2");
        auto testControl3_1 = MIDIButton  ("MIDI Button 2");
        auto testControl4_1 = MacroButton ("Macro Button 2");

        controlGrid.setControl  (0, 0, &testControl0_0);
        controlGrid.setControl  (1, 0, &testControl1_0);
        controlGrid.setControl  (2, 0, &testControl2_0);
        controlGrid.setControl  (3, 0, &testControl3_0);
        controlGrid.setControl  (4, 0, &testControl4_0);

        controlGrid.setControl  (0, 1, &testControl0_1);
        controlGrid.setControl  (1, 1, &testControl1_1);
        controlGrid.setControl  (2, 1, &testControl2_1);
        controlGrid.setControl  (3, 1, &testControl3_1);
        controlGrid.setControl  (4, 1, &testControl4_1);

        logMessage  (controlGrid.print (I2P::Terminal& slot));


        logMessage ("Done testing Controls::ControlGrid class _KabukiTheater_! :-)");
    }
};
static ControlGridTests testUnit_ControlGrid;
#endif  //< _DebugKabukiTheater
    
}   //< namespace _HMI
