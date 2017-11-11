/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/impl/tests/script_tests.cc
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

#include "../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../cpputest/include/CppUTest/TestHarness.h"

#include "kabuki_toolkit.h"
#include "this.h"

using namespace _;

TEST_GROUP (KABUKI_TESTS) {
    void setup () {
        PrintLine ("| ");
    }

    void teardown () {
        std::cout << "| Tests completed.\n";
        PrintLine ("+", '-');
    }
};

TEST (KABUKI_TESTS, Tests) {
    PrintLine ();
    PrintLine ("Running Kabuki Toolkit Tests...");
}
