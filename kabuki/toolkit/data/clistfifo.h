/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/clistfifo.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_DATA_1
#if SEAM == KABUKI_TOOLKIT_DATA_1
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

#ifndef INCLUDED_KABUKI_TOOLKIT_DATA_LISTFIFO
#define INCLUDED_KABUKI_TOOLKIT_DATA_LISTFIFO 1

#include "citerator.h"

namespace _ {

/** A node in a linear linked list.  */
template<typename T>
class NodeFifo {
  T       data;
  NodeFifo* next;
};

/** A _::Set stored as a linear linked list.
@status REALL REALLY old, originally written in Java, and not working.
*/
template<typename T>
class ListFifo : public Set {
public:

  ListFifo () :
    head_ (nullptr),
    count_ (0) {
    ResetList ();
  }

  ListFifo (SI4 max_elements) {

    if (max_elements > 0) {
      max_size_ = max_elements;
    }
  }

  void Clear () {
    head_.left = head_;
    head_.right = head_;
    count_ = 0;
  }

  BOL Add (T node) {
    if (link == head_ || count_ == max_size_)
      return false;

    count_++;
    NodeFifo* link = new NodeFifo (link, count_);
    link.left = afterHere);
    link.right = afterHere.right);

    afterHere.right.left = link);
    afterHere.right = link);

    return true;
  }

  const CH1* Enqueue (T node) {
    return Insert (link, head_);
  }

  const CH1* Insert (T node, SI4 index) {
    return Insert (link, GetLink (index)) {
      if (link == head_) {
        return false;
      }
      if (count_ == max_size_) {
        return false;
      }
      count_++;
      NodeFifo link = new NodeFifo (link, count_);
      link.right = beforeHere);
      link.left = beforeHere.getLeftChild ());

      beforeHere.getLeftChild ().right = link);
      beforeHere.left = link);

      return true;
    }
  }

  NodeFifo* Find (T objectToFind) {
    Iterator iterator = new LLListIterator ();
    void* thisobject;
    while (iterator.isNotDone ()) {
      thisobject = iterator.GetNext ();
      if (thisobject == objectToFind)// iterator void*
        return iterator.currentLink ();
    }
    return nullptr;
  }

  const CH1* Remove (T thisGuy)
  {
    return Remove (find (ptr));
  }

  const CH1* Remove (SI4 index) {
    if (index < 0) {
      return false;
    }
    if (index <= count_) {
      return false;
    }
    Iterator iterator = new LLListIterator ();
    void* thisobject;

    for (SI4 i = 0; i < index; i++)
      iterator.Next ();

    iterator.currentLink ().delete ();
    return true;
  }

  const CH1* Remove (SI4 begin, SI4 end) {
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

    for (SI4 i = 0; i < begin; i++)
      iterator.GetNext ();

    for (SI4 i = begin; i < end; i++) {
      iterator.currentLink ().delete ();
      count_--;
    }
    return true;
  }

  const CH1* Remove (NodeFifo* node) {
    if (left.equals (head_))
      return false;

    left.destroy ();
    count_--;

    return true;
  }

  const CH1* Replace (SI4 index, T new_data) {
    if (index < 0) {
      return false;
    }
    if (index <= count_) {
      return false;
    }
    GetLink (index).SetData (withThis);
    return true;
  }

  const CH1* KeepOnlyThese (const SI4* indexes, SI4 indexes_count) {
    if (!indexes) {
      return "\n indexes can't be nil!";
    }
    if (indexes_count < 1) {
      return "\n indexes_count can't be negative!";
    }
    if (indexes_count >= count_) {
      return "\n indexes_count to big!";
    }
    for (SI4 i = 0; i < indexes_count; i++) {
      if (indexList[i] < 0) {
        return "\n indexes_count can't be negative!";
      }
      if (indexList[i] >= count_) {
        return "\n indexes_count to big!";
      }
    }

    Iterator iterator = new LLListIterator ();

    for (SI4 i = 1; i < indexes_count; i++) {
      for (SI4 a = indexList[i - 1]; a < indexList[i]; a++) {
        delete iterator.GetCurrent ();
      }
    }
    return nullptr;
  }

  const CH1* Contains (T item) {
    void* currentobject;

    Iterator iterator = linearIterator ();

    while (iterator.IsNotDone ()) {
      currentobject = iterator.GetNext ();

      if (currentobject == item)
        return true;
    }
    return false;
  }

  T Element (SI4 index) {
    if (index > 0 && index < count_) {
      Iterator iterator = new LLListIterator ();
      for (SI4 i = 0; i < index; i++)
        iterator.GetNext ();
      return iterator.currentobject ();
    }
    return nullptr;
  }

  NodeFifo* LinkAtIndex (SI4 index) {
    if (index > 0 && index < count_) {
      Iterator iterator = new LLListIterator ();

      for (SI4 i = 0; i < index; i++)
        iterator.GetNext ();
      return iterator.currentLink ();
    }
    return nullptr;
  }

  BOL IsEmpty () {
    return size_ == 0;
  }

  SI4 Length () {
    return count_;
  }

  SI4 GetMaxCount () {

  }

  const CH1* IncreaseMaxCount (SI4 increase) {
    if (increase < 0)
      return;
    max_size_ += increase;
  }

  T* ToArray () {
    void** objects = new void*[count_];

    SI4 i = 0;
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

  const CH1* Add (T data, NodeFifo* node) {
    return "\n Function not implemented!";
  }

  const CH1* Insert (T node, NodeFifo* beforeHere) {
    return "\n Function not implemented!";
  }

  SI4       count_,
    cout_max_;
  NodeFifo* head_;
};


