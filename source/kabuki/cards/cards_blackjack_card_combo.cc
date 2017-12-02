/** kabuki::cards
    @file       ~/source/kabuki/cards/blackjack_card_combo.cc
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
#include "blackjack_card_combo.h"

namespace kabuki { namespace cards {

BlackjackCardCombo::BlackjackCardCombo (const CardStack& cards, bool aces_high) :
    CardCombo (cards, aces_high) {
    Set (0, 1, 0, 1, aces_high);
}

int BlackjackCardCombo::GetValue () {
    int total = 0;
    Card* card;
    for (int i = 0; i < GetCount (); ++i) {
        card = GetCard (i);
        int pip = card->GetPip ();

        if (pip == Card::kAce) {
            total += AcesHigh ()?10:1;
        } else {
            // Then just add the point to the score like normal.
            total += card->GetValue ();
        }
    }
    return total;
}

}   //< namespace cards
}   //< namespace kabuki
