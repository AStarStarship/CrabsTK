/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/data/list_fifo.h
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

#ifndef HEADER_FOR_KABUKI_DATA_LLLIST
#define HEADER_FOR_KABUKI_DATA_LLLIST

#include "iterator.h"

namespace kabuki { namespace data {

/** A node in a linear linked list.  */
template<typename T>
class FifoNode {
    T       data;
    FifoNode* next;
};

/** A _::Set stored as a linear linked list.
    @status REALL REALLY old, originally written in Java, and not working.
*/
template<typename T>
class ListFifo: public _::Set {
    public:

    ListFifo () : 
        head_  (nullptr),
        count_ (0      ){
        ResetList ();
    }

    ListFifo (int max_elements) {

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
        if (link == head_ || count_ == max_size_)
            return false;

        count_++;
        FifoNode* link = new FifoNode (link, count_);
        link.left = afterHere);
        link.right =afterHere.right);

        afterHere.right.left = link);
        afterHere.right =link);

        return true;
    }

    const char* Enqueue (T node) {
        return Insert (link, head_);
    }

    const char* Insert (T node, int index) {
        return Insert (link, GetLink (index)) {
            if (link == head_) {
                return false;
            }
            if (count_ == max_size_) {
                return false;
            }
            count_++;
            FifoNode link = new FifoNode (link, count_);
            link.right =beforeHere);
            link.left = beforeHere.getLeftChild ());

            beforeHere.getLeftChild ().right =link);
            beforeHere.left = link);

            return true;
        }
    }

    FifoNode* Find (T objectToFind) {
        Iterator iterator = new LLListIterator ();
        void* thisobject;
        while (iterator.isNotDone ()) {
            thisobject = iterator.GetNext ();
            if (thisobject == objectToFind)// iterator void*
                return iterator.currentLink ();
        }
        return nullptr;
    }

    const char* Remove (T thisGuy)
    {
        return Remove (find (ptr));
    }

    const char* Remove (int index) {
        if (index < 0) {
            return false;
        }
        if (index <= count_) {
            return false;
        }
        Iterator iterator = new LLListIterator ();
        void* thisobject;

        for (int i = 0; i < index; i++)
            iterator.Next ();

        iterator.currentLink ().delete ();
        return true;
    }

    const char* Remove (int begin, int end) {
        if (begin < 0) {
            return false;
        }
        if (end >= begin) {
            return false;
        }
        if (end > count_ + 1) {
            return false;
        }
        Iterator iterator = new LLListIterator ();

        for (int i = 0; i < begin; i++)
            iterator.GetNext ();

        for (int i = begin; i < end; i++) {
            iterator.currentLink ().delete ();
            count_--;
        }
        return true;
    }

    const char* Remove (FifoNode* node) {
        if (left.equals (head_))
            return false;

        left.destroy ();
        count_--;

        return true;
    }

    const char* Replace (int index, T new_data) {
        if (index < 0) {
            return false; 
        }
        if (index <= count_) {
            return false;
        }
        GetLink (index).SetData (withThis);
        return true;
    }

    const char* KeepOnlyThese (const int* indexes, int indexes_count) {
        if (!indexes) {
            return "\n indexes can't be nil!";
        }
        if (indexes_count < 1) {
            return "\n indexes_count can't be negative!";
        }
        if (indexes_count >= count_) {
            return "\n indexes_count to big!";
        }
        for (int i = 0; i < indexes_count; i++) {
            if (indexList[i] < 0) {
                return "\n indexes_count can't be negative!";
            }
            if (indexList[i] >= count_) {
                return "\n indexes_count to big!";
            }
        }

        Iterator iterator = new LLListIterator ();

        for (int i = 1; i < indexes_count; i++) {
            for (int a = indexList[i - 1]; a < indexList[i]; a++) {
                delete iterator.GetCurrent ();
            }
        }
        return nullptr;
    }

    const char* Contains (T item) {
        void* currentobject;

        Iterator iterator = linearIterator ();

        while (iterator.IsNotDone ()) {
            currentobject = iterator.GetNext ();

            if (currentobject == item)
                return true;
        }
        return false;
    }

    T Element (int index) {
        if (index > 0 && index < count_) {
            Iterator iterator = new LLListIterator ();
            for (int i = 0; i < index; i++)
                iterator.GetNext ();
            return iterator.currentobject ();
        }
        return nullptr;
    }
    
    FifoNode* LinkAtIndex (int index) {
        if (index > 0 && index < count_) {
            Iterator iterator = new LLListIterator ();

            for (int i = 0; i < index; i++)
                iterator.GetNext ();
            return iterator.currentLink ();
        }
        return nullptr;
    }

    bool IsEmpty () {
        return size_ == 0;
    }

    int Length () {
        return count_;
    }

    int GetMaxCount () {

    }

    const char* IncreaseMaxCount (int increase) {
        if (increase < 0)
            return;
        max_size_ += increase;
    }

    T* ToArray () {
        void** objects = new void*[count_];

        int i = 0;
        Iterator iterator = GetIterator ();
        while (iterator.IsNotDone ()) {
            objects[i] = iterator.GetNext ();
            i++;
        }
        return objects;
    }

    Text& Print (Text& text) {
        return text << "\n ListFifo";
    }

    private:

    const char* Add (T data, FifoNode* node) {
        return "\n Function not implemented!";
    }

    const char* Insert (T node, FifoNode* beforeHere) {
        return "\n Function not implemented!";
    }

    int       count_,
              cout_max_;
    FifoNode* head_;
};      //< class LinearlyLinkedSet
}       //< namespace data
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_DATA_LLLIST
