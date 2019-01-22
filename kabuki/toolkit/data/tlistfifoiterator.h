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

#ifndef INCLUDED_KABUKI_TOOLKIT_DATA_FIFOLIST
#define INCLUDED_KABUKI_TOOLKIT_DATA_FIFOLIST

#include "iterator.h"

namespace _ {

/** @warning This is some of the first code I EVER wrote; please don't make fun
of me for how bad this is. */

template<typename T>
class LLListIterator {

public:
  /*
  LLListIterator () {
      currentLink = head_;
  }

  T currentobject () {
      return currentLink->data ();
  }

  T getNextobject () {
      int = index;
      index++;
      if (index > size_ ()) {
          index = 1;
          currentLink = head_;
      }
      currentLink = currentLink.getRightChild ();
      return currentLink.data ();
  }

  T GetPrevobject () {
      inde = index;
      index--;

      if (index < 1) {
          index = size_ ();
          currentLink = head_;
      }

      currentLink = currentLink.getLeftChild ();

      return currentLink.data ();
  }

  LlNode* getCurrentLink () {
      return currentLink;
  }

  LlNode* getNextLink () {
      inde = index;
      index++;
      if (index > size_ ()) {
          index = 1;
          currentLink = head_;
      }
      currentLink = currentLink.getRightChild ();
      return currentLink;
  }

  LlNode getPrevLink () {
      inde = index;
      index--;
      if (index < 1) {
          index = size_ ();
          currentLink = head_;
      }
      currentLink = currentLink.getLeftChild ();
      return currentLink;
  }

  bool isNotDone () {
      if (index > inde)// iterating forwards
      {
          if (currentLink.getRightChild () == head_)
              return false;
      } else                     // iterating backwards
      {
          if (currentLink.getLeftChild () == head_)
              return false;
      }
      return true;
  }

  bool isNotDone (int forwardsOrBackwards)// Value to be < or > 0
  {
      if (forwardsOrBackwards > 0)// iterating forwards
      {
          if (currentLink.getRightChild () == head_)
              return false;
      } else                     // iterating backwards
      {
          if (currentLink.getLeftChild () == head_)
              return false;
      }
      return true;
  }

  void removeLastobject () {
      if (index > inde)// iterating forwards
      {
          remove (currentLink.getLeftChild ());
          LLListIterator iterator = new LLListIterator ();
          iterator.setIndex (index - 1);
      } else {
          remove (currentLink.getRightChild ());
          LLListIterator iterator = new LLListIterator ();
          iterator.setIndex (index);
      }
  }

  void replaceLastobject (T item) {
      currentLink.getLeftChild ().setData (item);
  }

  void insertobject (T ptr) {
      add (ptr, currentLink);
  }

  int direction ()// returns position if last iterator went forwards
  {
      if (index > inde)// and -1 if the last iterator was backwards
          return 1;

      return -1;
  }

  void setDirection (int positiveNegative) {
      if (positiveNegative < 0)
          inde = inde + 1;
      else
          inde = inde - 1;
  }

  bool setIndex (int toHere) {
      if (toHere < 1 || toHere > size_)
          return false;
      if (toHere > index) {
          if (toHere < index - toHere) {
              for (int i = index; i < toHere; i++)
                  currentLink = currentLink.getRightChild ();
          } else {
              currentLink = linkAtIndex (toHere);
          }
      } else {
          if (toHere < toHere - index) {
              for (int i = index; i > toHere; i--)
                  currentLink = currentLink.getLeftChild ();
          } else {
              currentLink = linkAtIndex (toHere);
          }
      }
      return true;
  }

  int index () {
      return index;
  }

  int size_ () {
      return size_;
  }*/
};

}       //< namespace data
}       //< namespace kt
#endif  //< INCLUDED_KABUKI_TOOLKIT_DATA_FIFOLIST
#endif
