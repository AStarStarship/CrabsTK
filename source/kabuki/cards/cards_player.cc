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

namespace kabuki { namespace cards {

Player::Player (const char* player_name, int start_points, bool is_dealer) :
    name_ (player_name),
    num_wins_ (0),
    num_points_ (start_points < 1?1:start_points),
    hand_ () {
    // Nothing to do here!
}

bool Player::PlayOrPass (Hand& other) {
    return false;
}

const char* Player::GetName () {
    return name_;
}

void Player::SetName (const char* name) {
    if (name == nullptr)
        return;
    name_ = name;
}

int Player::GetState () {
    return state_;
}

const char* Player::SetState (int state) {
    state_ = state;
    return nullptr;
}

Hand* Player::GetHand () {
    return hand_;
}

void Player::TakeHand (Hand* hand) {
    if (hand == nullptr)
        return;
    hand_ = hand;
}

int Player::GetNumPoints () {
    return num_points_;
}

int Player::AddPoints (int num_points) {
    if (num_points < 0)
        return -1;

    num_points_ += num_points;

    return num_points_;
}

int Player::RemovePoints (int num_points) {
    if (num_points < 0)
        return -1;

    if (num_points > num_points_)
        return num_points_ - num_points;

    num_points_ -= num_points;

    return 0;
}

int Player::GetNumWins () {
    return num_wins_;
}

void Player::AddWin () {
    ++num_wins_;
}

void Player::ResetWins () {
    num_wins_ = 0;    //< Reset the num_wins_.
}

void Player::Print () {
    cout << "\n| Player     : " << name_
        << "\n| num_points_: " << num_points_
        << "\n| num_windows: " << num_wins_;

    PrintLine ('-');
    hand_->Print ();
}

}   //< namespace cards
}   //< namespace kabuki
