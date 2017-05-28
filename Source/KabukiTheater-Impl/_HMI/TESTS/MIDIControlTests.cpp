/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HMI/TESTS/MIDIControlTests.hpp
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

#include <_Theater/HMI/MIDIControl.hpp>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (ControlArrayTests)
{
    void setup ()
    {
        printf ("Testing Controls::MIDIControl\n");
        _::printLine ('~');
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (_Theater_HMI_Control_Tests, ProjectTests1)
{
    MIDIControl a ("MIDI Control A");
    a.print ();

    printf ("Testing setCC ()");
    a.setCC (127);
    CHECK (a.getCC () == 127);
    a.setCC (128);
    CHECK (a.getCC () == 127);
    a.setCC (-1);
    printf ("a:\n");
    a.print ();
    CHECK (a.getCC () == 0);
    printf ("Done testing Controls::MIDIControl class _KabukiTheater_. :-)\n");
}
