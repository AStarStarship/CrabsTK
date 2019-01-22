/** TypeCraf/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/crafting_table.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_RECIPE_H
#define TYPECRAFT_RECIPE_H

#include "Item.h"

namespace typecraft {

/** A Crafting Table recipe.
    Recipes do not take up the entire crafting table. For this reason, the
    width and height of the recipe are stored.
*/
class Recipe : public Item {
 public:
  /** Default constructs a empty CraftingTable. */
  Recipe();

  /** Prints the Recipe to the Console. */
  void Print();

 private:
  char *name_,                  //< The name of the Recipe.
      *description_;            //< A description of the Recipe.
  int width_,                   //< The width of the recipe in ingredients.
      height_;                  //< The height of the recipe in ingredients.
  ItemType ingredients_[3][3];  //< The ingredients.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_RECIPE_H
