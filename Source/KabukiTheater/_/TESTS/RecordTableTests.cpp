/** The Chinese Room
    @version 0.x
    @file   /.../TESTS/RecordTableTests.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2017 Cale McCollough <calemccollough.github.io>

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.


        @warning Be careful with unsigned int math because on some systems 
        uints will wrap around instead of clamping to zero or 0x...ff.
*/

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>

#include <ChineseRoom\RecordTable.h>
#include <ChineseRoom\Print.h>

using namespace _;

TEST_GROUP(RecordTableTests)
{
    void setup()
    {
        // Init stuff
    }

    void teardown()
    {
        printf("\n\nRecordTableTests completed.\n\n");
        //system("PAUSE");
    }

};

TEST(RecordTableTests, FirstTest)
{
    byte index;
    RecordTable* t;
    t = createRecordTable(8, 48);
    CHECK(t == nullptr)

    t = createRecordTable(8, 128);
    CHECK(t != nullptr)
    
    index = t->add("D");
    CHECK_EQUAL(0, index)
    CHECK_EQUAL(0, t->find("D"))

    index = t->add("C");
    CHECK_EQUAL(1, index)
    CHECK_EQUAL(0, t->find("D"))
    CHECK_EQUAL(1, t->find("C"))

    index = t->add("B");
    CHECK_EQUAL(2, index)
    CHECK_EQUAL(0, t->find("D"))
    CHECK_EQUAL(1, t->find("C"))
    CHECK_EQUAL(2, t->find("B"))

    index = t->add("A");
    CHECK_EQUAL(3, index)
    CHECK_EQUAL(0, t->find("D"))
    CHECK_EQUAL(1, t->find("C"))
    CHECK_EQUAL(2, t->find("B"))
    CHECK_EQUAL(3, t->find("A"))

    index = t->add("abc");
    CHECK_EQUAL(4, index)
    CHECK_EQUAL(4, t->find("abc"))

    index = t->add("bac");
    CHECK_EQUAL(5, index)
    CHECK_EQUAL(4, t->find("abc"))
    CHECK_EQUAL(5, t->find("bac"))

    index = t->add("cba");
    CHECK_EQUAL(6, index)
    CHECK_EQUAL(4, t->find("abc"))
    CHECK_EQUAL(5, t->find("bac"))
    CHECK_EQUAL(6, t->find("cba"))

    index = t->add("cab");
    CHECK_EQUAL(7, index)
    CHECK_EQUAL(4, t->find("abc"))
    CHECK_EQUAL(5, t->find("bac"))
    CHECK_EQUAL(6, t->find("cba"))
    CHECK_EQUAL(7, t->find("cab"))
    
    index = t->add("test");
    CHECK_EQUAL(index, RecordTable::TableFull)
    
    destroy(t);
}
