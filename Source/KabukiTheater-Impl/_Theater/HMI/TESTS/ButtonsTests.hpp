/** Kabuki Theater
    @file       /.../Source/KabukiTheater/HMI/UnitTest.hpp
    @author     Cale McCollough <cale.mccollough@gmail.com>
    @license    Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

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
#include <_Theater/HMI/Label.hpp>
#include <_Theater/HMI/DMXControl.hpp>
#include <_Theater/HMI/MIDIControl.hpp>
#include <_Theater/HMI/DMXButton.hpp>
#include <_Theater/HMI/MIDIButton.hpp>
#include <_Theater/HMI/MacroButton.hpp>
#include <_Theater/HMI/ControlArray.hpp>
#include <_Theater/HMI/ControlGrid.hpp>

namespace _Theater { namespace HMI {

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
static UnitTest testUnit_KabukiTheater__Controls;
}   //< namespace _Theater/HMI
}   //< namespace _Theater

#endif // DUBUG
