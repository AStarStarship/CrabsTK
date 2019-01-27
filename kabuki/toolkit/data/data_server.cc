/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/data_server.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_DATA_1
#if SEAM == KABUKI_TOOLKIT_DATA_1
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

#include "cserver.h"

namespace _ {

Server::Server ():
  Room ("kabuki.security.Server") {
}

const Op* Server::Star (wchar_t index, Expr* expr) {
  static const Op kThis = { "kabuki.toolkit.data.Server",
    OpFirst ('A'), OpLast ('A'),
    "Datagram security server.", 0
  };
  //void* args[1];
  switch (index) {
    case '?': return ExprQuery (expr, &kThis);
    case 'A': {
      static const Op kOpA = { "foo",
        Params<0> (), Params<0> (),
        "Why does everyone use foo?", 0
      };
      if (!expr) return &kOpA;
    }
  }
  return nullptr;
}

} //< namespace _
#endif
