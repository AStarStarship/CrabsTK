/** The Chinese Room
    @version 0.x
    @file    /.../Config.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 Cale McCollough <calemccollough.github.io>
        
                        All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
    
    @brief This file contains the system configuration for ChineseRoom and apps that use it.
    @desc  
*/

#ifndef CHINESEROOM_GLOBAL_H
#define CHINESEROOM_GLOBAL_H

#include "Config.h"

namespace _ {

static uint_t stackHeight = 0,    //< The size of stack.
    maxStackHeight = InitStackHeight;

#if MUL >= 0 //_Dynamic_

static byte* stackHeight = new byte[stackHeight];

uint_t getStackHeight()
{
    return stackHeight;
}

void setStackheight(uint_t newHeight)
{
    stackHeight = newHeight;
}

#endif

}       //< namespace _

#endif  //< CHINESEROOM_GLOBAL_H
