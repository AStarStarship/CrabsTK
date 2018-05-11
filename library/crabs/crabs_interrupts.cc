/** Kabuki Toolkit
#include <script_interrupts.h>
    @version 0.x
    @file    ~/library/crabs/crabs_exceptions.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
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
#include "interrupts.h"

#if USING_CRABS_INTERRUPTS

namespace _ {

#if USING_PRINTER
const char* RoomCrashException::what () const throw () {
    return "External crash";
}
#endif

}       //< namespace _
#endif  //< USING_CRABS_INTERRUPTS
