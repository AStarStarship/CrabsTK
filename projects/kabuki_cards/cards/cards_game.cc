/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/arcade_card_game.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include "game.h"

using namespace std;
using namespace _;
using namespace kabuki::id;

namespace kabuki {
namespace cards {

Game::Game (UserList& users, const char* game_name, int min_players,
                    int max_players) :
    state_     (0),
    name_ (game_name),
    users_     (users),
    observers_ (max_players) {
}

Game::~Game () {
    // We don't delete the observers_.
}

const char* Game::GetName () {
    return name_;
}

int32_t Game::GetState () {
    return state_;
}

bool Game::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

int Game::GetNumPlayers () {
    return observers_.size ();
}

int Game::GetMinPlayers () {
    return min_players_;
}

int Game::GetMaxPlayers () {
    return observers_.capacity ();
}

void Game::PrintObservers () {
    for (size_t i = 0; i < observers_.size (); ++i) {
        cout << observers_[i]->Print ();
    }
}

int Game::Join (User* user) {
    if (user == nullptr) {
        return -1;
    }
    observers_.push_back (user);
    return observers_.size ();
}

int Game::Leave (User* user) {
    for (int i = observers_.size (); i > 0; --i) {
        User* user = observers_[i];
        // user will never be nil.
        if (user->Equals (user)) {
            auto it = observers_.begin ();
            observers_.erase (it + i);
            return observers_.size ();
        }
    }
    return -1;
}

std::vector<id::User*>& Game::GetObservers () {
    return observers_;
}

Text& Game::Print (Text& txt) {
    txt << "\n| " << GetName ();
}

const Operation* Game::Star (uint index, Expression* expr) {
    static const Operation This = { "Client",
        OperationCount (0), OperationFirst ('A'),
        "kabuki::cards Script client.", 0
    };
    void* args[4];
    User* user;
    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "SetState",
                Params<1, UI1> (), Params<0> (),
                "Sets the client state.", 0
            };
            if (!expr) return &OpA;
            byte state;
            if (!ExpressionArgs (expr, Params<1, UI1> (), Args (args, &state)))
            {
                return expr->result;
            }
            SetState (state);
            return nullptr;
        }
        case 'B': {
            static const Operation OpB = { "Print",
                Params<1, STR, kMaxMessageLength + 1> (), Params<0> (),
                "Sets the client state.", 0
            };
            if (!expr) return &OpB;
            char buffer[kMaxMessageLength + 1];
            if (!ExpressionArgs (expr, Params<1, STR, kMaxMessageLength + 1> (),
                                 Args (args, buffer))) {
                return expr->result;
            }
            cout << buffer;
            return nullptr;
        }
        case 'C': {
            static const uint_t* kRxHeaderC = Params<1, SI4,
                STR, User::kMaxStatusLength + 1,
                STR, Handle::kMaxLength + 1> ();
            static const Operation OpC = { "SetPlayer",
                kRxHeaderC, Params<0> (),
                "Sets the player at the given #index to the given "
                "#dislpay_name.", 0
            };
            if (!expr) return &OpC;
            int32_t player_number;
            char status[User::kMaxStatusLength + 1],
                 handle[Handle::kMaxLength];
            if (!ExpressionArgs (expr, kRxHeaderC, Args (args, &player_number,
                                                   status, handle))) {
                return expr->result;
            }
            if (player_number < 0) {
                return Result (expr, Bin::kErrorInvalidArgument,
                               Params<1, SI4> ());
            }
            if (player_number > kMaxPlayers) {
                return Result (expr, Bin::kErrorInvalidArgument,
                               Params<1, SI4> ());
            }
            observers_.reserve (player_number);
            user = observers_[player_number];
            user->SetStatus (status);
            user->GetHandle ().SetKey (handle);
            return nullptr;
        }
    }
    return nullptr;
}

const char* DefaultPlayAgainText () {
    static const char play_again[] = "\n| Do you want to play again?"
        "\n| Enter y to continue, or n to quit."
        "\n< \0";
    return play_again;
}

}       //< namespace cards
}       //< namespace kabuki
