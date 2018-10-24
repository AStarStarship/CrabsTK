/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/block.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_BLOCK_H
#define TYPECRAFT_BLOCK_H

#include "../../../kabuki-toolkit/source/kabuki/data/include/array.h"
#include "item.h"

using namespace _::data;

namespace typecraft {

/** A MineCraft block that may contain items when it breaks.
    When you break a Block in MineCraft, it may give you zero or more Item(s)
    all of the same type.
*/
class Block : public Item {
 public:
  /** Constructor creates a Block with the given count of the same Item. */
  Block(int type, int variant, int count = 0);

  /** Gets the type of Block. */
  int GetNumItems();

  /** Gets the type. */
  Item::Type Getype();

  /** Mines a block.
      @return Returns 0 if not mined and */
  int Mine(Item* tool);

 private:
  int strenth_,          //< Block strength.
      regeneration_;     //< How fast the block regenerates.
  Array<Item*>* items_;  //< Items contained in this block.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_BLOCK_H
