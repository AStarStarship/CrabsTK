/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HMI/TESTS/MacroButtonTests.cpp
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

#include <_Theater/HMI/MacroButton.hpp>
#include <CppUTest\CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (ControlArrayTests)
{
    void setup ()
    {
        printf ("Testing Controls::MacroButton class _KabukiTheater_:\n");
        _::printLine ('~');
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (ControlArrayTests, ProjectTests1)
{
    auto macroA = MacroButton ("Macro Button A");

    printf ("Testing const char* print (I2P::Terminal& slot)...");
    printf (macroA.print (I2P::Terminal& slot));
    printf ("Testing add  (Parameter<int>*)");
    macroA.add (new DMXControl ("Macro 1"));
    macroA.add (new MIDIControl ("Macro 2"));
    macroA.add (new DMXButton ("Macro 3"));
    macroA.add (new MIDIButton ("Macro 4"));
    printf ("Added " + const char*  (macroA.numControls ()) + " test controls.\n" + macroA.print (I2P::Terminal& slot));
    printf ("Testing void trigger ()");
    macroA.trigger ();
    printf ("Testing Parameter<int>* remove  (index);");
}
