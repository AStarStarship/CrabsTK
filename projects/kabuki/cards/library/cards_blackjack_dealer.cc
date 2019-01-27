/** Kabuki Arcade
    @file       ~/source/kabuki/arcade/cards/blackjack_dealer.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "blackjack_player_ai.h"

using namespace std;
using namespace _;
using namespace kabuki::id;

namespace kabuki { namespace cards {

BlackjackDealer::BlackjackDealer (id::User* user, int64_t buy_in, int64_t ante,
                                  int64_t min_bet, SI4 min_players,
                                  SI4 max_players) :
    Dealer (user, buy_in, ante, min_bet, min_players, max_players) {
    // Nothing to do here :-)
}

BlackjackDealer::~BlackjackDealer () {

}

void Dealer::AddPlayer (Player* player) {
    players_.push_back (player);
}

BOL BlackjackDealer::RaiseAnte (int64_t value) {
    // You have to have enough points to play the game or else we have to exit.
    if (value < 0) {
        return false;
    }
    for (size_t i = 0; i < players_.size (); ++i) {
        Player* player = players_[i];
        if (ante_ + 10 > player->GetUser ()->GetValue ()) {
            return player->SetState (BlackjackPlayer::kStateObserving);
        }
    }

    ante_ += value;
    return true;
}

SI4 BlackjackDealer::CalcScore (SI4 ace_value) {
    SI4 score = 0;  //< Always set the variable before you start using it!!!

    Card* card;
    for (SI4 i = 0; i < hand_.GetVisibleCards ().Length (); ++i) {
        card = hand_.GetVisibleCards ().GetCard (i);
        SI4 denomination = card->GetDenomination ();
        score += ((denomination == Card::kAce)?ace_value:denomination);
    }

    return score;
}

void BlackjackDealer::RestartGame () {

}

void BlackjackDealer::BeginRound () {
    stock_.Shuffle (pack_);
    if (players_.size () < BlackjackGame::kMinPlayers) {
        return;
    }
    Player* player;
    for (size_t i = players_.size (); i > 0; --i) {
        player = players_[i];
        User* user = player->GetUser ();
        if (user->GetValue () < ante_) {
            cout << "\n| " << user->GetHandleKey ()
                 << " is out of the game. ";
            RemovePlayer (i);
        }
    }
    if (GetUser ()->GetValue () < ante_) {
        return;
    }

    GetUser ()->AddValue (ante_);

    for (size_t i = 0; i < players_.size (); ++i) {
        players_[i]->GetUser ()->AddValue (ante_);
    }

    // Then put the points into the pot.
    ResetPot ();
    AddToPot (players_.size () * ante_);

    stock_.Shuffle (pack_);

    // Deal hand in a circle with deal at the end.
    for (size_t i = players_.size (); i > 0; --i) {
        players_[i]->GetHand ().GetVisibleCards ().Push (Draw ());
    }
    GetHand ().GetVisibleCards ().Push (Draw ());
    // ... and to deal a new hand.
    for (size_t i = players_.size (); i > 0; --i) {
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

SI4 BlackjackDealer::Compare (Hand& other) {
        return BlackjackCompareHands (hand_, other);
    }

BOL BlackjackDealer::Wins (Hand& other) {
    return BlackjackCompareHands (hand_, other) > 0;
}

SI4 BlackjackDealer::AddAgent (id::User* user) {
    Player* player = new BlackjackPlayerAi (user, GetStock ());
    players_.push_back (player);
    return players_.size () - 1;
}

Text& BlackjackDealer::PrintStats (Text& txt) {
    txt << txt.Line ('_')
        << "\n| Ante: " << ante_ << ""
        << "\n| Initial Stats:";

    for (size_t i = 0; i < players_.size (); ++i) {
        players_[i]->PrintStats (txt);
    }
    PrintStats (txt);
    return txt << "\n| Starting round of Blackjack...\n\n";
}

/*
void BlackjackDealer::PrintStats () {
    PrintLine ('~');
    cout << "Round: " << round_number_ << "\n";


    PrintLine ('~');
}*/

Text& BlackjackDealer::Print (_::Text& txt) {
    SI4 num_players = GetPlayersCount ();
    if (!num_players) {
        return txt << "\n| Players: none";
    }
    txt << "\n| Players:";
    for (SI4 i = 0; i < num_players - 1; ++i) {
        players_[i]->Print (txt);
    }
    return players_[num_players - 1]->Print ();
}

