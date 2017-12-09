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

#include "card_game.h"

using namespace std;
using namespace _;
using namespace kabuki::id;

namespace kabuki {
namespace cards {

CardGame::CardGame (UserList& users, const char* game_name, int min_players,
                    int max_players) :
    state_     (0),
    game_name_ (game_name),
    users_     (users),
    observers_ (max_players) {
}

CardGame::~CardGame () {
    // We don't delete the observers_.
}

const char* CardGame::GetGameName () {
    return game_name_;
}

uint CardGame::GetState () {
    return state_;
}

bool CardGame::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

int CardGame::GetNumPlayers () {
    return observers_.GetCount ();
}

int32_t CardGame::GetMinPlayers () {
    return min_players_;
}

int32_t CardGame::GetMaxPlayers () {
    return observers_.GetSize ();
}

void CardGame::PrintObservers () {
    for (int i = 0; i < observers_.GetCount (); ++i) {
        observers_[i]->Print ();
    }
}

void CardGame::Print () {
}

int CardGame::AddObserver (User* user) {
    if (user == nullptr) {
        return -1;
    }
    return observers_.Push (user);
}

int CardGame::RemoveObserver (User* user) {
    for (int i = observers_.GetCount (); i > 0; --i) {
        User* user = observers_[i];
        // user will never be nil.
        if (user->Equals (user)) {
            return observers_.Remove (i);
        }
    }
    return -1;
}

Array<id::User*>& CardGame::GetObservers () {
    return observers_;
}

const Operation* CardGame::Star (uint index, _::Expression* expr) {
    static const Operation This = { "CardsClient",
        NumOperations (0), FirstOperation ('A'),
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
            if (!ExprArgs (expr, Params<1, UI1> (), Args (args, &state)))
                return expr->result;
            SetState (state);
            return nullptr;
        }
        case 'B': {
            static const Operation OpB = { "Print",
                Params<1, STX, kMaxMessageLength + 1> (), Params<0> (),
                "Sets the client state.", 0
            };
            if (!expr) return &OpB;
            char buffer[kMaxMessageLength + 1];
            if (!ExprArgs (expr, Params<1, STX, kMaxMessageLength + 1> (),
                           Args (args, buffer)))
                return expr->result;
            cout << buffer;
            return nullptr;
        }
        case 'C': {
            static const uint_t* kRxHeaderC = Params<1, SI4,
                STX, User::kDefaultMaxDislpayNameLength + 1,
                STX, Handle::kDefaultMaxLength + 1> ();
            static const Operation OpC = { "SetPlayer",
                kRxHeaderC, Params<0> (),
                "Sets the player at the given #index to the given "
                "#dislpay_name.", 0
            };
            if (!expr) return &OpC;
            int32_t player_number;
            char status[User::kDefaultMaxDislpayNameLength + 1],
                 handle[Handle::kDefaultMaxLength];
            if (!ExprArgs (expr, kRxHeaderC, Args (args, &player_number,
                                                   status, handle)))
                return expr->result;
            if (player_number < 0) {
                return Result (expr, Bin::InvalidArgumentError,
                               Params<1, SI4> ());
            }
            if (player_number > kMaxPlayers) {
                return Result (expr, Bin::InvalidArgumentError,
                               Params<1, SI4> ());
            }
            observers_.Grow (player_number);
            user = observers_[player_number];
            user->SetStatus (status);
            user->GetHandle ().SetKey (handle);
            return nullptr;
        }
    }
    return nullptr;
}

const char* DefaultPlayAgainString () {
    static const char play_again[] = "\n| Do you want to play again?"
        "\n| Enter y to continue, or n to quit."
        "\n< \0";
    return play_again;
}

}       //< namespace cards
}       //< namespace kabuki
