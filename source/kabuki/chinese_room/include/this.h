/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/Source/ChineseRoom/this.h
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

#ifndef CHINESE_ROOM_THIS_H
#define CHINESE_ROOM_THIS_H

#include "room.h"

namespace _ {

/** Default empty room.
    
*/
struct This: public Room {
    public:

    This ():
        Room (1024)
    {

    }
};

}       //< namespace _
#endif  //< CHINESE_ROOM_THIS_H
