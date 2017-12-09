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

Dealer::Dealer (id::User* user, int32_t buy_in, int32_t ante, int32_t min_bet, 
                int32_t min_players, int32_t max_players, int32_t num_decks) :
    round_number_  (0),
    pot_           (0),
    buy_in_        (buy_in),
    ante_          (ante),
    min_bet_       (min_bet),
    min_players_   (min_players),
    current_player_(0),
    num_responses_ (0),
    pack_          (num_decks),
    stock_         (pack_),
    players_       ((int32_t)((max_players < BlackjackGame::kMinPlayers)?
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

int32_t Dealer::GetRoundNumber () {
    return round_number_;
}

int32_t Dealer::GetNumPlayers () {
    return players_.GetCount ();
}

int32_t Dealer::RemovePlayer (const char* handle) {
    for (int32_t i = 0; i < players_.GetCount (); ++i) {
        if (players_[i]->GetUser()->GetStatus () == handle) {
            players_.Remove (i);
            return i;
        }
    }
    return 0;
}

int32_t Dealer::RemovePlayer (int32_t session) {
    // Note: Arrays in C++ always start at element 0 (ie.e players_[0])
    if (session < 0)
        return -1;

    if (session >= players_.GetCount ())
        return 1;

    players_.Remove (session);
    return true;
}

Player* Dealer::GetPlayer (int32_t session) {
    if (session < 0)
        return nullptr;
    if (session > players_.GetCount ())
        return nullptr;
    return players_[session];
}

int32_t Dealer::GetCurrentPlayer () {
    return current_player_;
}

bool Dealer::SetCurrentPlayer (int32_t session) {
    return current_player_ = session;
}

Deck& Dealer::GetPack () {
    return pack_;
}

CardStack& Dealer::GetStock () {
    return stock_;
}

int32_t Dealer::GetPotTotal () {
    return pot_;
}

void Dealer::SetPotTotal (int32_t newPotTotal) {
    if (newPotTotal < 0)
        return;
    pot_ = newPotTotal;
}
void Dealer::AddToPot (int32_t pointsToAdd) {
    if (pointsToAdd < 0)
        return;
    pot_ += pointsToAdd;
}

int32_t Dealer::GetBuyIn () {
    return buy_in_;
}

void Dealer::SetBuyIn (int32_t newAnte) {
    if (newAnte < 0)
        return;
    buy_in_ = newAnte;
}

int32_t Dealer::GetAnte () {
    return ante_;
}

void Dealer::SetAnte (int32_t ante) {
    if (ante < 0)
        return;
    ante_ = ante;
}

int32_t Dealer::GetMinBet () {
    return min_bet_;
}

const char* Dealer::SetMinBet (int32_t min_bet) {
    if (min_bet < 0)
        return "min_bet can't be negative";
    min_bet_ = min_bet;
    return nullptr;
}

int32_t Dealer::GetMinPlayers () {
    return min_players_;
}

int32_t Dealer::GetMaxPlayers () {
    return max_players_;
}

int32_t Dealer::GetPlayerNumber () {
    return min_bet_;
}

bool Dealer::SetPlayerNumber (int32_t player_number) {
    if (player_number < 0)
        return false;
    if (player_number >= players_.GetCount ())
        return false;
    current_player_ = player_number;
    return true;
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

    for (int32_t i = 0; i < players_.GetCount (); ++i) {
        players_[i]->RestartGame ();
    }

    pot_ = players_.GetCount () * ante_;

    for (int32_t i = players_.GetCount (); i > 0; --i) {
        players_[i]->RemovePoints (ante_);
    }
    RemovePoints (ante_);
}

Card* Dealer::Draw () {
    return stock_.Draw ();
}

void Dealer::Redeal () {
    //for (int32_t i = 0; i < players_.GetCount (); ++i) {
    //    Deal (players_[i].Get);
    //}
}

void Dealer::Print () {
    cout << "\n| Dealer:\n| Pot Total: " << pot_;
    for (int32_t i = 0; i < players_.GetCount (); ++i) {
        players_[i]->Print ();
    }
}

}   //< namespace cards
}   //< namespace kabuki
