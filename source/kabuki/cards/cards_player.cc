/** kabuki:cards
    @file    ~/source/kabuki/cards/player.cc
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
#include "player.h"

using namespace _;
using namespace std;

namespace kabuki {
namespace cards {

Player::Player (CardStack& stock, const char* status,
                int start_points, bool is_dealer) :
    num_wins_ (0),
    num_points_ (start_points < 1?1:start_points),
    hand_ (52),
    stock_ (stock) {
    SetStatus (status);
}

const char* Player::GetDislpayName () {
    return name_;
}

const char* Player::SetDislpayName (const char* name) {
    if (name == nullptr) {
        return "name can't be nil";
    }
    delete name;
    return nullptr;
}

void Player::SetName (const char* name) {
    if (name == nullptr)
        return;
    name_ = name;
}

bool Player::IsDealer () {
    return is_dealer_;
}

void Player::SetIsDealer (bool is_dealer) {
    is_dealer = is_dealer_;
}

int Player::GetState () {
    return state_;
}

const char* Player::SetState (int state) {
    state_ = state;
    return nullptr;
}

Hand& Player::GetHand () {
    return hand_;
}

int Player::GetNumPoints () {
    return num_points_;
}

bool Player::SetNumPoints (int num_points) {
    if (num_points < 0) {
        return false;
    }
    num_points_ = num_points;
    return true;
}

int Player::AddPoints (int num_points) {
    if (num_points < 0)
        return -1;

    num_points_ += num_points;

    return num_points_;
}

bool Player::RemovePoints (int num_points) {
    if (num_points < 0) {
        return false;
    }
    if (num_points > num_points_) {
        return false;
    }
    num_points_ -= num_points;

    return true;
}

int Player::GetNumWins () {
    return num_wins_;
}

bool Player::SetNumWins (int num_wins) {
    if (num_wins < 0) {
        return false;
    }
    num_wins_ = num_wins;
    return true;
}

void Player::AddWin () {
    ++num_wins_;
}

void Player::ResetWins () {
    num_wins_ = 0;    //< Reset the num_wins_.
}

void Player::Print () {
    cout << "\n| Player    : " << name_ << " points: " << num_points_ 
         << " wins  : " << num_wins_;

    PrintLine ('-');
    hand_.Print ();
}

}   //< namespace cards
}   //< namespace kabuki
