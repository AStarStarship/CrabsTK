/** kabuki:cards
    @file    ~/source/kabuki/cards/dealer.cc
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

#include "dealer.h"
#include "blackjack_game.h"

using namespace std;

namespace kabuki { namespace cards {

Dealer::Dealer (id::User* user, int buy_in, int ante, int min_bet, int min_players,
                int max_players) :
    round_number_ (0),
    pot_          (0),
    buy_in_       (buy_in),
    ante_         (ante),
    min_bet_      (min_bet),
    pack_         (Deck ()),
    stock_        (pack_),
    players_      ((int)((max_players < BlackjackGame::kMinPlayers)?
                   BlackjackGame::kMinPlayers : max_players)) {
    if (ante < 0) {
        ante = 0;
    }
    if (min_bet < 1){
        min_bet = 1;
    }
}

Dealer::~Dealer () {
    // Nothing to do here :-)
}

int Dealer::GetRoundNumber () {
    return round_number_;
}

int Dealer::GetNumPlayers () {
    return players_.GetCount ();
}

int Dealer::RemovePlayer (const char* handle) {
    for (int i = 0; i < players_.GetCount (); ++i) {
        if (players_[i]->GetUser()->GetStatus () == handle) {
            players_.Remove (i);
            return i;
        }
    }
    return 0;
}

int Dealer::RemovePlayer (int player_number_) {
    // Note: Arrays in C++ always start at element 0 (ie.e players_[0])
    if (player_number_ < 0)
        return -1;

    if (player_number_ >= players_.GetCount ())
        return 1;

    players_.Remove (player_number_);
    return true;
}

Player* Dealer::GetPlayer (int player_number_) {
    if (player_number_ < 0)
        return nullptr;
    if (player_number_ > players_.GetCount ())
        return nullptr;

    return players_[player_number_];
}

Deck& Dealer::GetPack () {
    return pack_;
}

CardStack& Dealer::GetStock () {
    return stock_;
}

int Dealer::GetPotTotal () {
    return pot_;
}

void Dealer::SetPotTotal (int newPotTotal) {
    if (newPotTotal < 0)
        return;
    pot_ = newPotTotal;
}
void Dealer::AddToPot (int pointsToAdd) {
    if (pointsToAdd < 0)
        return;
    pot_ += pointsToAdd;
}

int Dealer::GetBuyIn () {
    return buy_in_;
}

void Dealer::SetBuyIn (int newAnte) {
    if (newAnte < 0)
        return;
    buy_in_ = newAnte;
}

int Dealer::GetAnte () {
    return ante_;
}

void Dealer::SetAnte (int newAnte) {
    if (newAnte < 0)
        return;
    ante_ = newAnte;
}

int Dealer::GetMinBet () {
    return min_bet_;
}

void Dealer::SetMinBet (int newValue) {
    if (newValue < 0)
        return;
    min_bet_ = newValue;
}
int Dealer::GetMinPlayers () {
    return min_players_;
}

int Dealer::GetMaxPlayers () {
    return max_players_;
}

void Dealer::RestartGame () {
    if (players_.GetCount () < min_players_) {
        std::cout << "\n| Not enough players!";
        return;
    }
    stock_.Shuffle (pack_);

    round_number_ = 1;
    ante_ = buy_in_;
    pot_ = 0;

    for (int i = 0; i < players_.GetCount (); ++i) {
        players_[i]->RestartGame ();
    }

    pot_ = players_.GetCount () * ante_;

    for (int i = players_.GetCount (); i > 0; --i) {
        players_[i]->RemovePoints (ante_);
    }
    RemovePoints (ante_);
}

Card* Dealer::Draw () {
    return stock_.Draw ();
}

void Dealer::Redeal () {
    //for (int i = 0; i < players_.GetCount (); ++i) {
    //    Deal (players_[i].Get);
    //}
}

void Dealer::Print () {
    cout << "\n| Dealer:\n| Pot Total: " << pot_;
    for (int i = 0; i < players_.GetCount (); ++i) {
        players_[i]->Print ();
    }
}

}   //< namespace cards
}   //< namespace kabuki
