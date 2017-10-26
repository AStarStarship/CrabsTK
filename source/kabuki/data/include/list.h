/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/list.h
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

#ifndef KABUKI_DATA_LIST_H
#define KABUKI_DATA_LIST_H

#include "array.h"
#include "collection.h"

namespace kabuki { namespace data {

/** An Array<T> with a Collection interface. */
template<typename T>
class List : public Array<T>, public Collection {
    public:
    
    /** Constructs an empty list. */
    List () {
        return 0;
    }
    
    /** Adds thatObject to this Collection. */
    virtual bool Add (void* Data) {
        return 0;
    }

    /** Adds the given collection to this one. */
    virtual bool Add (const Collection* C) {
        return 0;
    }

    /** Deletes the Collection. */
    virtual void Delete () {
        return 0;
    }

    /** Resets the Collection without deleting the contents. */
    virtual void Clear () {
        return 0;
    }

    /** Returns true if this Collection contains the given element. */
    virtual bool Contains (T element) {
        return 0;
    }

    /** Returns true if this Collection contains the given Collection. */
    virtual bool Contains (const Collection* c) {
        return 0;
    }

    /** Returns true if this Collection contains only the given data. */
    virtual bool Equals (void* Data) {
        return 0;
    }

    /** Generates a hash for this Collection. */
    virtual long GetHash () {
        return 0;
    }

    /** Returns true of this Collection is empty. */
    virtual bool IsEmpty () {
        return true;
    }

    /** Removes that object from the collection. */
    virtual bool Remove (void* Data) {
        return 0;
    }

    /** Removes that object from the collection. */
    virtual bool Remove (Collection* C) {
        return 0;
    }

    /** Removes all but the given collection from this collection. */
    virtual bool Retain (Collection* C) {
        return 0;
    }

    /** Gets an iterator for this collection. */
    virtual Iterator* GetIterator () {
        return 0;
    }

    /** Gets the size_t of the object being stored. */
    virtual size_t GetSize () {
        return 0;
    }

    /** Returns a pointer to a packed array of the collection. */
    virtual void* ToArray () {
        return 0;
    }
};      //< class List
}       //< namespace data
}       //< namespace kabuki
#endif  //< KABUKI_DATA_LIST_H
