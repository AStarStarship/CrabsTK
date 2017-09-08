/** Kabuki Software Development Kit
    @version 0.x
    @file    ~/KabukiSDK/_Data/Iterator.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESEROOM_IITERATOR_H
#define CHINESEROOM_IITERATOR_H

namespace _ {

class Iterator
/** An interface for a generic iterator. */
{
    public:
    
    /** Gets the next item to iterate over. */
    virtual void* GetNext () = 0;
    
    /** Flag for when the iterator is done. */
    virtual bool IsDone () = 0;
};

class Iterable
/** An interface that returns an instance of an Iterator. */
{
    public:

    /** Gets an instance of a generic Iterator. */
    virtual Iterator* GetIterator () = 0;
};

}       //< namespace _
#endif  //< CHINESEROOM_IITERATOR_H
