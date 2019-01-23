/* TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    /project/typecraft/cworld.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#ifndef INCLUDED_TYPECRAFT_WORLD
#define INCLUDED_TYPECRAFT_WORLD

#include "cchunk.h"

using namespace _::data;

namespace typecraft {

/* A TypeCraft World.

All world's start out as a N x N Square Matrix. Once a player gets one maximum
view distance from an unexplored Chunk, a new chunk is added onto the map.

# Example

```
                         x=0
-N  |==|==|==|==|==|==|==|==|==|==|==|==|==|==|==| +N
    |
    |                         +==+
    |                         |  |
    |                         +==+
    |                         |  |
    |  +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
    |  +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |        |  |  |  |  |  |  |  |  |  |  |
    |        +==+==+==+==+==+==+==+==+==+==+
    |        |  |  |  |  |  |  |  |  |  |  |
    |        +==+==+==+==+==+==+==+==+==+==+
y=0 |        |  |  |  |  |  |  |  |  |  |  |
    |        +==+==+==+==+==+==+==+==+==+==+
    |  +==+==+==+==+==+==+==+==+==+==+==+==+==+
    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
    |  +==+==+==+==+==+==+==+==+==+==+==+==+==+
    |              +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |              |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
    |              +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |              |  |
    |              +==+
    |              |  |
    |        +==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |        |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
    |        +==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                +==+
    |                |  |
    |                +==+
```

*/
class World {
 public:
  enum {
    kHeight = 256,  //< The height of a MineCraft world: 256.
  };

  /* Constructs a world with the default size. */
  World();

  /* Gets inventory count. */
  int GetCount();

  /* Adds an Item. */
  int AddItem(Item* item);

  /* Removes the Item with the given index.
  @param  The index of the Item to remove.
  @return Returns nullptr if the index is out of bounds and a pointer to
  the Item it removed upon success. */
  Item* RemoveItem(int index);

  /* Deletes the entire World. */
  void DeleteAll();

  /* Prints the World to the console. */
  void Print();

 private:
  const char* name_;  //< The World's name.
  int difficulty_;    //< The difficulty level.
  Chunk* zone;        //< The current Chunk the PLayer is in.
  ArX<Chunk*>
      quad_[4];  //< The 4 Quadrant Chunks ordered like a Cartesian plane.
};

}  // namespace typecraft
#endif  //< INCLUDED_TYPECRAFT_WORLD
