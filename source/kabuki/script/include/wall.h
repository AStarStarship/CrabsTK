/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/wall.h
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

#pragma once
#include <stdafx.h>

#ifndef SCRIPT_WALL_H
#define SCRIPT_WALL_H

#include "door.h"

namespace _ {

/** A singled contiguous buffer in a Chinese Room.
    Only one single wall is required for a Chinese Room, but when more memory 
    is needed a new Wall may be created and destroyed dynamically. This gives 
    the user .
*/
struct Wall {
    byte is_dynamic,        //< Flag for if using dynamic memory.
        num_doors,          //< The number of doors in this group.
        max_num_doors,      //< The number of doors allocated in memory.
        reserved;           //< Reserved for memory alignment.
    Wall* door_one;         //< The doors in door that messages pass through.
};

/** Initializes a Door at the beginning of the given buffer. */
KABUKI Wall* WallInit (uint_t size);

/** Gets a pointer to the array of pointers to Door(string). */
KABUKI Door** WallGetDoors (Wall* wall);

/** Adds a Door to the slot.
    @return Returns nullptr if the Door is full and a pointer to the Door in the 
            buffer upon success. */
KABUKI int WallAddDoor (Wall* wall, Door* door);

/** Gets the Door from the Door at the given index. */
KABUKI Door* WallGetDoor (Wall* wall, int index);

/** Deletes the Door from the Door at the given index. */
KABUKI void WallDelete (Wall* wall, int index);

/** Prints the given Door to the stdout. */
KABUKI void WallPrint (Wall* wall);

}       //< namespace _
#endif  //< SCRIPT_WALL_H
