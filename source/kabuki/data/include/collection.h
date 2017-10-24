/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/collection.h
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

#ifndef KABUKI_DATA_BTREE_H
#define KABUKI_DATA_BTREE_H

#include "config.h"

namespace kabuki { namespace data {
}       //< namespace data
}       //< namespace kabuki
#endif  //< KABUKI_DATA_BTREE_H

namespace _Data {
    
class ICollection
/*< Interface for a generic collection. */
{
    public:
    
    virtual bool Add (void* Data) = 0;
    /*< Adds thatObject to this Collection. */
    
    virtual bool Add (const Collection* C) = 0;
    /*< Adds the given collection to this one. */
    
    virtual void Delete () = 0;
    /*< Deletes the Collection. */
    
    virtual void Clear () = 0;
    /*< Resets the Collection without deleting the contents. */

    virtual bool contains (void* Data) = 0;
    /*< Returns true if this Collection contains thatObect. */
    
    virtual bool contains (const Collection* c) = 0;
    /*< Returns true if this Collection contains thatCollection. */

    virtual bool equals (void* Data) = 0;
    /*< Returns true if this Collection contains only the given data. */

    virtual long GetHash () = 0;
    /*< Generates a hash for this Collection. */
    
    virtual bool IsEmpty () = 0;
    /*< Returns true of this Collection is empty. */

    virtual bool Remove (void* Data) = 0;
    /*< Removes that object from the collection. */
    
    virtual bool Remove (Collection* C) = 0;
    /*< Removes that object from the collection. */

    virtual bool Retain (Collection* C) = 0;
    /*< Removes all but the given collection from this collection. */

    virtual IIterator* GetIterator () = 0;
    /*< Gets an iterator for this collection. */
    
    virtual size_t GetSize () = 0
    /*< Gets the size_t of the object being stored. */

    virtual void* ToArray () = 0;
    /*< Returns a pointer to a packed array of the collection. */
};
}
