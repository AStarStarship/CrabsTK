/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/blackjack/blackjackCardCombo.h
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
#ifndef HEADER_FOR_KABUKI_ARCADE_CARDS_BLACKJACKCARDCOMBO
#define HEADER_FOR_KABUKI_ARCADE_CARDS_BLACKJACKCARDCOMBO

#include "card_combo.h"

namespace kabuki { namespace cards {
/** Class that represents a CardCombo in a game.
    There are two major things that will effect a combination's rank besides
    suit rank; the presence of jokers, and if aces are high or low.
*/
class BlackjackCardCombo : public CardCombo { 
    public:

    /** Default constructor.
    */
    BlackjackCardCombo (const CardStack& cards, bool aces_high);

    virtual ~BlackjackCardCombo ();

    /** Function that returns the point value based on the ace value. */
    int GetValue ();
    
};      //< class BlackjackCardCombo
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_CARDS_BLACKJACKCARDCOMBO
