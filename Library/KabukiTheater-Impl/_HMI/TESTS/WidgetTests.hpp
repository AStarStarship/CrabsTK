/** Kabuki Theater
    @file    /.../KabukiTheater-Library/KabukiTheater-Impl/_HMI/TESTS/WidgetTests.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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


class WidgetUnitTest : public UnitTest
{
    public:

    WidgetUnitTest () : UnitTest ("Running Controls::Control tests...") {}

    void runTest ()
    {
        beginTest ("Testing Controls::Widget class _KabukiTheater\n" + ConsoleLine ('~'));

        Widget testWidget = Widget ("Test Widget");

        beginTest ("Testing const char* ToString ()");

        LogMessage  (testWidget.ToString ());

        LogMessage ("Testing insertPage  (const char*&)");

        const char* pageNameString;
        char pageIndex = '1';

        for  (int i = 0; i < 6; ++i)
        {
            pageNameString = "Page " + pageIndex;
            LogMessage ("Inserting Page " + const char*  (pageIndex) + "...");
            expect (!testWidget.insertPage  (pageNameString), "Error: Failed to insert page " + pageNameString + "!\n");
            ++pageIndex;
        }

        LogMessage  (testWidget.ToString ());

        beginTest ("Testing contains  (const char*&)...");

        expect (!testWidget.contains ("Page 1"), "Error: testWidget does not contians \"Page 1\"");

        beginTest ("Testing deletePage  (int)... ");

        int debugCode;

        debugCode = testWidget.deletePage  (1);

        expect  (debugCode == 1, "Error: testWidget.deletePage  (1) returrned invalid debugCode: " + debugCode);

        testWidget.deletePage  (2);
        testWidget.deletePage  (3);
        debugCode = testWidget.deletePage  (3);
        expect  (debugCode == 1, "Error: debugCdode was not equal to 1: " + debugCode);
        testWidget.deletePage  (3);
        testWidget.deletePage  (1);
        testWidget.deletePage  (2);
        testWidget.deletePage  (2);
        testWidget.deletePage  (2);
        testWidget.deletePage  (1);
        testWidget.deletePage  (1);
        testWidget.deletePage  (1);
        LogMessage  (testWidget.ToString ());

        LogMessage ("Testing inserting duplicate page names...");

        for  (int i = 0; i < 5; ++i)
        {
            debugCode = testWidget.insertPage ("Untitled");
            LogMessage ((i + 1) + ".) Debug code: " + debugCode);
        }
        debugCode = testWidget.insertPage ("Untitled");

        testWidget.deletePage ("Untitled 4");
        debugCode = testWidget.insertPage ("Untitled");

        LogMessage  (testWidget.ToString ());


        beginTest ("Testing copy constructor...");
        Widget testWidgetCopy  (testWidget);
        LogMessage  (testWidgetCopy.ToString ());

        LogMessage ("Done testing Widget class _KabukiTheater");
    }
};