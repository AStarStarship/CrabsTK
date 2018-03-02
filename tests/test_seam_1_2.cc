/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_2.cc
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

#include <stdafx.h>
#include "../projects/script_itos_benchmark/script_itos_benchmark.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2

#include "../source/crabs/print_itos.h"
#include "script_itos_competition.h"

TEST_GROUP (SEAM_1_2_TESTS) {
    void setup () {
        puts ("\n\n\nTesting SEAM_1_2");
    }

    void teardown () {
        putchar ('\n');
        system ("PAUSE");
    }
};

TEST (SEAM_1_2_TESTS, TestPrinter) {
}
#endif  //< #if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2
