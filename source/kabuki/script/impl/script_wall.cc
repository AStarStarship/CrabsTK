/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_wall.cc
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

#include <stdafx.h>
#include "../include/wall.h"

namespace _ {

Wall* WallInit (uint_t size) {
    Wall* wall = New<Wall, uint_t> (size, 1);
    if (wall == nullptr) return nullptr;
    wall->is_dynamic = 0;
    wall->num_doors = 0;
    wall->max_num_doors;
    wall->door_one = nullptr;
    return wall;
}

Door** WallGetDoors (Wall* wall) {
    return reinterpret_cast<Door**> (wall->door_one);
}

int WallAddDoor (Wall* wall, Door* door) {
    if (wall == nullptr) 
        return 0;
    if (door == nullptr)
        return 0;

    int num_doors = wall->num_doors;
    if (num_doors + 1 >= wall->max_num_doors)
        return 0;
    WallGetDoors (wall)[num_doors] = door;
    wall->num_doors = num_doors + 1;
    return num_doors + 1;
}

Door* WallGetDoor (Wall* wall, int index) {
    if (wall == nullptr)
        return nullptr;
    if (index >= wall->num_doors)
        return nullptr;
    return WallGetDoors (wall)[index];
}

void WallDelete (Wall* wall, int index) {
    if (wall == nullptr)
        return;
    if (index >= wall->num_doors)
        return;
    Door** doors = WallGetDoors (wall);
    Door* door = doors[index];
    door->~Door ();
    for (int i = index; i < wall->num_doors; ++i)
        doors[index] = doors[index + 1];
    --wall->num_doors;
}

void WallPrint (Wall* wall) {
    if (wall == nullptr) return;
    printf ("\nDoor:\nis_dynamic %s\nnum_doors: %u\nmax_num_doors: %u\n", 
            wall->is_dynamic ? "true" : "false", wall->num_doors, wall->max_num_doors);
}

}       //< namespace _
