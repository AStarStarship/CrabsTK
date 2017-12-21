/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/tests/test_kabuki_data.cc
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
#include "global.h"

#include "../source/kabuki/data/global.h"

using namespace _;
using namespace std;

TEST_GROUP (SCRIPT_DATA_TESTS) {

    void setup () {
        std::cout << Text ().Line ();
        std::cout << "\n|  + Running kabuki::data tests...";
    }

    void teardown () {
        std::cout << "\n| kabuki::data completed.";
    }
};

TEST (SCRIPT_DATA_TESTS, AllTests) {
    
}
