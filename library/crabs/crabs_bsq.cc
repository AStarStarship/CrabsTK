/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/crabs/crabs_bsq.cc
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3

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

Printer PrintBsq (Printer out_, const uint_t* params) {
    uint_t num_params = *params++,
           i,
           type,
           value = 0;

    out_ << "Param<";
    if (num_params > _::kParamsMax) {
        out_ << "\nInvalid num_params: " << num_params;
        return out_;
    }
    out_ << num_params << ": ";
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type = value & 0x1f;    //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        out_ << TypeString ((type_t)value) << ", ";
        if (type >= STR) {
            if (value) {
                out_ << "\nError: arrays may only be created from POD "
                         "types.";
                return out_;
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            out_ << value;
        }
        else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                out_ << "Multi-dimensional Array:" << value << ", ";
            }
            // Then it's an array.
            ++i;
            switch (value) {        //< Print out the Array type.
                case 0: {
                    break;
                }
                case 1: {
                    value = *params++;
                    out_ << "UI1:" << value << ", ";
                    break;
                }
                case 2: {
                    value = *params++;
                    out_ << "UI2:" << value << ", ";
                    break;
                }
                case 3: {
                    value = *params++;
                    out_ << "UI4:" << value << ", ";
                    break;
                }
                case 4: {
                    value = *params++;
                    out_ << "UI8:" << value << ", ";
                    break;
                }
                case 5: {
                    value = *params++;
                    if (value == 0) {
                        out_ << "UI1:[0]";
                        break;
                    }
                    out_ << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        out_ << value << ", ";
                    }
                    value = *params++;
                    out_ << value << "]";
                    break;
                }
                case 6: {
                    value = *params++;
                    if (value == 0) {
                        out_ << "UI2:[0]";
                        break;
                    }
                    out_ << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        out_ << value << ", ";
                    }
                    value = *params++;
                    out_ << value << "]";
                    break;
                }
                case 7: {
                    value = *params++;
                    if (value == 0) {
                        out_ << "UI4:[0]";
                        break;
                    }
                    out_ << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        out_ << value << ", ";
                    }
                    value = *params++;
                    out_ << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    out_ << TypeString (value) << ", ";
    if (value == STR) {
        ++i;
        value = *params++;
        out_ << value;
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
                out_ << "UI1:" << value << ", ";
                break;
            }
            case 2:
            {
                value = *params++;
                out_ << "UI2:" << value << ", ";
                break;
            }
            case 3:
            {
                value = *params++;
                out_ << "UI4:" << value << ", ";
                break;
            }
            case 4:
            {
                value = *params++;
                out_ << "UI5:" << value << ", ";
                break;
            }
            case 5:
            {
                value = *params++;
                if (value == 0) {
                    out_ << "UI1:[0]";
                    break;
                }
                out_ << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    out_ << value << ", ";
                }
                value = *params++;
                out_ << value << "]";
                break;
            }
            case 6:
            {
                value = *params++;
                if (value == 0) {
                    out_ << "UI2:[0]";
                    break;
                }
                out_ << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    out_ << value << ", ";
                }
                value = *params++;
                out_ << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    out_ << "UI4:[0]";
                    break;
                }
                out_ << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    out_ << value << ", ";
                }
                value = *params++;
                out_ << value << "]";
                break;
            }
        }
    }
    out_ << '>';
    return out_;
}

}       //< namespace _

#endif  //> #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
