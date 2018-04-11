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
#include "printer.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

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

char* PrintBsq (const uint_t* params, char* buffer, char* buffer_end) {
    uint_t num_params = *params++,
        i,
        type,
        value = 0;

    Printer print (buffer, buffer_end);

    print << "Param<";
    if (num_params > _::kParamsMax) {
        print << "\nInvalid num_params: " << num_params;
        return buffer;
    }
    print << num_params << ": ";
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        print << TypeString (value) << ", ";
        if (type >= STR) {
            if (value) {
                print << "\nError: arrays may only be created from POD "
                    "types.";
                return buffer;
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            print << value;
        }
        else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                print << "Multi-dimensional Array:" << value << ", ";
            }
            // Then it's an array.
            ++i;
            switch (value) {        //< Print out the Array type.
                case 0: {
                    break;
                }
                case 1: {
                    value = *params++;
                    print << "UI1:" << value << ", ";
                    break;
                }
                case 2: {
                    value = *params++;
                    print << "UI2:" << value << ", ";
                    break;
                }
                case 3: {
                    value = *params++;
                    print << "UI4:" << value << ", ";
                    break;
                }
                case 4: {
                    value = *params++;
                    print << "UI8:" << value << ", ";
                    break;
                }
                case 5: {
                    value = *params++;
                    if (value == 0) {
                        print << "UI1:[0]";
                        break;
                    }
                    print << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        print << value << ", ";
                    }
                    value = *params++;
                    print << value << "]";
                    break;
                }
                case 6: {
                    value = *params++;
                    if (value == 0) {
                        print << "UI2:[0]";
                        break;
                    }
                    print << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        print << value << ", ";
                    }
                    value = *params++;
                    print << value << "]";
                    break;
                }
                case 7: {
                    value = *params++;
                    if (value == 0) {
                        print << "UI4:[0]";
                        break;
                    }
                    print << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        print << value << ", ";
                    }
                    value = *params++;
                    print << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    print << TypeString (value) << ", ";
    if (value == STR) {
        ++i;
        value = *params++;
        print << value;
    }
    else if (value > 31) {
        // Then it's an array.
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        ++i;
        switch (value) {
            case 0:
            {
                break;
            }
            case 1:
            {
                value = *params++;
                print << "UI1:" << value << ", ";
                break;
            }
            case 2:
            {
                value = *params++;
                print << "UI2:" << value << ", ";
                break;
            }
            case 3:
            {
                value = *params++;
                print << "UI4:" << value << ", ";
                break;
            }
            case 4:
            {
                value = *params++;
                print << "UI5:" << value << ", ";
                break;
            }
            case 5:
            {
                value = *params++;
                if (value == 0) {
                    print << "UI1:[0]";
                    break;
                }
                print << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    print << value << ", ";
                }
                value = *params++;
                print << value << "]";
                break;
            }
            case 6:
            {
                value = *params++;
                if (value == 0) {
                    print << "UI2:[0]";
                    break;
                }
                print << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    print << value << ", ";
                }
                value = *params++;
                print << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    print << "UI4:[0]";
                    break;
                }
                print << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    print << value << ", ";
                }
                value = *params++;
                print << value << "]";
                break;
            }
        }
    }
    print << '>' << print;
    return buffer;
}

}       //< namespace _

#endif  //< #if MAJOR_SEAM == 1 && MINOR_SEAM >= 4
