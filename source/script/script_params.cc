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
Text& ParamsPrint (const uint_t* params, Text& text) {
    if (!params) {
        return text << "null";
    }
    if (params < (const uint_t*)256) {
        return text << reinterpret_cast<uintptr_t> (params) << '\n';
    }
    uint_t num_params = *params++,
        i,
        type,
        value = 0;

    text << "Param<";
    if (num_params > _::kMaxNumParams) {
        return text << "\n| Invalid num_params: " << num_params;
    }
    printf ("%u: ", num_params);
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type = value & 0x1f;   //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        text << TypeText (value) << ", ";
        if ((type >= STR) && (value <= ST4)) {
            if (value) {
                return text << "\n| Error: arrays may only be created from POD types.";
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            text << value;
        } else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                text << "Multi-dimensional Array:" << value << ", ";
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
                    text << "UI1:" << value << ", ";
                    break;
                }
                case 2:
                {
                    value = *params++;
                    text << "UI2:" << value << ", ";
                    break;
                }
                case 3:
                {
                    value = *params++;
                    text << "UI4:" << value << ", ";
                    break;
                }
                case 4:
                {
                    value = *params++;
                    text << "UI8:" << value << ", ";
                    break;
                }
                case 5:
                {
                    value = *params++;
                    if (value == 0) {
                        text << "UI1:[0]";
                        break;
                    }
                    text << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        text << value << ", ";
                    }
                    value = *params++;
                    text << value << "]";
                    break;
                }
                case 6:
                {
                    value = *params++;
                    if (value == 0) {
                        text << "UI2:[0]";
                        break;
                    }
                    text << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        text << value << ", ";
                    }
                    value = *params++;
                    text << value << "]";
                    break;
                }
                case 7:
                {
                    value = *params++;
                    if (value == 0) {
                        text << "UI4:[0]";
                        break;
                    }
                    text << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        text << value << ", ";
                    }
                    value = *params++;
                    text << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    text << TypeText (value) << ", ";
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
                text << "UI1:" << value << ", ";
                break;
            }
            case 2:
            {
                value = *params++;
                text << "UI2:" << value << ", ";
                break;
            }
            case 3:
            {
                value = *params++;
                text << "UI4:" << value << ", ";
                break;
            }
            case 4:
            {
                value = *params++;
                text << "UI5:" << value << ", ";
                break;
            }
            case 5:
            {
                value = *params++;
                if (value == 0) {
                    text << "UI1:[0]";
                    break;
                }
                text << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    text << value << ", ";
                }
                value = *params++;
                text << value << "]";
                break;
            }
            case 6:
            {
                value = *params++;
                if (value == 0) {
                    text << "UI2:[0]";
                    break;
                }
                text << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    text << value << ", ";
                }
                value = *params++;
                text << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    text << "UI4:[0]";
                    break;
                }
                text << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    text << value << ", ";
                }
                value = *params++;
                text << value << "]";
                break;
            }
        }
    }
    return text << '>';
}
#endif  //< SCRIPT_USING_TEXT

}       //< namespace _

#if SCRIPT_USING_TEXT
#include "text.h"
using namespace _;
_::Text& operator<< (_::Text& text, const uint_t* params) {
    return text << ParamsPrint (params, text);
}
#endif  //< SCRIPT_USING_TEXT
