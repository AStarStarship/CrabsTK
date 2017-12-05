/** kabuki::cards
    @file    ~/source/kabuki/cards/cards_blackjack_game.cc
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

#include "blackjack_game.h"

using namespace _;
using namespace std;

namespace kabuki { namespace cards {

const int BlackjackGame::kDenominations[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10
};

BlackjackGame::BlackjackGame (int initNumPlayers) :
    CardGame ("Blackjack", initNumPlayers, 2, 13) {
    min_ante_ = kDefaultAnte;
    if (initNumPlayers < kMinPlayers) {
        ante_ = initNumPlayers; //< Ante per round.
    }

    // Set the ante
    ante_ = min_ante_ < kMinAnte?kMinAnte:min_ante_;
    points_pot_  = 0;  //< Points in the pot.
    round_number_ = 1; //< Current round number.
    BeginRound ();       //< Starts a new game.
}

bool BlackjackGame::RaiseAnteBy10 () {
    // You have to have enough points to play the game or else we have to exit.

    for (int i = 0; i < players_.GetCount (); ++i) {
        Player* player = players_.Element (i);
        if (ante_ + 10 > player->GetNumPoints ()) {
            return player->SetState (kStateOutOfGame);
        }
    }

    ante_ += 10;
    return true;
}

void BlackjackGame::StartNewGame () {
    CardGame::NewGame ();
    round_number_ = 1;
    ante_ = min_ante_;
    points_pot_ = 0;

    for (int i = 0; i < players_.GetCount (); ++i) {
        players_.Element (i)->NewGame ();
    }
    dealer_.StartNewGame ();

    points_pot_ = num_players_ * ante_;

    for (int i = players_.GetCount (); i > 0; --i) {
        players_[i]->RemovePoints (ante_);
    }
    dealer_.RemovePoints (ante_);

    BeginRound ();
}

void BlackjackGame::BeginRound () {
    Player* player;
    for (int i = players_.GetCount (); i > 0; --i) {
        player = players_[i];
        if (player->GetNumPoints () < ante_) {

        }
    }
    if (dealer_.GetNumPoints () < ante_) {
        return;
    }

    dealer_.RemovePoints (ante_);

    for (int i = 0; i < players_.GetCount (); ++i) {
        players_.Element (i)->RemovePoints (ante_);
    }

    // Then put the points into the pot.
    points_pot_ = num_players_ * ante_;
    // The fist part of every card game is to shuffle the Deck...
    stock_.Shuffle (pack_);

    // ... and to deal a new hand.
    for (int i = players_.GetCount (); i > 0; --i) {
        players_[i]->GetHand ().GetVisibleCards ().Push (stock_.Draw ());
    }
    dealer_.GetHand ().GetVisibleCards ().Push (dealer_.GetStock ().Draw ());
    // ... and to deal a new hand.
    for (int i = players_.GetCount (); i > 0; --i) {
        players_[i]->GetHand ().GetVisibleCards ().Push (stock_.Draw ());
    }
    dealer_.GetHand ().GetHiddenCards ().Push (dealer_.GetStock ().Draw ());
}

void BlackjackGame::EndRound () {
    // Do we need to wait for the clients to respond?
}

void BlackjackGame::PrintStats () {
    PrintLine ("> ", '_');
    cout << "\n> Ante: " << ante_ << ".\n" <<
        "\n> Initial Stats:\n";
    for (int i = 0; i < players_.GetCount (); ++i) {
        players_[i]->PrintStats ();
    }
    dealer_.PrintStats ();
    cout << "\n> Starting round of Blackjack...\n\n";
}

bool BlackjackGame::PlayGameInConsole () {
    stock_.Shuffle (pack_);
    BeginRound ();

    enum {
        kBufferSize = 80,
    };
    
    char input[kBufferSize]; //< temp variable for console text input.

    // You have to have enough points to play the game or else we have to exit.
    if (players_[0]->GetNumPoints () < ante_) {
        cout << "You lose!";
        return false;
    }

    if (dealer_.GetNumPoints () < ante_) {
        cout << "\n> You just wiped out the house!!!"
                "\n> Your the ultimate Blackjack Champion!!!\n\n";
        return false;
    }

    // Add some test players
    BlackjackPlayer* player = new BlackjackPlayer (dealer_.GetStock ());
    players_.Push (player);

    PrintStats ();

    // Game loop for a round of Blackjack. It loops until we return either true or false.
    while (true) {
        BeginRound ();

        cout << "\n>";
        PrintLine ("> ", '_');
        cout << "\n> Round " << round_number_
            << ":\n";

        dealer_.PrintStats ();
        for (int i = 0; i < players_.GetCount (); ++i) {
            players_[i]->PrintStats ();
        }

        // The players need to either hit or hold first.

        bool inputValid = false;
        //< Always presume the data is false until proven correct.

        while (!inputValid) {
            // Read a line from the terminal, and convert it to lower case letters.
            KeyboardString ("\n> hit or hold?\n", input, kBufferSize);

            if (StringEquals (input, "hit")) {
                dealer_.GetHand ().GetVisibleCards ().Push (dealer_.GetStock ().Draw ());
                cout << "\n| ";
                for (int i = 0; i < players_.GetCount (); ++i) {
                    cout << "\n| " << i;
                    players_[i]->Print ();
                }
                inputValid = true;
            } else if (StringEquals (input, "hold")) {
                inputValid = true;
                players_[0]->SetState (BlackjackPlayer::kStateHolding);
            } else if (StringEquals (input, "exit") || StringEquals (input, "quit")) {
                cout << "\n| Exiting the game...\n";
                inputValid = true;
                return false;
            } else {
                cout << "\n> Please input 'hit' or 'hold':\n";
            }
        }

        // Dealer goes last
        dealer_.PlayRound ();

        // Now process the logic for if someone won the round.

        // The way we know that the round is over is when everyone is holding. 

        bool everyone_is_holding = dealer_.IsHolding ();
        for (int i = players_.GetCount (); i > 0; --i) {
            if (players_[i]->IsHolding ()) {
                everyone_is_holding = false;
                break;
            }
        }

        if (everyone_is_holding) {
            // Then everyone is holding so we need to process the end of round game logic.

            if (dealer_.HandWins (players_[0]->GetHand ())) //< If the dealer wins, it trumps all other players.
            {
                cout << "\n|\n| Dealer wins ({:-()";
                dealer_.Print ();
                players_[0]->Print ();
                cout << "\n|";
                dealer_.AddPoints (points_pot_);
                dealer_.AddWin ();
            } else if (players_[0]->HandWins (dealer_.GetHand ())) {
                cout << "\n|\n| Players wins!!!";
                dealer_.Print ();
                players_[0]->Print ();
                players_[0]->AddPoints (points_pot_);
                players_[0]->AddWin ();
                PrintLine ("|", '$');
            }

            return true; // Exits loop and plays another game of Blackjack.
        }

        /** The loop is almost over, so we need to check again if we can
            still play the next round. We checked when we first entered the 
            PlayBlackjackInConsole (Player, Player) function, so this is why
            we checked at the end of the loop as opposed to the
            beginning. */
        if (players_[0]->GetNumPoints () < ante_) {
            cout << "You lose!";
            return false;
        }

        if (dealer_.GetNumPoints () < ante_) {
            cout << "\n| You just wiped out the house!!!"
                 << "\n| Your the ultimate Blackjack Champion!!!";
            return false;
        }

        round_number_++;
    }
    delete players_[0];
    players_.Pop ();
}

const _::Operation* BlackjackGame::Star (uint index, _::Expression* expr) {
    static const Operation This = { "Blackjack",
        NumOperations (0), FirstOperation ('A'),
        "Insert directions on how to play blackjack here.", 0 };

    switch (index) {
        case '?': return &This;
    }
    return nullptr;
}

}       //< namespace cards
}       //< namespace kabuki
