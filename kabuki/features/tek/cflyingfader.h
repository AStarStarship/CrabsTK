/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cflyingfader.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_FLYING_FADER
#define INCLUDED_KABUKI_FEATURE_TEK_FLYING_FADER 1

namespace _ {

/* A bank of one or more groups of flying faders.
    Flying faders on most mixers have pages full of controls, and work similar
    to the RotaryEncoder class.
*/
class FlyingFader {
 public:
  /* Constructs a blank flying fader. */
  FlyingFader();

  /* Prints this object to a console. */
  void Print();

  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
};

class FlyingFaderOp {
 public:
  FlyingFaderOp(FlyingFader* ff);

  virtual const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  FlyingFader* ff_;  //< Pointer to the selected FlyingFader object.
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_FLYING_FADER
