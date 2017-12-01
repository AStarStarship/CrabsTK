/** kabuki:cards
    @file    /.../KabukiTheater-Examples/kabuki_cards/kabuki_cards/Blackjack/Dealer.cc
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

namespace kabuki { namespace cards {

Dealer::Dealer (int ante, int min_bet, int min_cards_per_hand,
                int max_cards_per_hand, int max_players, int deck_size) :
    pack_    (Deck ()),
    stock_   (pack_),
    players_ (OwnedArray<Player> ()) {
    SetValues (ante, min_bet, min_cards_per_hand, max_cards_per_hand,
               max_players, deck_size);
}

Dealer::~Dealer () {
    // Nothing to do here :-)
}

int Dealer::GetNumPlayers () {
    return players_.GetCount ();
}

int Dealer::GetMinNumCardsPerHand () {
    return min_cards_per_hand_;
}

int Dealer::GetDeckSize () {
    return deck_size_;
}

int Dealer::SetDeckSize (int value) {
    if (value < 1)
        return -1;
    deck_size_ = value;
}

int Dealer::GetNumDecks () {
    return num_decks_;
}

int Dealer::SetNumDecks (int value) {
    if (value < 1)
        return -1;
    num_decks_ = value;

    pack_ = Deck ()
}

int Dealer::GetMaxNumCardsPerHand () {
    return max_cards_per_hand_;
}

int Dealer::SetMinNumCardsPerHand (int num_cards) {
    if (num_cards < 1)
        return -1;
    min_cards_per_hand_ = num_cards;
}

int Dealer::GetMaxNumCardsPerHand () {
    return max_cards_per_hand_;
}

int Dealer::SetMinNumCardsPerHand (int num_cards) {
    if (num_cards < 1)
        return -1;
    min_cards_per_hand_ = num_cards;
}

int Dealer::SetMaxNumCardsPerHand (int num_cards) {
    if (num_cards < min_cards_per_hand_)
        return -1;
    max_cards_per_hand_ = num_cards;
}

void Dealer::AddPlayer (Player* newPlayer) {
    players_.Add (newPlayer);
}

int Dealer::RemovePlayer (const char* playerName) {
    for (int i = 0; i < players_.size (); ++i) {
        if (players_[i]->GetName () == playerName) {
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

    if (player_number_ >= players_.size ())
        return 1;

    players_.Remove (player_number_);
    return true;
}

Player* Dealer::GetPlayer (int player_number_) {
    if (player_number_ < 0)
        return nullptr;
    if (player_number_ > players_.size ())
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
    return pot_total_;
}

void Dealer::SetPotTotal (int newPotTotal) {
    if (newPotTotal < 0)
        return;
    pot_total_ = newPotTotal;
}
void Dealer::AddToPot (int pointsToAdd) {
    if (pointsToAdd < 0)
        return;
    pot_total_ += pointsToAdd;
}

int Dealer::GetStartingAnte () {
    return starting_ante_;
}

void Dealer::SetStartingAnte (int newAnte) {
    if (newAnte < 0)
        return;
    starting_ante_ = newAnte;
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
int Dealer::GetMaxPlayers () {
    return max_players_;
}

void Dealer::SetMaxPlayers (int max_players) {
    if (max_players < 0)
        return;
    max_players_ = max_players;
}

void Dealer::StartNewGame () {
    stock_.Reset ();
    stock_.Shuffle ();
    Redeal ();
}

void Dealer::Redeal () {
    for (Player* player : players_) {
        Hand hand (stock_, 2, Deck::kFullDeckSize);
        player->DealHand ();
    }
}

void Dealer::Print () {
    cout << "\n| Dealer:\n| Pot Total: " << pot_total_;
}

void Dealer::SetValues (int ante, int min_bet, int min_cards_per_hand,
                        int max_cards_per_hand, int newMaxNumPlayer,
                        int deck_size) {
    if (ante < 0)
        ante = 0;

    if (min_bet < 1)
        min_bet = 1;

    if (max_cards_per_hand < 1)
        max_cards_per_hand = 1;

    if (max_cards_per_hand < 1)
        max_cards_per_hand = 1;

    if (newMaxNumPlayer < 1)
        newMaxNumPlayer = 1;

    pot_total_ = 0;
    starting_ante_ = ante;
    ante_ = ante_ = ante;
    min_bet_ = min_bet;
    min_cards_per_hand_ = min_cards_per_hand;
    max_cards_per_hand_ = max_cards_per_hand;
    player_number_ = newMaxNumPlayer;
}

}   //< namespace cards
}   //< namespace kabuki
