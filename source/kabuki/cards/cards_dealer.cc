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

namespace kabuki { namespace cards {

Dealer::Dealer (int ante, int min_bet, int min_cards_per_hand,
                int max_cards_per_hand, int max_players, int deck_size) :
    pack_     (Deck ()),
    stock_    (pack_),
    players_  () {
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

bool Dealer::SetDeckSize (int value) {
    if (value < 1)
        return false;
    deck_size_ = value;
    pack_.Reshuffle ();
    return true;
}

int Dealer::GetNumDecks () {
    return num_decks_;
}

bool Dealer::SetNumDecks (int value) {
    if (value < 1)
        return false;
    num_decks_ = value;

    pack_.Reshuffle ();
    return true;
}

int Dealer::GetMaxNumCardsPerHand () {
    return max_cards_per_hand_;
}

bool Dealer::SetMinNumCardsPerHand (int num_cards) {
    if (num_cards < 1)
        return false;
    min_cards_per_hand_ = num_cards;
    return true;
}

bool Dealer::SetMaxNumCardsPerHand (int num_cards) {
    if (num_cards < min_cards_per_hand_)
        return false;
    max_cards_per_hand_ = num_cards;
    return true;
}

void Dealer::AddPlayer (Player* newPlayer) {
    players_.Push (newPlayer);
}

int Dealer::RemovePlayer (const char* playerName) {
    for (int i = 0; i < players_.GetCount (); ++i) {
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
    stock_.Shuffle (pack_);
}

void Dealer::Deal (CardStack& stack, int num_cards) {
    for (int i = 0; i < num_cards; ++i) {
        stack.Push (stock_.Draw ());
    }
}

void Dealer::Redeal () {
    //for (int i = 0; i < players_.GetCount (); ++i) {
    //    Deal (players_[i].Get);
    //}
}

void Dealer::PrintDealer () {
    cout << "\n| Dealer:\n| Pot Total: " << pot_total_;
}

void Dealer::SetValues (int ante, int min_bet, int min_cards_per_hand,
                        int max_cards_per_hand, int max_num_players,
                        int deck_size) {
    if (ante < 0)
        ante = 0;

    if (min_bet < 1)
        min_bet = 1;

    if (max_cards_per_hand < 1)
        max_cards_per_hand = 1;

    if (max_cards_per_hand < 1)
        max_cards_per_hand = 1;

    if (max_num_players < 1)
        max_num_players = 1;

    pot_total_ = 0;
    starting_ante_ = ante;
    ante_ = ante_ = ante;
    min_bet_ = min_bet;
    min_cards_per_hand_ = min_cards_per_hand;
    max_cards_per_hand_ = max_cards_per_hand;
    player_number_ = max_num_players;         //< Dealer is always 
}

}   //< namespace cards
}   //< namespace kabuki
