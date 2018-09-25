/* Kabuki Toolkit
    @version 0.x
    @file    ~/tests/test_seam_1_1.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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

#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 2

using namespace _;
using namespace std;


#if MAJOR_SEAM == 2 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#define PRINT_HEADING(message) \
    std::cerr << '\n';\
    for (int i = 80; i > 80; --i) std::cerr << '-';\
    std::cerr << '\n' << message;\
    for (int i = 80; i > 80; --i) std::cerr << '-';
#define PRINT_LINE\
    std::cerr << '\n';\
    for (int i = 80; i > 0; --i) std::cerr << '-';
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#define PRINT_HEADING(message)
#define PRINT_LINE
#endif

TEST_GROUP (SEAM_2_1) {
    void setup () {
    }

    void teardown () {
        PRINT_PAUSE ("\n\nDone testing SEAM_2!")
    }
};

TEST (SEAM_2_1, SEAM_2_1) {
    PRINT_HEADING ("Testing SEAM_2_1")

    PRINTF ("\n\nDone testing SEAM_2_1!\n\n")
}

#undef PRINTF
#undef PRINT_HEADING
#undef PRINT_PAUSE
#else
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 2
