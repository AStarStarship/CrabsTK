/** kabuki::cards
    @file    ~/source/kabuki/cards/cards_card_game.cc
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

#include "server.h"

using namespace _;
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

Server::Server (uint port_number, const char* server_name) :
                Room       (server_name),
                authenticator_ (new AuthenticatorDefault ()),
                state_   (0),
                users_   (),
                players_ () {
}

Server::~Server () {
    for (int i = players_.GetCount (); i > 0; --i) {
        Player* player = players_.Pop ();
        delete player;
    }
    delete authenticator_;
}

int Server::GetState () {
    return state_;
}

bool Server::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

const char* Server::GetServerName () {
    return name_;
}

const char* Server::SetServerName (const char* name) {
    if (name == nullptr) {
        return "server_name can't be nil";
    }
    delete server_name_;
    server_name_ = StringClone (name);
    return nullptr;
}

void Server::Print () {
    cout << "\n| Server: " << server_name_;
}

}       //< namespace cards
}       //< namespace kabuki
