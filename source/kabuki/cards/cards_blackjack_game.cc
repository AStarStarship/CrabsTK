/** Kabuki Toolkit
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
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

const int BlackjackGame::kDenominations[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };

BlackjackGame::BlackjackGame (id::UserList& users, id::User* dealer_user,
                              int64_t buy_in, int64_t ante, int64_t min_bet,
                              int min_players, int max_players) :
    Game       (users, "Blackjack", min_players, max_players),
    round_number_  (0),
    pot_           (0),
    num_ai_players (0),
    dealer_ (new BlackjackDealer (dealer_user, buy_in, ante, min_bet, 
                                  min_players, max_players)) {
    RestartGame ();
}

BlackjackGame::~BlackjackGame () {
    delete dealer_;
}

void BlackjackGame::RestartGame () {
    dealer_->RestartGame ();
    BeginRound ();
}

void BlackjackGame::BeginRound () {
    dealer_->BeginRound ();
}

void BlackjackGame::EndRound () {
    // Do we need to wait for the clients to respond?
    dealer_->EndRound ();
}

int BlackjackGame::GetRoundNumber () {
    return round_number_;
}

bool BlackjackGame::SetRoundNumber (int value) {
    if (value < 0) {
        return false;
    }
    round_number_ = value;
    return true;
}

int BlackjackGame::GetPot () {
    return pot_;
}

bool BlackjackGame::SetPot (int value) {
    if (value < 0) {
        return false;
    }
    pot_ = value;
    return true;
}

BlackjackDealer* BlackjackGame::GetDealer () {
    return dealer_;
}

/*
bool BlackjackGame::PlayGameInConsole () {
    enum {
        kBufferSize = 80,
    };
    
    char input[kBufferSize]; //< temp variable for console text input.

    BeginRound ();

    // You have to have enough points to play the game or else we have to exit.
    if (observers_[0]->GetNumPoints () < ante_) {
        cout << "You lose!";
        return false;
    }

    if (dealer_->GetNumPoints () < ante_) {
        cout << "\n| You just wiped out the house!!!"
                "\n| Your the ultimate Blackjack Champion!!!\n\n";
        return false;
    }

    PrintStats ();

    // Game loop for a round of Blackjack. It loops until we return either true or false.
    while (true) {
        BeginRound ();

        cout << "\n|";
        PrintLine ('_');
        cout << "\n| Round " << round_number_
            << ":\n";

        dealer_->PrintStats ();
        for (int i = 0; i < GetObservers ().size (); ++i) {
            observers_[i]->PrintStats ();
        }

        // The players need to either hit or hold first.

        bool inputValid = false;
        //< Always presume the data is false until proven correct.

        while (!inputValid) {
            // Read a line from the terminal, and convert it to lower case letters.
            KeyboardText ("\n| hit or hold?\n", input, kBufferSize);

            if (StrandEquals (input, "hit")) {
                CardStack& cards = dealer_->GetHand ().GetVisibleCards ();
                cards.Push (dealer_->GetStock ().Draw ());
                cout << "\n| ";
                for (int i = 0; i < GetObservers ().size (); ++i) {
                    cout << "\n| " << i;
                    observers_[i]->Print (txt);
                }
                inputValid = true;
            } else if (StrandEquals (input, "hold")) {
                inputValid = true;
                observers_[0]->SetState (BlackjackPlayer::kStateHolding);
            } else if (StrandEquals (input, "exit") || StrandEquals (input, "quit")) {
                cout << "\n| Exiting the game...\n";
                inputValid = true;
                return false;
            } else {
                cout << "\n| Please input 'hit' or 'hold':\n";
            }
        }

        // Dealer goes last
        dealer_->PlayRound ();

        // Now process the logic for if someone won the round.

        // The way we know that the round is over is when everyone is holding. 

        bool everyone_is_holding = dealer_->IsHolding ();
        for (int i = GetObservers ().size (); i > 0; --i) {
            if (observers_[i]->IsHolding ()) {
                everyone_is_holding = false;
                break;
            }
        }

        if (everyone_is_holding) {
            // Then everyone is holding so we need to process the end of round game logic.

            if (dealer_->Wins (observers_[0]->GetHand ())) //< If the dealer wins, it trumps all other players.
            {
                cout << "\n|\n| Dealer wins ({:-()";
                dealer_->Print (txt);
                observers_[0]->Print (txt);
                cout << "\n|";
                dealer_->AddPoints (pot_);
                dealer_->AddWin ();
            } else if (observers_[0]->Wins (dealer_->GetHand ())) {
                cout << "\n|\n| Players wins!!!";
                dealer_->Print (txt);
                observers_[0]->Print (txt);
                observers_[0]->AddPoints (pot_);
                observers_[0]->AddWin ();
                PrintLine ('$');
            }

            return true; // Exits loop and plays another game of Blackjack.
        }

        // The loop is almost over, so we need to check again if we can
        // still play the next round. We checked when we first entered the 
        // PlayBlackjackInConsole (Player, Player) function, so this is why
        // we checked at the end of the loop as opposed to the
        // beginning.
        if (observers_[0]->GetNumPoints () < ante_) {
            cout << "You lose!";
            return false;
        }

        if (dealer_->GetNumPoints () < ante_) {
            cout << "\n| You just wiped out the house!!!"
                 << "\n| Your the ultimate Blackjack Champion!!!";
            return false;
        }

        round_number_++;
    } 
    return false;
}*/

