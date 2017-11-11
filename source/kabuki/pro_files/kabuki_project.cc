/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/pro_files/kabuki_tests.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "../../pro/include/project.h"

using namespace kabuki::pro;
using namespace std;

class ChineseRoom : public Project {
    public:

    ChineseRoom () :
        Project ("Chinese-Room") {
        PrintBreak ("<\n< Creating-Chinese Project...\n", '-');

        AddSchedule ("Uniprinter_Class");
        Schedule* s = GetSchedule ("Uniprinter_Class");
        if (!s) {
            cout << "<\n< Error: finding Uniprinter_Class schedule.\n<\n";
            return;
        }
        s->Add (new Task ("Finish unit tests for scanner. Test "
                          "memory alignment."));
        
        AddSchedule ("Book_Class");
        s = GetSchedule ("Book_Class");
        if (!s) {
            cout << "<\n< Error: finding Book_Class schedule.\n<\n";
            return;
        }
        s->Add (new Task ("Finish Dictionary data functions. Use same "
                          "memory alignment algorithm as the scanner."));
        Print ();
        PrintBreak ("<\n< Done creating Chinese-Room Project...\n<", '-');
    }
};

class KabukiToolkit: public Project
{
    public:
    
    KabukiToolkit () :
        Project ("kabuki")
    {
        PrintBreak ("<\n< Creating kabuki Project...\n<", '-');

        AddSchedule ("Examples");
        Schedule* s = GetSchedule ("Examples");
        if (!s) {
            cout << "<\n< Error: finding Examples schedule.\n<\n";
            return;
        }
        s->Add (new Task ("Make Serial example for mbed, MinGW, OS X, and "
                          "Linux."));
        
        Print ();
        PrintBreak ("<\n< Done creatingK kabuki Project...\n<", '-');
    }
};
