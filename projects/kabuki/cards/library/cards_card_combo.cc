/* Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/arcade_card_combo.cc
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

#include "card_combo.h"

namespace kabuki { namespace cards {

CardCombo::CardCombo (const CardStack& cards, BOL aces_high,
                      SI4 joker_pip_1, SI4 joker_suit_1,
                      SI4 joker_pip_2, SI4 joker_suit_2) {
    Set (aces_high, joker_pip_1, joker_suit_1, joker_pip_2, joker_suit_2);
}

void CardCombo::Set (BOL aces_high, SI4 joker_pip_1, SI4 joker_suit_1,
                     SI4 joker_pip_2, SI4 joker_suit_2) {
    if (joker_pip_1 < 1)
        joker_pip_1 = 1;
    else if (joker_pip_1 > 13)
        joker_pip_1 = 13;

    if (joker_pip_2 < 1)
        joker_pip_2 = 1;
    else if (joker_pip_2 > 13)
        joker_pip_2 = 13;

    if (joker_suit_1 < 1)
        joker_suit_1 = 1;
    else if (joker_suit_1 > 4)
        joker_suit_1 = 4;

    if (joker_suit_2 < 1)
        joker_suit_2 = 1;
    else if (joker_suit_2 > 4)
        joker_suit_2 = 4;

    joker_pip_1_ = joker_pip_1;
    joker_suit_1_ = joker_suit_1;
    joker_pip_2_ = joker_pip_2;
    joker_suit_2_ = joker_suit_2;
    aces_high_ = aces_high;
}

CardCombo::~CardCombo () {
    // Nothing to do here :-)
}

SI4 CardCombo::GetJokerPip1 () {
    return joker_pip_1_;
}

BOL CardCombo::SetJokerPip1 (SI4 value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_pip_1_ = value;
    return true;
}

SI4 CardCombo::GetJokerSuitValue1 () {
    return joker_suit_1_;
}

BOL CardCombo::SetJokerSuitValue1 (SI4 value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_suit_1_ = value;
    return true;
}

SI4 CardCombo::GetJokerPip2 () {
    return joker_pip_2_;
}

BOL CardCombo::SetJokerPip2 (SI4 value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_pip_2_ = value;
    return true;
}

SI4 CardCombo::GetJokerSuitValue2 () {
    return joker_suit_2_;
}

BOL CardCombo::SetJokerSuitValue2 (SI4 value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_suit_2_ = value;
    return true;
}

SI4 CardCombo::AcesHigh () {
    return aces_high_;
}

void CardCombo::SetAcesHigh (SI4 value) {
    aces_high_ = value;
}

}   //< namespace cards
}   //< namespace kabuki
