/** Kabuki Software Development Kit
    @version 0.x
    @file    /.../KabukiSDK/_Data/IIterator.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_IITERATOR_H
#define CHINESEROOM_IITERATOR_H

namespace _ {

class IIterator
/*< An interface for a generic iterator. */
{
    public:
    
    virtual void* getNext() = 0;
    
    virtual void reset() = 0;
    
    virtual size_t getSize() = 0;
};

class IIterable
/*< An interface that returns an instance of an IIterator. */
{
    public:

    virtual IIterator* getIterator() = 0;
};

}       //< namespace _

#endif  //< CHINESEROOM_IITERATOR_H
