/* Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/player.cc
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

#include "player.h"

using namespace _;
using namespace std;

namespace kabuki {
namespace cards {

Player::Player (id::User* user, BOL is_dealer) :
    user_       (user),
    is_turn_    (false),
    is_dealer_  (false),
    num_wins_   (0),
    hand_       (52) {
}

Player::~Player () {}

id::User* Player::GetUser () {
    return user_;
}

const CH1* Player::GetHandle () {
    return user_->GetHandleKey ();
}

BOL Player::IsTurn () {
    return is_turn_;
}

void Player::SetIsTurn (BOL is_turn) {
    is_turn_ = is_turn;
}

BOL Player::IsDealer () {
    return is_dealer_;
}

void Player::SetIsDealer (BOL is_dealer) {
    is_dealer = is_dealer_;
}

SI4 Player::GetState () {
    return state_;
}

const CH1* Player::SetState (SI4 state) {
    state_ = state;
    return nullptr;
}

Hand& Player::GetHand () {
    return hand_;
}

SI4 Player::TakeCardUp (Card* card) {
    if (card == nullptr) {
        return -1;
    }
    return hand_.GetVisibleCards ().Push (card);
}

SI4 Player::TakeCardDown (Card* card) {
    if (card == nullptr) {
        return -1;
    }
    return hand_.GetHiddenCards ().Push (card);
}

SI4 Player::GetNumWins () {
    return num_wins_;
}

BOL Player::SetNumWins (SI4 num_wins) {
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

_::Text& Player::Print (_::Text& txt) {
    txt << "\n| "      << GetHandle ()
        << " points: " << GetUser()->GetValue ()
        << " wins  : " << num_wins_;

    PrintLine (txt);
    return hand_.Print (txt);
}

}   //< namespace cards
}   //< namespace kabuki
