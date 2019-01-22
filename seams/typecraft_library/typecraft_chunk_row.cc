/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/typecraft_chunk_row.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_CHUNKROW_H
#define TYPECRAFT_CHUNKROW_H

#include "chunk.h"

namespace typecraft {

/** A row of Chunk(s) that can grow on the positive and negative directions.
    Chunks are designed to grow in the Cartesian quadrants 1 and 2, or 3 and 4.
*/
class ChunkRow {
 public:
  enum {

  };

  ChunkRow(int max_size);

  /** Gets inventory count. */
  int GetCount();

  /** Adds a Chunk to the top of the Stack. */
  int AddChunk(Chunk* chunk);

  /** Gets the Chunk at the given Index.
      @return Returns null if the index is out of bounds. */
  Chunk* GetChunk(int index);

  /** Prints the World to the console. */
  void Print();

 private:
  int row_;                  //< The row number of the chunk.
  _::ArX<Chunk*> cunks_[4];  //< A stack of Chunks.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_CHUNKROW_H
