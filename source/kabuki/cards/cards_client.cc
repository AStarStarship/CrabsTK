/** kabuki::cards
    @file    ~/source/kabuki/cards/cards_card_game.cc
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

#include "client.h"

using namespace _;
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

Client::Client () {
}

Client::~Client () {
    for (int i = players_.GetCount (); i > 0; --i) {
        Player* player = players_.Pop ();
        delete player;
    }
}

int Client::GetState () {
    return state_;
}

bool Client::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

const char* Client::GetName () {
    return name_;
}

Deck& Client::GetPack () {
    return pack_;
}

int Client::GetRoundNumber () {
    return round_number_;
}

int Client::GetNumPlayers () {
    return num_players_;
}

int Client::SetNumPlayers (int value) {
    if (value < min_players_)
        return -1;
    if (value > max_players_)
        return 1;

    min_players_ = value;
    return 0;
}

int Client::GetMinPlayers () {
    return min_players_;
}

int Client::SetMinPlayers (int value) {
    if (value >= max_players_)
        return -1;

    min_players_ = value;
    return 0;
}

int Client::GetMaxPlayers () {
    return max_players_;
}

int Client::SetMaxPlayers (int value) {
    if (value < min_players_)
        return -1;

    max_players_ = value;
    return 0;
}

void Client::NewGame () {
    round_number_ = 1;
    BeginRound ();
}

void Client::PrintPlayers () {
    for (int i = 0; i < players_.GetCount (); ++i) {
        players_.Element (i)->Print ();
    }
}

void Client::PrintRoundStatsString () {
    PrintLine ("|", '~');
    cout << "Round: " << round_number_ << "\n";


    PrintLine ("> ", '~');
}

void Client::Print () {
    PrintLine (" ", '_');
    cout << "\n> Card Game   : " << name_
         << "\n> Num Players : " << num_players_ << " Min: " << min_players_ 
         << " Max: " << max_players_
         << "\n> Round Number: " << round_number_
         << "\n> Num Players : " << players_.GetCount ();

    PrintPlayers ();
    PrintLine ("|", '_');
}

const _::Operation* Client::Star (uint index, _::Expression* expr) {

}

const char* DefaultPlayAgainString () {
    static const char play_again[] = "\n> Do you want to play again?"
        "\n> Enter y to continue, or n to quit."
        "\n< \0";
    return play_again;
}

}       //< namespace cards
}       //< namespace kabuki
