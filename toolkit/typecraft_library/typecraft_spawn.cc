/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/block.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_SPAWN_H
#define TYPECRAFT_SPAWN_H

#include "item.h"

namespace typecraft {

/** Each individual spawn attempt succeeds only if all of the following
conditions are met:

There must be no players and no player spawn points within a 24 block distance
(spherical) of the spawning block If it's a squid or a guardian, then... the
spawning block must contain liquid (water) If it's a squid, then... the spawning
block must be between level 46 and 62, inclusive the spawning mob must not be
obstructed by other mobs If it's a guardian, then... the difficulty must not be
peaceful 95% chance of failure if the spawning block has sky exposure (details)
If it's not a squid or a guardian, then...
the block directly below it must have a solid top surface (opaque, upside down
slabs / stairs and others) the block directly below it may not be bedrock or
barrier the spawning block and the block above must be made of a material that
does not block movement, must be non-liquid and not powered, and cannot be
rails, powered rails, detector rails, or activator rails. the spawning mob
hitbox must not be obstructed by solid blocks, other mobs or liquids If it's a
bat, then... the spawning block must be at level 62 or below If the real-time
day is between October 20 and November 3, then the light level must be 7 or
darker. Otherwise the light level must be 4 or darker. If it's an animal (other
than a mooshroom or ocelot), then... the light level of the spawning block must
be 9 or brighter the block directly below the spawning block must be grass If
it's a mooshroom, then... the light level of the spawning block must be 9 or
brighter the block directly below the spawning block must be mycelium If it's an
ocelot, then... the spawning block must be above level 63 the block directly
below the spawning block must be grass or leaves there is a 1⁄3 chance the spawn
will fail If it's a hostile mob (except as noted below), then... the difficulty
must not be peaceful the light level of the spawning block must be 7 or darker
(with exception during thunderstorms), and more light increases the chance that
the spawn will fail sunlight falling on the spawning block further increases the
chance that the spawn will fail, by up to 50% If it's a slime, then... the light
checks in the general hostile mob check don't apply one of the following must be
true: option 1: the spawning block must be in a chunk eligible to spawn slimes
(see the slime page for details) the spawning block must be below level 40 90%
chance of failure option 2: the spawning block must be in a swamp biome the
spawning block be on level 51 through 69 inclusive chance of failure based on
the phase of the moon the light level of the spawning block must be 7 or darker,
and more light increases the chance that the spawn will fail 50% chance of
failure If it's a ghast, then... the light checks in the general hostile mob
check don't apply there is a 95% chance the spawn will fail, thus ghasts only
spawn 1⁄20 as often as other mobs If it's a magma cube or zombie pigman, then...
the light checks in the general hostile mob check don't apply
If it's a blaze, then...
the light checks in the general hostile mob check don't apply
the light level must be 11 or darker
If it's a skeleton in the Nether, then...
there is an 80% chance of spawning a wither skeleton instead

*/

Spawn::Spawn(const char* name);

int Spawn::GetType() {}

/** Sets the type. */
const char* Spawn::Setype(int type) {}

void Spawn::Mine() {}
}  // namespace typecraft
#endif  //< TYPECRAFT_SPAWN_H
