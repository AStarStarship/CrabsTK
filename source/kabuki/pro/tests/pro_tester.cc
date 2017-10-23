/** Kabuki Starship
    @file    ~/source/kabuki-impl/TESTS/KabukiTheaterTests.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#include "../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "stdafx.h"

using namespace std;

int main (int args_count, char** args)
{
    cout << "Kabuki Toolkit Unit Test.\n\n";
    /*
    enum {
        BUFFER_SIZE = 256,
    };

    char buffer[BUFFER_SIZE];   //< Buffer for keyboard input.
    const char* result;         //< The result of a command.

    Project project ("Root");       //< The root project.
    ProjectNode root (&project);    //< @todo Replace with array based stack.
    ProjectNode* stack = &root;     //< Stack pointer

    PrintLines (20);
    RunTests (project, stack);

    PrintLines (20);
    cout << "< Welcome to Kabuki Pro: The Personal and Professional Project\n"
        "< Manager.\n"
        "< \n"
        "< Type '?' for help\n";

    //* cursor;         //< Used to scroll through keyboard input string.
    //int index;
    bool loop = true;   //< Loop control variable.
    while (loop) {
        stack->Print ();
        cin.clear ();
        ReadString ("", buffer, BUFFER_SIZE);
        result = project.Command (buffer, stack);
        stack = Pop (stack, result);
        if (stack == nullptr)
            loop = false;
    }
    system ("PAUSE");*/

    return CommandLineTestRunner::RunAllTests (args_count, args);
}
