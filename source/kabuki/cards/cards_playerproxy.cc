/** Kabuki Toolkit
    @file    ~/source/kabuki/cards/cards_playerproxy.cc
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
#include "player_proxy.h"

using namespace _;
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

PlayerProxy::PlayerProxy (Deck& pack) :
    status_        (new char[User::kMaxStatusLength + 1]),
    num_wins_      (0),
    num_points_    (0),
    pack_          (pack),
    visible_cards_ (Deck::kFullDeckSize) {
    SetHandle      ("Steve");
    SetDislpayName ("Steve");
}

PlayerProxy::~PlayerProxy () {
    delete status_;
}

const char* PlayerProxy::GetDislpayName () {
    return status_;
}

const char* PlayerProxy::SetDislpayName (const char* name) {
    if (name == nullptr) {
        return "name can't be nil";
    }
    StrandWrite (status_, status_ + User::kMaxStatusLength + 1, name);
    return nullptr;
}

const char* PlayerProxy::GetHandle () {
    return handle_;
}

const char* PlayerProxy::SetHandle (const char* handle) {
    if (!handle) {
        return "\n| Error: handle can't be nil!";
    }
    delete handle_;
    handle = StrandClone (handle);
    return nullptr;
}

bool PlayerProxy::IsDealer () {
    return is_dealer_;
}

void PlayerProxy::SetIsDealer (bool is_dealer) {
    is_dealer = is_dealer_;
}

int PlayerProxy::GetState () {
    return state_;
}

const char* PlayerProxy::SetState (int32_t state) {
    state_ = state;
    return nullptr;
}

int PlayerProxy::GetNumPoints () {
    return num_points_;
}

bool PlayerProxy::SetNumPoints (int num_points) {
    if (num_points < 0) {
        return false;
    }
    num_points_ = num_points;
    return true;
}

int PlayerProxy::GetNumWins () {
    return num_wins_;
}

bool PlayerProxy::SetNumWins (int num_wins) {
    if (num_wins < 0) {
        return false;
    }
    num_wins_ = num_wins;
    return true;
}

CardStack& PlayerProxy::GetVisibleCards () {
    return visible_cards_;
}

void PlayerProxy::DeleteVisibleCards () {
    for (int i = 0; i < visible_cards_.Length (); ++i) {
        delete visible_cards_.Draw ();
    }
}

int PlayerProxy::AddCard (byte pip, byte suit) {
    if (pip < 0) {
        return -1;
    }
    if (pip > 13) {
        return -1;
    }
    return visible_cards_.Push (new Card (pip, pack_.Suits ()[suit], 0));
}

Text& PlayerProxy::Print (Text& txt) {
    return txt << "\n| " << status_ << ": points: " << num_points_ 
               << " wins  : " << num_wins_
               << txt.Line ()
               << visible_cards_.Print (txt);
}

const Operation* PlayerProxy::Star (uint index, Expression* expr) {
    static const Operation This { "PlayerProxy",
        OperationCount (6), OperationFirst ('A'),
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
            return ExpressionArgs (expr, Params<1, STR, User::kMaxStatusLength> (),
                             Args (args, status_));
        }
        case 'B': {
            static const Operation OpB { "SetIsDealer",
                Params<0> (), Params<1, STR> (),
                "Sets the is_dealer_ flag.", 0
            };
            if (!expr) return &OpB;
            byte is_dealer;
            if (!ExpressionArgs (expr, Params<1, BOL> (), Args (args, &is_dealer)))
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
            return ExpressionArgs (expr, Params<1, UI4> (), Args (args, &state_));
        }
        case 'D': {
            static const Operation OpD { "SetNumPoints",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpD;
            return ExpressionArgs (expr, Params<1, UI4> (), Args (args, &num_points_));
        }
        case 'E': {
            static const Operation OpE { "SetNumWins",
                Params<0> (), Params<1, SI4> (),
                "Sets the state_.", 0
            };
            if (!expr) return &OpE;
            return ExpressionArgs (expr, Params<1, UI4> (), Args (args, &num_wins_));
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
            if (ExpressionArgs (expr, Params<2, UI1, UI1> (), Args (args, &pip, &suit)))
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
