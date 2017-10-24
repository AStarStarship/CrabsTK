/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/iterator.h
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

class IIterator
/*< An interface for a generic iterator. */
{
    public:
    
    virtual void* GetNext () = 0;
    
    virtual void Reset () = 0;
    
    virtual size_t GetSize () = 0;
};
}   //< namespace _Data