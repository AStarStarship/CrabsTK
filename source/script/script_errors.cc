/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_errors.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
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
#include "error.h"

namespace _ {

const char** ErrorString () {
    static const char* kErrors[] = {
        "Input nil!",               //<  0
        "Input too low!",           //<  1
        "Input too high!",          //<  2
        "Buffer overflow",          //<  4
        "Buffer underflow",         //<  5
        "Varint overflow",          //<  6
        "Invalid hash",             //<  7
        "Invalid type",             //<  8
        "Invalid index",            //<  9
        "Invalid equerry",          //< 10
        "Invalid argument number",  //< 11 
        "Invalid door",             //< 12
        "Too many parameters",      //< 13
        "Stack overflow",           //< 14
        "Invalid subset",           //< 15
        "Too many pops",            //< 16
        "Text buffer overflow",     //< 17
        "Invalid error handler",    //< 18
        "Invalid operation",        //< 19
        "Array overflow",           //< 20
        "Invalid Set",              //< 21
        "Malformed UTF-8",          //< 22
        "Malformed UTF-16",         //< 23
        "Malformed UTF-32",         //< 24
        "Bin Locked",               //< 25
        "Invalid args",             //< 26
        "Invalid credentials",      //< 27
        "Object locked"             //< 28
        "Implementation error",     //< 29
    };
    return kErrors;
}

const char* ErrorString (Error error) {
    if (error >= kErrorImplementation) {
        return ErrorString ()[kErrorImplementation];
    }
    return ErrorString ()[error];
}

}   //< namespace _
