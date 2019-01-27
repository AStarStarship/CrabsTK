/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/tserver.h
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
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

#ifndef INCLUDED_KABUKI_TOOLKIT_DATA_SERVER
#define INCLUDED_KABUKI_TOOLKIT_DATA_SERVER

namespace _ {

class Server: public _::Room {
  public:

  typedef enum States {
    kStateShuttingDown = 0, //< Server State 0: Shutting down.
    kStateBooting      = 1, //< Server state 1: Booting.
    kStateServing      = 2, //< Server state 2: Serving.
  } State;

  /* Constructs the default server. */
  Server ();

  /* Abstract Script Op(s).
  @param index The index of the expression.
  @param expr  The Expr to read and write from.
  @return      Returns nil upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure. */
  virtual const _::Op* Star (wchar_t index, _::Expr* expr);

  private:

};
} //< namespace _
#endif
#endif
