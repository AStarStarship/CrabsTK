/* Kabuki Arcade
    @file       ~/source/kabuki/arcade/cards/blackjack_card_combo.cc
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
#include "blackjack_card_combo.h"

namespace kabuki { namespace cards {

BlackjackCardCombo::BlackjackCardCombo (const CardStack& cards, BOL aces_high) :
    CardCombo (cards, aces_high) {
    Set (0, 1, 0, 1, aces_high);
}

BlackjackCardCombo::~BlackjackCardCombo () {
    // Nothing to do here :-)
}

SI4 BlackjackCardCombo::GetValue () {
    SI4 total = 0;
    Card* card;
    for (SI4 i = 0; i < Length (); ++i) {
        card = GetCard (i);
        SI4 pip = card->GetPip ();

        if (pip == Card::kAce) {
            total += AcesHigh ()?10:1;
        } else {
            // Then just add the point to the score like normal.
            total += card->GetDenomination ();
        }
    }
    return total;
}

}   //< namespace cards
}   //< namespace kabuki
