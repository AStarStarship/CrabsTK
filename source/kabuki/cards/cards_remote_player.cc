/** kabuki:cards
    @file    ~/source/kabuki/cards/remote_player.cc
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
#include "remote_player.h"

using namespace _;
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

RemotePlayer::RemotePlayer () :
    dislpay_name_ (new char[kMaxDislpayNameLength + 1]),
    num_wins_     (0),
    num_points_   (start_points < 1?1:start_points),
    hand_         (Deck::kNumCardsFullDeck),
    stock_        (stock) {
    if (SetDisplayName (dislplay_name))
        SetDisplayName ("Steve");
}

const char* RemotePlayer::GetDislpayName () {
    return display_name_;
}

const char* RemotePlayer::SetDislpayName (const char* name) {
    if (name == nullptr) {
        return "name can't be nil";
    }
    display_name_ = TextWrite (dislpay_name_, 
                               dislpay_name_ + User::kMaxDislpayNameLength + 1,
                               name);
    return nullptr;
}

bool RemotePlayer::IsDealer () {
    return is_dealer_;
}

void RemotePlayer::SetIsDealer (bool is_dealer) {
    is_dealer = is_dealer_;
}

int RemotePlayer::GetState () {
    return state_;
}

const char* RemotePlayer::SetState (int32_t state) {
    state_ = state;
    return nullptr;
}

Hand& RemotePlayer::GetHand () {
    return hand_;
}

int RemotePlayer::GetNumPoints () {
    return num_points_;
}

bool RemotePlayer::SetNumPoints (int num_points) {
    if (num_points < 0) {
        return false;
    }
    num_points_ = num_points;
    return true;
}

int RemotePlayer::AddPoints (int num_points) {
    if (num_points < 0)
        return -1;

    num_points_ += num_points;

    return num_points_;
}

bool RemotePlayer::RemovePoints (int num_points) {
    if (num_points < 0) {
        return false;
    }
    if (num_points > num_points_) {
        return false;
    }
    num_points_ -= num_points;

    return true;
}

int RemotePlayer::GetNumWins () {
    return num_wins_;
}

bool RemotePlayer::SetNumWins (int num_wins) {
    if (num_wins < 0) {
        return false;
    }
    num_wins_ = num_wins;
    return true;
}

CardStack& RemotePlayer::GetVisibleCards () {
    return visible_cards_;
}

void RemotePlayer::DeleteVisibleCards () {
    for (int i = 0; i < visible_cards_.GetCount (); ++i) {
        delete visible_cards_.Pop ();
    }
}

void RemotePlayer::Print () {
    cout << "\n> " << display_name_ << ": points: " << num_points_ 
         << " wins  : " << num_wins_;

    PrintLine ('-');
    hand_.Print ();
}

const Operation* RemotePlayer::Star (uint index, _::Expression* expr) {
    static const Operation This { "RemotePlayer",
        NumOperations (6), FirstOperation ('A'),
        "A remotely controlled player in an abstract card game.", 0
    };
    void* args[1];
    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA { "SetDisplayName",
                Params<0> (), Params<1, STX, User::kMaxDislpayNameLength> (),
                "Sets the display_name_.", 0
            };
            if (!expr) return &OpA;
            return ExprArgs (expr, Params<1, STX, User::kMaxDislpayNameLength> (),
                             Args (args, dislpay_name_));
        }
        case 'B': {
            static const Operation OpB { "SetIsDealer",
                Params<0> (), Params<1, STX> (),
                "Sets the is_dealer_ flag.", 0
            };
            if (!expr) return &OpB;
            byte is_dealer;
            if (!ExprArgs (expr, Params<1, BOL> (), Args (args, &is_dealer)))
                return expr->result;
            is_dealer_ = (is_dealer == 0);
            return nullptr;
        }
        case 'C': {
            static const Operation OpC { "SetState",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpC;
            return ExprArgs (expr, Params<1, UI4> (), Args (args, &state_)));
        }
        case 'D': {
            static const Operation OpD { "SetNumPoints",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpD;
            return ExprArgs (expr, Params<1, UI4> (), Args (args, &state_)));
        }
        case 'E': {
            static const Operation OpE { "SetNumWins",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpE;
            return ExprArgs (expr, Params<1, UI4> (), Args (args, &state_)));
        }
    }
    return nullptr;
}

}   //< namespace cards
}   //< namespace kabuki
