/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/mirror.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_MIRROR_H
#define CHINESE_ROOM_MIRROR_H

#include "portal.h"
#include "automata.h"

namespace _ {

/** Reflection.
    Reflections are used for modifying 
    @see https://en.wikipedia.org/wiki/Reflection_(computer_programming) */
class Mirror : public Automata {
    public:

    /** Constructs an empty reflection. */
    Mirror ();
    
    /** Primes the pump. */
    virtual void Prime ();

    /** Length of the reflection. */
    virtual uint_t Length ();
    
    /** Feeds a byte through the Portal. */
    virtual void Feed (byte b);

    /** Pulls the next  byte through the Portal. */
    virtual byte Pull ();

    /** Sets the Automata. */
    Automata* SetAutomata (Automata* automata);

    /** Gets the Automata. */
    Automata* GetAutomata ();

    private:
    
    Automata* a;    //< The async monoid.
};

/** Reads the given set from the reflection. */
const Set* Read (Mirror* mirror, const uint_t* params, void** args);

/** Writes the given set to the reflection. */
const Set* Write (Mirror* mirror, const uint_t* params, void** args);

}       //< namespace _
#endif  //< CHINESE_ROOM_MIRROR_H
