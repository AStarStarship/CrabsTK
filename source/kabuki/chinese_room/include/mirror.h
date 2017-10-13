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

/** Initializes the mirror with the given tx mirror.
    @param tx The io mirror.
    @param is_dynamic Flag for if we're using dynamic memory or not. */
inline Mirror LogInit (MonoidTx* tx, bool is_dynamic = false);

/** Prints a message with the given header to the given Tx slot. */
inline ticket_t Write (MonoidTx* tx, byte member, const char * string);

/** Prints a message with the given header to the given Tx slot. */
inline ticket_t Write (Mirror& mirror, byte member, const char * string);

/** Returns a reference to the mirror for this assembly. */
template<uint_t kNumber>
inline Mirror& Logbag () {
    static byte* buffer = Buffer<kNumber, kLogSize> ();
    static Mirror s = LogInit (TxInit (buffer, kLogSize));
    return s;
}

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, int8_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, uint8_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, int16_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, uint16_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, int32_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, uint32_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, int64_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, uint64_t value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, float value);

/** Operator << prints the given value as a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, double value);

/** Operator << prints a string to the mirror. */
inline Mirror& operator<< (Mirror& mirror, const char * s);

}       //< namespace _
#endif  //< CHINESE_ROOM_MIRROR_H
