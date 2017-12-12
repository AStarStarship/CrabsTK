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

RemotePlayer::RemotePlayer (Deck& pack) :
    status_        (new char[User::kDefaultMaxDislpayNameLength + 1]),
    num_wins_      (0),
    num_points_    (0),
    pack_          (pack),
    visible_cards_ (Deck::kFullDeckSize) {
    SetHandle      ("Steve");
    SetDislpayName ("Steve");
}

RemotePlayer::~RemotePlayer () {
    delete status_;
}

const char* RemotePlayer::GetDislpayName () {
    return status_;
}

const char* RemotePlayer::SetDislpayName (const char* name) {
    if (name == nullptr) {
        return "name can't be nil";
    }
    TextWrite (status_, status_ + User::kDefaultMaxDislpayNameLength + 1, name);
    return nullptr;
}

const char* RemotePlayer::GetHandle () {
    return handle_;
}

const char* RemotePlayer::SetHandle (const char* handle) {
    if (name == nullptr) {
        return "handle can't be nil";
    }
    TextWrite (handle_, status_ + User::kDefaultMaxDislpayNameLength + 1,
               handle);
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
        delete visible_cards_.Draw ();
    }
}

int RemotePlayer::AddCard (byte pip, byte suit) {
    if (pip < 0) {
        return -1;
    }
    if (pip > 13) {
        return -1;
    }
    return visible_cards_.Push (new Card (pip, pack_.Suits ()[suit], 0));
}

void RemotePlayer::Print () {
    cout << "\n| " << status_ << ": points: " << num_points_ 
         << " wins  : " << num_wins_;

    PrintLine ('-');
    visible_cards_.Print ();
}

const Operation* RemotePlayer::Star (uint index, _::Expression* expr) {
    static const Operation This { "RemotePlayer",
        NumOperations (6), OperationFirst ('A'),
        "A remotely controlled player in an abstract card game.", 0
    };
    void* args[1];
    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA { "SetStatus",
                Params<0> (), Params<1, STR, User::kDefaultMinDislpayNameLength> (),
                "Sets the status_.", 0
            };
            if (!expr) return &OpA;
            return ExprArgs (expr, Params<1, STR, User::kDefaultMaxDislpayNameLength> (),
                             Args (args, status_));
        }
        case 'B': {
            static const Operation OpB { "SetIsDealer",
                Params<0> (), Params<1, STR> (),
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
            return ExprArgs (expr, Params<1, UI4> (), Args (args, &state_));
        }
        case 'D': {
            static const Operation OpD { "SetNumPoints",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpD;
            return ExprArgs (expr, Params<1, UI4> (), Args (args, &num_points_));
        }
        case 'E': {
            static const Operation OpE { "SetNumWins",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpE;
            return ExprArgs (expr, Params<1, UI4> (), Args (args, &num_wins_));
        }
        case 'F': {
            static const Operation OpF { "DeleteCards",
                Params<0> (), Params<0> (),
                "Deletes all the cards in this hand.", 0
            };
            if (!expr) return &OpF;
            DeleteVisibleCards ();
            return nullptr;
        }
        case 'G': {
            static const Operation OpG { "AddCard",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpG;
            byte pip,
                 suit;
            if (ExprArgs (expr, Params<2, UI1, UI1> (), Args (args, &pip, &suit)))
                return expr->result;
            if (pip < 0) {
                return Result (expr, Bin::kErrorInvalidArgs,
                               Params<2, UI1, UI1> (), 1);
            }
            if (pip > 13) {
                return Result (expr, Bin::kErrorInvalidArgs,
                               Params<2, UI1, UI1> (), 1);
            }
            if (suit < 0) {
                return Result (expr, Bin::kErrorInvalidArgs,
                               Params<2, UI1, UI1> (), 1);
            }
            if (suit > 3) {
                return Result (expr, Bin::kErrorInvalidArgs,
                               Params<2, UI1, UI1> (), 1);
            }
            AddCard (pip, suit);
        }
    }
    return nullptr;
}

}   //< namespace cards
}   //< namespace kabuki
