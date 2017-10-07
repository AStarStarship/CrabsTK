/** The Chinese Room
    @version 0.x
    @file    ~/chinese_room/impl/wall.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
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

#include "../include/wall.h"

namespace _ {

Wall* WallInit (uint_t size) {
    Wall* w = New<Wall, uint_t> (size, 1);
    if (w == nullptr) return nullptr;
    w->is_dynamic = 0;
    w->num_doors = 0;
    w->max_num_doors;
    w->door_one = nullptr;
}

Door** GetDoors (Wall* wall) {
    return reinterpret_cast<Door**> (d->door_one);
}

Door* AddDoor (Wall* wall, Door* t) {
    if (s == nullptr) return nullptr;
    if (t == nullptr) return nullptr;
    byte num_doors = d->num_doors;
    if (num_doors + 1 >= d->max_num_doors) return nullptr;
    GetDoors (s)[num_doors] = t;
    ++d->num_doors;
}

Door* GetDoor (Wall* wall, char_t index) {
    if (s == nullptr) return nullptr;
    if (index >= d->num_doors)
        return nullptr;
    return GetDoors (s)[index];
}

void Delete (Wall* wall, char_t index) {
    if (s == nullptr) return;
    if (index >= d->num_doors)
        return;
    Door** ts = GetDoors (s);
    Door* t = ts[index];
    t->~Door ();
    for (byte i = index; i < d->num_doors; ++i)
        ts[index] = ts[index + 1];
    --d->num_doors;
}

void Print (Wall* wall) {
    if (s == nullptr) return;
    printf ("\nDoor:\nis_dynamic %s\nnum_doors: %u\nmax_num_doors: %u\n", 
            d->is_dynamic ? "true" : "false", d->num_doors, d->max_num_doors);
}

}       //< namespace _
