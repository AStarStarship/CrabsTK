/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_project.cc
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

#include "../pro/include/module.h"

using namespace kabuki::_;
using namespace kabuki::pro;

class ChineseRoom : public Project
{
    public:

    ChineseRoom () :
        Project ("Chinese-Room")
    {
        PrintBreak ("<\n< Creating-Chinese Project...\n", '-');

        AddSchedule ("Expression_Class");
        Schedule* s = GetSchedule ("Expression_Class");
        if (!s) {
            cout << "<\n< Error: finding Expression_Class schedule.\n<\n";
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
