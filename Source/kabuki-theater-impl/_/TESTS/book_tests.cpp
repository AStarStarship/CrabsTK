/** Kabuki Theater
    @version 0.x
    @file    /.../source/kabuki-theater-impl/_/TESTS/book_tesets.cpp
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

TEST_GROUP (BookTests) {
    void setup () {
        PrintLineBreak ("+ Running BookTests...", 20);
    }

    void teardown () {
        std::cout << "  BookTests completed.\n";
        //system ("PAUSE");
    }
};

TEST (BookTests, FirstBookTest) {
    PrintLineBreak ("  + Running BookTests\n", 10);

    PrintLineBreak ("  - Running Book2...\n", 5, ' ');
    byte index;
    Book2* book = Init2 (Buffer<0, 248 - sizeof (Book2)> (), 8, 256, 128);

    CHECK (book != nullptr)

    CHECK_EQUAL (0, index = Add2 (book, "D", (byte)0xFF))
    Print (book);
    CHECK_EQUAL (0, index)
    CHECK_EQUAL (0, Find2 (book, "D"))

    index = Add2 (book, "C", (byte)0xFF);
    CHECK_EQUAL (1, index)
    CHECK_EQUAL (0, Find2 (book, "D"))
    CHECK_EQUAL (1, Find2 (book, "C"))

    index = Add2 (book, "B", (byte)0xFF);
    CHECK_EQUAL (2, index)
    CHECK_EQUAL (0, Find2 (book, "D"))
    CHECK_EQUAL (1, Find2 (book, "C"))
    CHECK_EQUAL (2, Find2 (book, "B"))

    index = Add2 (book, "A", (byte)0xFF);
    CHECK_EQUAL (3, index)
    CHECK_EQUAL (0, Find2 (book, "D"))
    CHECK_EQUAL (1, Find2 (book, "C"))
    CHECK_EQUAL (2, Find2 (book, "B"))
    CHECK_EQUAL (3, Find2 (book, "A"))

    index = Add2 (book, "abc", (byte)0xFF);
    CHECK_EQUAL (4, index)
    CHECK_EQUAL (4, Find2 (book, "abc"))

    index = Add2 (book, "bac", (byte)0xFF);
    CHECK_EQUAL (5, index)
    CHECK_EQUAL (4, Find2 (book, "abc"))
    CHECK_EQUAL (5, Find2 (book, "bac"))

    index = Add2 (book, "cba", (byte)0xFF);
    CHECK_EQUAL (6, index)
    CHECK_EQUAL (4, Find2 (book, "abc"))
    CHECK_EQUAL (5, Find2 (book, "bac"))
    CHECK_EQUAL (6, Find2 (book, "cba"))

    index = Add2 (book, "cab", (byte)0xFF);
    CHECK_EQUAL (7, index)
    CHECK_EQUAL (4, Find2 (book, "abc"))
    CHECK_EQUAL (5, Find2 (book, "bac"))
    CHECK_EQUAL (6, Find2 (book, "cba"))
    CHECK_EQUAL (7, Find2 (book, "cab"))

    index = Add2 (book, "test", (byte)0xFF);
    CHECK_EQUAL (index, ~0)
    
}
