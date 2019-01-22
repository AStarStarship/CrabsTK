/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/typecraft_inventory.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#include "chunk.h"

namespace typecraft {

Chunk::Chunk() {}

int Chunk::GetX() { return x_; }

int Chunk::GetY() { return y_; }

int Chunk::GetZ() { return z_; }

Block* Chunk::GetBlock(int x, int y, int z) {
  if (x < 0) return nullptr;
  if (y < 0) return nullptr;
  if (z < 0) return nullptr;
  if (x >= kSize) return nullptr;
  if (y >= kSize) return nullptr;
  if (z >= kHeight) return nullptr;
  return blocks_[x][y][z];
}

void Chunk::SetBlock(Block* block, int const x, int y, int z) {
  if (x < 0) return;
  if (y < 0) return;
  if (z < 0) return;
  if (x >= kSize) return;
  if (y >= kSize) return;
  if (z >= kHeight) return;
  blocks_[x][y][z]->Change(block);
}

}  // namespace typecraft
