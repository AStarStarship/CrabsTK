/** Kabuki Toolkit
    @file    ~/tests/seam_1/test_seam_1.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#include "test_seam_1.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF(format, ...) printf (format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#define PRINT_HEADING(message) \
    std::cerr << '\n';\
    for (int i = 80; i > 80; --i) std::cout << '-';\
    std::cerr << '\n' << message << "\n|";\
    for (int i = 80; i > 80; --i) std::cout << '-';\
    std::cerr << '\n';
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#define PRINT_HEADING(message)
#endif

using namespace _;

TEST_GROUP (SEAM_1_3) {
    void setup () {
    }

    void teardown () {
        std::cout << '\n';
        system ("PAUSE");
    }
};

TEST (SEAM_1_3, SEAM_1_3A) {

    int i = 0; //< Shared looping variable.

    PRINT_HEADING ("\nTesting COut ().Print ()... ");

    COUT << "Testing " << 1 << ", 2, " << -3;
    
    PRINT_HEADING ("\nTesting SEAM_1_3... ");

    PRINTF ("\n\nTesting ASCII Object Types");

    PRINTF ("\n\nTesting _::Stack...\n\nPushing items on to the Stack...\n");

    Stack<> stack (8);

    for (i = 0; i <= 10; ++i)
        stack.Push (i);

    COUT << stack;

    PRINTF ("\nPopping itmes off the Stack...\n");

    for (i = 10; i > 0; i--)
        CHECK_EQUAL (i, stack.Pop ())

    COUT << stack;

    PRINTF ("\n\nDone testing _::Stack!")

    /*
    PRINTF ("\n\nTest _::Array...\n\n");
    static const int array_3d_exected[2][2][2] = { { { 0, 1 }, { 2, 3 } },
                                                   { { 4, 5 }, { 6, 7 } }
                                                 };
    const int* test = Dimensions<2, 2, 2> ();
    Array<int> test_array (test);
    *test_array.Elements () = { { { 0, 1 }, { 2, 3 } },
                                { { 4, 5 }, { 6, 7 } }
                              };
    i = 0;
    int* array_base = test_array.Elements ();
    for (int z = 0; z < 2; ++z)
        for (int y = 0; y < 2; ++y)
            for (int x = 0; x < 2; ++x)
                CHECK_EQUAL (i++, array_3d_exected[x][y][z])
    
    PRINT_PAUSE ("\n\nDone _::Array!")
    PRINT_HEADING ("\n\nTest _::MemoryCopy...\n\n");

    enum { kTestCharsCount = 256 * 1024 };
    char test_chars[kTestCharsCount];
    for (int i = kTestCharsCount - 1; i > 0; --i)
        test_chars[i] = i % 256;
    char test_chars_result[kTestCharsCount * 2];
    for (int i = 15; i > 0; --i) {
        char* result = MemoryCopy (test_chars_result + i, kTestCharsCount,
                                   test_chars_result, kTestCharsCount);
        CHECK (result)
        CHECK (!MemoryCompare (test_chars_result + i, kTestCharsCount,
                               test_chars_result, kTestCharsCount))
    }


    PRINTF ("\n\nDone testing _::MemoryCopy!")
    */

    PRINT_HEADING ("\n\nTest _::List...\n\n");

    List<> list (36, 960);
    CHECK (list.This ())

    PRINTF ("\nPushing items on to the List stack...\n");

    const int test_ints[] = { '1', '2', '3', '4' };

    const int list_test_count = 12;
    for (int i = 0; i < 4; ) {
        COUT << "\ni:" << i;
        list.Push (SVI, &test_ints[i++]);
        COUT << '\n' << list << '\n'
             << "\n\n" << Socket (list.This (), list.This ()->size);
    }
    system ("PAUSE");
    const float test_floats[] = { 9.0, 10.0, 11.0, 12.0 };
    for (int i = 0; i < 4; ) {
        COUT << "\ni:" << i + 4;
        list.Push (FLT, &test_floats[i++]);
        COUT << '\n' << list << '\n'
             << "\n\n" << Socket (list.This (), list.This ()->size);
    }

    const char* test_strings[] = {
        "Test",
        " 1, ",
        " 2, ",
        " 3"
    };
    for (int i = 0; i < 4; ) {
        COUT << "\ni:" << i + 8;
        list.Push (STR, test_strings[i++]);
        COUT << '\n' << list << '\n'
             << "\n\n" << Socket (list.This (), list.This ()->size);
    }

    for (int i = list_test_count - 1; i > 0; --i)
        list.Pop ();

    PRINTF ("\n\nDone _::List!")
    
    /*
    PRINTF ("\n\nTesting _::Book...\n\n");

    int16_t index;

    Book2 book (8, kBufferSize);

    CHECK (book != nullptr)

    index = book.Add (book, "D", (byte)0xFF);

    CHECK_EQUAL (0, index)
    book.Print (book);
    CHECK_EQUAL (0, index)
    index = book.Find (book, "D");
    CHECK_EQUAL (0, index)
    PRINT_PAUSE ("\n");
    index = book.Add (book, "C", (byte)0xFF);
    CHECK_EQUAL (1, index)
    index = book.Find (book, "D");
    CHECK_EQUAL (0, index)
    index = book.Find (book, "C");
    CHECK_EQUAL (1, index)

    index = book.Add (book, "BIn", (byte)0xFF);
    CHECK_EQUAL (2, index)
    index = book.Find (book, "D");
    CHECK_EQUAL (0, index)
    index = book.Find (book, "C");
    CHECK_EQUAL (1, index)
    index = book.Find (book, "BIn");
    CHECK_EQUAL (2, index)

    index = book.Add (book, "A", (byte)0xFF);
    CHECK_EQUAL (3, index)
    index = book.Find (book, "D");
    CHECK_EQUAL (0, index)
    index = book.Find (book, "C");
    CHECK_EQUAL (1, index)
    index = book.Find (book, "BIn");
    CHECK_EQUAL (2, index)
    index = book.Find (book, "A");
    CHECK_EQUAL (3, index)

    index = book.Add (book, "abc", (byte)0xFF);
    CHECK_EQUAL (4, index)
    index = book.Find (book, "abc");
    CHECK_EQUAL (4, index)

    index = book.Add (book, "bac", (byte)0xFF);
    CHECK_EQUAL (5, index)
    index = book.Find (book, "abc");
    CHECK_EQUAL (4, index)
    index = book.Find (book, "bac");
    CHECK_EQUAL (5, index)

    index = book.Add (book, "cba", (byte)0xFF);
    CHECK_EQUAL (6, index)
    index = book.Find (book, "abc");
    CHECK_EQUAL (4, index)
    index = book.Find (book, "bac");
    CHECK_EQUAL (5, index)
    index = book.Find (book, "cba");
    CHECK_EQUAL (6, index)

    index = book.Add (book, "cab", (byte)0xFF);
    CHECK_EQUAL (7, index)
    index = book.Find (book, "abc");
    CHECK_EQUAL (4, index)
    index = book.Find (book, "bac");
    CHECK_EQUAL (5, index)
    index = book.Find (book, "cba");
    CHECK_EQUAL (6, index)
    index = book.Find (book, "cab");
    CHECK_EQUAL (7, index)

    index = book.Add (book, "test", (byte)0xFF);
    CHECK_EQUAL (index, -1)

    PRINT_PAUSE ("\n\nDone _::Book!")*/

    PRINT_PAUSE ("\n\nDone Testing SEAM_1_3! ({:-)-+=<")
}


#undef PRINTF
#undef PRINT_PAUSE
#undef PRINT_HEADING
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
