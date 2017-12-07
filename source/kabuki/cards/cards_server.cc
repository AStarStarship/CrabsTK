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

#include "server.h"

using namespace _;
using namespace kabuki::id;
using namespace std;

namespace kabuki { namespace cards {

Server::Server (uint port_number, const char* server_name) :
                    server_name_ ((server_name_ == nullptr) ? "Unnamed Server" :server_name),
                    state_   (0),
                    dealer_  (nullptr),
                    users_   (users) {
    if (num_players < 1) {
        num_players = 1;
    }
    if (min_players < 1) {
        min_players = 1;
    }
    if (max_players < 1) {
        max_players = 1;
    }
    if (min_players > max_players) {
        min_players = max_players;
    }
    if (num_players < min_players) {
        num_players = min_players;
    }
    else if (num_players > max_players) {
        num_players = max_players;
    }
    // By process of elimination we know that num_players_ is bounded
    // safely between the min and max values.

    num_players_ = num_players;
    min_players_ = min_players;
    max_players_ = max_players;
}

Server::~Server () {
    for (int i = players_.GetCount (); i > 0; --i) {
        Player* player = players_.Pop ();
        delete player;
    }
}

int Server::GetState () {
    return state_;
}

bool Server::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

const char* Server::GetServerName () {
    return name_;
}

const char* Server::SetServerName (const char* name) {
    if (name == nullptr) {
        return "server_name can't be nil";
    }
    delete server_name_;
    server_name_ = StringClone (name);
}

Deck& Server::GetPack () {
    return pack_;
}

int Server::GetRoundNumber () {
    return round_number_;
}

int Server::GetNumPlayers () {
    return num_players_;
}

int Server::SetNumPlayers (int value) {
    if (value < min_players_) {
        return -1;
    }
    if (value > max_players_) {
        return 1;
    }
    min_players_ = value;
    return 0;
}

int Server::GetMinPlayers () {
    return min_players_;
}

int Server::SetMinPlayers (int value) {
    if (value >= max_players_) {
        return -1;
    }
    min_players_ = value;
    return 0;
}

int Server::GetMaxPlayers () {
    return max_players_;
}

int Server::SetMaxPlayers (int value) {
    if (value < min_players_) {
        return -1;
    }
    max_players_ = value;
    return 0;
}

void Server::NewGame () {
    round_number_ = 1;
    BeginRound ();
}

void Server::PrintPlayers () {
    for (int i = 0; i < players_.GetCount (); ++i) {
        players_.Element (i)->Print ();
    }
}

void Server::PrintRoundStatsString () {
    PrintLine ("|", '~');
    cout << "Round: " << round_number_ << "\n";


    PrintLine ("> ", '~');
}

void Server::Print () {
    PrintLine (" ", '_');
    cout << "\n> Card Game   : " << name_
         << "\n> Num Players : " << num_players_ << " Min: " << min_players_ 
         << " Max: " << max_players_
         << "\n> Round Number: " << round_number_
         << "\n> Num Players : " << players_.GetCount ();

    PrintPlayers ();
    PrintLine ("|", '_');
}

const char* DefaultPlayAgainString () {
    static const char play_again[] = "\n> Do you want to play again?"
        "\n> Enter y to continue, or n to quit."
        "\n< \0";
    return play_again;
}

}       //< namespace cards
}       //< namespace kabuki
