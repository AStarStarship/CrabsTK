/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/ai/t_perceptron.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AI_1
#ifndef KABUKI_TOOLKIT_AI_PERCEPTRON_T
#define KABUKI_TOOLKIT_AI_PERCEPTRON_T 1

#include "t_synapse.h"

namespace _ {

/* A TPerceptron.
A TPerceptron needs to get it's input from either floating-point or
integers.

@code
x_1 ---\        ______     ______
    w_1 \       | ____ |     |   __ |
x_2 -----\      | \     |     |   |   |
    w_2   |-----|  \    |---->|   |   |----> Y Output
...      /      |  /    |  h  |   |   |
x_n ----/       | /___ |     | __|0  |
    w_n         |_____|     |_____|
@endcode
*/
template<typename Value>
class TPerceptron {
public:

  /* Constructs a unconnected TPerceptron. */
  TPerceptron () :
    x_ (nullptr) {}

  /* Gets the perceptron y. */
  Value& Y () { return y_; }

  /* Sets the perceptron value. */
  void SetY (Value& y) { y_ = y; }

  /* Gets the perceptron x TArray. */
  Value* X () { return y_; }

  /* Gets the perceptron weight TArray. */
  Value* Weights () { return &w_[0]; }

  /* Gets the count of the axions. */
  size_t AxionCount () { w_.Count (); }

  /* Updates the TPerceptron. */
  void Update () {
    size_t n = w_.size ();
    DASSERT (n);

    Value sum = 0.0;
    Value* x = x_,
      *w = &w_[0];
    for (--n; n > 0; --n)
      sum += *x++ * *w++;
    y_ = sum;
  }

private:

  Value& y_;        //< Y output address TArray.
  Value* x_;        //< X input values TArray.
  TArray<Value> w_; //< Weight input values TArray.
};

} //< namespace _
#endif
#endif
