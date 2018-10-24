/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/mob.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_MOB_H
#define TYPECRAFT_MOB_H

#include "entity.h"

namespace typecraft {

class Mob : public Entity {
 public:
  Mob(const char* name, );

  /** Constructs an Mob with the given max_size. */
  Mob(int max_size);

  /** Destructor */
  ~Mob();

  /** Prints the Item to the console. */
  virtual void Print();

 private:
};

// Hostile Mobs

class Blaze : public Mob {
 public:
  /** Constructs a new Blaze. */
  Blaze();
};

class CaveSpider : public Mob {
 public:
  /** Constructs a new CaveSpider. */
  CaveSpider();
};

class Creeper : public Mob {
 public:
  /** Constructs a new Creeper. */
  Creeper();
};

class Ghast : public Mob {
 public:
  /** Constructs a new Ghast. */
  Ghast();
};

class MagmaCube : public Mob {
 public:
  /** Constructs a new MagmaCube. */
  MagmaCube();
};

class Silverfish : public Mob {
 public:
  /** Constructs a new Silverfish. */
  Silverfish();
};

class Skeleton : public Mob {
 public:
  /** Constructs a new Skeleton. */
  Skeleton();
};

/** A slime.
    Small slimes are passive, and Medium and Large Slimes are Hostile. */
class Slime : public Mob {
 public:
  /** Constructs a new Slime. */
  Slime();
};

/** A Spider.
    Starts being and remains hostile if light level is below 10, otherwise
   Neutral.
*/
class Spider : public Mob {
 public:
  /** Constructs a new Spider. */
  Spider();
};

class SpiderJockey {
 public:
  /** Constructs a new BlazeSpiderJockey. */
  SpiderJockey();
};

class Zombie {
 public:
  /** Constructs a new BlazeZombie. */
  Zombie();
};

class ZombieVillager {
 public:
  /** Constructs a new ZombieVillager. */
  ZombieVillager();
};

class WitherSkeleton {
 public:
  /** Constructs a new WitherSkeleton. */
  WitherSkeleton();
};

class Witch {
 public:
  /** Constructs a new Witch. */
  Witch();
};

class Evoker {
 public:
  /** Constructs a new Evoker. */
  Evoker();
};

class Vindicator {
 public:
  /** Constructs a new Vindicator. */
  Vindicator();
};

class Vex {
 public:
  /** Constructs a new Vex. */
  Vex();
};

class ChickenJockey {
 public:
  /** Constructs a new ChickenJockey. */
  ChickenJockey();
};

class Endermite {
 public:
  /** Constructs a new Endermite. */
  Endermite();
};

class Guardian {
 public:
  /** Constructs a new Guardian. */
  Guardian();
};

class ElderGuardian {
 public:
  /** Constructs a new ElderGuardian. */
  ElderGuardian();
};

class KillerRabbit {
 public:
  /** Constructs a new KillerRabbit. */
  KillerRabbit();
};

class BabyZombie {
 public:
  /** Constructs a new BabyZombie. */
  BabyZombie();
};

class Shulker {
 public:
  /** Constructs a new Shulker. */
  Shulker();
};

class SkeletonTrapHorse {
 public:
  /** Constructs a new SkeletonTrapHorse. */
  SkeletonTrapHorse();
};

class SkeletonHorseman {
 public:
  /** Constructs a new SkeletonHorseman. */
  SkeletonHorseman();
};

class HuskFace {
 public:
  /** Constructs a new HuskFace. */
  HuskFace();
};

class Husk {
 public:
  /** Constructs a new Husk. */
  Husk();
};

class StrayFace {
 public:
  /** Constructs a new StrayFace. */
  StrayFace();
};

class Stray {
 public:
  /** Constructs a new Stray. */
  Stray();
};

// Neutral Mobs

class Enderman {
 public:
  /** Constructs a new StrayFace. */
  Enderman();
};

class ZombiePigman {
 public:
  /** Constructs a new ZombiePigman. */
  ZombiePigman();
};

class BetterWolfFace {
 public:
  /** Constructs a new BetterWolfFace. */
  BetterWolfFace();
};

/** A Wolf mob.
    Tamable */
class Wolf {
 public:
  /** Constructs a new Wolf. */
  Wolf();
};
/** A spider.
   (Hostile if light level is below 10, otherwise Neutral) */
class Spider {
 public:
  /** Constructs a new Spider. */
  Spider();
};

class PolarBear {
 public:
  /** Constructs a new PolarBear. */
  PolarBear();
};

/** A big ass smell farm Llama.
    aggressive to untamed wolves, attacks once if player attacks. */
class Llama {
 public:
  /** Constructs a new Llama. */
  Llama();
};

// Passive Mobs

/** A Chicken. */
class Chicken {
 public:
  /** Constructs a new Chicken. */
  Chicken();
};

/** A Cow. */
class Cow {
 public:
  /** Constructs a new Cow. */
  Cow();
};

/** A Mooshroom. */
class Mooshroom {
 public:
  /** Constructs a new Mooshroom. */
  Mooshroom();
};

/** A Pig. */
class Pig {
 public:
  /** Constructs a new Pig. */
  Pig();
};

/** A Sheep. */
class Sheep {
 public:
  /** Constructs a new Sheep. */
  Sheep();
};

/** A Squid. */
class Squid {
 public:
  /** Constructs a new Squid. */
  Squid();
};

/** A Villager. */
class Villager {
 public:
  /** Constructs a new Villager. */
  Villager();
};

/** A Bat. */
class Bat {
 public:
  /** Constructs a new Bat. */
  Bat();
};

/** A Ocelot.
    Tamable. */
class Ocelot {
 public:
  /** Constructs a new Ocelot. */
  Ocelot();
};

/** A Horse. */
class Horse {
 public:
  /** Constructs a new Horse. */
  Horse();
};

/** A Donkey. */
class Donkey {
 public:
  /** Constructs a new Donkey. */
  Donkey();
};

/** A Mule. */
class Mule {
 public:
  /** Constructs a new Mule. */
  Mule();
};

/** A Cat.
    Domesticated Ocelot*/
class Cat {
 public:
  /** Constructs a new Cat. */
  Cat();
};

/** A Dog.
    Domesticated Wolf. */
class Dog {
 public:
  /** Constructs a new Dog. */
  Dog();
};

/** A Rabbit. */
class Rabbit {
 public:
  /** Constructs a new Rabbit. */
  Rabbit();
};

/** A Parrot. */
class Parrot {
 public:
  /** Constructs a new Parrot. */
  Parrot();
};

}  // namespace typecraft
#endif  //< TYPECRAFT_MOB_H
