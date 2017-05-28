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


#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <_Theater/HMI/ControlMatrix.hpp>

using namespace _Theater::HMI;

TEST_GROUP (_Theater_HMI_ControlMatrixTests)
{
    void setup ()
    {
        printf ("Testing Controls::ControlMatrix class.");
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (_Theater_HMI_ControlMatrixTests, _Theater_HMI_ControlMatrixTests_1)
{
    printf ("Testing const char* print (I2P::Terminal& slot):\n");
    ControlMatrix cm (5, 2);
    cm.print ();

    printf ("Testing setControl ()");

    DMXControl  testControl0_0 ("DMX Control 1");
    MIDIControl testControl1_0 ("MIDI Control 1");
    DMXButton   testControl2_0 ("DMX Button 1");
    MIDIButton  testControl3_0 ("MIDI Button 1");
    MacroButton testControl4_0 ("Macro Button 1");

    DMXControl  testControl0_1 ("DMX Control 2");
    MIDIControl testControl1_1 ("MIDI Control 2");
    DMXButton   testControl2_1 ("DMX Button 2");
    MIDIButton  testControl3_1 ("MIDI Button 2");
    MacroButton testControl4_1 ("Macro Button 2");

    cm.setControl (0, 0, &testControl0_0);
    cm.setControl (1, 0, &testControl1_0);
    cm.setControl (2, 0, &testControl2_0);
    cm.setControl (3, 0, &testControl3_0);
    cm.setControl (4, 0, &testControl4_0);

    cm.setControl (0, 1, &testControl0_1);
    cm.setControl (1, 1, &testControl1_1);
    cm.setControl (2, 1, &testControl2_1);
    cm.setControl (3, 1, &testControl3_1);
    cm.setControl (4, 1, &testControl4_1);

    cm.print ();

    printf ("Done testing Controls::ControlMatrix class _KabukiTheater_! :-)");
}
