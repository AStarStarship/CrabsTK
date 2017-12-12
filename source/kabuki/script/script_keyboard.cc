/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_keyboard.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#include <stdafx.h>

#include "keyboard.h"

using namespace std;

namespace _ {

#if USING_SCRIPT_KEYBOARD

char KeyboardChar (const char* header) {
    cout << header;
    char c = getchar ();
    cout << '\n';
    return c;
}

int KeyboardInt (const char* header) {
    int number;
    cout << header;
    cin.clear ();
    cin >> number;
    return number;
}

float KeyboardFloat (const char* header) {
    float number;
    cout << header;
    cin.clear ();
    cin >> number;
    return number;
}

void KeyboardText (const char* header, char* target, char* target_end) {
    if (!target) {
        return;
    }
    if (target > target_end) {
        return;
    }
    cout << header;
    cin.get (target, target_end - target, '\n');
    cin.clear ();
    cin.ignore (target_end - target, '\n');
}
#endif  //< USING_SCRIPT_KEYBOARD
}       //< namespace _
