/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/lllist.h
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

#ifndef KABUKI_DATA_LLLIST_H
#define KABUKI_DATA_LLLIST_H

#include "iterator.h"

namespace kabuki { namespace data {

/** A node in a linear linked list.  */
template<typename T>
class LlNode {
    T       data;
    LlNode* next;
};

/*< A linear linked list. */
template<typename T>
class LlList {
    public:

    LlList () {
        head_ = new LlNode<T> (0);
        ResetList ();
    }

    LlList (int max_elements) {
        head_ = new LlNode (0);
        resetList ();

        if (max_elements > 0) {
            max_size_ = max_elements;
        }
    }

    void Clear () {
        head_.left = head_;
        head_.right =head_;
        count_ = 0;
    }

    bool Add (T node) {
        if (newLink == head_ || count_ == max_size_)
            return false;

        count_++;
        LlNode newLinkedObj = new LlNode (newLink, count_);
        newLinkedObj.left = afterHere);
        newLinkedObj.right =afterHere.right);

        afterHere.right.left = newLinkedObj);
        afterHere.right =newLinkedObj);

        return true;
    }

    bool Insert (T node) {
        return add (newLink, head_);
    }

    bool Insert (T node, int index) {
        return insert (newLink, linkAtIndex (index)) {
            if (newLink == head_ || count_ == max_size_)
                return false;

            count_++;
            LlNode newLinkedObj = new LlNode (newLink, count_);
            newLinkedObj.right =beforeHere);
            newLinkedObj.left = beforeHere.getLeftChild ());

            beforeHere.getLeftChild ().right =newLinkedObj);
            beforeHere.left = newLinkedObj);

            return true;
        }
    }

    bool InsertAfter (T node, LlNode* afterHere);

    bool Insert (T node, LlNode* beforeHere);

    LlNode* Find (T objectToFind) {
        Iterator iterator = new LLListIterator ();
        void* thisobject;
        while (iterator.isNotDone ()) {
            thisobject = iterator.getNext ();
            if (thisobject == objectToFind)// iterator void*
                return iterator.currentLink ();
        }
        return nullptr;
    }

    bool Remove (T thisGuy)
    {
        return Remove (find (ptr));
    }

    bool Remove (int index) {
        if (atIndex > 0 && atIndex <= count_) {
            Iterator iterator = new LLListIterator ();
            void* thisobject;

            for (int i = 0; i < atIndex; i++)
                iterator.Next ();

            iterator.currentLink ().delete ();
            return true;
        }
        return false;
    }

    bool Remove (int index1, int index2) {
        // Index1 can not be less than zero or greater than index1
        if (index1 < 0 || index2 > count_ + 1 || index2 < index1) {
            Iterator iterator = new LLListIterator ();

            for (int i = 0; i < index1; i++)
                iterator.getNext ();

            for (int i = index1; i < index2; i++) {
                iterator.currentLink ().delete ();
                count_--;
            }
            return true;
        }
        return false;
    }

    bool Remove (LlNode* node) {
        if (leftChild.equals (head_))
            return false;

        leftChild.destroy ();
        count_--;

        return true;
    }

    bool Replace (int index, T new_data) {
        if (atIndex > 0 && atIndex <= count_) {
            linkAtIndex (atIndex).setData (withThis);
            return true;
        }
        return false;
    }

    bool KeepOnlyThese (const int* indexes) {
        if (indexList.length <= count_) {
            for (int i = 0; i < indexList.length; i++)
                if (indexList[i] < 0 || indexList[i] > count_)
                    return false;

            Iterator iterator = new LLListIterator ();

            for (int i = 1; i < indexList.length; i++) {
                for (int a = indexList[i - 1]; a < indexList[i]; a++)
                    iterator.currentLink ().delete ();
            }
        }
        return true;
    }

    bool Contains (T item) {
        void* currentobject;

        Iterator iterator = linearIterator ();

        while (iterator.isNotDone ()) {
            currentobject = iterator.getNext ();

            if (currentobject == item)
                return true;
        }
        return false;
    }

    T Element (int index) {
        if (atIndex > 0 && atIndex < count_) {
            Iterator iterator = new LLListIterator ();
            for (int i = 0; i < atIndex; i++)
                iterator.getNext ();
            return iterator.currentobject ();
        }
        return nullptr;
    }
    
    LlNode* LinkAtIndex (int index) {
        if (atIndex > 0 && atIndex < count_) {
            Iterator iterator = new LLListIterator ();

            for (int i = 0; i < atIndex; i++)
                iterator.getNext ();
            return iterator.currentLink ();
        }
        return nullptr;
    }

    bool IsEmpty () {
        return size == 0;
    }

    int GetCount () {
        return count_;
    }

    int GetMaxCount ()

    const char* RandomText (int numChars);

    void IncreaseMaxCount (int increase) {
        if (increase < 0)
            return;
        max_size_ += increase;
    }

    T* ToArray () {
        void** objectArray = new void*[count_];

        int i = 0;
        Iterator iterator = Iterator ();
        while (iterator.IsNotDone ()) {
            objectArray[i] = iterator.GetNext ();
            i++;
        }
        return objectArray;
    }

    Iterator* GetIterator () {
        return new LLListIterator ();
    }

    private;

    int     count_;
    LlNode* head_;
};

}       //< namespace data
}       //< namespace kabuki
#endif  //< KABUKI_DATA_LLLIST_H
