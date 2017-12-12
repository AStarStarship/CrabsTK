/** kabuki:cards
    @file    ~/source/kabuki/cards/cards_card_combo.cc
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

#include "card_combo.h"

namespace kabuki { namespace cards {

CardCombo::CardCombo (const CardStack& cards, bool aces_high,
                      int joker_pip_1, int joker_suit_1,
                      int joker_pip_2, int joker_suit_2) {
    Set (aces_high, joker_pip_1, joker_suit_1, joker_pip_2, joker_suit_2);
}

void CardCombo::Set (bool aces_high, int joker_pip_1, int joker_suit_1,
                     int joker_pip_2, int joker_suit_2) {
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

int CardCombo::GetJokerPip1 () {
    return joker_pip_1_;
}

bool CardCombo::SetJokerPip1 (int value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_pip_1_ = value;
    return true;
}

int CardCombo::GetJokerSuitValue1 () {
    return joker_suit_1_;
}

bool CardCombo::SetJokerSuitValue1 (int value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_suit_1_ = value;
    return true;
}

int CardCombo::GetJokerPip2 () {
    return joker_pip_2_;
}

bool CardCombo::SetJokerPip2 (int value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_pip_2_ = value;
    return true;
}

int CardCombo::GetJokerSuitValue2 () {
    return joker_suit_2_;
}

bool CardCombo::SetJokerSuitValue2 (int value) {
    if (value < 0)
        return false;
    if (value > 13)
        return false;
    joker_suit_2_ = value;
    return true;
}

int CardCombo::AcesHigh () {
    return aces_high_;
}

void CardCombo::SetAcesHigh (int value) {
    aces_high_ = value;
}

}   //< namespace cards
}   //< namespace kabuki
