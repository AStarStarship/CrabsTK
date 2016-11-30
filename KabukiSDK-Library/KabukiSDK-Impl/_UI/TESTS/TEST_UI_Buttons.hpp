/** iSymmetric Engine
    @file       /.../Source/KabukiSDK/Controls/UnitTest.hpp
    @author     Cale McCollough <cale.mccollough@gmail.com>
    @license    Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                        All rights reserved (R).

        This program is free software: you can redistribute it and/or modify it 
        under the terms of the GNU General Public License as published by the 
        Free Software Foundation, either version 3 ofthe License, or (at your 
        option) any later version.

        This program is distributed in the hope that it will be useful, but 
        WITHOUT ANY WARRANTY; without even the implied warranty of 
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
        General Public License for more details.

        You should have received a copy of the GNU General Public License 
        along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
 
#pragma once

#if DEBUG
#include <_UI/Label.hpp>
#include <_UI/DMXControl.hpp>
#include <_UI/MIDIControl.hpp>
#include <_UI/DMXButton.hpp>
#include <_UI/MIDIButton.hpp>
#include <_UI/MacroButton.hpp>
#include <_UI/ControlArray.hpp>
#include <_UI/ControlGrid.hpp>

namespace _UI {

class UnitTests : public UnitTest
{
    public:

    UnitTests () : UnitTest ("_::Controls Unit Tests") {}

    void runTest ()
    {
        Array<UnitTest*> unitTestsArray;
        unitTestsArray.add (&testUnit_Label);
        unitTestsArray.add (&testUnit_DMXControl);
        unitTestsArray.add (&testUnit_MIDIControl);
        unitTestsArray.add (&testUnit_DMXButton);
        unitTestsArray.add (&testUnit_MIDIButton);
        unitTestsArray.add (&testUnit_MacroButton);
        unitTestsArray.add (&testUnit_ControlArray);
        unitTestsArray.add (&testUnit_ControlGrid);

        UnitTestRunner unitTestRunner;
        unitTestRunner.runTests (unitTestsArray);
    }
};
static UnitTest testUnit_KabukiSDK__Controls;
}   //< namespace _UI

#endif // DUBUG