/** A node in a linear linked list.  */
template<typename T>
class NodeFifo {
  T       data;
  NodeFifo* next;
};

/** A _::Set stored as a linear linked list.
    @status REALL REALLY old, originally written in Java, and not working.
*/
template<typename T>
class ListFifo : public Set {
public:

  ListFifo () :
    head_ (nullptr),
    count_ (0) {
    ResetList ();
  }

  ListFifo (SI4 max_elements) {

    if (max_elements > 0) {
      max_size_ = max_elements;
    }
  }

  void Clear () {
    head_.left = head_;
    head_.right = head_;
    count_ = 0;
  }

  BOL Add (T node) {
    if (link == head_ || count_ == max_size_)
      return false;

    count_++;
    NodeFifo* link = new NodeFifo (link, count_);
    link.left = afterHere);
    link.right = afterHere.right);

    afterHere.right.left = link);
    afterHere.right = link);

    return true;
  }

  const CH1* Enqueue (T node) {
    return Insert (link, head_);
  }

  const CH1* Insert (T node, SI4 index) {
    return Insert (link, GetLink (index)) {
      if (link == head_) {
        return false;
      }
      if (count_ == max_size_) {
        return false;
      }
      count_++;
      NodeFifo link = new NodeFifo (link, count_);
      link.right = beforeHere);
      link.left = beforeHere.getLeftChild ());

      beforeHere.getLeftChild ().right = link);
      beforeHere.left = link);

      return true;
    }
  }

  NodeFifo* Find (T objectToFind) {
    Iterator iterator = new LLListIterator ();
    void* thisobject;
    while (iterator.isNotDone ()) {
      thisobject = iterator.GetNext ();
      if (thisobject == objectToFind)// iterator void*
        return iterator.currentLink ();
    }
    return nullptr;
  }

  const CH1* Remove (T thisGuy)
  {
    return Remove (find (ptr));
  }

  const CH1* Remove (SI4 index) {
    if (index < 0) {
      return false;
    }
    if (index <= count_) {
      return false;
    }
    Iterator iterator = new LLListIterator ();
    void* thisobject;

    for (SI4 i = 0; i < index; i++)
      iterator.Next ();

    iterator.currentLink ().delete ();
    return true;
  }

  const CH1* Remove (SI4 begin, SI4 end) {
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

    for (SI4 i = 0; i < begin; i++)
      iterator.GetNext ();

    for (SI4 i = begin; i < end; i++) {
      iterator.currentLink ().delete ();
      count_--;
    }
    return true;
  }

  const CH1* Remove (NodeFifo* node) {
    if (left.equals (head_))
      return false;

    left.destroy ();
    count_--;

    return true;
  }

  const CH1* Replace (SI4 index, T new_data) {
    if (index < 0) {
      return false;
    }
    if (index <= count_) {
      return false;
    }
    GetLink (index).SetData (withThis);
    return true;
  }

  const CH1* KeepOnlyThese (const SI4* indexes, SI4 indexes_count) {
    if (!indexes) {
      return "\n indexes can't be nil!";
    }
    if (indexes_count < 1) {
      return "\n indexes_count can't be negative!";
    }
    if (indexes_count >= count_) {
      return "\n indexes_count to big!";
    }
    for (SI4 i = 0; i < indexes_count; i++) {
      if (indexList[i] < 0) {
        return "\n indexes_count can't be negative!";
      }
      if (indexList[i] >= count_) {
        return "\n indexes_count to big!";
      }
    }

    Iterator iterator = new LLListIterator ();

    for (SI4 i = 1; i < indexes_count; i++) {
      for (SI4 a = indexList[i - 1]; a < indexList[i]; a++) {
        delete iterator.GetCurrent ();
      }
    }
    return nullptr;
  }

  const CH1* Contains (T item) {
    void* currentobject;

    Iterator iterator = linearIterator ();

    while (iterator.IsNotDone ()) {
      currentobject = iterator.GetNext ();

      if (currentobject == item)
        return true;
    }
    return false;
  }

  T Element (SI4 index) {
    if (index > 0 && index < count_) {
      Iterator iterator = new LLListIterator ();
      for (SI4 i = 0; i < index; i++)
        iterator.GetNext ();
      return iterator.currentobject ();
    }
    return nullptr;
  }

  NodeFifo* LinkAtIndex (SI4 index) {
    if (index > 0 && index < count_) {
      Iterator iterator = new LLListIterator ();

      for (SI4 i = 0; i < index; i++)
        iterator.GetNext ();
      return iterator.currentLink ();
    }
    return nullptr;
  }

  BOL IsEmpty () {
    return size_ == 0;
  }

  SI4 Length () {
    return count_;
  }

  SI4 GetMaxCount () {

  }

  const CH1* IncreaseMaxCount (SI4 increase) {
    if (increase < 0)
      return;
    max_size_ += increase;
  }

  T* ToArray () {
    void** objects = new void*[count_];

    SI4 i = 0;
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

  const CH1* Add (T data, NodeFifo* node) {
    return "\n Function not implemented!";
  }

  const CH1* Insert (T node, NodeFifo* beforeHere) {
    return "\n Function not implemented!";
  }

  SI4       count_,
    cout_max_;
  NodeFifo* head_;
};
}       //< namespace _
#endif
#endif
