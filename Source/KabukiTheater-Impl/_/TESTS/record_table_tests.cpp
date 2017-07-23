/** Kabuki Theater
    @version 0.x
    @file    /.../Source/KabukiTheater-Impl/_/TESTS/record_table_tests.cpp
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


        @warning Be careful with unsigned int math because on some systems 
        uint(s) will wrap around instead of clamping to zero or 0x...ff.
*/

#include <CppUTest\CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>

#include <_\record_table.h>

using namespace _;

TEST_GROUP (RecordTableTests)
{
    void setup ()
    {
        PrintLineBreak ("+ Running RecordTableTests...", 5);
    }

    void teardown ()
    {
        std::cout << "  RecordTableTests completed.\n";
        //system ("PAUSE");
    }

};

TEST (RecordTableTests, RecordTableTests)
{
    std::cout << "  - Running RecordTableTest1...\n";
    byte index;
    RecordTable* rt = RecordTableInit (Buffer<0, 128> (), 8, 128);
    
    CHECK (rt != nullptr)
    
    index = Add (rt, "D");
    Print (rt);
    CHECK_EQUAL (0, index)
    CHECK_EQUAL (0, Find (rt, "D"))

    index = Add (rt, "C");
    CHECK_EQUAL (1, index)
    CHECK_EQUAL (0, Find (rt, "D"))
    CHECK_EQUAL (1, Find (rt, "C"))

    index = Add (rt, "B");
    CHECK_EQUAL (2, index)
    CHECK_EQUAL (0, Find (rt, "D"))
    CHECK_EQUAL (1, Find (rt, "C"))
    CHECK_EQUAL (2, Find (rt, "B"))

    index = Add (rt, "A");
    CHECK_EQUAL (3, index)
    CHECK_EQUAL (0, Find (rt, "D"))
    CHECK_EQUAL (1, Find (rt, "C"))
    CHECK_EQUAL (2, Find (rt, "B"))
    CHECK_EQUAL (3, Find (rt, "A"))

    index = Add (rt, "abc");
    CHECK_EQUAL (4, index)
    CHECK_EQUAL (4, Find (rt, "abc"))

    index = Add (rt, "bac");
    CHECK_EQUAL (5, index)
    CHECK_EQUAL (4, Find (rt, "abc"))
    CHECK_EQUAL (5, Find (rt, "bac"))

    index = Add (rt, "cba");
    CHECK_EQUAL (6, index)
    CHECK_EQUAL (4, Find (rt, "abc"))
    CHECK_EQUAL (5, Find (rt, "bac"))
    CHECK_EQUAL (6, Find (rt, "cba"))

    index = Add (rt, "cab");
    CHECK_EQUAL (7, index)
    CHECK_EQUAL (4, Find (rt, "abc"))
    CHECK_EQUAL (5, Find (rt, "bac"))
    CHECK_EQUAL (6, Find (rt, "cba"))
    CHECK_EQUAL (7, Find (rt, "cab"))
    
    index = Add (rt, "test");
    CHECK_EQUAL (index, kRecordTableFull)
}
