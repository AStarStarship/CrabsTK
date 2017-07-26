/** Kabuki Theater
    @version 0.x
    @file   /.../source/kabuki-theater-impl/_/TESTS/BookTests.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

                    All right reserved (R).

    Licensed under the Apache License, Version 2.0 (the "License"); you may
    not use this file except in compliance with the License. You may obtain
    a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <CppUTest\CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>

#include <_\console.h>

using namespace _;

TEST_GROUP (RoomTests)
{
    void setup ()
    {
        printf ("\n+ Running RoomTests...\n");
    }

    void teardown ()
    {
        printf ("  Room tests completed.\n");
        //system ("PAUSE");
    }
};

TEST (RoomTests, RoomTestOne)
{
    printf ("- Running RoomTestOne...\n");
}
