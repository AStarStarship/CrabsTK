/** Kabuki Toolkit
    @file    ~/projects/test_kabuki_toolkit.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "global.h"

int main (int args_count, char** args) {
    std::cout << '|';
    for (int i = 79; i > 0; --i) {
        std::cout << '-';
    }
    std::cout << "\n Kabuki Toolkit Tests\n";
    for (int i = 79; i > 0; --i) {
        std::cout << '-';
    }
    std::cout << "\n\n";
    
    return CommandLineTestRunner::RunAllTests (args_count, args);
}
