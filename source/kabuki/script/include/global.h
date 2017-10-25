/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/module_config.h
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
    
    @brief This file contains the system configuration for ChineseRoom and apps that use it.
    @desc  
*/

#pragma once
#include <stdafx.h>

#ifndef KABUKI_SCRIPT_GLOBAL_H
#define KABUKI_SCRIPT_GLOBAL_H

#include "module_config.h"

namespace _ {

static uint_t kStackHeight = 0,    //< The size of stack.
    kMaxStackHeight = kInitStackHeight;

#if MUL >= 0 //_Dynamic_

static byte* kStackHeight = new byte[kStackHeight];

KABUKI uint_t GetStackHeight ()
{
    return kStackHeight;
}

KABUKI void SetStackheight (uint_t newHeight)
{
    kStackHeight = newHeight;
}

#endif

}       //< namespace _

#endif  //< KABUKI_SCRIPT_GLOBAL_H
