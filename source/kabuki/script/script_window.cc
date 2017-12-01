/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/bin_portal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include "window.h"

namespace _ {

Window::Window (Bin* bin, Bout* bout):
    size_bytes_ (0),
    bin_        (bin),
    bout_       (bout)
{

}

void Window::Prime () {

}

uint_t Window::Length () {
    return 0;
}

void Window::Feed (byte b) {

}

byte Window::Pull () {
    return 0;
}

const Operation* Window::Star (uint index, Expression* expr) {
    return 0;
}

}       //< namespace _
