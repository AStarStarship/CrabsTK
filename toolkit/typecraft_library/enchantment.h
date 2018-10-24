/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/enchantment.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_ENCHANTMENT_H
#define TYPECRAFT_ENCHANTMENT_H

#include "array.h"
#include "item_group.h"

namespace typecraft {

/** # Summary of enchantments

| Name                  | Summary                             | JEID | PEID | ML
| W  |
|:---------------------:|:-----------------------------------:|:----:|:----:|:---:|:--:|
| Aqua Affinity         | Increases underwater mining rate    | 6    | 8    | I
| 2  | | Bane of Arthropods    | Increases damage to arthropods      | 18   | 11
| V   | 5  | | Blast Protection      | Reduces explosion damage            | 3
| 3    | IV  | 2  | | Curse of Binding **   | Prevents removal of item | 10   |
—    | I   | 1  | | Curse of Vanishing ** | Item destroyed on death | 71   | —
| I   | 1  | | Depth Strider         | Increases underwater movement speed | 8
| 7    | III | 2  | | Efficiency            | Increases mining speed | 32   | 15
| V   | 10 | | Feather Falling       | Reduces fall damage                 | 2
| 2    | IV  | 5  | | Fire Aspect           | Sets target on fire | 20   | 13 |
II  | 2  | | Fire Protection       | Reduces fire damage                 | 1 | 1
| IV  | 5  | | Flame                 | Arrows set target on fire           | 50
| 21   | I   | 2  | | Fortune               | Increases block drops | 35   | 18
| III | 2  | | Frost Walker          | Walk on water                       | 9
| 25   | II  | 2  | | Infinity              | Shooting consumes no arrows | 51
| 22   | I   | 1  | | Knockback             | Increases knockback | 19   | 12 |
II  | 5  | | Looting               | Increases mob loot                  | 21 |
14   | III | 2  | | Luck of the Sea       | Increases fishing luck | 61   | 23
| III | 2  | | Lure                  | Increases fishing rate              | 62
| 24   | III | 2  | | Mending               | Repair with experience | 70   | 26
| I   | 2  | | Power                 | Increases arrow damage              | 48
| 19   | V   | 10 | | Projectile Protection | Reduces projectile damage | 4    |
4    | IV  | 5  | | Protection            | Reduces most damage | 0    | 0    |
IV  | 10 | | Punch                 | Increases arrow knockback           | 49 |
20   | II  | 2  | | Respiration           | Extends underwater breathing time |
5    | 6    | III | 2  | | Sharpness             | Increases damage | 16   | 9
| V   | 10 | | Silk Touch            | Mined blocks drop themselves        | 33
| 16   | I   | 1  | | Smite                 | Increases damage to undead | 17 |
10   | V   | 5  | | Sweeping Edge *       | Increases sweeping attack damage |
22   | —    | III | 2  | | Thorns                | Damages attackers | 7    | 5
| III | 1  | | Unbreaking            | Increases effective durability      | 34
| 17   | III | 5  |

* Key: JEID = Java Edition ID. PEID = Pocket Edition ID; ML = Max Level; W =
Weight; * = Java Edition Only; ** = Java and Legacy Console editions only] */

class Enchantment {
 public:
  /** Constructs an Enchantment. */
  Inventory();

  /** Gets inventory count. */
  int GetCount();

  /** Adds an Item. */
  int AddItem(Item* item);

  /** Removes the Item with the given index.
      @param  The index of the Item to remove.
      @return Returns nullptr if the index is out of bounds and a pointer to
              the Item it removed upon success. */
  Item* RemoveItem(int index);

  /** Deletes the entire Inventory. */
  void DeleteAll();

  /** Prints the Inventory to the console. */
  void Print();

 private:
  _::Array<ItemGroup*> items_;  //< An array of Inventory Items.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_ENCHANTMENT_H
