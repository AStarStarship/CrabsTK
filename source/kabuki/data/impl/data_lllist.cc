/** Kabuki Software Development Kit
    @file     ~/source/kabuki/data/LLList.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include "../include/lllist.h"

namespace kabuki { namespace data {

LlList::LlList ()


LlList::LlList (int max_elements)
{
    head_ = new LlNode (0);
    resetList ();

    if (max_elements > 0)
    {
        max_size_ = max_elements;
    }
}


LlList::LlList (File listHead)
{
    head_ = new LlNode (listHead);
    resetList ();
}

void LlList::resetList ()


bool LlList::add (void* newLink)
{
    return insert (newLink, head_);
}

bool LlList::insert (void* newLink)


bool LlList::insert (void* newLink, int indexNum)


bool LlList::add (void* newLink, LlNode afterHere)


bool LlList::insert (void* newLink, LlNode beforeHere)


LlNode LlList::find (void* objectToFind)


bool LlList::remove (void* ptr) 

bool LlList::remove (int atIndex)


bool LlList::remove (int index1, int index2)


bool LlList::remove (LlNode* leftChild)


bool LlList::replace (int atIndex, void* withThis)


bool LlList::keepOnlyThese (int[] indexList)


bool LlList::contains (void* o)


void* LlList::element (int atIndex)

LlNode* LlList::getLinkAtIndex (int atIndex)


bool LlList::isEmpty ()
{
    if (size_ == 0)
        return true;
    return false;
}

int LlList::GetSize ()


int LlList::GetMaxSize ()
{
    return max_size_;
}

void LlList::IncreaseMaxSize (int increase)


void** LlList::ToArray ()


Iterator LlList::GetIterator ()


Iterator* GetLinearIterator ()
{
    return new LLListIterator ();
}

}   //< namespace data
}   //< namespace kabuki
