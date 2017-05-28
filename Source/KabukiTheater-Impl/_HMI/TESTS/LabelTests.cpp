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

#include <_Theater/HMI/Label.hpp>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (ControlArrayTests)
{
    void setup ()
    {
        printf ("Controls::Label Class.\n");
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (ControlArrayTests, ProjectTests1)
{
    const char* invalidString = "Invalid label ~",
        * validString1 = "Valid label 1",
        *validString2 = "Valid label !@%^",
        *validString3 = "Valid label &* ()";

    Label validLabel1 (validString1);
    Label validLabel2 (validString1);
    Label validLabel3 (validString3);
    Label invalidLabel (invalidString);

    printf ("Testing valid string input.");

    printf ("Testing int isValid  (const char* &)");
    printf ("isValidString(validString1): %b", isValidLabel (validString1));
    CHECK (isValidLabel (validString1) == Valid);
    printf ("Testing invalid string input.");
    CHECK (isValidLabel (invalidString) != Valid);
    CHECK (isValidLabel ("") != 0);
    CHECK (isValidLabel ("123456789012345678901") != 0);

    printf ("Testing int compare  (const char* &)\n");
    invalidLabel.print ();
    printf ("Testing valid comparisons...");
    CHECK (!validLabel1.Compare (validString1));
    printf ("Testing invalid comparisons...");
    CHECK (validLabel1.compare (validString2) != Comperable);
    CHECK (validLabel2.compare (validString3) != Comperable);

    printf ("Attempting to setLabel to the invalidString");
    //auto expectedLabel = "Test string 1";
    validLabel1.setLabel (invalidString);
    CHECK (!validLabel1.Compare (invalidLabel));
    printf ("Setting the label to validString3");
    validLabel1.setLabel (validString3);
    CHECK (validLabel1.getLabel () == validString3);

    printf ("Done testing Controls::Label class _KabukiTheater_.");
}
