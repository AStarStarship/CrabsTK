/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/t_dataclient.h
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
#ifndef KABAUKI_DATA_CLIENT
#define KABAUKI_DATA_CLIENT 1

namespace _ {

/* Data Client.
 */
class TDataClient : public _::Room {
 public:
  typedef enum States {
    kStateDisconnected = 0,
    kStateAwaitingConnection = 1,
    kStateConnected = 2
  } State;

  /* Default constructor. */
  TDataClient () {}

  /* Virtual destructor. */
  virtual ~TDataClient() {}

  /* Prints this object to the console. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    return o << "\nTDataClient";
  }

  /* Abstract Script Op(s).
  @param index The index of the expression.
  @param expr  The Expr to read and write from.
  @return      Returns nil upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure. */
  virtual const _::Op* Star (wchar_t index, _::Expr* expr) {
    return nullptr;
  }

 private:
  SI4 state_;  // TDataClient state.

};
}       //< namespace _
#endif
#endif
