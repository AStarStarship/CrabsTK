/** Kabuki Starship
    @version 0.x
    @file    ~/source/kabuki/al/al_tests.cpp
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

#include "../../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../../cpputest/include/CppUTest/TestHarness.h"

#include "../../source/kabuki/data/global.h"

using namespace _;

TEST_GROUP (SCRIPT_DATA_TESTS) {

    void setup () {
        Print (Text ().Line ());
        Print ("\n|  + Running kabuki::data tests...");
    }

    void teardown () {
        Print ("\n| kabuki::data completed.");
    }
};

TEST (SCRIPT_DATA_TESTS, AllTests) {
    
}
