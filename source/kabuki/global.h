/** Kabuki Toolkit: Chinese Room
    @version 0.x
    @file    ~/source/kabuki/this.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef KABUKI_THIS_H
#define KABUKI_THIS_H

#include "script/include/module_all.h"

namespace _ {

/** Default empty room.
    
*/
struct This: public Room {
    public:

    This (uintptr_t* buffer, uintptr_t size) :
        Room (buffer, size) {
        
    }
};

}       //< namespace _
#endif  //< KABUKI_THIS_H
