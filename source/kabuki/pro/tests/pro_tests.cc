/** Kabuki Starship
    @version 0.x
    @file    ~/source/kabuki/pro/pro_files/pro_tests.cpp
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include "../../../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../../../cpputest/include/CppUTest/TestHarness.h"

#include "../include/module.h"

using namespace _;

TEST_GROUP (KABUKI_PRO)
{
    void setup ()
    {
        std::cout << "\n|  + Running kabuki::al tests...\n";
    }

    void teardown ()
    {
        std::cout << "| kabuki::al completed.\n";
    }
};

TEST (KABUKI_PRO, Tests) {
    char* test1 = "test -s \"Task 1\" -d \"Details\" "
                  "-w 25.0 -a 1.0 -g 0.5 ";
                  "-r \"Wrong! All wrong!\" -R \"I got it all right!\" ";
    char* test2 = "test -s \"Task 1\" -d \"Details\" "
                  "-w 25.0 -a 1.0 -g 0.5 ";
                  "-r \"Wrong! All wrong!\" -R \"I got it all right!\" "
                  "-B @3PM -E @4PM -S @3:30PM -P @6:58PM ";
    const char* result;     //< Pointer to the result of a command.


    PrintBreak ("<", '-', 10);
    cout << "<\n< Testing Kabuki Pro...\n<\n";

    //TestDateParser ();

    //root.AddProject (new ChineseRoom ());
    //root.AddProject (new KabukiToolkit ());

    cout << "<\n< Testing Task.Command (char*):const char*\n<\n";
    root.AddProject (new Project ("test"));

    cout << ((result = root.Command ("test -p", stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command (test1, stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command ("test -p", stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command (test2, stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command ("test -p", stack)) == NULL ? "" : result);

    cout << "<\n<\n< Done testing Kabuki Pro...\n";
    PrintBreak ("<", '-');
    system ("PAUSE");
}


