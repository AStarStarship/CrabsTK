/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/crabs_args.cc
    @author  Cale McCollough <https://calemccollough.github.io>
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
#include "bsq.h"

#if MAJOR_SEAM == 1 && MINOR_SEAM >= 3

namespace _ {

uint_t BsqParamNumber (const uint_t* params, int param_number) {
    if (!params)
        return 0;
    uint_t num_params = *params++;
    if (param_number > num_params)
        return NIL;
    int i;
    for (i = 0; i < param_number; ++i) {
        uint_t value = params[i];
        if (value == STR)
            ++param_number;
        else if (value > 31) {  // It's an array!
            value = value >> 5;
            if (value < 4) {    // It's a single dimension!
                param_number += 2;
                break;
            } else if (value > 7) { // Gratuitous explanation points!
                //PRINTF ("\nError";
                return NIL;
            }
            else {
                param_number += params[i] + 1;
            }
        }
    }
    return params[i];
}

}       //< namespace _

#endif  //< MAJOR_SEAM == 1 && MINOR_SEAM >= 3
