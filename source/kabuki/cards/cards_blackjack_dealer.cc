/** kabuki::cards
    @file       ~/source/kabuki/cards/blackjack_dealer.cc
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

#include "blackjack_dealer.h"
#include "blackjack_game.h"
#include "blackjack_player.h"

using namespace _;
using namespace std;

namespace kabuki { namespace cards {

BlackjackDealer::BlackjackDealer (id::User* user, int buy_in, int ante, int min_bet,
                                  int min_players, int max_players) :
    Dealer (user, buy_in, ante, min_bet, min_players, max_players) {
    // Nothing to do here :-)
}

BlackjackDealer::~BlackjackDealer () {

}

void Dealer::AddPlayer (Player* player) {
    players_.Push (player);
}

bool BlackjackDealer::RaiseAnte (int value) {
    // You have to have enough points to play the game or else we have to exit.
    if (value < 0) {
        return false;
    }
    for (int i = 0; i < players_.GetCount (); ++i) {
        Player* player = players_[i];
        if (ante_ + 10 > player->GetNumPoints ()) {
            return player->SetState (BlackjackPlayer::kStateObserving);
        }
    }

    ante_ += value;
    return true;
}

int BlackjackDealer::GetScore (int ace_value) {
    int score = 0;  //< Always set the variable before you start using it!!!

    Card* card;
    for (int i = 0; i < hand_.GetVisibleCards ().GetCount (); ++i) {
        card = hand_.GetVisibleCards ().GetCard (i);
        int denomination = card->GetDenomination ();
        score += ((denomination == Card::kAce)?ace_value:denomination);
    }

    return score;
}

void BlackjackDealer::RestartGame () {

}

void BlackjackDealer::BeginRound () {
    stock_.Shuffle (pack_);
    if (players_.GetCount () < BlackjackGame::kMinPlayers) {
        return;
    }
    Player* player;
    for (int i = players_.GetCount (); i > 0; --i) {
        player = players_[i];
        if (player->GetNumPoints () < ante_) {

        }
    }
    if (GetNumPoints () < ante_) {
        return;
    }

    RemovePoints (ante_);

    for (int i = 0; i < players_.GetCount (); ++i) {
        players_.Element (i)->RemovePoints (ante_);
    }

    // Then put the points into the pot.
    Dealer::SetPotTotal (players_.GetCount () * ante_);

    stock_.Shuffle (pack_);

    // Deal hand in a circle with deal at the end.
    for (int i = players_.GetCount (); i > 0; --i) {
        players_[i]->GetHand ().GetVisibleCards ().Push (Draw ());
    }
    GetHand ().GetVisibleCards ().Push (Draw ());
    // ... and to deal a new hand.
    for (int i = players_.GetCount (); i > 0; --i) {
        players_[i]->GetHand ().GetVisibleCards ().Push (Draw ());
    }
    GetHand ().GetHiddenCards ().Push (Draw ());
}

void BlackjackDealer::PlayRound () {

}

void BlackjackDealer::EndRound () {

}

void BlackjackDealer::EndGame () {

}

int BlackjackDealer::Compare (Hand& other) {
        return BlackjackCompareHands (hand_, other);
    }

bool BlackjackDealer::Wins (Hand& other) {
    return BlackjackCompareHands (hand_, other) > 0;
}

void BlackjackDealer::PrintStats () {
    PrintLine ("|", '_');
    cout << "\n| Ante: " << ante_ << ".\n" <<
        "\n| Initial Stats:\n";
    for (int i = 0; i < players_.GetCount (); ++i) {
        players_[i]->PrintStats ();
    }
    PrintStats ();
    cout << "\n| Starting round of Blackjack...\n\n";
}

/*
void BlackjackDealer::PrintStats () {
    PrintLine ("|", '~');
    cout << "Round: " << round_number_ << "\n";


    PrintLine ("|", '~');
}*/

void BlackjackDealer::Print () {
    for (int i = 0; i < players_.GetCount (); ++i) {
        players_[i]->Print ();
    }
}

}   //< namespace cards
}   //< namespace kabuki
