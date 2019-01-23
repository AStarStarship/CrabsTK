/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/cclient.h
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
#ifndef INCLUDED_KABAUKI_DATA_CLIENT
#define INCLUDED_KABAUKI_DATA_CLIENT

namespace _ {

/** Security guard handle datagram encryption.
 */
class Client : public _::Room {
 public:
  typedef enum States {
    kStateDisconnected = 0,
    kStateAwaitingConnection = 1,
    kStateConnected = 2
  } State;

  /** Default constructor. */
  Client();

  /** Virtual destructor. */
  virtual ~Client();

  /** Gets the FSM state. */
  SI4 GetState();

  /** Virtual function sets the FSM state to a positive value. */
  virtual bool SetState(SI4 state);

  /** Prints this object to the console. */
  virtual _::TUTF<Char>& Print(_::TUTF<Char>& printer);

  /** Abstract Script Op(s).
      @param index The index of the expression.
      @param expr  The Expr to read and write from.
      @return      Returns nil upon success, a Set header upon query, and an
                   error_t ticket upon Read-Write failure. */
  virtual const _::Op* Star(wchar_t index, _::Expr* expr) = 0;

 private:
  SI4 state_;  // Client state.

};
}       //< namespace _
#endif  //< INCLUDED_KABAUKI_DATA_CLIENT
#endif  //< #if SEAM >= KABUKI_TOOLKIT_DATA_1
