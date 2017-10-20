/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/table.h
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

#ifndef CHINESE_ROOM_TABLE_H
#define CHINESE_ROOM_TABLE_H

#include "utils.h"
#include "error.h"

namespace _ {

/** A table designed to work with Star (s) with less than 192 members.
    
    # Memory Layout

    @code
       _____________________________________________________
       |_______ | Key 1                                    |
       |_______ | ...                          Keys        |
       |_______ v Key N                                    |
       |___________________________________________________|
       |                                                   |
       |               Header Buffer Space                 |
       |___________________________________________________|
       |_______                                            |
       |_______   Collision List N          Hash Table     |
       |_______ ^ ...                        Collision     |
       |_______ | Collision List 0              List       |
       |___________________________________________________|
       |_______                          Element Size = 1  |
       |_______   Buffer Indexes                           |
       |_______                             Hash Table     |
       |_______ ^ Collision Table N          Unsorted      |
       |_______ | ...                         Indexes      |
       |        | Collision table 0                        |
       |___________________________________________________|
       |_______                          Element Size = 1  |
       |_______   Buffer Indexes                           |
       |_______                             Hash Table     |
       |_______ ^ Collision Index N          Collision     |
       |_______ | ...                         Indexes      |
       |        | Collision Index 0                        |
       |___________________________________________________|
       |_______                         Element Size = 2   |
       |_______   Buffer Indexes                           |
       |_______                              16-bit        |
       |_______ ^ Key Offset N                 key         |
       |_______ | ...                        Offsets       |
       |        | Key Offset 1                             |
       |___________________________________________________|
       |_______                         Element Size = 2   |
       |_______   Buffer Indexes                           |
       |_______                              16-bit        |
       |_______ ^ Sorted Hash N              Hashes        |
       |_______ | ...                                      |
       |        | Sorted Hash 1                            |
       |___________________________________________________|
     ^ |                                                   |
     | |                 Header (8 bytes)                  |
    0x0|___________________________________________________|
    @endcode
*/
struct Table {
    byte num_keys,              //< Number of keys.
        max_keys;               //< Number of buffered indexes.
    uint16_t pile_size,         //< Size of the collision table pile.
        size;                   //< Size of this object in bytes.
};

enum {
    kMinTableSize = 64,   //< The min size of a Table
    kInvalidRecord      = 255,  //< Invalid Record index.
    kMaxNumOperations      = 192,  //< The max number of members in a particular scope.
    kNoCollidingRecords = 255,  //< 
    kRecordOverflow     = 254,  //< 
    kTableFull    = 253,  //< 
    kOverheadPerRecord  = sizeof (hash16_t) + sizeof (uint16_t) + sizeof (byte) + 
        sizeof (byte),
};

//inline void Print (Table* rt);

/** Constructs a streamable hash table with enough buffer space for the 
    max_keys.
    @post    Users might want to call the isValid () function after 
             construction to verify the integrity of the object.
    @warning The reservedNumOperations must be aligned to a 32-bit value, and 
             it will get rounded up to the next higher multiple of 4. */
KABUKI Table* TableInit (byte* buffer, byte max_keys, uint16_t total_size);
    
/** Adds the given key and returns the index 64-255 of the index of the 
    op.
    @return Returns an index 64-255 */
KABUKI byte TableAdd (Table* table, const char* key);

/** Attempts to find the given key.
    @return Returns 0 upon failure, and valid index upon success. */
KABUKI byte TableFind (const Table* table, const char* key);

/** Prints this object out to the console. */
KABUKI void TablePrint (Table* table);

/** Attempts to find the given key.
    @return Returns 0 upon failure, and valid index upon success. */
KABUKI byte Find (const Table* rt, const char * key);

/** Prints this object out to the console. */
KABUKI void Print (Table* rt);

}       //< namespace _
#endif  //< CHINESE_ROOM_TABLE_H
