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

#include "global.h"

using namespace std;
using namespace _;
using namespace kabuki::id;

TEST_GROUP (ID_TESTS)
{
    void setup () {
        std::cout << "\n|  + Running kabuki::id tests...\n|";
    }

    void teardown () {
        std::cout << "\n| kabuki::id tests completed.";
        PrintLine ("|", '-');
    }
};

TEST (ID_TESTS, UserListTests) {
    cout << "\n| Testing User class...";

    UserList users;
    CHECK_EQUAL (1, users.Add ("user_1"));
    CHECK_EQUAL (-1, users.Add ("user_1"));
    CHECK_EQUAL (1, users.Add ("user_2"));

    users.Print ();

    system ("PAUSE");
}
