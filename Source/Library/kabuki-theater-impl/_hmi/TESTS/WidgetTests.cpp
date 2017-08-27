/** Kabuki Theater
    @file    /.../Source-Library/KabukiTheater-Impl/_Theater/HMI/TESTS/WidgetTests.hpp
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

#include <_Theater\HMI\Widget.hpp>
#include <CppUTest\CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>
#include <stdio.h>

using namespace _Theater::HMI;

TEST_GROUP (_Theater_HMI_Widget_Tests)
{
    void setup ()
    {
        // Init stuff
        printf ("Running Running _Theater::HMI::Widget tests...");
    }

    void teardown ()
    {
        printf ("\n\nRunning _Theater::HMI::Widget tests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (_Theater_HMI_Widget_Tests, _Theater_HMI_Widget_Tests_1)
{
    printf ("Testing Controls::Widget class _KabukiTheater_\n");
    _::printLine ('~');

    Widget w ("Test Widget");

    printf ("Testing const char* ToString ()\n");

    w.print ();

    printf ("Testing insertPage  (const char*&)\n");

    const char* pageNameString;
    char pageIndex = '1';

    for (int i = 0; i < 6; ++i)
    {
        pageNameString = "Page " + pageIndex;
        printf ("Inserting Page %s...", pageNameString);
        CHECK (!w.insertPage (pageNameString), "Error: Failed to insert page " + pageNameString + "!\n");
        ++pageIndex;
    }

    printf (w.ToString ());

    printf ("Testing contains  (const char*&)...");

    CHECK (!w.contains ("Page 1"), "Error: w does not contians \"Page 1\"");

    printf ("Testing deletePage  (int)... ");

    int debugCode;

    debugCode = w.deletePage (1);

    CHECK (debugCode == 1, "Error: w.deletePage  (1) returrned invalid debugCode: " + debugCode);

    w.deletePage (2);
    w.deletePage (3);
    debugCode = w.deletePage (3);
    CHECK (debugCode == 1, "Error: debugCdode was not equal to 1: " + debugCode);
    w.deletePage (3);
    w.deletePage (1);
    w.deletePage (2);
    w.deletePage (2);
    w.deletePage (2);
    w.deletePage (1);
    w.deletePage (1);
    w.deletePage (1);
    w.print ();

    printf ("Testing inserting duplicate page names...");

    for (int i = 0; i < 5; ++i)
    {
        debugCode = w.insertPage ("Untitled");
        printf ((i + 1) + ".) Debug code: " + debugCode);
    }
    debugCode = w.insertPage ("Untitled");

    w.deletePage ("Untitled 4");
    debugCode = w.insertPage ("Untitled");

    printf (w.ToString ());


    printf ("Testing copy constructor...");
    Widget wc (w);
    wc.print ();

    printf ("Done testing Widget class _KabukiTheater_");
}
