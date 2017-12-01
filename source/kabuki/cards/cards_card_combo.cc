/** kabuki:cards
    @file    /.../KabukiTheater-Examples/kabuki_cards/kabuki_cards/Blackjack/CardCombo.cc
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

CardCombo::CardCombo (CardStack& copyStack, int joker_pip_value1,
                      int joker_suit_value_1, int joker_pip_value_2,
                      int joker_suit_value_2, int aces_high) {
    SetValues (joker_pip_value1, joker_suit_value_1, joker_pip_value_2,
               joker_suit_value_2, aces_high);
}

void CardCombo::SetValues (int joker_pip_value1, int joker_suit_value_1,
                           int joker_pip_value_2, int joker_suit_value_2,
                           int aces_high) {
    if (joker_pip_value1 < 1)
        joker_pip_value1 = 1;
    else if (joker_pip_value1 > 13)
        joker_pip_value1 = 13;

    if (joker_pip_value_2 < 1)
        joker_pip_value_2 = 1;
    else if (joker_pip_value_2 > 13)
        joker_pip_value_2 = 13;

    if (joker_suit_value_1 < 1)
        joker_suit_value_1 = 1;
    else if (joker_suit_value_1 > 4)
        joker_suit_value_1 = 4;

    if (joker_suit_value_2 < 1)
        joker_suit_value_2 = 1;
    else if (joker_suit_value_2 > 4)
        joker_suit_value_2 = 4;

    joker_pip_value_1_ = joker_pip_value1;
    joker_suit_value_1_ = joker_suit_value_1;
    joker_pip_value_2_ = joker_pip_value_2;
    joker_suit_value_2_ = joker_suit_value_2;
    aces_high_ = aces_high;
}

int CardCombo::GetJokerPipValue1 () {
    return joker_pip_value_1_;
}

int CardCombo::SetJokerPipValue1 (int value) {
    if (value < 0)
        return -1;
    if (value > 13)
        return 1;
    joker_pip_value_1_ = value;
}

int CardCombo::GetJokerSuitValue1 () {
    return joker_suit_value_1_;
}

int CardCombo::SetJokerSuitValue1 (int value) {
    if (value < 0)
        return -1;
    if (value > 13)
        return 1;
    joker_suit_value_1_ = value;
}

int CardCombo::GetJokerPipValue2 () {
    return joker_pip_value_2_;
}

int CardCombo::SetJokerPipValue2 (int value) {
    if (value < 0)
        return -1;
    if (value > 13)
        return 1;
    joker_pip_value_2_ = value;
}

int CardCombo::GetJokerSuitValue2 () {
    return joker_suit_value_2_;
}

int CardCombo::SetJokerSuitValue2 (int value) {
    if (value < 0)
        return -1;
    if (value > 13)
        return 1;
    joker_suit_value_2_ = value;
}

int CardCombo::GetAcesHigh () {
    return aces_high_;
}

int CardCombo::SetAcesHigh (int value) {
    aces_high_ = value;
}

}   //< namesapce cards
}   //< namesapce kabuki
