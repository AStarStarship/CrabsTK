/** kabuki::script
#include <script_types.h>
#include <script_args.h>
    @version 0.x
    @file    ~/source/kabuki/script/include/script_args.cc
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
#include "args.h"

using namespace std;

namespace _ {

#if USE_MORE_ROM
void ParamsPrint (const uint_t* params) {
    if (params == 0) {
        cout << "null\n";
        return;
    }
    if (params < (const uint_t*)256) {
        cout << reinterpret_cast<uintptr_t> (params) << '\n';
        return;
    }
    uint_t num_params = *params++,
           i,
           type,
           value = 0;

    cout << "Param<";
    if (num_params > kMaxNumParams) {
        printf ("Invalid num_params: %u>\n", num_params);
        return;
    }
    printf ("%u: ", num_params);
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        type  = value & 0x1f;   //< Mask off type.
        value = value >> 5;     //< Shift over array type.
        cout << TypeString (value) << ", ";
        if ((type >= STR) && (value <= ST4)) {
            if (value) {
                cout << "\n| Error: arrays may only be created from POD types.";
                return;
            }
            // Print out the max length of the string.
            ++i;
            value = *params++;
            printf ("%u, ", value);
        } else if (value > 31) {
            if (value > 127) {      //< It's a multi-dimensional array.
                cout << "Multi-dimensional Array:" << value << ", ";
            }
            // Then it's an array.
            ++i;
            switch (value) {        //< Print out the Array type.
                case 0: {
                    break;
                }
                case 1: {
                    value = *params++;
                    cout << "UI1:" << value << ", ";
                    break;
                }
                case 2: {
                    value = *params++;
                    cout << "UI2:" << value << ", ";
                    break;
                }
                case 3: {
                    value = *params++;
                    cout << "UI4:" << value << ", ";
                    break;
                }
                case 4: {
                    value = *params++;
                    cout << "UI8:" << value << ", ";
                    break;
                }
                case 5: {
                    value = *params++;
                    if (value == 0) {
                        cout << "UI1:[0]";
                        break;
                    }
                    cout << "UI1:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        cout << value << ", ";
                    }
                    value = *params++;
                    cout << value << "]";
                    break;
                }
                case 6: {
                    value = *params++;
                    if (value == 0) {
                        cout << "UI2:[0]";
                        break;
                    }
                    cout << "UI2:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        cout << value << ", ";
                    }
                    value = *params++;
                    cout << value << "]";
                    break;
                }
                case 7: {
                    value = *params++;
                    if (value == 0) {
                        cout << "UI4:[0]";
                        break;
                    }
                    cout << "UI4:[" << value << ": ";
                    for (uint_t i = value; i != 0; --i) {
                        value = *params++;
                        cout << value << ", ";
                    }
                    value = *params++;
                    cout << value << "]";
                    break;
                }
            }
        }
    }
    // Do the last set without a comma.
    value = *params++;
    cout << TypeString (value) << ", ";
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
            case 0: {
                break;
            }
            case 1: {
                value = *params++;
                cout << "UI1:" << value << ", ";
                break;
            }
            case 2: {
                value = *params++;
                cout << "UI2:" << value << ", ";
                break;
            }
            case 3: {
                value = *params++;
                cout << "UI4:" << value << ", ";
                break;
            }
            case 4: {
                value = *params++;
                cout << "UI5:" << value << ", ";
                break;
            }
            case 5: {
                value = *params++;
                if (value == 0) {
                    cout << "UI1:[0]";
                    break;
                }
                cout << "UI1:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    cout << value << ", ";
                }
                value = *params++;
                cout << value << "]";
                break;
            }
            case 6: {
                value = *params++;
                if (value == 0) {
                    cout << "UI2:[0]";
                    break;
                }
                cout << "UI2:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    cout << value << ", ";
                }
                value = *params++;
                cout << value << "]";
                break;
            }
            case 7:
            {
                value = *params++;
                if (value == 0) {
                    cout << "UI4:[0]";
                    break;
                }
                cout << "UI4:[" << value << ": ";
                for (uint_t i = value; i != 0; --i) {
                    value = *params++;
                    cout << value << ", ";
                }
                value = *params++;
                cout << value << "]";
                break;
            }
        }
    }
    cout << '>';
}

uint_t ParamNumber (const uint_t* params, byte param_number) {
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
            if (value < 4) {    // It's a single dimension
                param_number += 2;
                break;
            } else if (value > 7) {
                cout << "Error";
                return NIL;
            }
            else {
                param_number += params[i] + 1;
            }
        }
    }
    return params[i];
}
#endif  //< USE_MORE_ROM

}       //< namespace _
