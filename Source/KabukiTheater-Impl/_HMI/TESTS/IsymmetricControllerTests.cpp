/** Kabuki Toolkit
    @file    /.../KabukiToolkit/_Dev/DMXDevice.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

#include <_Theater/HMI/IsymmetricController.hpp>;
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (ControlArrayTests)
{
    void setup ()
    {
        printf ("Testing IsymmetricController class _KabukiToolkit...");
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (ControlArrayTests, ProjectTests1)
{
    printf ("Testing IsymmetricController class _KabukiToolkit\n");
    _::printLine ('~');

    IsymmetricController controller ();

    printf ("Testing ToString ().");
    printf (controller.print ());

    //testISCS.addTemplate ();

    printf ("Done testing IsymmetricController class _KabukiToolkit");
}

class _KabukiToolkit Template;
class _KabukiToolkit MacroControl;

class _KabukiToolkit iSymmetricControlSurfaceTests : public UnitTest
{
    public:
    ISymmetricControlSurfaceUnitTest () : UnitTest () {}

    void runTest ()
    {
    }
};

}   //< namespace HUD

