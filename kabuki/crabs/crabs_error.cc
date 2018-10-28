/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/crabs/crabs_errors.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= _0_0_4_0__00_00
// Dependencies:
#include "error.h"
// End dependencies.

namespace _ {

const char** ErrorStrings() {
  static const char* kErrors[] = {
      "Not an error",             //<  0
      "Input nil",                //<  1
      "Input too low",            //<  2
      "Input too high",           //<  3
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
      "Invalid op",               //< 19
      "Array overflow",           //< 20
      "Invalid Set",              //< 21
      "Malformed UTF-8",          //< 22
      "Malformed UTF-16",         //< 23
      "Malformed UTF-32",         //< 24
      "BIn Locked",               //< 25
      "Invalid args",             //< 26
      "Invalid credentials",      //< 27
      "Obj locked"                //< 28
      "Implementation error",     //< 29
  };
  return kErrors;
}

const char* ErrorString(Error error) {
  if (error >= kErrorImplementation) {
    return ErrorStrings()[kErrorImplementation];
  }
  return ErrorStrings()[error];
}

}  // namespace _
#endif  //> #if SEAM >= _0_0_4_0__00_00
