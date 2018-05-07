/** Kabuki Toolkit
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
#include "test_kt.h"

#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

using namespace _;
using namespace std;


#if MAJOR_SEAM == 2 && MINOR_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

void TestSeam2_1 () {

    printf ("\n    Testing SEAM_2_1... ");




    PRINTF ("\n Done testing SEAM_2_1!\n\n");
}

#undef PRINT_PAUSE
#undef PRINTF
#else
void TestSeam2_1 () {}
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
