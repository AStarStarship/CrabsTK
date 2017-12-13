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

TEST_GROUP (ID_TESTS) {
    void setup () {
        std::cout << "\n|  + Running kabuki::id tests...\n|";
    }

    void teardown () {
        std::cout << "\n| kabuki::id tests completed.";
        PrintLine ('-');
    }
};
/*
TEST (ID_TESTS, UserListTests) {
    cout << "\n| Testing User class...";

    static const char* test_users[7][2] = { { "user_1", "password" },
                                            { "user_2", "pears"    },
                                            { "user_3", "apples"   },
                                            { "user_4", "olives"   },
                                            { "user_5", "crazy"    },
                                            { "user_6", "lunatic"  },
                                            { "user_1", "olives"   } };
    UserList users;
    int32_t session;
    uint64_t public_key;

    const char* handle,
              * password;
    cout << "\n|\n| Creating valid test users...";

    for (int32_t i = 0; i < 6; ++i) {
        handle   = test_users[i][0];
        password = test_users[i][1];
        cout << "\n| Attempting to add \"" << handle << "\":\""
             << password << "\" at index:" << i;
        session = users.Register (handle, password);
        users.Print ();
        cout << "\n| index: " << session;
        CHECK_EQUAL (i + 1, session)
    }

    users.Print ();

    cout << "\n|\n| Creating invalid test users...";

    for (int i = 6; i < 7; ++i) {
        handle = test_users[i][0];
        password = test_users[i][1];
        cout << "\n| Attempting to add \"" << handle << "\":\""
             << password << "\" at index:" << i;
        session = users.Register (handle, password);
        cout << "\n| result:" << session;
        CHECK_EQUAL (-1, session)
    }

    users.Print ();

    cout << "\n|\n| Testing UserList::LogIn users...";

    for (int32_t i = 0; i < 5; ++i) {
        handle = test_users[i][0];  
        password = test_users[i][1];
        cout << "\n| Attempting to log in \"" << handle << "\":\""
            << password << "\" at index:" << i;
        public_key = users.LogIn (handle, password);
        cout << "\n| result:" << public_key;
    }

    cout << "\n|\n| Done testing UserList ({:->})";
}*/
