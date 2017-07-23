/** Kabuki Theater
    @version 0.x
    @file   /.../Source/KabukiTheater-Impl/_/TESTS/UtilsTests.cpp
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

#include <_\utils.h>

using namespace _;

TEST_GROUP (UtilsTests)
{
    void setup () {
        printf ("\n+ Running UtilsTests...\n");
    }

    void teardown () {
        printf ("  UtilsTests completed.\n");
        //system ("PAUSE");
    }
};

TEST (UtilsTests, HexTest)
{
    printf ("  - Running HexTest...\n");
    for (int i = 0; i < 16; ++i)
    {
        int value = ToByte (NibbleToLowerCaseHex (i));
        CHECK_EQUAL (i, value)
        value = ToByte (NibbleToUpperCaseHex (i));
        CHECK_EQUAL (i, value)
   }
    for (int i = 0; i < 256; ++i)
    {
        int value = ToByte (ToLowerCaseHex (i));
        CHECK_EQUAL (i, value)
        value = ToByte (ToUpperCaseHex (i));
        CHECK_EQUAL (i, value)
    }
}
