/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/t_iterator.h
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
#if SEAM == KABUKI_TOOLKIT_DATA_1
#ifndef INCLUDED_KABUKI_TOOLKIT_DATA_ITERATOR
#define INCLUDED_KABUKI_TOOLKIT_DATA_ITERATOR

#include "config.h"

namespace _ {

/* An interface for an object iterator. */
template<typename T>
struct Iterator {
  /* Iterates to the next item in the sequence. */
  virtual T* Next () = 0;

  /* Resets the iterator to the beginning. */
  virtual void Reset () = 0;

  /* Indicates if the iteration is complete.
  @return Returns 0 when the iteration is complete. */
  virtual SI4 IsDone () = 0;
};

/* Interface for an Iterable Collection of objects. */
template<typename T>
struct Iterable {
  /* Gets an instance of an Iterator. */
  virtual Iterator<T>* GetIterator () = 0;
};      //< Iterable

}       //< namespace _
#endif
#endif
