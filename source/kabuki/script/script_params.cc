/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/script_args.cc
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
#include "params.h"

using namespace std;

namespace _ {

uint_t ParamsNumber (const uint_t* params, int param_number) {
    if (params == nullptr)
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
                cout << "\n| Error";
                return NIL;
            }
            else {
                param_number += params[i] + 1;
            }
        }
    }
    return params[i];
}

#if SCRIPT_USING_TEXT
Text& ParamsPrint (const uint_t* params, Text& txt) {
    if (!params) {
        return txt << "null";
    }
    if (params < (const uint_t*)256) {
        return txt << reinterpret_cast<uintptr_t> (params) << '\n';
    }
    uint_t num_params = *params++,
        i,
        type,
        value = 0;

    txt << "Param<";
    if (num_params > _::kMaxNumParams) {
        return txt << "\n| Invalid num_params: " << num_params;
    }
    printf ("%u: ", num_params);
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type = value & 0x1f;   //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        txt << TypeText (value) << ", ";
        if ((type >= STR) && (value <= ST4)) {
            if (value) {
                return txt << "\n| Error: arrays may only be created from POD types.";
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            txt << value;
        } else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                txt << "Multi-dimensional Array:" << value << ", ";
            }
            // Then it's an array.
            ++i;
            switch (value) {        //< Print out the Array type.
                case 0:
                {
                    break;
                }
                case 1:
                {
                    value = *params++;
                    txt << "UI1:" << value << ", ";
                    break;
                }
                case 2:
                {
                    value = *params++;
                    txt << "UI2:" << value << ", ";
                    break;
                }
                case 3:
                {
                    value = *params++;
                    txt << "UI4:" << value << ", ";
                    break;
                }
                case 4:
                {
                    value = *params++;
                    txt << "UI8:" << value << ", ";
                    break;
                }
                case 5:
                {
                    value = *params++;
                    if (value == 0) {
                        txt << "UI1:[0]";
                        break;
                    }
                    txt << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        txt << value << ", ";
                    }
                    value = *params++;
                    txt << value << "]";
                    break;
                }
                case 6:
                {
                    value = *params++;
                    if (value == 0) {
                        txt << "UI2:[0]";
                        break;
                    }
                    txt << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        txt << value << ", ";
                    }
                    value = *params++;
                    txt << value << "]";
                    break;
                }
                case 7:
                {
                    value = *params++;
                    if (value == 0) {
                        txt << "UI4:[0]";
                        break;
                    }
                    txt << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        txt << value << ", ";
                    }
                    value = *params++;
                    txt << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    txt << TypeText (value) << ", ";
    if ((value == STR) || (value == ST2) || (value == ST4)) {
        ++i;
        value = *params++;
        printf ("%u", value);
    } else if (value > 31) {
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
                txt << "UI1:" << value << ", ";
                break;
            }
            case 2:
            {
                value = *params++;
                txt << "UI2:" << value << ", ";
                break;
            }
            case 3:
            {
                value = *params++;
                txt << "UI4:" << value << ", ";
                break;
            }
            case 4:
            {
                value = *params++;
                txt << "UI5:" << value << ", ";
                break;
            }
            case 5:
            {
                value = *params++;
                if (value == 0) {
                    txt << "UI1:[0]";
                    break;
                }
                txt << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    txt << value << ", ";
                }
                value = *params++;
                txt << value << "]";
                break;
            }
            case 6:
            {
                value = *params++;
                if (value == 0) {
                    txt << "UI2:[0]";
                    break;
                }
                txt << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    txt << value << ", ";
                }
                value = *params++;
                txt << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    txt << "UI4:[0]";
                    break;
                }
                txt << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    txt << value << ", ";
                }
                value = *params++;
                txt << value << "]";
                break;
            }
        }
    }
    return txt << '>';
}
#endif  //< SCRIPT_USING_TEXT

}       //< namespace _

#if SCRIPT_USING_TEXT
#include "text.h"
using namespace _;
_::Text& operator<< (_::Text& txt, const uint_t* params) {
    return txt << ParamsPrint (params, txt);
}
#endif  //< SCRIPT_USING_TEXT
