/** Kabuki Theater
    @file    /.../Source-Library/KabukiTheater-Impl/_Theater/HMI/TESTS/Module.hpp
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

#include <_Theater/HMI/DMXButton.hpp>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (ControlArrayTests)
{
    void setup ()
    {
        printf ("Controls::DMXButton class _KabukiTheater_\n");
        _::printLine ('-');
        putchar ('\n');
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (ControlArrayTests, ProjectTests1)
{
    DMXButton a ("DMX Button A");

    printf ("Testing const char* print (I2P::Terminal& slot)...");
    a.print ();

    printf ("Testing void processPress () and void processDepress ()");
    a.press ();
    CHECK (a.isPressed ());
    a.depress ();
    CHECK (!a.isPressed ());

    printf ("Testing void toggle ()");
    a.setButtonAction (ButtonAction::Latching);
    a.toggle ();
    CHECK (a.getValue () == a.getMaxValue ());
    a.toggle ();
    CHECK (a.getValue () == a.getMinValue ());
}
