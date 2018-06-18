/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/store/store_client.cc
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

#ifndef HEADER_FOR_KABAUKI_STORE_CLIENT
#define HEADER_FOR_KABAUKI_STORE_CLIENT

#include "inventory.h"

namespace _ {

/** Store client with shopping cart.
 */
class Client : public Room {
 public:
  typedef enum States {
    kStateDisconnected = 0,
    kStateAwaitingConnection = 1,
    kStateConnected = 2
  } State;

  enum {
    kMaxPlayers = 64 * 1024,    //< Man number of players.
    kDefaultConsoleWidth = 80,  //< Num rows in a DOS console.
    kMaxMessageLength = 140,    //< Max message length.
    kMaxGameNameLength = 63,    //< Max game name length.
  };

  /** Default constructor. */
  Client();

  /** Virtual destructor. */
  virtual ~Client();

  /** Prints this object to the console. */
  virtual Printer& Print(Printer& printer = Printer());

  /** Abstract Script Operation(s).
      @param index The index of the expression.
      @param expr  The Expression to read and write from.
      @return      Returns null upon success, a Set header upon query, and an
                   error_t ticket upon Read-Write failure. */
  virtual const Op* Star(wchar_t index, Expr* expr);

 protected:
  Authenticator* authenticator_;
  char game_name_[kMaxGameNameLength];  //< Current game name.
  uint32_t xstate_,                     //< Client app state.
      round_number_;                    //< Current round number.
  User user_;                           //< User.
  Cart cart_;                           //< Client shopping cart.

};  //< class Client

}  // namespace _
#endif  //< HEADER_FOR_KABAUKI_STORE_CLIENT
