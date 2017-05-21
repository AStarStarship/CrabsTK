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
/** Unit test for the DMXControl class _KabukiTheater_. */
class _KabukiTheater_ DMXControlTests : public UnitTest
{
    public:
    
    DMXControlTests () : UnitTest ("Controls::DMXControl") {}

    void runTest () 
    {
        logMessage  (ConsoleLine ('-'));
        DMXControl controlA ("DMX Control A");

        logMessage ("Printing controlA.print (I2P::Terminal& slot)\n" + controlA.print (I2P::Terminal& slot));

        logMessage ("Testing copy constructor\n");
        DMXControl controlB  (controlA);
        logMessage ("Printing controlB.print (I2P::Terminal& slot)\n" + controlB.print (I2P::Terminal& slot));
        logMessage ("Testing int Compare  (const Control&)");

        expect  (controlA.Compare  (controlB) == 0);
        int CompareValue = controlA.Compare  (controlB);
        logMessage ("Testing int Compare  (const Control&): " + CompareValue);
        expect  (CompareValue == 0);

        beginTest ("Testing valid inputs...");

        controlA = DMXControl ("", 1, 1, 0, 255, 8);

        logMessage ("Printing controlA.print (I2P::Terminal& slot)...\n" + controlA.print (I2P::Terminal& slot));

        controlA.setLabel ("DMX Control B");
        expect  (controlA.label () == "DMX Control B");
        controlA.setInitValue  (35);
        logMessage  (controlA.print (I2P::Terminal& slot));
        expect  (controlA.initValue () == 35);
        controlA.setLSBValue  (66);
        expect  (controlA.lSBValue () == 66);
        controlA.setMaxValue  (245);
        expect  (controlA.maxValue () == 245);
        controlA.setMinValue  (70);
        expect  (controlA.minValue () == 70);

        beginTest ("Retesting copy constructor");

        controlB = DMXControl  (controlA);
        CompareValue = controlA.Compare  (controlB);
        expect (!CompareValue);

        beginTest ("Testing invalid input...");

        controlA.setWordSize  (5);
        logMessage ("controlA.wordSize (): " + const char*  (controlA.wordSize ()));
        expect  (controlA.wordSize () == 7);
        controlA.setChannel  (555);
        expect  (controlA.channel () == DMXControl::NumChannels);
        controlA.setChannel (-5);
        expect  (controlA.channel () == 0);
        controlA.setChannel  (1);
        expect  (controlA.channel () == 1);

        logMessage ("Done testing DMXControl class _KabukiTheater_");
    }
};
static DMXControlTests testUnit_DMXControl;
#endif  //< _DebugKabukiTheater
    
}   //< namespace _HMI
