/** Kabuki Toolkit
    @file    ~/source/data/data_client.cc
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

#pragma once
#include <pch.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KABAUKI_DATA_CLIENT
#define HEADER_FOR_KABAUKI_DATA_CLIENT

#include "config.h"

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
  int GetState();

  /** Virtual function sets the FSM state to a positive value. */
  virtual bool SetState(int state);

  /** Prints this object to the console. */
  virtual _::Printer& Print(_::Printer& printer);

  /** Abstract Script Op(s).
      @param index The index of the expression.
      @param expr  The Expr to read and write from.
      @return      Returns nil upon success, a Set header upon query, and an
                   error_t ticket upon Read-Write failure. */
  virtual const _::Op* Star(wchar_t index, _::Expr* expr) = 0;

 private:
  int state_;  // Client state.

};  //< class Client
}  // namespace _ {
#endif  //< HEADER_FOR_KABAUKI_DATA_CLIENT
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
