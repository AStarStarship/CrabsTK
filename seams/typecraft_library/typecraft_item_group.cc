/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/inventory.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_ITEMGROUP_H
#define TYPECRAFT_ITEMGROUP_H

#include "array.h"
#include "item.h"

namespace typecraft {

class ItemGroup {
 public:
  /** Constructs an ItemGroup with the given max_size. */
  ItemGroup(int max_size);

  /** Destructor */
  ~ItemGroup();

  /** Gets the inventory count. */
  int GetCount();

  /** Gets the max size of the Inventory. */
  int GetSize();

  /** Gets the Item at the given index. */
  Item* GetItem(int index);

  /** Adds the Item at the given index.
      @param  item Th Item to add.
      @return Returns -1 if the Inventory is full. */
  int AddItem(Item* item);

  /** Removes the Item at the given index.
  @param  The index of the Item to remove.
  @return Returns nullptr if the Index is out of bounds. */
  Item* RemoveItem(int index);

  /** Prints the ItemGroup to the console. */
  void Print();

 private:
  _::ArX<Item*> items_;  //< An array of Inventory Items.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_ITEMGROUP_H