Text& BlackjackGame::Print (_::Text& txt) {
    return txt << txt.Line ('_')
               << "\n|         Game: " << name_
               << "\n| Num Observers: " << observers_.size ()
               << " Min: " << GetMinPlayers ()
               << " Max: " << GetMaxPlayers ()
               << "\n| Num Observers: " << observers_.size ()
               << dealer_->Print (txt)
               << txt.Line ('_');
}

const _::Operation* BlackjackGame::Star (uint index, _::Expression* expr) {
    static const Operation This = { "Blackjack",
        OperationCount (1), OperationFirst ('A'),
        "Insert directions on how to play blackjack here.", 0
    };
    void* args[1];
    int32_t player_uid;
    uint64_t session,
             public_key;

    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "join",
                Params<1, UI8, UI8> (), Params <0> (),
                "Adds the #user_session with #public_key to the list of game "
                "observers pending authentication.", 0 };
            if (!expr) return &OpA;
            if (ExpressionArgs (expr, Params<1, SI4> (), Args (args, &player_uid))) {
                return expr->result;
            }
            User* user = GetObservers ()[player_uid];
            if (user == nullptr) { // Invalid player_uid!
                return nullptr;
            }
            GetObservers ().push_back (user);

            return nullptr;
        }
        case 'B': {
            static const Operation OpB = { "leave",
                Params<1, UI8, UI8> (), Params <0> (),
                "Triggers #user_session with #public_key to Leave the game "
                "observer queue pending authentication.", 0 };
            if (!expr) return &OpB;
            if (ExpressionArgs (expr, Params<2, UI8, UI8> (), Args (args, &session, 
                                                              &public_key))) {
                return expr->result;
            }
            User* user = GetObservers ()[player_uid];
            if (user == nullptr) { // Invalid player_uid!
                return nullptr;
            }
            if (session != user->GetSession ()) {
                return Result (expr, Bin::kErrorAuthenticationError);
            }
            GetObservers ().push_back (user);

            return nullptr;
        }
    }
    index -= 'C';
    if (index == 0) {
        return Push (expr, dynamic_cast<Operand*> (dealer_));
    }
    Player* player = dealer_->GetPlayer (index - 1);
    if (!player) {
        return Result (expr, Bin::kErrorInvalidIndex);
    }
    return Result (expr, Bin::kErrorInvalidIndex);
}

const char* BlackjackGame:: Sudo (const char* text,
                                        const char* strand_end) {
    if (!text) {
        return nullptr;
    }
    if (text >= strand_end) {
        return "\n| Error: nil strand_end pointer in Star (const char*, "
               "const char*):const char*!";
    }

    if (text > strand_end) {
        return "\n| Error: text buffer overflow in Star (const char*, "
               "const char*):const char*!";
    }



    return nullptr;
}

}       //< namespace cards
}       //< namespace kabuki
