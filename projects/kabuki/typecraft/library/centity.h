/* TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    /project/typecraft/inventory.h
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
#ifndef TYPECRAFT_PLAYER_H
#define TYPECRAFT_PLAYER_H

#include "cinventory.h"

namespace typecraft {

class Entity {
 public:
  Entity(const CH1* name = "Steve");

  /* Gets the X coordinate. */
  inline SI4 GetX();

  /* Sets the X Value. */
  inline void SetX(SI4 value);

  /* Gets the Y coordinate. */
  inline SI4 GetY();

  /* Sets the X Value. */
  inline void SetY(SI4 value);

  /* Gets the Y coordinate. */
  inline SI4 GetZ();

  /* Sets the X Value. */
  inline void SetZ(SI4 value);

  /* Gets the max number of blocks an Entity can reach to manipulate. */
  SI4 GetReach();

  /* Sets the max number of blocks an Entity can reach to manipulate.
      @return Returns zero upon success and the input value if it is < 0 . */
  SI4 SetReach(SI4 value);

  /* Gets the Entity's health. */
  SI4 GetHealth();

  /* Sets the Entity's health.
      @return Returns zero upon success and the input value if it is < 0 . */
  SI4 SetHealth(SI4 value);

  /* Gets the Entity's max health. */
  SI4 GetMaxHealth();

  /* Sets the max number of blocks an Entity can reach to manipulate.
      @return Returns zero upon success and the input value if it is < 0 . */
  SI4 SetMaxHealth(SI4 value);

  /* Gets the Entity's armor rating. */
  SI4 GetArmor();

  /* Sets the Entity's armor rating.
      @return Returns zero upon success and the input value if it is < 0 . */
  SI4 SetArmor(SI4 value);

  /* Gets the Entity's height measured in half blocks. */
  SI4 Height();

  /* Sets the Entity's height in half blocks.
      @return Returns zero upon success and the input value if it is < 0 . */
  SI4 SetHeight(SI4 value);

  /* Gets the Entity's hunger rating. */
  SI4 GetHunger();

  /* Sets the Entity's hunger rating.
      @return Returns zero upon success and the input value if it is < 0 . */
  SI4 SetHunger(SI4 value);

  /* Print's the Entity to the console. */
  void Print();

  /* Gets a reverence to the Inventory. */
  Inventory& GetInventory();

  /* Prints the Entity to the console. */
  virtual void Print();

 private:
  SI4 x_,                //< The Entity's x position.
      y_,                //< The Entity's y position.
      z_,                //< The Entity's z position.
      reach_,            //< How far the Entity can reach to Mine/Attack/etc.
      health_,           //< The Entity's health.
      max_health,        //< The Entity's max health.
      attack_,           //< The Entity's attack rating.
      armor_,            //< The Entity's armor rating.
      hunger_,           //< The Entity's hunger rating.
      height_;           //< The height of the Entity stored in half blocks.
  Inventory inventory_;  //< An Entity's Inventory.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_PLAYER_H
