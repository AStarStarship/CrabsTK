/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HMI/TESTS/Module.hpp
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

#include <_Theater/HMI/DMXControl.hpp>
#include <CppUTest\CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (ControlArrayTests)
{
    void setup ()
    {
        printf ("Controls::DMXControl");
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (ControlArrayTests, ProjectTests1)
{
    _::printLine ('-');
    DMXControl controlA ("DMX Control A");

    printf ("Printing controlA.print (I2P::Terminal& slot)\n");
    controlA.print ();

    printf ("Testing copy constructor\n");
    DMXControl controlB (controlA);
    printf ("Printing controlB.print (I2P::Terminal& slot)\n");
    controlB.print ();
    printf ("Testing int Compare  (const Control&)");

    CHECK (controlA.Compare (controlB) == 0);
    int CompareValue = controlA.compare (controlB);
    printf ("Testing int Compare  (const Control&): " + CompareValue);
    CHECK (CompareValue == 0);

    printf ("Testing valid inputs...");

    controlA = DMXControl ("", 1, 1, 0, 255, 8);

    printf ("Printing controlA.print (I2P::Terminal& slot)...\n");
    controlA.print ();

    controlA.setLabel ("DMX Control B");
    CHECK (controlA.label () == "DMX Control B");
    controlA.setInitValue (35);
    controlA.print ()
    CHECK (controlA.initValue () == 35);
    controlA.setLSBValue (66);
    CHECK (controlA.lSBValue () == 66);
    controlA.setMaxValue (245);
    CHECK (controlA.maxValue () == 245);
    controlA.setMinValue (70);
    CHECK (controlA.minValue () == 70);

    printf ("Retesting copy constructor");

    controlB = DMXControl (controlA);
    CompareValue = controlA.Compare (controlB);
    CHECK (!CompareValue);

    printf ("Testing invalid input...");

    controlA.setWordSize (5);
    printf ("controlA.wordSize (): %i\n", controlA.wordSize ());
    CHECK (controlA.wordSize () == 7);
    controlA.setChannel (555);
    CHECK (controlA.channel () == DMXControl::NumChannels);
    controlA.setChannel (-5);
    CHECK (controlA.channel () == 0);
    controlA.setChannel (1);
    CHECK (controlA.channel () == 1);

    printf ("Done testing DMXControl class _KabukiTheater_");
}
