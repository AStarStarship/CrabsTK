/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/t_treebinaryiterator.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
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
#ifndef INCLUDED_KABUKI_TOOLKIT_DATA_TREEBINARYITERATOR
#define INCLUDED_KABUKI_TOOLKIT_DATA_TREEBINARYITERATOR

#include "iterator.h"

namespace _ {

/* @warning This is some of the first code I EVER wrote; please don't make fun
of me for how bad this is. */

template <typename T>
class TTreeBinaryIterator {
 public:
  BinaryTreeIterator() { current = root_; }

  T* CurrentObject() { return current->Data(); }

  T* NextObject() {
    T* next = current->Data();
    if (next) current;
  }

  T* PrevObject() {
    prev_index = index_;
    index_--;
    if (index_ < 1) {
      index_ = size_();
      current = head;
    }
    current = current->prev;
    return current->data;
  }

  Link Current() { return current; }

  Link Next() {
    prev_index = index_;
    index_++;
    if (index_ > size_()) {
      index_ = 1;
      current = head;
    }
    current = current->next;
    return current;
  }

  Link* Prev() {
    prev_index = index_;
    index_--;
    if (index_ < 1) {
      index_ = size_();
      current = head;
    }
    current = current->prev;
    return current;
  }

  BOL IsNotDone() {
    if (index_ > prev_index) { // iterating forwards
      if (current->next == head) return false;
    } else { // iterating backwards
      if (current->prev == head) return false;
    }
    return true;
  }
  BOL IsNotDone(SI4 forwardsOrBackwards)  // Value to be < or > 0
  {
    if (forwardsOrBackwards > 0) { // iterating forwards
      if (current->next == head) return false;
    } else {  // iterating backwards
      if (current->prev == head) return false;
    }
    return true;
  }
  void removeLastObject() {
    if (index_ > prev_index) { // iterating forwards
      remove(current->prev);

      BinaryTreeIterator iteration = new BinaryTreeIterator();
      iteration.SetIndex(index_ - 1);
    } else {
      remove(current->next);
      Iterator iteration = new Iterator();
      iteration.SetIndex(index_);
    }
  }

  void ReplaceLastObject(T* withThis) { current->prev.data = withThis; }

  void InsertObject(T* ptr) { add(ptr, current); }

  SI4 Direction() { // returns Point if last iteration went forwards
    if (index_ > prev_index)  // and -1 if the last iteration was backwards
      return 1;

    return -1;
  }

  void SetDirection(SI4 value) {
    if (value < 0)
      prev_index = prev_index + 1;
    else
      prev_index = prev_index - 1;
  }

  BOL SetIndex(SI4 toHere) {
    if (toHere < 1 || toHere > size_) return false;
    if (toHere > index_) {
      if (toHere < index_ - toHere) {
        for (SI4 i = index_; i < toHere; i++) current = current->next;
      } else {
        current = linkAtIndex(toHere);
      }
    } else {
      if (toHere < toHere - index_) {
        for (SI4 i = index_; i > toHere; i--) current = current->prev;
      } else {
        current = linkAtIndex(toHere);
      }
    }
    return true;
  }

  SI4 Index() { return index_; }

  SI4 Size() { return size_; };
}  // namespace _
}  // namespace _
#endif  //< INCLUDED_KABUKI_TOOLKIT_DATA_TREEBINARYITERATOR
#endif
