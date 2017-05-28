/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HMI/TESTS/ControlArrayTests.hpp
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

#include <_Theater/HMI/ControlArray.hpp>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (_Theater_HMI_ControlArrayTests)
{
    void setup ()
    {
        printf ("Controls::ControlArray class\n");
        _::printLine ('-');
        putchar ('\n');
    }

    void teardown ()
    {
        printf ("\n\n_Theater::HMI::ControlArray tests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (_Theater_HMI_ControlArrayTests, _Theater_HMI_ControlArrayTests_1)
{
    ControlArray a ();

    printf ("Testing const char* print (I2P::Terminal& slot)...");
    a.print ();

    DMXControl  c1 ("Control 1", 50, 33);
    MIDIControl c2 ("Control 2", 11, 102);

    printf ("testing void setControl  (Control*)");

    a.setControl (0, &c1);
    a.setControl (1, &c2);

    printf ("Added 2 controls to a:\n");
    a.print ();

    DMXControl  c3 ("Control 3", 27, 66);
    MIDIControl c4 ("Control 4", 62, 24);

    a.addControl (&c3);
    a.addControl (&c4);

    printf ("Attempted to add 2 more controls a:\n");
    a.print ();

    DMXButton   c5 ("Control 5", 27, 66);
    MIDIButton  c6 ("Control 6", 62, 24);
    MacroButton c7 ("Control 7");
    c7.add (&c1);
    c7.add (&c2);
    c7.add (&c3);
    c7.add (&c4);
    c7.add (&c5);
    c7.add (&c6);

    a.addControl (&c5);
    a.addControl (&c6);
    a.addControl (&c7);

    printf ("Attempted to add some buttons:\n");
    a.print ();

    printf ("Controls::ControlArray tests completed successfully. :-)");
}
