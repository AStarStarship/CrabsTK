/** Kabuki Pro
    @file   ~/Kabuki_Pro/Source/Console/main.cpp
    @author Cale McCollough
    @brief  Demo app inputs a todo list of things to do to prepare for the 2017
            solar eclipse.
*/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "tests.h"

using namespace std;

int main () {
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
    system ("PAUSE");
}


