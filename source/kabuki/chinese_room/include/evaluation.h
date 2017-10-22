/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/evaluation.h
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

#ifndef CHINESE_ROOM_EVALUATION_H
#define CHINESE_ROOM_EVALUATION_H

#include "args.h"
#include "utils.h"

namespace _ {

/** An evaluation is a word-aligned Expression and Result.



    @code
    Higher addresses
      ____________
     |   Result   |
     |     |      |
     |     v      |
     |============|
     |   Buffer   |
     |============|
     |     ^      |
     |     |      |
     | Expression |
     |____________|
    Lower addresses

    @endcode
*/
struct Evaluation {
    uint_t    size,         //< Size of the buffer.
              result_index, //< Starting index of the ring buffer data.
              expr_index,   //< Stopping index of the ring buffer data.
              reserved;     //< Reserved for memory alignment.
    uintptr_t expression;   //< Word-aligned start of the first expression.
};

}       //< namespace _
#endif  //< CHINESE_ROOM_EVALUATION_H
