/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/array.h
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

#ifndef KABUKI_DATA_ARRAY_H
#define KABUKI_DATA_ARRAY_H

#include "config.h"

namespace kabuki { namespace data {
}       //< namespace data
}       //< namespace kabuki
#endif  //< KABUKI_DATA_ARRAY_H

class ArrayIterator;

class Array
/*< An collection of owned items stored in a packed array. */
{
    public:
    
    Array (size_t element_size, int initBufferSize);
    /*< Constructs an array with the given element and buffer size. */
    
    ~Array ();
    
    void clear ();
    /*< Resets the Collection without deleting the contents. */
    
    bool add (void* ptr);
    /*< Adds the given object from the given pointer. */
    
    bool add (const Array& a);
    /*< Adds the given array to this one. */
    
    bool contains (void* Data);
    /*< Returns true if this array contains the given data. */
    
    bool contains (const Array& a);
    /*< Returns true if this Collection contains thatCollection. */
    
    bool equals (void* Data);
    /*< Returns true if this Collection contains only the given data. */
    
    hash_t getHash ();
    /*< Generates a hash for this Collection. */
    
    bool isEmpty ();
    /*< Returns true of this Collection is empty. */

    bool remove (void* Data);
    /*< Removes that object from the collection. */
    
    bool remove (Array& a);
    /*< Removes that object from the collection. */
    
    bool remove (Collection& c);
    /*< Removes that object from the collection. */

    bool retain (Collection& c);
    /*< Removes all but the given collection from this collection. */

    /*< Gets an iterator for this collection. */
    ArrayIterator* GetIterator ();

    /*< Gets the size_t of the object being stored. */
    size_t GetSize ();

    /*< Returns a Script Array packed array of the collection. */
    void* ToScript ();
    
    private:
    
    size_t element_size;     //< The sizeof this object size.
    int numItems;           //< The number of items.
    
    void* base;             //< Pointer to the dynamic memory.
};

}