const Operation* BlackjackDealer::Star (uint index, Expression* expr) {
    static const Operation This = { "BlackjackPlayer",
        OperationCount (0), OperationFirst ('A'),
        "Player in a Blackjack game.", 0 };
    void* args[2];
    CH1 handle[id::Handle::kMaxLength],
        tweet[141];
    SI4 session;
    uint64_t public_key,
             num_points;
    BlackjackPlayer* player;

    switch (index) {
        case '?': return ExpressionOperation (expr, &This);
        case 'A': {
            static const Operation OpA = { "Hit",
                Params<0> (), Params<0> (),
                "Signals a player to \"hit\" and not take any more cards "
                "this round given correct #session and #public_key.", 0 };
            if (!expr) return &OpA;
            if (ExpressionArgs (expr, Params<2, SI4, UI8> (),
                                Args (args, &session, &public_key))) {
                return expr->result;
            }
            player = dynamic_cast<BlackjackPlayer*> (GetPlayer (current_player_));
            if (player == nullptr) {
                return Result (expr, Bin::kErrorInvalidOperand);
            }
            if (!player->GetUser ()->IsAuthentic (session, public_key)) {
                return Result (expr, Bin::kErrorAuthenticationError);
            }
            player->Hit ();
            PlayRound ();
            return nullptr;
        }
        case 'B': {
            static const Operation OpB = { "Hold",
                Params<2, SI4, UI8> (), Params<0> (),
                "Signals a player to \"hold\" and not take any more cards "
                "this round given correct #session and #public_key.", 0 };
            if (!expr) return &OpB;
            if (ExpressionArgs (expr, Params<2, SI4, UI8> (), Args (args, &session,
                &public_key))) {
                return expr->result;
            }
            player = dynamic_cast<BlackjackPlayer*> (GetPlayer (session));
            if (!player) {
                return Result (expr, Bin::kErrorAuthenticationError);
            }
            if (public_key != player->GetUser ()->GetSessionKey ()) {
                return Result (expr, Bin::kErrorAuthenticationError);
            }
            player->Hold ();
            PlayRound ();
            return nullptr;
        }
        case 'C': {
            static const Operation OpC = { "TweetAll",
                Params<2, STR, id::Handle::kMaxLength, STR, 141> (), 
                       Params<0> (),
                "Sends a message of 140 chars or less to this player.", 0 };
            if (!expr) return &OpC;
            if (ExpressionArgs (expr, Params<2, 
                          STR, id::Handle::kMaxLength, STR, 141> (),
                          Args (args, handle, tweet))) {
                return expr->result;
            }
            cout << "\n| Message from @" << handle << "\n| " << tweet;
        }
        case 'D': {
            static const Operation OpD = { "BuyValue",
                Params<3, SI4, UI8, UI8> (),
                Params<UI8> (),
                "Attempts to buy the specified number of coins.", 0 };
            if (!expr) return &OpD;
            if (ExpressionArgs (expr, Params<3, SI4, UI8, UI8> (),
                          Args (args, &session, &public_key, &num_points))) {
                return expr->result;
            }
            player = reinterpret_cast<BlackjackPlayer*> (GetPlayer (session));
            if (!player) {
                return Result (expr, Bin::kErrorAuthenticationError);
            }
            if (public_key != player->GetUser ()->GetSessionKey ()) {
                return Result (expr, Bin::kErrorAuthenticationError);
            }
            cout << "\n| Message from @" << handle << "\n| " << tweet;
            return nullptr;
        }
    }
    return nullptr;
}

const CH1* BlackjackDealer::Sudo (const CH1* text, const CH1* strand_end) {
    const CH1* token_end;
    SI4 num_players;

    BlackjackPlayer* player = dynamic_cast<BlackjackPlayer*> (GetPlayer ());
    text = TextSkipSpaces (text, strand_end);
    if (!text) {
        return nullptr;
    }
    if (current_player_ < GetMinPlayers ()) {
        return "";
    }
    token_end = TokenEnd (text, strand_end);
    if (TokenCompare (text, strand_end, "in")) {
        num_players = GetPlayersCount ();
        if (num_players < BlackjackGame::kMinPlayers) {
        }
        if (!player) {
            cout << "\n| Error: no player is selected to buy in!";
        }
    } else if (TokenCompare (text, strand_end, "raise")) {
        if (!player) {
            cout << "\n| Error: no player is selected to raise!";
        }
    } else if (TokenCompare (text, strand_end, "hit")) {
        if (!player) {
            cout << "\n| Error: no player is selected to Hit!";
        }
    } else if (TokenCompare (text, strand_end, "hold")) {
        if (!player) {
            cout << "\n| Error: no player is selected to Hold!";
        }
    }
    return token_end;
}

}   //< namespace cards
}   //< namespace kabuki
