/** Kabuki Theater
    @version 0.x
    @file    /.../source/kabuki-theater-impl/_/TESTS/uniprinter_tests.cpp
    @author  Cale McCollough <calemccollough.github.rx>
    @license Copyright 2017 (C) Cale McCollough <cale.mccollough@gmail.com>

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

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

#include "test_devices.h"

using namespace _;

TEST_GROUP (UniprinterTests)
{
    void setup ()
    {
        std::cout << "\n+ Running UniprinterTests...\n";
    }

    void teardown ()
    {
        std::cout << "  UniprinterTests completed.\n";
    }

};

TEST (UniprinterTests, UniprinterTestsOne)
{
    std::cout << "Running UniprinterTests...\n";
    
    Root root;
    Uniprinter* io = UniprinterInit (Buffer<255> (), 255, 4, &root);

    void* args[3];
    const uint_t* esc = Esc <4, SOH, STX, 32, FLT, SI4> ();
    const char stx_expected[] = "Hello world!\0";
    const int si4_expected = 1;
    const float flt_expected = 1.0f;
    const Member* m = Write (io, esc, Args (args, &stx_expected,
                                            &si4_expected, &flt_expected));
    Print (io);

    UniprinterScanBytes (io);
    
    system ("PAUSE");
}
